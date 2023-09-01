#include "nethandle.h"
#include "../codec/codec.h"
#include "player/player.h"
#include <map>
#include <level/level.h>
#include "level/RankMgr.h"

using namespace std;

static map<uv_tcp_t*, PeerData*> s_clientMap;
static char s_send_buff[64 * 1024];

extern PlayerMgr g_playerMgr;
extern LevelMgr g_levelMgr;
extern RankMgr g_rankMgr;

// ����һ��pb���󵽿ͻ���
bool SendPBToClient(uv_tcp_t* client, uint16_t cmd, ::google::protobuf::Message* msg) {
    //string data = msg->SerializeAsString();
    //int len = encode(s_send_buff, cmd, data.c_str(), (int)data.length());
    //return sendData((uv_stream_t*)client, s_send_buff, len);
    string data;
    msg->SerializeToString(&data);
    
    int len = encode(s_send_buff, cmd, data.c_str(), (int)data.length());
    return sendData((uv_stream_t*)client, s_send_buff, len);
}

bool OnNewClient(uv_tcp_t* client) {
    bool result = false;
    PeerData* peerData = malloc_peer_data();
    if (!peerData) {
        goto Exit0;
    }
    s_clientMap.insert(pair<uv_tcp_t*, PeerData*>(client, peerData));
    result = true;
Exit0:
    return result;
}

bool OnCloseClient(uv_tcp_t* client) {
    cout << "OnCloseClient" << endl;
    string player_id;
    {
        //�ҵ�playerid,�Ҳ�����ʱû��¼
        auto player_id_iter = g_playerMgr.m_playerMap.find(client);
        if (player_id_iter == g_playerMgr.m_playerMap.end()) {
            goto Exit0;
        }
        //�е�¼���Ϳ�����û����Ҫ�����
        player_id = player_id_iter->second->PlayerID;
        g_levelMgr.on_player_diconnect(player_id);
    }


    //g_levelMgr.on_player_diconnect(player_id);
    g_playerMgr.on_client_close(client);

Exit0:

    auto it = s_clientMap.find(client);
    if (it != s_clientMap.end()) {
        free_peer_data(it->second);
        cout << "Debug..s_clientMap_erasing.." << endl;
        s_clientMap.erase(it);
    }

    return true;
}

// �յ�����㷢����������

bool OnRecv(uv_tcp_t* client, const char* data, int len) {
    bool result = false;
    PeerData* peerData = nullptr;

    auto it = s_clientMap.find(client);
    if (it == s_clientMap.end()) {
        goto Exit0;
    }
    peerData = it->second;

    if (sizeof(peerData->recv_buf) - peerData->nowPos < len) {
        goto Exit0;
    }

    memcpy(peerData->recv_buf + peerData->nowPos, data, len);
    peerData->nowPos += len;

    // ����Э�����⣬��������ճ��
    while (true) {
        int packPos = check_pack(peerData->recv_buf, peerData->nowPos);
        if (packPos < 0) {          // �쳣
            goto Exit0;
        }
        else if (packPos > 0) {     // Э������������
            Packet pack;        // Э�����ʽ
            // ���룬�õ�Э��ź����л��������
            if (!decode(&pack, peerData->recv_buf, packPos)) {
                goto Exit0;
            }
            // ����һ�����ݰ�
            if (!_OnPackHandle(client, &pack)) {
                goto Exit0;
            }
            if (packPos >= peerData->nowPos) {
                peerData->nowPos = 0;
            }
            else {
                memmove(peerData->recv_buf, peerData->recv_buf + packPos, peerData->nowPos - packPos);
                peerData->nowPos -= packPos;
            }
        }
        else {
            // û����ȫ����������
            break;
        }
    }

    result = true;
Exit0:
    return result;
}
//bool OnRecv(uv_tcp_t* client, const char* data, int len) {
//    bool result = false;
//    PeerData* peerData = nullptr;
//    int offset = 0;
//    int remainLen = 0;
//
//    auto it = s_clientMap.find(client);
//    if (it == s_clientMap.end()) {
//        goto Exit0;
//    }
//    peerData = it->second;
//
//    if (sizeof(peerData->recv_buf) - peerData->nowPos < len) {
//        goto Exit0;
//    }
//    memcpy(peerData->recv_buf + peerData->nowPos, data, len);
//    peerData->nowPos += len;
//    remainLen = peerData->nowPos;
//
//    // ����Э�����⣬��������ճ��
//    while (true) {
//        int packPos = check_pack(peerData->recv_buf + offset, remainLen);
//        if (packPos < 0) {          // �쳣
//            goto Exit0;
//        }
//        else if (packPos > 0) {     // Э������������
//            Packet pack;        // Э�����ʽ
//            // ���룬�õ�Э��ź����л��������
//            if (!decode(&pack, peerData->recv_buf, packPos)) {
//                goto Exit0;
//            }
//            // ����һ�����ݰ�
//            if (!_OnPackHandle(client, &pack)) {
//                goto Exit0;
//            }
//            if (packPos >= peerData->nowPos) {
//                peerData->nowPos = 0;
//                remainLen = 0;
//            }
//            else {
//                offset += packPos;
//                remainLen -= packPos;
//            }
//        }
//        else {
//            // û����ȫ����������
//            break;
//        }
//
//        if (peerData->nowPos > 0) {
//            memmove(peerData->recv_buf, peerData->recv_buf + offset, peerData->nowPos - offset);
//            peerData->nowPos -= offset;
//        }
//    }
//
//    result = true;
//Exit0:
//    return result;
//}

// ����һ����ɵ���Ϣ��
bool _OnPackHandle(uv_tcp_t* client, Packet* pack) {
    bool result = false;
    int len = 0;
    // todo �����յ������ݰ�
    fprintf(stdout, "OnPackHandle: cmd:%d, len:%d, client:%llu\n", pack->cmd, pack->len, (uint64_t)client);
    switch (pack->cmd) {
        case CLIENT_PING:           // ����ͻ��˵�ping
        {
            fprintf(stdout, "client ping, client:%llu\n", (uint64_t)client);
            len = encode(s_send_buff, SERVER_PONG, nullptr, 0);
            sendData((uv_stream_t*)client, s_send_buff, len);
            break;
        }
        case CLIENT_ADD_REQ:       // ����ͻ��˷����һ���ӷ���������
        {
            AddReq req;
            req.ParseFromArray(pack->data, pack->len);
            if (!_OnAdd(client, &req)) {
                goto Exit0;
            }
            break;
        }
        case CLIENT_LOGIN_REQ:
        {
            PlayerLoginReq req;
            req.ParseFromArray(pack->data, pack->len);
            g_playerMgr.player_login(client, &req);
            break;
        }
        case CLIENT_CREATE_REQ:
        {
            PlayerCreateReq req;
            req.ParseFromArray(pack->data, pack->len);
            g_playerMgr.player_create(client, &req);
            g_rankMgr.add_new_player_in_rank(req.playerid());
            break;
        }
        case CLIENT_ANNOUNCE_REQ:
        {
            g_playerMgr.announce_request(client);
            break;
        }
        case CLIENT_GET_SHOP_CONTENT_REQ:
        {
            GetShopContentReq req;
            req.ParseFromArray(pack->data, pack->len);
            g_playerMgr.send_shop_content(client, &req); 
            break;
        }
        case CLIENT_GET_BACKPACK_CONTENT_REQ:
        {
            GetBackPackContentReq req;
            req.ParseFromArray(pack->data, pack->len);
            g_playerMgr.send_backpack_content(client, &req);
            break;
        }
        case CLIENT_BUY_ITEM_REQ:
        {
            BuyItemReq req;
            req.ParseFromArray(pack->data, pack->len);
            g_playerMgr.buy_item(client,&req);
            //g_playerMgr.send_backpack_content(client, &req);
           /* GetBackPackContentReq req;
            req.ParseFromArray(pack->data, pack->len);
            g_playerMgr.send_backpack_content(client, &req);*/
            break;
        }
        case CLIENT_LEVEL_CMD::CLIENT_LEVEL_LIST_REQ:
        {
            LevelListReq req;
            req.ParseFromArray(pack->data, pack->len);
            if (!g_levelMgr.on_level_list_req(client, &req))
            {
                fprintf(stderr,"��Ӧ���ID %s �����б����\n",req.playerid().c_str());
            };
            break;
        }
        case CLIENT_LEVEL_CMD::CLIENT_LEVEL_DETAIL_REQ:
        {
            LevelDetailReq req;
            req.ParseFromArray(pack->data, pack->len);
            if (!g_levelMgr.on_level_detail_req(client, &req))
            {
                fprintf(stderr, "��Ӧ���ID %s �����������\n", req.playerid().c_str());
            }
            break;
        }
        case CLIENT_LEVEL_CMD::CLIENT_LEVEL_DATA_REQ:
        {
            LevelDataReq req;
            req.ParseFromArray(pack->data, pack->len);
            if (!g_levelMgr.on_level_data_req(client, &req))
            {
                fprintf(stderr, "��Ӧ���ID %s �������ݳ���\n", req.playerid().c_str());
            }
            break;
        }
        case CLIENT_LEVEL_CMD::CLIENT_LEVEL_START:
        {
            LevelStart req;
            req.ParseFromArray(pack->data, pack->len);
            if (!g_levelMgr.on_level_start_req(client, &req))
            {
                fprintf(stderr, "���ID %s ����ʼ��Ϸ�ı�������\n", req.playerid().c_str());
            }

            break;
        }
        case CLIENT_LEVEL_CMD::CLIENT_LEVEL_SETTLE_REQ:
        {
            LevelSettleReq req;
            req.ParseFromArray(pack->data, pack->len);
            if (!g_levelMgr.on_level_settle_req(client,&req))
            {
                fprintf(stderr,"���ID %s ������㱨������\n", req.playerid().c_str());
            }
            break;
        }
        case CLIENT_LEVEL_CMD::CLIENT_LEVEL_RANK_REQ:
        {
            RankReq req;
            req.ParseFromArray(pack->data, pack->len);
            if (!g_rankMgr.on_rank_req(client, &req))
            {
                fprintf(stderr, "���ID %s �������а����\n", req.playerid().c_str());
            }
            break;
        }
        case CLIENT_LEVEL_CMD::CLIENT_LEVEL_UPDATE_STATE:
        {
            TCCamp::LevelState levelState;
            levelState.ParseFromArray(pack->data, pack->len);
            g_levelMgr.on_player_level_save(client, &levelState);
            break;
            
        }
        default:
            fprintf(stderr, "invalid cmd:%d\n", pack->cmd);
            return false;
    }
    result = true;
Exit0:
    return result;
}

bool _OnAdd(uv_tcp_t* client, AddReq* req) {
    AddRsp rsp;
    rsp.set_result(req->a() + req->b());
    rsp.set_a(req->a());
    rsp.set_b(req->b());
    printf("client request: %d + %d = %d\n", req->a(), req->b(), rsp.result());
    return SendPBToClient(client, SERVER_ADD_RSP, &rsp);
}


