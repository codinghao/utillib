#ifndef _NET_CONNECTOR_H_
#define _NET_CONNECTOR_H_

class NetConnector
{
public:
    NetConnector(NetEpoll& service)
        : m_Service(service)
    {
        m_Event.Socket.Create();
        m_Event.Socket.SetNonBlock();
        m_Event.ReadHandle = EventHandle(this, NetConnector::OnConnected);
        m_Event.WriteHandle = EventHandle(this, NetConnector::OnConnected);
    }

    ~NetConnector()
    {

    }

    void Connect(PeerAddr& peer)
    {
        m_Event.Socket.Connect(PeerAddr& peer);
        m_Service.AddEvent(m_Event);
    }

    void OnConnected(Event& ev)
    {
        if ()
    }

private:
    Event m_Event;
    NetEpoll& m_Service;
};


#endif
