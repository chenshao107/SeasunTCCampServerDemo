#pragma once

#include "../proto/player.pb.h"
#include "../proto/level.pb.h"
#include <set>
#include <map>
#include <vector>
#include "uv.h"
#include "player/player.h"
using namespace std;
using namespace TCCamp;


struct SettleStatus {
	SettleStatus():m_level_id(0) {};
	SettleStatus(int level_id) :m_level_id(level_id) {};

	bool m_is_start = false;
	int m_level_id;
	int64_t m_start_time = 0;
	int		m_spend_time = 0;
};


class LevelMgr {
public:

	LevelMgr();
	~LevelMgr();

	//��Ӧ�ؿ��б�����
	bool on_level_list_req(uv_tcp_t* client, LevelListReq* req);
	//��Ӧ�ؿ���������
	bool on_level_detail_req(uv_tcp_t* client, LevelDetailReq* req);
	//��Ӧ�ؿ���ͼ��������
	bool on_level_data_req(uv_tcp_t* client, LevelDataReq* req);
	//��Ϸ��ʼ��ʱ
	bool on_level_start_req(uv_tcp_t* client,LevelStart * req);
	//��Ϸ����
	bool on_level_settle_req(uv_tcp_t* client, LevelSettleReq* req);
	//��Ϸ����
	void on_player_diconnect(string PlayerID);
	//����Ƿ��д浵
	bool have_save(string playerID);
	//����
	void reconnect(uv_tcp_t* client,string PlayerID);
	//��Ӧ��Ҹ��´浵״̬��Ӧ
	bool on_player_level_save(uv_tcp_t* client, LevelState* req);
private:
	char __level_list_pb[4096] = {};//�ؿ��б�pb�����ڿ��ٻذ�
	char __level_list_pb_len = 0;	//�ؿ��б��pb������
	//�ؿ����鱨�ı�Ҳ�������ڿ��ٲ�ѯ�ؿ�id�Ƿ�Ϸ�
	map<int, vector<char>> __level_detail_packet_network_byte_Map;

	//����״̬
	//���һ�������˹ؿ��������ݣ����������У��ȴ���ʼ���Ķ�λ��ʼʱ��
	map<string, SettleStatus> __settle_status_Map;
	//״̬pb
	map<string, LevelState*> __Level_state_Map;
	//���عؿ��б��ڴ�
	bool __load_level_list();
	//���ع������鵽�ڴ�
	bool __load_level_detail();
	//���سɹ���񣬷� ��rsp���ݴ���
	bool __load_one_level_detail_to_pb(LevelDetailRsp &rsp,int level_id);
	//������Ҵ浵״̬
	bool __update_level_state(Player* player, LevelState* level_state);
	//����浵���ݵ�����
	bool __save_savefile(string playerId, const char* data, int len);
	string  __get_save_file_path(string playerID);
};




