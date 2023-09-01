#pragma once
#include "uv.h"
#include "../utils/utils.h"
#include "../codec/codec.h"
#include "../proto/player.pb.h"
#include "../proto/level.pb.h"

using namespace TCCamp;


bool SendPBToClient(uv_tcp_t* client, uint16_t cmd, ::google::protobuf::Message* msg);
bool OnNewClient(uv_tcp_t* client);
bool OnCloseClient(uv_tcp_t* client);
bool OnRecv(uv_tcp_t* client, const char* data, int len);

bool _OnPackHandle(uv_tcp_t* client, Packet* pack);
bool _OnAdd(uv_tcp_t* client, AddReq* req);