#include "level/RankMgr.h"
#include "../../cJSON/cJSON.h"
#include "../../proto/level.pb.h"
#include <filedb/filedb.h>
#include "../../codec/codec.h"
#include "../../utils/utils.h"
#include <fstream>

static const int BUF_SIZE = 1024 * 5;

#define LEVEL_SCORE_RANGE	"./config/level_score_range.json"
#define PLAYER_LIST_TXT		"./data/player_list/player_list.txt"
#define GENERATE_PLAYER_ID_LIST_SCRIPT	"get_all_player_id.bat"

RankMgr g_rankMgr;

extern PlayerMgr g_playerMgr;

char buf[BUF_SIZE];

RankMgr::RankMgr()
{

	//从玩家列表文件中获取所有玩家id
	vector<string> player_idVector = __get_all_player_id();

	//加载玩家数据进内存，排序
	vector<TCCamp::RankItem> rank_Vector;
	for (string player_id : player_idVector)
	{
		rank_Vector.push_back(__get_rank_item_by_playerId(player_id));
	}
	sort(rank_Vector.begin(), rank_Vector.end(), 
		[](TCCamp::RankItem a, TCCamp::RankItem b) {return a.score() > b.score(); });
	int rank = 0;
	for (auto &i : rank_Vector) __rank_list.push_back(i);
}

RankMgr::~RankMgr()
{
}


//更新排行榜
bool RankMgr::update(const TCCamp::RankItem& rank_item)
{
	__is_need_to_update = true;
	//清空缓存表
	__rank_rsp_pb_cache.clear();

	//找到玩家
	auto iter = __rank_list.begin();
	for (; iter != __rank_list.end(); iter++)
	{
		if (iter->playerid() == rank_item.playerid())
			break;
	}

	if (iter == __rank_list.end())
	{
		printf("排行榜中找不到玩家%s\n", rank_item.playerid().c_str());
		goto Exit0;
	}
	{
		//比记录高，需要更新
		bool is_insert = false;
		if (rank_item.score() > iter->score())
		{
			__rank_list.erase(iter);
			for (auto insert_iter = __rank_list.begin(); insert_iter!=__rank_list.end();insert_iter++)
			{
				if (insert_iter->score() < rank_item.score())
				{
					is_insert = true;
					__rank_list.insert(insert_iter,rank_item);
					break;
				}
			}
			//没被插入说明得插在最后
			if (!is_insert)
			{
				__rank_list.push_back(rank_item);
			}
		}
	}
Exit0:
	return true;
}

string RankMgr::get_rank_pb_string(int page, Player* player_p)
{
	auto page_cache_iter = __rank_rsp_pb_cache.find(page);
	//缓存中没有
	if (page_cache_iter == __rank_rsp_pb_cache.end())
	{
		//生成缓冲
		__generate_cache(page);
		page_cache_iter = __rank_rsp_pb_cache.find(page);
	}

	//组合玩家自己那部分的报文
	TCCamp::RankItem* player_rank = new TCCamp::RankItem;
	player_rank->set_name(player_p->Name);
	player_rank->set_playerid(player_p->PlayerID);
	player_rank->set_gold(player_p->money);
	player_rank->set_score(player_p->score);
	player_rank->set_rank(-1);
	//查询排名
	{
		int i = 0;
		for (auto item : __rank_list)
		{
			if (item.playerid() == player_p->PlayerID)
			{
				player_rank->set_rank(i);
				break;
			}
			i++;
		}
	}
	if (player_rank->rank() < 0)
	{
		printf("在排行表中找不到玩家%s\n", player_p->PlayerID.c_str());
		page_cache_iter->second.set_resultcode(TCCamp::LEVEL_RANK_RESULT_SERVER_ERROR);
		goto Exit0;
	}
	page_cache_iter->second.set_allocated_playerrank(player_rank);
	page_cache_iter->second.set_resultcode(TCCamp::LEVEL_RANK_RESULT_OK);

Exit0:

	string ret;
	page_cache_iter->second.SerializeToString(&ret);
	//释放报文中对应的玩家内存
	page_cache_iter->second.release_playerrank();
	return ret;
}

bool RankMgr::on_rank_req(uv_tcp_t* client, RankReq* req)
{
	Player* player_p = g_playerMgr.find_player(req->playerid());
	string pb_string = get_rank_pb_string(req->page(), player_p);
	int len = encode(buf, TCCamp::SERVER_LEVEL_RANK_RSP, pb_string.c_str(), pb_string.size());
	return sendData((uv_stream_t*)client, buf, len);
}

bool RankMgr::add_new_player_in_rank(string playerId)
{
	bool result = false;
	ofstream ofs;

	//加入内存排行榜中管理
	{
		TCCamp::RankItem rankItem = __get_rank_item_by_playerId(playerId);
		this->update(rankItem);
	}
	result = true;
Exit0:
	return result;

}


//前面调用的函数已经保证map里没有该键的项了,并且page小于0
bool RankMgr::__generate_cache(int page)
{
	__rank_rsp_pb_cache[page] =TCCamp::RankRsp();
	auto __rank_rsp_iter = __rank_rsp_pb_cache.find(page);

	//组装rsp
	__rank_rsp_iter->second.set_resultcode(TCCamp::LEVEL_RANK_RESULT_CODE::LEVEL_RANK_RESULT_OK);

	//如果该页的起始已经超过玩家数，返回空
	if (page * __player_per_page > __rank_list.size())
	{
		return true;
	}

	auto iter = __rank_list.begin();
	int rank = 0;
	{
		//迭代器向前推到要加载的玩家
		int count = page * __player_per_page;
		rank += count;
		while (count--)
		{
			iter++;
		}
	}
	for (int i = 0; iter != __rank_list.end() && i < __player_per_page; i++,iter++)
	{
		TCCamp::RankItem* rank_item = __rank_rsp_iter->second.add_rank();
		rank_item->set_playerid(iter->playerid());
		rank_item->set_gold(iter->gold());
		rank_item->set_name(iter->name());
		rank_item->set_score(iter->score());
		rank_item->set_rank(rank++);
	}

	return true;
}

vector<string> RankMgr::__get_all_player_id()
{
	system(GENERATE_PLAYER_ID_LIST_SCRIPT);
	bool result = false;
	vector<string> player_id_vector;
	ifstream fs;
	fs.open(PLAYER_LIST_TXT, ios::in);
	string s;
	if (!fs.is_open()) {
		printf("%s文件打开失败\n", PLAYER_LIST_TXT);
		result = false;
		goto Exit0;
	}
	while (!fs.eof())
	{
		fs >> s;
		//防止最后多读一行一样的
		if (!player_id_vector.empty()&&player_id_vector.back() == s)break;
		player_id_vector.push_back(s);
		//防止多读最后一行
		fs.get(); // 读取最后的回车符  
		if (fs.peek() == '/n')
			break;
	}
	result = true;
	
Exit0:

	if (result) {
		printf("读取玩家列表如下：\n");
		for (string s : player_id_vector)printf("%s\n",s.c_str());
		return player_id_vector;
	}
	else
	{
		getchar();
		exit(1);
	}

}

TCCamp::RankItem RankMgr::__get_rank_item_by_playerId(string playerID)
{
	bool result = false;
	int len = 0;
	cJSON* root=nullptr;
	TCCamp::RankItem rank_item;
	switch (len = load(playerID.c_str(), buf, BUF_SIZE))
	{
	case -1:
		printf("读取玩家%s数据文件不存在\n", playerID.c_str());
		result = false;
		goto Exit0;
		break;
	case -2:
		printf("读取玩家%s数据缓冲区不足\n", playerID.c_str());
		result = false;
		goto Exit0;
		break;
	default:
		break;
	}
	{
		buf[len] = '\0';
		root = cJSON_Parse(buf);
		if (!root) {
			result = false;
			printf("玩家%s数据解析失败\n ", playerID.c_str());
			goto Exit0;
		}
		rank_item.set_gold(cJSON_GetObjectItem(root, "money")->valueint);
		rank_item.set_name(cJSON_GetObjectItem(root, "Name")->valuestring);
		rank_item.set_playerid(playerID);
		rank_item.set_score(cJSON_GetObjectItem(root, "Score")->valueint);
		result = true;
	}


Exit0:
	if (root)cJSON_Delete(root);
	if (result)
	{
		return rank_item;
	}
	else {
		getchar();
		exit(1);
	}
}


