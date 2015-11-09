/*
 * RoseDB.cpp
 * 
 *@auther Zhihao Wang <wangzhihaocn@163.com>
 *
 * @copyright 2015
 *
 */

#include "RoseDB.h"

int main(int argc, char* argv[])
{
    NetService srv;
    PeerAddr peer("0.0.0.0", 5525);
    srv.Start(peer);
    srv.Run();

    return 0;
}
