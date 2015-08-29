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
        m_Event.m_ReadHandle = EventHandle(this, &NetConnector::OnConnected);
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
		Close();
	    }	
	}

        m_Service.AddEvent(m_Event, EVENT_WRITEABLE);
    }

    void OnConnected(Event* ev)
    {
	if (m_Event.m_Socket.GetSockErr())
	{
	    Close();
	}
	else
	{
	    std::cout << "Connect success!" << std::endl;
	}
    }

    void Close()
    {
	m_Event.m_Socket.Close();
    }

private:
    Event m_Event;
    NetService& m_Service;
};

#endif
