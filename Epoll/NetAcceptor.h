#ifndef _NET_ACCEPTOR_H_
#define _NET_ACCEPTOR_H_

#include "NetService.h"

class NetAcceptor
{
public:
    NetAcceptor(NetService& service)
        : m_Service(service)
    {
    }

    ~NetAcceptor()
    {
	m_Event.m_Socket.Close();
    }

    int Create(PeerAddr& addr, int maxListen)
    {
        m_Event.m_Socket.Create();

        m_Event.m_Socket.SetNonBlock();
        m_Event.m_Socket.SetReuseAddr();

        if (bind(m_Event.m_Socket.Native(), addr.ToSockaddr(), sizeof(addr)) == -1)
        {
            return -1;
        }

        if (listen(m_Event.m_Socket.Native(), maxListen) == -1)
        {
            return -1;
        }

        return 0;
    }

    void AddAcceptEvent(EventHandle&  acceptHandle)
    {
        m_Event.m_ReadHandle = acceptHandle;

        m_Service.AddEvent(m_Event, EVENT_READABLE);
    }

    int Accept(NetSocket& socket, PeerAddr& addr)
    {
        socklen_t addrLen = 0;
        socket.SetNative(accept(m_Event.m_Socket.Native(), addr.ToSockaddr(), &addrLen));
        if(socket.Invalid())
        {
            return -1;
        }

        return 0;
    }

private:
    NetService& m_Service;
    Event m_Event;
};

#endif
