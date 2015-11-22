#ifndef _NET_CONNECTOR_H_
#define _NET_CONNECTOR_H_

#include "NetService.h"

class NetConnector;
typedef Delegate<void (NetConnector*)> ConnectorHandle;

class NetConnector
{
public:
    NetConnector(NetService& service)
	: m_Service(service)
    {
	m_Event.m_Socket.Create();
	m_Event.m_Socket.SetNonBlock();
    }

    ~NetConnector()
    {
	Close();
    }

    void Connect(PeerAddr& peer)
    {
	if(m_Event.m_Socket.Connect(peer) == -1)
	{
	    if (errno != EINPROGRESS)
		OnConnectFaild();
	}

	m_Event.m_WriteHandle = EventHandle(this, &NetConnector::OnConnected);
	m_Service.AddEvent(m_Event, EVENT_WRITEABLE);
    }

    void OnConnected(SocketEvent* ev)
    {
	if (m_Event.m_Socket.GetSockErr())
	{
	    OnConnectFaild();
	}
	else
	{
	    m_Service.DelEvent(m_Event, EVENT_WRITEABLE);
	    m_OnConnectedHandle(this);
	}
    }

    void OnConnectFaild()
    {
	Close();
	m_OnConnectFaildHandle(this);
    }

    void Close()
    {
	m_Event.m_Socket.Close();
    }

    void BindConnected(ConnectorHandle& handle) { m_OnConnectedHandle = handle; }
    void BindConnectFaild(ConnectorHandle& handle) { m_OnConnectFaildHandle = handle; }

    NetSocket& GetSocket() { return m_Event.m_Socket; }

private:
    SocketEvent m_Event;
    NetService& m_Service;
    ConnectorHandle m_OnConnectedHandle;
    ConnectorHandle m_OnConnectFaildHandle;
};

#endif
