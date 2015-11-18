#ifndef _NET_SERVER_H_
#define _NET_SERVER_H_

#include "Net.h"
#include "NetAcceptor.h"
#include "NetSession.h"

class NetServer;
typedef Delegate<void (NetServer*, NetSocket&, PeerAddr&)> ServerHandle;

class NetServer
{
public:
    NetServer(NetService& service)
        : m_Service(service)
        , m_Acceptor(service)
    {
    }

    ~NetServer()
    {
        Exit();
    }

    void Start(PeerAddr& addr)
    {
        m_Acceptor.Create(addr, 1000);
        EventHandle eventHandle = EventHandle(this, &NetServer::OnAccept);
        m_Acceptor.AddAcceptEvent(eventHandle);
    }

    void OnAccept(Event* ev)
    {
        NetSocket sock;
        PeerAddr addr;
        int ret = m_Acceptor.Accept(sock, addr);
        if (ret == -1)
        {
            if (errno != EAGAIN && errno != EINTR)
            {
                Exit();
                return ;
            }
        }

        EventHandle eventHandle = EventHandle(this, &NetServer::OnAccept);

        m_OnAccepted(this, sock, addr);
        m_Acceptor.AddAcceptEvent(eventHandle);
    }

    void Exit()
    {
        m_Service.Exit();
    }

    void BindAccepted(ServerHandle& handle) { m_OnAccepted = handle; }

private:
    NetService& m_Service;
    NetAcceptor m_Acceptor;
    ServerHandle m_OnAccepted;
};


#endif
