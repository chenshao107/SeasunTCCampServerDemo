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

	//������б��ļ��л�ȡ�������id
	vector<string> player_idVector = __get_all_player_id();

	//����������ݽ��ڴ棬����
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


//�������а�
bool RankMgr::update(const TCCamp::RankItem& rank_item)
{
	__is_need_to_update = true;
	//��ջ����
	__rank_rsp_pb_cache.clear();

	//�ҵ����
	auto iter = __rank_list.begin();
	for (; iter != __rank_list.end(); iter++)
	{
		if (iter->playerid() == rank_item.playerid())
			break;
	}

	if (iter == __rank_list.end())
	{
		printf("���а����Ҳ������%s\n", rank_item.playerid().c_str());
		goto Exit0;
	}
	{
		//�ȼ�¼�ߣ���Ҫ����
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
			//û������˵���ò������
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
	//������û��
	if (page_cache_iter == __rank_rsp_pb_cache.end())
	{
		//���ɻ���
		__generate_cache(page);
		page_cache_iter = __rank_rsp_pb_cache.find(page);
	}

	//�������Լ��ǲ��ֵı���
	TCCamp::RankItem* player_rank = new TCCamp::RankItem;
	player_rank->set_name(player_p->Name);
	player_rank->set_playerid(player_p->PlayerID);
	player_rank->set_gold(player_p->money);
	player_rank->set_score(player_p->score);
	player_rank->set_rank(-1);
	//��ѯ����
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
		printf("�����б����Ҳ������%s\n", player_p->PlayerID.c_str());
		page_cache_iter->second.set_resultcode(TCCamp::LEVEL_RANK_RESULT_SERVER_ERROR);
		goto Exit0;
	}
	page_cache_iter->second.set_allocated_playerrank(player_rank);
	page_cache_iter->second.set_resultcode(TCCamp::LEVEL_RANK_RESULT_OK);

Exit0:

	string ret;
	page_cache_iter->second.SerializeToString(&ret);
	//�ͷű����ж�Ӧ������ڴ�
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

	//�����ڴ����а��й���
	{
		TCCamp::RankItem rankItem = __get_rank_item_by_playerId(playerId);
		this->update(rankItem);
	}
	result = true;
Exit0:
	return result;

}


//ǰ����õĺ����Ѿ���֤map��û�иü�������,����pageС��0
bool RankMgr::__generate_cache(int page)
{
	__rank_rsp_pb_cache[page] =TCCamp::RankRsp();
	auto __rank_rsp_iter = __rank_rsp_pb_cache.find(page);

	//��װrsp
	__rank_rsp_iter->second.set_resultcode(TCCamp::LEVEL_RANK_RESULT_CODE::LEVEL_RANK_RESULT_OK);

	//�����ҳ����ʼ�Ѿ���������������ؿ�
	if (page * __player_per_page > __rank_list.size())
	{
		return true;
	}

	auto iter = __rank_list.begin();
	int rank = 0;
	{
		//��������ǰ�Ƶ�Ҫ���ص����
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
		printf("%s�ļ���ʧ��\n", PLAYER_LIST_TXT);
		result = false;
		goto Exit0;
	}
	while (!fs.eof())
	{
		fs >> s;
		//��ֹ�����һ��һ����
		if (!player_id_vector.empty()&&player_id_vector.back() == s)break;
		player_id_vector.push_back(s);
		//��ֹ������һ��
		fs.get(); // ��ȡ���Ļس���  
		if (fs.peek() == '/n')
			break;
	}
	result = true;
	
Exit0:

	if (result) {
		printf("��ȡ����б����£�\n");
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
		printf("��ȡ���%s�����ļ�������\n", playerID.c_str());
		result = false;
		goto Exit0;
		break;
	case -2:
		printf("��ȡ���%s���ݻ���������\n", playerID.c_str());
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
			printf("���%s���ݽ���ʧ��\n ", playerID.c_str());
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


