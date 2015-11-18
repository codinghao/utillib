#ifndef _PEER_ADDR_H_
#define _PEER_ADDR_H_

#include "Compact.h"

#define PEER_ADDR_SIZE sizeof(sockaddr_in)

class PeerAddr
{
public:
    PeerAddr()
    {

    }
    PeerAddr(const char* _ip, ushort _port, ushort _family = AF_INET)
    {
        m_Addr.sin_family = _family;
        m_Addr.sin_addr.s_addr = ::inet_addr(_ip);
        m_Addr.sin_port = ::htons(_port);
    }

    PeerAddr(const struct sockaddr_in& _sockAddr)
    {
        m_Addr = _sockAddr;
    }

    sockaddr* ToSockaddr()
    {
        return (sockaddr*)&m_Addr;
    }

    const char* Ip() 
    {
        return inet_ntoa(m_Addr.sin_addr);
    }

    const ushort Port()
    {
        return ::ntohs(m_Addr.sin_port);
    }
private:
    struct sockaddr_in m_Addr;
};

#endif
