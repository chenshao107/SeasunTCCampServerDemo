#include "./level.h"
#include "../3rd/src/cJSON/cJSON.h"
#include "filedb/filedb.h"
#include "../../netpack/nethandle.h"
#include <string>
#include "level/RankMgr.h"
#include <fstream>

extern PlayerMgr g_playerMgr;
extern  RankMgr g_rankMgr;

LevelMgr g_levelMgr;



#define  LEVEL_LIST_SAVE_PATH  "./config/level_list.json"

#define  LEVEL_DETAIL_SAVE_PRE_PATH   "./config/level_detail"

#define LEVEL_DATA_SAVE_PRE_PATH "./config/level_data"






const int BUF_SIZE = 1024 * 8;

char load_buf[BUF_SIZE] = {0};

char send_buf[1024 * 8];


LevelMgr::LevelMgr()
{
	if (!this->__load_level_list())
	{
		printf("加载关卡列表出错\n");
		getchar();
		exit(1);
	}
	if (!this->__load_level_detail())
	{
		printf("加载关卡详情出错\n");
		getchar();

		exit(1);
	}
}


LevelMgr::~LevelMgr()
{
}

bool LevelMgr::on_level_list_req(uv_tcp_t* client, LevelListReq* req)
{	


	bool result = false;
	LevelListRsp rsp;
	Player* player_p = g_playerMgr.find_player(req->playerid());

	if (!player_p)
	{
		printf("请求副本列表的玩家不存在");
		rsp.set_resultcode(LEVEL_LIST_RESULT_CODE::LEVEL_LIST_RESULT_PLAYER_NOT_ONLINE);
		result = false;
		goto Exit0;
	}


	if (__level_list_pb_len == 0)
	{
		result = false;
		printf("内存中关卡列表pb长度为0，出错\n");
		rsp.set_resultcode(LEVEL_LIST_RESULT_CODE::LEVEL_LIST_RESULT_SERVER_ERROR);
		goto Exit0;
	}
	//正常，只需用内存的pb包回。
	result = true;


Exit0:

	if (result)
	{
		//保存的pb包已经经过编码，所有可以直接发
		return sendData((uv_stream_t*)client, __level_list_pb, __level_list_pb_len);
	}
	else SendPBToClient(client, SERVER_LEVEL_LIST_RSP, &rsp);


	return result;
}

bool LevelMgr::on_level_detail_req(uv_tcp_t* client, LevelDetailReq* req)
{
	bool result = false;
	int len = 0;
	LevelDetailRsp rsp;

	//寻找关卡数据
	auto level_detail_data_map_iter = __level_detail_packet_network_byte_Map.find(req->levelid());

	Player* player_p = g_playerMgr.find_player(req->playerid());

	if (!player_p)
	{
		printf("请求副本列表的玩家不在线");
		rsp.set_resultcode(LEVEL_DETAIL_RESULT_CODE::LEVEL_DETAIL_RESULT_PLAYER_NOT_ONLINE);
		goto Exit0;
	}

	//检验关卡id是否存在,同时检测内存中对应的网络包是否存在
	{
		if (level_detail_data_map_iter == __level_detail_packet_network_byte_Map.end())
		{
			printf("在内存中找不到关卡%d的详情数据\n", req->levelid());
			rsp.set_resultcode(LEVEL_DETAIL_RESULT_CODE::LEVEL_DETAIL_RESULT_LEVELID_ILLEGAL);
			result = false;
			goto Exit0;
		}

		if (level_detail_data_map_iter->second.size() == 0)
		{
			printf("在内存中关卡%d的详情数据为空\n", req->levelid());
			rsp.set_resultcode(LEVEL_DETAIL_RESULT_CODE::LEVEL_DETAIL_RESULT_SERVER_ERROR);
			result = false;
			goto Exit0;
		}
		result = true;
	}

Exit0:
	if (result)
	{
		vector<char>& data_vector = level_detail_data_map_iter->second;
		char* data_array_p = &(data_vector[0]);
		int len = data_vector.size();
		return sendData((uv_stream_t*)client, data_array_p,len);

	}
	else
	{
		SendPBToClient(client, TCCamp::SERVER_LEVEL_CMD::SERVER_LEVEL_DETAIL_RSP, &rsp);
		return result;
	}
}

bool LevelMgr::on_level_data_req(uv_tcp_t* client, LevelDataReq* req)
{
	bool result = false;
	TCCamp::LevelDataRsp rsp;
	int len = 0;

	//判断玩家是否在线
	{
		Player* player_p = g_playerMgr.find_player(req->playerid());
		if (!player_p) {
			fprintf(stdout,"请求拉取副本数据的玩家不在线\n");
			rsp.set_resultcode(TCCamp::LEVEL_DATA_RESULT_CODE::LEVEL_DATA_RESULT_PLAYER_NOT_ONLINE);
			goto Exit0;
		}
	}

	//尝试读取本地数据
	{
		char level_data_path[512] = {};
		//组装存储路径
		sprintf(level_data_path, "%s//%d", LEVEL_DATA_SAVE_PRE_PATH, req->levelid());
		len = readCfg(level_data_path, load_buf, BUF_SIZE);
		switch (len)
		{
		case -1:
			printf("文件不存在，非法关卡id，也可能服务器文件缺失\n");
			result = false;
			rsp.set_resultcode(LEVEL_DATA_RESULT_CODE::LEVEL_DATA_RESULT_LEVELID_ILLEGAL);
			goto Exit0;
			break;
		case -2:
			printf("缓冲区不足\n");
			rsp.set_resultcode(LEVEL_DATA_RESULT_CODE::LEVEL_DETA_RESULT_SERVER_ERROR);
			result = false;
			goto Exit0;
			break;
		default:
			result = true;
			break;
		}

	}
Exit0:
	int len1 = encode(send_buf, TCCamp::SERVER_LEVEL_CMD::SERVER_LEVEL_DATA_RSP, load_buf,len);
	//同时将玩家加入结算状态
	__settle_status_Map[req->playerid()] = SettleStatus(req->levelid());
	return sendData((uv_stream_t*)client, send_buf,len1);
	return result;
}

bool LevelMgr::on_level_start_req(uv_tcp_t* client, LevelStart* req)
{
	bool result = false;
	string player_id = req->playerid();
	auto iter = __settle_status_Map.find(player_id);
	if (iter == __settle_status_Map.end())
	{
		printf("找不到玩家%s结算状态\n", player_id.c_str());
		result = false;
		goto Exit0;
	}

	iter->second.m_is_start = true;
	iter->second.m_start_time = time(NULL);
	iter->second.m_spend_time = 0;

	result = true;
Exit0:

	return result;
}

bool LevelMgr::on_level_settle_req(uv_tcp_t* client, LevelSettleReq* req)
{
	bool result = false;
	LevelSettleRsp rsp;
	auto settle_status_map_iter = __settle_status_Map.find(req->playerid());

	//找一下该玩家的游戏状态
	if (settle_status_map_iter == __settle_status_Map.end())
	{
		result = false;
		rsp.set_resultcode(LEVEL_SETTLE_RESULT_CODE::LEVEL_SETTLE_RESULT_CAN_NOT_FIND_GAME);
		goto Exit0;
	}

	//如果游戏没开始
	if (!settle_status_map_iter->second.m_is_start)
	{
		result = false;
		rsp.set_resultcode(LEVEL_SETTLE_RESULT_CODE::LEVEL_SETTLE_RESULT_CAN_NOT_FIND_GAME);
		goto Exit0;
	}

	//检测时间会不会差距过大，大于60s，或者服务器计时比游戏时间还短,判作弊
	{
		int64_t server_timing = time(NULL) - settle_status_map_iter->second.m_start_time;

		//加上这个花费时间，因为断线重连之前的积累时间回在m_spend_time里
		server_timing += settle_status_map_iter->second.m_spend_time;

		if (server_timing+1 < req->spendtime() || server_timing - req->spendtime() >60)
		{
			printf("服务器计时%lld\t 客户端计时%f", server_timing, req->spendtime());
			result = false;
			rsp.set_resultcode(LEVEL_SETTLE_RESULT_CODE::LEVEL_SETTLE_RESULT_CHEATING);
			goto Exit0;
		}
	}

	//结算
	{
		int score = (req->killghost() * 100 + (30 - req->spendtime()) * 300);
		if (score < 0)score = 0;
		rsp.set_resultcode(LEVEL_SETTLE_RESULT_CODE::LEVEL_SETTLE_RESULT_OK);
		rsp.set_score(score);

		Player* play = g_playerMgr.find_player(req->playerid());
		if (!play)
		{
			rsp.set_resultcode(LEVEL_SETTLE_RESULT_CODE::LEVEL_SETTLE_RESULT_SERVER_ERROR);
			goto Exit0;
		}

		//将金币加上
		play->money += req->gold();
		//更新最高分
		if (score > play->score) {
			play->score = score;
			RankItem rankItem;
			rankItem.set_gold(play->money);
			rankItem.set_name(play->Name);
			rankItem.set_playerid(play->PlayerID);
			rankItem.set_score(play->score);
			g_rankMgr.update(rankItem);
		}
		result = true;
	}

Exit0:
	//把游戏状态删了，
	__settle_status_Map.erase(req->playerid());

	//把存档清理
	auto save_iter = __Level_state_Map.find(req->playerid());
	if (save_iter == __Level_state_Map.end())
	{
		printf("结算时，找不到玩家的存档状态，可能出错\n");
	}
	else
	{
		//回收内存
		delete save_iter->second;
		__Level_state_Map.erase(save_iter);
	}


	//能找到存档就删除存档
	if (have_save(req->playerid()))
	{
		printf("找到玩家%s存档,准备删除\n", req->playerid().c_str());
		if (remove(__get_save_file_path(req->playerid()).c_str()))
		{
			printf("删除玩家存档文件失败\n");
		};
	};
	SendPBToClient(client,SERVER_LEVEL_CMD::SERVER_LEVEL_SETTLE_RSP, &rsp);
	return result;
}

void LevelMgr::on_player_diconnect(string playerID)
{

	bool result = false;
	if (__settle_status_Map.find(playerID) != __settle_status_Map.end())
	{
		int level_id = 0;
		{
			//拿到level_id
			auto settle_status_it = __settle_status_Map.find(playerID);
			if (settle_status_it == __settle_status_Map.end())
			{
				printf("在结算状态中找不到该玩家状态,不用存档\n");
				goto Exit0;
			}
			level_id = settle_status_it->second.m_level_id;
		}
		auto level_state_iter = __Level_state_Map.find(playerID);
		{
			if (level_state_iter == __Level_state_Map.end())
			{
				printf("找到了玩家结算状态，但没找到对应的存档，可能出错\n");

				//擦除玩家的结算状态
				__settle_status_Map.erase(playerID);
				goto Exit0;
			}
		}

		//创建存档pb数据
		ServerSaveFile save_pb_data;
		save_pb_data.set_levelid (level_id);
		save_pb_data.set_savetime(time(NULL));
		save_pb_data.set_spendtime(time(NULL) - __settle_status_Map[playerID].m_start_time);

		//在待保存的pb中，填写save项
		LevelState* state_in_pb_p =save_pb_data.mutable_save();
		state_in_pb_p->CopyFrom(*(level_state_iter->second));


		string s;
		save_pb_data.SerializeToString(&s);

		//对存档进行存盘
		__save_savefile(playerID, s.c_str(), s.size());

		//清理内存的字节流
		delete level_state_iter->second;
		__Level_state_Map.erase(level_state_iter);

		//擦除玩家的结算状态
		__settle_status_Map.erase(playerID);
	}

Exit0:
	return;

}


bool LevelMgr::have_save(string playerID)
{
	//检测玩家是否有存档
	std::ifstream file(__get_save_file_path(playerID).c_str());
	return file.good();
}

void LevelMgr::reconnect(uv_tcp_t* client,string playerID)
{
	ReconnectPkg reconnectpkg;
	printf("检测到玩家%s有存档，尝试发送存档给玩家\n", playerID.c_str());

	string filepath = "save/";
	filepath += playerID;
	const int buf_size = 1024 * 5;
	char buf[buf_size];
	load(filepath.c_str(), buf, buf_size);
	ServerSaveFile  save_file = ServerSaveFile();
	save_file.ParseFromArray(buf, buf_size);

	//如果存档时间过久，删除，不重连
	if (time(NULL) - save_file.savetime() > 60000)
	{
		printf("存档过期，不进行重连\n");
		goto Exit0;
	}


	if (__Level_state_Map.find(playerID) != __Level_state_Map.end())
	{
		printf("重连时发现有玩家%s内存存档,可能出错\n", playerID.c_str());
	}


	__Level_state_Map[playerID] = new LevelState();
	__Level_state_Map[playerID]->CopyFrom(save_file.save());
	
	{
		auto settle_status_map_iter = __settle_status_Map.find(playerID);
		if (settle_status_map_iter != __settle_status_Map.end())
		{
			printf("重连时发现玩家%s有个内存的结算状态，可能出错\n", playerID.c_str());
		}
	}

	{
		//将玩家加入结算状态
		SettleStatus settle_status;
		settle_status.m_start_time = true;
		settle_status.m_spend_time = save_file.spendtime();
		settle_status.m_level_id = save_file.levelid();
		__settle_status_Map[playerID] = settle_status;
	}
	//组装回包
	//包括副本生成数据
	{
		int len = 0;
		char level_data_path[512] = {};
		//组装存储路径
		sprintf(level_data_path, "%s/%d", LEVEL_DATA_SAVE_PRE_PATH, save_file.levelid());
		len = readCfg(level_data_path, load_buf, BUF_SIZE);
		switch (len)
		{
		case -1:
			printf("文件不存在，非法关卡id，也可能服务器文件缺失\n");
			goto Exit0;
			break;
		case -2:
			printf("缓冲区不足\n");
			goto Exit0;
			break;
		default:
			break;
		}
		LevelDataRsp* level_data_p = reconnectpkg.mutable_leveldata();
		level_data_p->ParseFromArray(load_buf, len);

	}


	//组装副本状态
	{
		LevelState* level_state_p = reconnectpkg.mutable_state();
		level_state_p->CopyFrom(save_file.save());

	}

	printf("发送存档\n");
	SendPBToClient(client, SERVER_LEVEL_RECONNECT, &reconnectpkg);
Exit0:
	return;
}


bool LevelMgr::__load_level_list()
{
	cJSON* root = nullptr;
	bool result = false;
	int len = 0;
	LevelListRsp rsp;

	switch (len = readCfg(LEVEL_LIST_SAVE_PATH, load_buf, BUF_SIZE))
	{
	case -1:
		printf("副本列表文件不存在\n");
		result = false;
		goto Exit0;
		break;
	case -2:
		printf("缓冲区不足\n");
		result = false;
		goto Exit0;
		break;
	default:
		break;
	}


	load_buf[len] = '\0';
	root = cJSON_Parse(load_buf);
	if (root == nullptr)
	{
		result = false;
		printf("json 解析出错\n");
		goto Exit0;
	}
	{
		//正常，组装消息
		cJSON* level_list_json = cJSON_GetObjectItem(root, "level_list");
		for (int i = 0; i < cJSON_GetArraySize(level_list_json); i++)
		{
			auto item_json_p = cJSON_GetArrayItem(level_list_json, i);
			auto item_pb_p = rsp.add_levellist();

			int level_id = cJSON_GetObjectItem(item_json_p, "level_id")->valueint;
			item_pb_p->set_levelid(level_id);
			
			//将id插入集合
			if (__level_detail_packet_network_byte_Map.find(level_id) != __level_detail_packet_network_byte_Map.end()) {
				printf("关卡id重复\n");
				result = false;
				goto Exit0;
			}
			__level_detail_packet_network_byte_Map.insert({ level_id,{} });


			printf(cJSON_GetObjectItem(item_json_p, "name")->valuestring);
			item_pb_p->set_name(cJSON_GetObjectItem(item_json_p, "name")->valuestring);
		}
		result = true;
		rsp.set_resultcode(LEVEL_LIST_RESULT_CODE::LEVEL_LIST_RESULT_OK);

	}


Exit0:
	if (result)
	{
		//加载成功，放入内存
		string data;
		rsp.SerializeToString(&data);

		__level_list_pb_len = encode(__level_list_pb,SERVER_LEVEL_CMD::SERVER_LEVEL_LIST_RSP, data.c_str(), (int)data.length());
		
		//回包要这么回
		//return sendData((uv_stream_t*)client, __level_list_pb, __level_list_pb_len);
	}
	if (root)cJSON_Delete(root);
	return result;
}

bool LevelMgr::__load_level_detail() {
	//遍历每一个前面加载过的关卡id，加载详情
	LevelDetailRsp rsp;
	bool result = false;
	for (auto& item : __level_detail_packet_network_byte_Map)
	{
		if (__load_one_level_detail_to_pb(rsp, item.first))
		{
			//加载成功，放入内存
			string data;
			rsp.SerializeToString(&data);

			int network_byte_len = encode(send_buf, SERVER_LEVEL_CMD::SERVER_LEVEL_DETAIL_RSP, data.c_str(), (int)data.length());

			//将编码后的字节写入内存map,有请求直接发
			vector<char>& pb_vector = item.second;
			pb_vector.resize(network_byte_len);
			for (int i = 0; i < network_byte_len; i++)
			{
				pb_vector[i] = send_buf[i];
			}

			//回包要这么回
			//return sendData((uv_stream_t*)client, vector<char>的数组起始指针, vector<char> 长度);
		}
		else
		{
			printf("加载关卡%d详情失败\n",item.first);
			result = false;
			goto Exit0;
		}
	}
	result = true;
Exit0:
	return result;
	
}


bool LevelMgr::__load_one_level_detail_to_pb(LevelDetailRsp& rsp,int level_id){
	bool result = false;
	cJSON* root = nullptr;
	int len = 0;
	char level_detail_path[512] = {};



	//组装存储路径
	sprintf(level_detail_path, "%s//%d.json", LEVEL_DETAIL_SAVE_PRE_PATH, level_id);
	len = readCfg(level_detail_path, load_buf, BUF_SIZE);
	switch (len)
	{
	case -1:
		printf("文件不存在，非法关卡id，也可能服务器文件缺失\n");
		result = false;
		goto Exit0;
		break;
	case -2:
		printf("缓冲区不足\n");
		result = false;
		goto Exit0;
		break;
	default:
		break;
	}


	load_buf[len] = '\0';
	root = cJSON_Parse(load_buf);
	if (root == nullptr)
	{
		printf("json 解析出错\n");
		result = false;
		goto Exit0;
	}
	//前面正常，组装pb
	{
		//判断cjson解析结果
		rsp.set_levelid(level_id);
		result = false;//默认解析失败，成功了再改为true
		cJSON* temp;
		{
			//name
			temp= cJSON_GetObjectItem(root, "name");
			if (!temp)
			{
				printf("关卡%d name json解析失败\n", level_id);
				goto Exit0;
			}
			rsp.set_name(temp->valuestring);

		}

		{
			//detail
			temp = cJSON_GetObjectItem(root, "detail");
			if (!temp)
			{
				printf("关卡%d detail json解析失败\n", level_id);
				goto Exit0;
			}
			rsp.set_detail(temp->valuestring);
		}

		{
			//ghost_quantity
			temp = cJSON_GetObjectItem(root, "ghost_quantity");
			if (!temp)
			{
				printf("关卡%d ghost_quantity json解析失败\n", level_id);
				goto Exit0;
			}
			rsp.set_ghostquantity(temp->valueint);
		}

		{
			//gargoyle_quantity
			temp = cJSON_GetObjectItem(root, "gargoyle_quantity");
			if (!temp)
			{
				printf("关卡%d gargoyle_quantity json解析失败\n", level_id);
				goto Exit0;
			}
			rsp.set_gargoylequantity(temp->valueint);
		}
		
		{
			//treasure_chest_qantity
			temp = cJSON_GetObjectItem(root, "treasure_chest_qantity");
			if (!temp)
			{
				printf("关卡%d treasure_chest_qantity json解析失败\n", level_id);
				goto Exit0;
			}
			rsp.set_treasurechestquantity(temp->valueint);
		}


		{
			//ghost_speed
			temp = cJSON_GetObjectItem(root, "ghost_speed");
			if (!temp)
			{
				printf("关卡%d ghost_speed json解析失败\n", level_id);
				goto Exit0;
			}
			rsp.set_gargoylequantity(temp->valuedouble);
		}
		{
			//player_speed 
			temp = cJSON_GetObjectItem(root, "player_speed");
			if (!temp)
			{
				printf("关卡%d player_speed json解析失败\n", level_id);
				goto Exit0;
			}
			rsp.set_gargoylequantity(temp->valuedouble);
		}

		{
			//player_hp
			temp = cJSON_GetObjectItem(root, "player_hp");
			if (!temp)
			{
				printf("关卡%d player_hp json解析失败\n", level_id);
				goto Exit0;
			}
			rsp.set_gargoylequantity(temp->valueint);
		}
		{
			//limit_time
			temp = cJSON_GetObjectItem(root, "limit_time");
			if (!temp)
			{
				printf("关卡%d limit_time json解析失败\n", level_id);
				goto Exit0;
			}
			rsp.set_gargoylequantity(temp->valueint);
		}

		rsp.set_resultcode(LEVEL_DETAIL_RESULT_CODE::LEVEL_DETAIL_RESULT_OK);
		result = true;
	}
Exit0:

	if (root)cJSON_Delete(root);
	return result;

}


bool LevelMgr::on_player_level_save(uv_tcp_t* client, LevelState* req) {
	bool result = false;
	auto player_iter = g_playerMgr.m_playerMap.find(client);
	if (player_iter == g_playerMgr.m_playerMap.end())
	{
		printf("找不到玩家去更新存档状态要新建存档\n");

		result = false;
		goto Exit0;
	}
	result = __update_level_state(player_iter->second, req);

Exit0:
	return result;
};




//保存玩家存档
bool LevelMgr::__save_savefile(string playerId, const char* data, int len)
{
	bool result = false;
	char save_path[1024];
	sprintf(save_path, "/save/%s", playerId.c_str());
	switch (save(save_path, data, len))
	{
	case -1:
		printf("打开文件失败\n");
		result = false;
		goto Exit0;
	case -2:
		printf("缓冲区不足\n");
		result = false;
		goto Exit0;
	default:
		break;
	}

	result = true;

Exit0:
	if (!result)
		printf("玩家%s存档失败\n", playerId.c_str());
	return result;
}

//更新状态
bool LevelMgr::__update_level_state(Player* player, LevelState* level_state) {
	bool result = false;
	auto iter = __Level_state_Map.find(player->PlayerID);
	LevelState* level_state_p = nullptr;
	if (iter == __Level_state_Map.end())
	{
		//建存档
		level_state_p = new LevelState();
		__Level_state_Map[player->PlayerID] = level_state_p;
	}
	else {
		level_state_p = iter->second;
	}

	
	level_state_p->CopyFrom(*level_state);
	////深拷贝
	//string pb_string;
	//level_state->SerializeToString(&pb_string);
	//__Level_state_Map[player->PlayerID]->ParseFromArray(pb_string.c_str(), pb_string.size());

	//__Level_state_Map[]
	
	return false;
}

string  LevelMgr::__get_save_file_path(string playerID)
{
	string filepath = "./data/save/";
	return filepath += playerID;
};