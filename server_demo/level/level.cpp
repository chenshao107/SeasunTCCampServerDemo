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
		printf("���عؿ��б����\n");
		getchar();
		exit(1);
	}
	if (!this->__load_level_detail())
	{
		printf("���عؿ��������\n");
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
		printf("���󸱱��б����Ҳ�����");
		rsp.set_resultcode(LEVEL_LIST_RESULT_CODE::LEVEL_LIST_RESULT_PLAYER_NOT_ONLINE);
		result = false;
		goto Exit0;
	}


	if (__level_list_pb_len == 0)
	{
		result = false;
		printf("�ڴ��йؿ��б�pb����Ϊ0������\n");
		rsp.set_resultcode(LEVEL_LIST_RESULT_CODE::LEVEL_LIST_RESULT_SERVER_ERROR);
		goto Exit0;
	}
	//������ֻ�����ڴ��pb���ء�
	result = true;


Exit0:

	if (result)
	{
		//�����pb���Ѿ��������룬���п���ֱ�ӷ�
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

	//Ѱ�ҹؿ�����
	auto level_detail_data_map_iter = __level_detail_packet_network_byte_Map.find(req->levelid());

	Player* player_p = g_playerMgr.find_player(req->playerid());

	if (!player_p)
	{
		printf("���󸱱��б����Ҳ�����");
		rsp.set_resultcode(LEVEL_DETAIL_RESULT_CODE::LEVEL_DETAIL_RESULT_PLAYER_NOT_ONLINE);
		goto Exit0;
	}

	//����ؿ�id�Ƿ����,ͬʱ����ڴ��ж�Ӧ��������Ƿ����
	{
		if (level_detail_data_map_iter == __level_detail_packet_network_byte_Map.end())
		{
			printf("���ڴ����Ҳ����ؿ�%d����������\n", req->levelid());
			rsp.set_resultcode(LEVEL_DETAIL_RESULT_CODE::LEVEL_DETAIL_RESULT_LEVELID_ILLEGAL);
			result = false;
			goto Exit0;
		}

		if (level_detail_data_map_iter->second.size() == 0)
		{
			printf("���ڴ��йؿ�%d����������Ϊ��\n", req->levelid());
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

	//�ж�����Ƿ�����
	{
		Player* player_p = g_playerMgr.find_player(req->playerid());
		if (!player_p) {
			fprintf(stdout,"������ȡ�������ݵ���Ҳ�����\n");
			rsp.set_resultcode(TCCamp::LEVEL_DATA_RESULT_CODE::LEVEL_DATA_RESULT_PLAYER_NOT_ONLINE);
			goto Exit0;
		}
	}

	//���Զ�ȡ��������
	{
		char level_data_path[512] = {};
		//��װ�洢·��
		sprintf(level_data_path, "%s//%d", LEVEL_DATA_SAVE_PRE_PATH, req->levelid());
		len = readCfg(level_data_path, load_buf, BUF_SIZE);
		switch (len)
		{
		case -1:
			printf("�ļ������ڣ��Ƿ��ؿ�id��Ҳ���ܷ������ļ�ȱʧ\n");
			result = false;
			rsp.set_resultcode(LEVEL_DATA_RESULT_CODE::LEVEL_DATA_RESULT_LEVELID_ILLEGAL);
			goto Exit0;
			break;
		case -2:
			printf("����������\n");
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
	//ͬʱ����Ҽ������״̬
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
		printf("�Ҳ������%s����״̬\n", player_id.c_str());
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

	//��һ�¸���ҵ���Ϸ״̬
	if (settle_status_map_iter == __settle_status_Map.end())
	{
		result = false;
		rsp.set_resultcode(LEVEL_SETTLE_RESULT_CODE::LEVEL_SETTLE_RESULT_CAN_NOT_FIND_GAME);
		goto Exit0;
	}

	//�����Ϸû��ʼ
	if (!settle_status_map_iter->second.m_is_start)
	{
		result = false;
		rsp.set_resultcode(LEVEL_SETTLE_RESULT_CODE::LEVEL_SETTLE_RESULT_CAN_NOT_FIND_GAME);
		goto Exit0;
	}

	//���ʱ��᲻������󣬴���60s�����߷�������ʱ����Ϸʱ�仹��,������
	{
		int64_t server_timing = time(NULL) - settle_status_map_iter->second.m_start_time;

		//�����������ʱ�䣬��Ϊ��������֮ǰ�Ļ���ʱ�����m_spend_time��
		server_timing += settle_status_map_iter->second.m_spend_time;

		if (server_timing+1 < req->spendtime() || server_timing - req->spendtime() >60)
		{
			printf("��������ʱ%lld\t �ͻ��˼�ʱ%f", server_timing, req->spendtime());
			result = false;
			rsp.set_resultcode(LEVEL_SETTLE_RESULT_CODE::LEVEL_SETTLE_RESULT_CHEATING);
			goto Exit0;
		}
	}

	//����
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

		//����Ҽ���
		play->money += req->gold();
		//������߷�
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
	//����Ϸ״̬ɾ�ˣ�
	__settle_status_Map.erase(req->playerid());

	//�Ѵ浵����
	auto save_iter = __Level_state_Map.find(req->playerid());
	if (save_iter == __Level_state_Map.end())
	{
		printf("����ʱ���Ҳ�����ҵĴ浵״̬�����ܳ���\n");
	}
	else
	{
		//�����ڴ�
		delete save_iter->second;
		__Level_state_Map.erase(save_iter);
	}


	//���ҵ��浵��ɾ���浵
	if (have_save(req->playerid()))
	{
		printf("�ҵ����%s�浵,׼��ɾ��\n", req->playerid().c_str());
		if (remove(__get_save_file_path(req->playerid()).c_str()))
		{
			printf("ɾ����Ҵ浵�ļ�ʧ��\n");
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
			//�õ�level_id
			auto settle_status_it = __settle_status_Map.find(playerID);
			if (settle_status_it == __settle_status_Map.end())
			{
				printf("�ڽ���״̬���Ҳ��������״̬,���ô浵\n");
				goto Exit0;
			}
			level_id = settle_status_it->second.m_level_id;
		}
		auto level_state_iter = __Level_state_Map.find(playerID);
		{
			if (level_state_iter == __Level_state_Map.end())
			{
				printf("�ҵ�����ҽ���״̬����û�ҵ���Ӧ�Ĵ浵�����ܳ���\n");

				//������ҵĽ���״̬
				__settle_status_Map.erase(playerID);
				goto Exit0;
			}
		}

		//�����浵pb����
		ServerSaveFile save_pb_data;
		save_pb_data.set_levelid (level_id);
		save_pb_data.set_savetime(time(NULL));
		save_pb_data.set_spendtime(time(NULL) - __settle_status_Map[playerID].m_start_time);

		//�ڴ������pb�У���дsave��
		LevelState* state_in_pb_p =save_pb_data.mutable_save();
		state_in_pb_p->CopyFrom(*(level_state_iter->second));


		string s;
		save_pb_data.SerializeToString(&s);

		//�Դ浵���д���
		__save_savefile(playerID, s.c_str(), s.size());

		//�����ڴ���ֽ���
		delete level_state_iter->second;
		__Level_state_Map.erase(level_state_iter);

		//������ҵĽ���״̬
		__settle_status_Map.erase(playerID);
	}

Exit0:
	return;

}


bool LevelMgr::have_save(string playerID)
{
	//�������Ƿ��д浵
	std::ifstream file(__get_save_file_path(playerID).c_str());
	return file.good();
}

void LevelMgr::reconnect(uv_tcp_t* client,string playerID)
{
	ReconnectPkg reconnectpkg;
	printf("��⵽���%s�д浵�����Է��ʹ浵�����\n", playerID.c_str());

	string filepath = "save/";
	filepath += playerID;
	const int buf_size = 1024 * 5;
	char buf[buf_size];
	load(filepath.c_str(), buf, buf_size);
	ServerSaveFile  save_file = ServerSaveFile();
	save_file.ParseFromArray(buf, buf_size);

	//����浵ʱ����ã�ɾ����������
	if (time(NULL) - save_file.savetime() > 60000)
	{
		printf("�浵���ڣ�����������\n");
		goto Exit0;
	}


	if (__Level_state_Map.find(playerID) != __Level_state_Map.end())
	{
		printf("����ʱ���������%s�ڴ�浵,���ܳ���\n", playerID.c_str());
	}


	__Level_state_Map[playerID] = new LevelState();
	__Level_state_Map[playerID]->CopyFrom(save_file.save());
	
	{
		auto settle_status_map_iter = __settle_status_Map.find(playerID);
		if (settle_status_map_iter != __settle_status_Map.end())
		{
			printf("����ʱ�������%s�и��ڴ�Ľ���״̬�����ܳ���\n", playerID.c_str());
		}
	}

	{
		//����Ҽ������״̬
		SettleStatus settle_status;
		settle_status.m_start_time = true;
		settle_status.m_spend_time = save_file.spendtime();
		settle_status.m_level_id = save_file.levelid();
		__settle_status_Map[playerID] = settle_status;
	}
	//��װ�ذ�
	//����������������
	{
		int len = 0;
		char level_data_path[512] = {};
		//��װ�洢·��
		sprintf(level_data_path, "%s/%d", LEVEL_DATA_SAVE_PRE_PATH, save_file.levelid());
		len = readCfg(level_data_path, load_buf, BUF_SIZE);
		switch (len)
		{
		case -1:
			printf("�ļ������ڣ��Ƿ��ؿ�id��Ҳ���ܷ������ļ�ȱʧ\n");
			goto Exit0;
			break;
		case -2:
			printf("����������\n");
			goto Exit0;
			break;
		default:
			break;
		}
		LevelDataRsp* level_data_p = reconnectpkg.mutable_leveldata();
		level_data_p->ParseFromArray(load_buf, len);

	}


	//��װ����״̬
	{
		LevelState* level_state_p = reconnectpkg.mutable_state();
		level_state_p->CopyFrom(save_file.save());

	}

	printf("���ʹ浵\n");
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
		printf("�����б��ļ�������\n");
		result = false;
		goto Exit0;
		break;
	case -2:
		printf("����������\n");
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
		printf("json ��������\n");
		goto Exit0;
	}
	{
		//��������װ��Ϣ
		cJSON* level_list_json = cJSON_GetObjectItem(root, "level_list");
		for (int i = 0; i < cJSON_GetArraySize(level_list_json); i++)
		{
			auto item_json_p = cJSON_GetArrayItem(level_list_json, i);
			auto item_pb_p = rsp.add_levellist();

			int level_id = cJSON_GetObjectItem(item_json_p, "level_id")->valueint;
			item_pb_p->set_levelid(level_id);
			
			//��id���뼯��
			if (__level_detail_packet_network_byte_Map.find(level_id) != __level_detail_packet_network_byte_Map.end()) {
				printf("�ؿ�id�ظ�\n");
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
		//���سɹ��������ڴ�
		string data;
		rsp.SerializeToString(&data);

		__level_list_pb_len = encode(__level_list_pb,SERVER_LEVEL_CMD::SERVER_LEVEL_LIST_RSP, data.c_str(), (int)data.length());
		
		//�ذ�Ҫ��ô��
		//return sendData((uv_stream_t*)client, __level_list_pb, __level_list_pb_len);
	}
	if (root)cJSON_Delete(root);
	return result;
}

bool LevelMgr::__load_level_detail() {
	//����ÿһ��ǰ����ع��Ĺؿ�id����������
	LevelDetailRsp rsp;
	bool result = false;
	for (auto& item : __level_detail_packet_network_byte_Map)
	{
		if (__load_one_level_detail_to_pb(rsp, item.first))
		{
			//���سɹ��������ڴ�
			string data;
			rsp.SerializeToString(&data);

			int network_byte_len = encode(send_buf, SERVER_LEVEL_CMD::SERVER_LEVEL_DETAIL_RSP, data.c_str(), (int)data.length());

			//���������ֽ�д���ڴ�map,������ֱ�ӷ�
			vector<char>& pb_vector = item.second;
			pb_vector.resize(network_byte_len);
			for (int i = 0; i < network_byte_len; i++)
			{
				pb_vector[i] = send_buf[i];
			}

			//�ذ�Ҫ��ô��
			//return sendData((uv_stream_t*)client, vector<char>��������ʼָ��, vector<char> ����);
		}
		else
		{
			printf("���عؿ�%d����ʧ��\n",item.first);
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



	//��װ�洢·��
	sprintf(level_detail_path, "%s//%d.json", LEVEL_DETAIL_SAVE_PRE_PATH, level_id);
	len = readCfg(level_detail_path, load_buf, BUF_SIZE);
	switch (len)
	{
	case -1:
		printf("�ļ������ڣ��Ƿ��ؿ�id��Ҳ���ܷ������ļ�ȱʧ\n");
		result = false;
		goto Exit0;
		break;
	case -2:
		printf("����������\n");
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
		printf("json ��������\n");
		result = false;
		goto Exit0;
	}
	//ǰ����������װpb
	{
		//�ж�cjson�������
		rsp.set_levelid(level_id);
		result = false;//Ĭ�Ͻ���ʧ�ܣ��ɹ����ٸ�Ϊtrue
		cJSON* temp;
		{
			//name
			temp= cJSON_GetObjectItem(root, "name");
			if (!temp)
			{
				printf("�ؿ�%d name json����ʧ��\n", level_id);
				goto Exit0;
			}
			rsp.set_name(temp->valuestring);

		}

		{
			//detail
			temp = cJSON_GetObjectItem(root, "detail");
			if (!temp)
			{
				printf("�ؿ�%d detail json����ʧ��\n", level_id);
				goto Exit0;
			}
			rsp.set_detail(temp->valuestring);
		}

		{
			//ghost_quantity
			temp = cJSON_GetObjectItem(root, "ghost_quantity");
			if (!temp)
			{
				printf("�ؿ�%d ghost_quantity json����ʧ��\n", level_id);
				goto Exit0;
			}
			rsp.set_ghostquantity(temp->valueint);
		}

		{
			//gargoyle_quantity
			temp = cJSON_GetObjectItem(root, "gargoyle_quantity");
			if (!temp)
			{
				printf("�ؿ�%d gargoyle_quantity json����ʧ��\n", level_id);
				goto Exit0;
			}
			rsp.set_gargoylequantity(temp->valueint);
		}
		
		{
			//treasure_chest_qantity
			temp = cJSON_GetObjectItem(root, "treasure_chest_qantity");
			if (!temp)
			{
				printf("�ؿ�%d treasure_chest_qantity json����ʧ��\n", level_id);
				goto Exit0;
			}
			rsp.set_treasurechestquantity(temp->valueint);
		}


		{
			//ghost_speed
			temp = cJSON_GetObjectItem(root, "ghost_speed");
			if (!temp)
			{
				printf("�ؿ�%d ghost_speed json����ʧ��\n", level_id);
				goto Exit0;
			}
			rsp.set_gargoylequantity(temp->valuedouble);
		}
		{
			//player_speed 
			temp = cJSON_GetObjectItem(root, "player_speed");
			if (!temp)
			{
				printf("�ؿ�%d player_speed json����ʧ��\n", level_id);
				goto Exit0;
			}
			rsp.set_gargoylequantity(temp->valuedouble);
		}

		{
			//player_hp
			temp = cJSON_GetObjectItem(root, "player_hp");
			if (!temp)
			{
				printf("�ؿ�%d player_hp json����ʧ��\n", level_id);
				goto Exit0;
			}
			rsp.set_gargoylequantity(temp->valueint);
		}
		{
			//limit_time
			temp = cJSON_GetObjectItem(root, "limit_time");
			if (!temp)
			{
				printf("�ؿ�%d limit_time json����ʧ��\n", level_id);
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
		printf("�Ҳ������ȥ���´浵״̬Ҫ�½��浵\n");

		result = false;
		goto Exit0;
	}
	result = __update_level_state(player_iter->second, req);

Exit0:
	return result;
};




//������Ҵ浵
bool LevelMgr::__save_savefile(string playerId, const char* data, int len)
{
	bool result = false;
	char save_path[1024];
	sprintf(save_path, "/save/%s", playerId.c_str());
	switch (save(save_path, data, len))
	{
	case -1:
		printf("���ļ�ʧ��\n");
		result = false;
		goto Exit0;
	case -2:
		printf("����������\n");
		result = false;
		goto Exit0;
	default:
		break;
	}

	result = true;

Exit0:
	if (!result)
		printf("���%s�浵ʧ��\n", playerId.c_str());
	return result;
}

//����״̬
bool LevelMgr::__update_level_state(Player* player, LevelState* level_state) {
	bool result = false;
	auto iter = __Level_state_Map.find(player->PlayerID);
	LevelState* level_state_p = nullptr;
	if (iter == __Level_state_Map.end())
	{
		//���浵
		level_state_p = new LevelState();
		__Level_state_Map[player->PlayerID] = level_state_p;
	}
	else {
		level_state_p = iter->second;
	}

	
	level_state_p->CopyFrom(*level_state);
	////���
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