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

	//���ڸ������а�
	bool update(const TCCamp::RankItem& rank_item);

	//���ض�Ӧҳ��pb�ַ��� 
	string get_rank_pb_string(int page,Player*player_p);

	bool on_rank_req(uv_tcp_t* client, RankReq* req);

	//��������
	bool add_new_player_in_rank(string playerId);

private:
	//ʱ�̱�֤���ű��һ���ԣ�������������ң�
	//����û�����Ϸ��ҲҪ�У�ֻ��Ϊ0�֡�
	//���е�rank�ֶ���Ҫ�ڷ���ʱ��д
	list<TCCamp::RankItem> __rank_list;

	//ÿһҳ��pb����
	map<int,TCCamp::RankRsp>__rank_rsp_pb_cache;

	//�Ƿ���Ҫ����
	int __is_need_to_update = false;

	//һҳ���ٸ�
	const int __player_per_page = 10;

	//���ɻ���
	bool __generate_cache(int page);


	//��ȡ�������id
	vector<string> __get_all_player_id();

	//��ȡ��ҵ���������
	TCCamp::RankItem __get_rank_item_by_playerId(string playerID);
};