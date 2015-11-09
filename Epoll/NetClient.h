#ifndef _NET_CLIENT_H_
#define _NET_CLIENT_H_

#include "DataBuffer.h"

#define MAX_CLIENT_REQ_DATA 64*1024*1024

class NetClient
{
public:
    NetClient(NetEpoll& service, NetSocket& socket, PeerAddr& addr)
	: m_PeerAddr(addr)
	, m_Service(service)
    {
	m_Event.m_Socket = socket;
	m_Event.m_Socket.SetNonBlock();
	AddReadEvent();
    }

    ~NetClient()
    {}

    void ProcessBuffer()
    {
	m_ReadBuffer.m_Data[m_ReadBuffer.m_Length] = '\0';
	
	std::cout << m_ReadBuffer.m_Data << std::endl;
    }

    void Send(char* data, uint len)
    {

    }

    void OnRead(Event* ev)
    {
	for(;;)
	{
	    int ret = m_Event.m_Socket.Read(m_ReadBuffer.m_Data, m_ReadBuffer.Remain());
	    if (ret == -1)
	    {
		if (errno == EAGAIN || errno == EINTR)
		    continue;
		
		Close();
		break;
	    }


	    m_ReadBuffer.m_Length += ret;
	    if (m_ReadBuffer.m_Length >= MAX_CLIENT_REQ_DATA)
	    {
		Close();
		break;
	    }

	    if (m_ReadBuffer.Remain() > 0)
	    {
		ProcessBuffer();
		AddReadEvent();
		break;
	    }

	    if (m_ReadBuffer.Remain() == 0)
		m_ReadBuffer.Resize();
	    
	}
    }

    void OnWrite(Event* ev)
    {

    }

    void AddReadEvent()
    {
	m_Event.m_ReadHandle = EventHandle(this, &NetClient::OnRead);
	m_Service.AddEvent(m_Event, EVENT_READABLE);
    }

    void AddWriteEvent()
    {
	m_Event.m_WriteHandle = EventHandle(this, &NetClient::OnWrite);
	m_Service.AddEvent(m_Event, EVENT_WRITEABLE);
    }

    void Close()
    {
	m_Service.DelEvent(m_Event, EVENT_READABLE);
	m_Service.DelEvent(m_Event, EVENT_WRITEABLE);
	m_Event.m_Socket.Close();

	delete this;
    }

private:
    Event m_Event;
    PeerAddr m_PeerAddr;
    NetEpoll& m_Service;
    DataBuffer m_ReadBuffer;
    DataBuffer m_WriteBuffer;
};

#endif
