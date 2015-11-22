#ifndef _NET_TIMER_H_
#define _NET_TIMER_H_

#define DEFAULT_TIMEOUT 500

class NetTimer
{
public:
    typedef std::multimap<longlong, TimerEvent> TimerEventMap;

    NetTimer()
	: m_TimerId(0)
    {}

    void AddTimer(TimerEvent& ev, longlong timeout)
    {
	ev.m_When = Now() + timeout;
	ev.m_Id = GetTimerId();
	m_TimerMap.insert(std::make_pair(ev.m_When, ev));
    }

    void DelTimer(TimerEvent& ev)
    {
	TimerEventMap::iterator it = m_TimerMap.find(ev.m_When);
	for (; it != m_TimerMap.end(); ++ it)
	{
	    if (it->first != ev.m_When)
		break ;

	    if (it->second.m_Id != ev.m_Id)
		continue;

	    m_TimerMap.erase(it);
	    break;
	}
    }

    int GetNearestTime()
    {
	if (m_TimerMap.empty())
	    return DEFAULT_TIMEOUT;

	TimerEventMap::iterator it = m_TimerMap.begin();
	longlong now = Now();
	if (it->first < now)
	    return 0;

	return (int)(it->first - now);
    }

    void ProcessTimer()
    {
	longlong now = Now();

	while(!m_TimerMap.empty())
	{
	    TimerEventMap::iterator it = m_TimerMap.begin();
	    if (it->first > now)
		break;

	    it->second.m_Handle(&(it->second));
	    m_TimerMap.erase(it);
	}
    }
private:
    ulonglong GetTimerId() { return m_TimerId++; }

private:
    TimerEventMap m_TimerMap;
    ulonglong m_TimerId;
};

#endif
