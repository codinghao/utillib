#ifndef _NET_SERVICE_H_
#define _NET_SERVICE_H_

#include "Net.h"
#include "NetEpoll.h"

class NetService
{
public:
    NetService()
    {
	m_Service.Create();
    }

    ~NetService()
    {
	Exit();
    }

    void AddEvent(Event& ev, int mask)
    {
	m_Service.AddEvent(ev, mask);
    }

    void DelEvent(Event& ev, int mask)
    {
	m_Service.DelEvent(ev, mask);
    }

    void Run()
    {
	m_Service.ProcessEvent();
    }

    void Exit()
    {
	m_Service.Close();
    }
private:
    NetEpoll m_Service;
};


#endif
