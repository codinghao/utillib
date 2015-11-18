/*
 * RoseDB.cpp
 * 
 *@auther Zhihao Wang <wangzhihaocn@163.com>
 *
 * @copyright 2015
 *
 */

#include "NetServer.h"


class Server
{
public:
    Server(NetService& service)
        : m_Server(service)
        , m_Service(service)
    {
        ServerHandle handle = ServerHandle(this, &Server::OnAccepted);
        m_Server.BindAccepted(handle);
    }

    void OnAccepted(NetServer* server, NetSocket& sock, PeerAddr& addr)
    {
        NetSession *session = new NetSession(m_Service, sock, addr);
        SessionHandle readHandle = SessionHandle(this, &Server::OnRead);
        session->BindOnRead(readHandle);
        SessionHandle brokenHandle = SessionHandle(this, &Server::OnConnectBroken);
        session->BindConnectBroken(brokenHandle);
    }

    uint OnRead(NetSession* session, char* data, uint len)
    {
        session->Send(data, len);
        return len;
    }

    uint OnConnectBroken(NetSession* session, char* data, uint len)
    {
        delete session;
        return 0;
    }

    void Start()
    {
        PeerAddr peer("0.0.0.0", 5525);
        m_Server.Start(peer);
    }

private:
    NetServer m_Server;
    NetService& m_Service;
};


int main(int argc, char* argv[])
{
    NetService service;
    Server server(service);
    server.Start();
    service.Run();
    return 0;
}
