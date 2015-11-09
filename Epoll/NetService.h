#ifndef _NET_SERVICE_H_
#define _NET_SERVICE_H_

#include "Net.h"
#include "NetAcceptor.h"
#include "NetClient.h"

class NetService
{
public:
    NetService()
	: m_Acceptor(m_Service)
    {
    }

    ~NetService()
    {

    }

    void Start(PeerAddr& addr)
    {
	m_Service.Create();
	m_Acceptor.Create(addr, 1000);
    	EventHandle eventHandle = EventHandle(this, &NetService::OnAccept);
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
		std::cout << "accept error, err : " << strerror(errno) << std::endl;
		return ;
	    }
	}
    	
	NetClient* cli = new NetClient(m_Service, sock, addr);

	EventHandle eventHandle = EventHandle(this, &NetService::OnAccept);
    	m_Acceptor.AddAcceptEvent(eventHandle);
    }

    void Run()
    {
	m_Service.ProcessEvent();
    }

    void Exit()
    {
    }
private:
    NetEpoll m_Service;
    NetAcceptor m_Acceptor;

    std::vector<NetClient*> m_ClientList;

};


#endif
