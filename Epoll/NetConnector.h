#ifndef _NET_CONNECTOR_H_
#define _NET_CONNECTOR_H_

#include "NetService.h"

class NetConnector
{
public:
    NetConnector(NetService& service)
        : m_Service(service)
    {
        m_Event.m_Socket.Create();
        m_Event.m_Socket.SetNonBlock();
        m_Event.m_WriteHandle = EventHandle(this, &NetConnector::OnConnected);
    }

    ~NetConnector()
    {

    }

    void Connect(PeerAddr& peer)
    {
        if(m_Event.m_Socket.Connect(peer) == -1)
	{
	    if (errno != EINPROGRESS)
	    {
		std::cout << "Connect fail!" << std::endl;
	    }	
	}

        m_Service.AddEvent(m_Event, EVENT_WRITEABLE);
    }

    void OnConnected(Event* ev)
    {
	if (m_Event.m_Socket.GetSockErr())
	{
	    std::cout << strerror(errno);
	    m_Service.DelEvent(m_Event, EVENT_READABLE|EVENT_WRITEABLE);
	    std::cout << "Connect fail!" << std::endl;
	}
	else
	{
	    std::cout << "Connect success!" << std::endl;
	}
    }

private:
    Event m_Event;
    NetService& m_Service;
};

#endif
