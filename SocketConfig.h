#pragma once

//服务器ip和端口
const char* const SERVER_IP = "127.0.0.1";
constexpr unsigned int SOCKET_PORT = 6666;

//包头大小（存储消息体的大小(不含消息体类型id)）
constexpr int MSG_HEAD_SIZE = 4;
//消息体id大小(枚举类型 4字节)
constexpr int MSG_TYPE_SIZE = 4;
//整个包头大小
constexpr int MSG_TOTAL_HEAD_SIZE = MSG_HEAD_SIZE + MSG_TYPE_SIZE;
//接收和发送缓冲大小
constexpr int BUFFER_SIZE = 1024;
//游戏外接受消息周期 ms
constexpr int OUT_GAME_MSG_TICK = 200;
