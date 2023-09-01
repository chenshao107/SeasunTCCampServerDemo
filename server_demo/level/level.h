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

	//响应关卡列表请求
	bool on_level_list_req(uv_tcp_t* client, LevelListReq* req);
	//响应关卡详情请求
	bool on_level_detail_req(uv_tcp_t* client, LevelDetailReq* req);
	//响应关卡地图数据请求
	bool on_level_data_req(uv_tcp_t* client, LevelDataReq* req);
	//游戏开始对时
	bool on_level_start_req(uv_tcp_t* client,LevelStart * req);
	//游戏结算
	bool on_level_settle_req(uv_tcp_t* client, LevelSettleReq* req);
	//游戏重连
	void on_player_diconnect(string PlayerID);
	//检测是否有存档
	bool have_save(string playerID);
	//重连
	void reconnect(uv_tcp_t* client,string PlayerID);
	//响应玩家更新存档状态响应
	bool on_player_level_save(uv_tcp_t* client, LevelState* req);
private:
	char __level_list_pb[4096] = {};//关卡列表pb，用于快速回包
	char __level_list_pb_len = 0;	//关卡列表的pb包长度
	//关卡详情报文表，也可以用于快速查询关卡id是否合法
	map<int, vector<char>> __level_detail_packet_network_byte_Map;

	//结算状态
	//玩家一旦请求了关卡生成数据，便会加入其中，等待开始报文定位开始时间
	map<string, SettleStatus> __settle_status_Map;
	//状态pb
	map<string, LevelState*> __Level_state_Map;
	//加载关卡列表到内存
	bool __load_level_list();
	//加载关切详情到内存
	bool __load_level_detail();
	//返回成功与否，否 则rsp内容错误
	bool __load_one_level_detail_to_pb(LevelDetailRsp &rsp,int level_id);
	//更新玩家存档状态
	bool __update_level_state(Player* player, LevelState* level_state);
	//保存存档数据到磁盘
	bool __save_savefile(string playerId, const char* data, int len);
	string  __get_save_file_path(string playerID);
};




