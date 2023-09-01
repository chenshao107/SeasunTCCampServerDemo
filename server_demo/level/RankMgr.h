#pragma once
#include "level.pb.h"
#include <list>
#include <map>
#include "player/player.h"


using namespace std;




class RankMgr
{
public:
	RankMgr();
	~RankMgr();

	//用于更新排行榜
	bool update(const TCCamp::RankItem& rank_item);

	//返回对应页的pb字符串 
	string get_rank_pb_string(int page,Player*player_p);

	bool on_rank_req(uv_tcp_t* client, RankReq* req);

	//添加新玩家
	bool add_new_player_in_rank(string playerId);

private:
	//时刻保证这张表的一致性，里面有所有玩家，
	//就算没玩过游戏的也要有，只是为0分。
	//其中的rank字段需要在发包时填写
	list<TCCamp::RankItem> __rank_list;

	//每一页的pb缓冲
	map<int,TCCamp::RankRsp>__rank_rsp_pb_cache;

	//是否需要更新
	int __is_need_to_update = false;

	//一页多少个
	const int __player_per_page = 10;

	//生成缓存
	bool __generate_cache(int page);


	//获取所有玩家id
	vector<string> __get_all_player_id();

	//获取玩家的排名数据
	TCCamp::RankItem __get_rank_item_by_playerId(string playerID);
};