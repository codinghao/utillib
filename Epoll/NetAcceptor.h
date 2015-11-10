#ifndef _NET_ACCEPTOR_H_
#define _NET_ACCEPTOR_H_

#include "Net.h"
#include "NetEpoll.h"

class NetAcceptor
{
public:
    NetAcceptor(NetEpoll& service)
        : m_Service(service)
    {
    }

    ~NetAcceptor()
    {
    }

    int Create(PeerAddr& addr, int maxListen)
    {
        m_Event.m_Socket.Create();

        m_Event.m_Socket.SetNonBlock();

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
    NetEpoll& m_Service;
    Event m_Event;
};

#endif
