#ifndef _NET_SERVICE_H_
#define _NET_SERVICE_H_

#include "Net.h"
#include "NetEpoll.h"
#include "NetTimer.h"

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

    void AddEvent(SocketEvent& ev, int mask)
    {
        m_Service.AddEvent(ev, mask);
    }

    void AddEvent(TimerEvent& ev, int timeout)
    {
	m_Timer.AddTimer(ev, timeout);
    }

    void DelEvent(SocketEvent& ev, int mask)
    {
        m_Service.DelEvent(ev, mask);
    }

    void DelEvent(TimerEvent& ev)
    {
	m_Timer.DelTimer(ev);
    }

    void Run()
    {
	for(;;)
	{
	    int timeout = m_Timer.GetNearestTime();
	    if(m_Service.EpollOnce(timeout) < 0)
		break;

	    m_Timer.ProcessTimer();
	}
    }

    void Exit()
    {
        m_Service.Close();
    }
private:
    NetEpoll m_Service;
    NetTimer m_Timer;
};

#endif
