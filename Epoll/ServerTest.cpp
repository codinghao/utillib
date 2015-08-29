/*
 * RoseDB.cpp
 * 
 *@auther Zhihao Wang <wangzhihaocn@163.com>
 *
 * @copyright 2015
 *
 */

#include "NetServer.h"

int main(int argc, char* argv[])
{
    NetService service;
    NetServer server(service);
    PeerAddr peer("0.0.0.0", 5525);
    server.Start(peer);
    service.Run();

    return 0;
}
