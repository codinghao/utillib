#ifndef _NET_H_
#define _NET_H_

#include "Compact.h"
#include "PeerAddr.h"
#include "NetSocket.h"
#include "Delegate.h"

#define EVENT_NONE      0
#define EVENT_READABLE  1
#define EVENT_WRITEABLE 2

struct SocketEvent;
typedef Delegate<void (SocketEvent*)> EventHandle;

struct SocketEvent
{
    SocketEvent()
	: m_Mask(0)
    {}
    int m_Mask;
    NetSocket m_Socket;

    EventHandle m_ReadHandle;
    EventHandle m_WriteHandle;
};

struct TimerEvent;
typedef Delegate<void (TimerEvent*)> TimerHandle;
struct TimerEvent
{
    TimerEvent()
       : m_When(0)
       , m_Id(0)
       , m_Ptr(NULL)
    {}
    longlong m_When;
    ulonglong m_Id;
    void* m_Ptr;
    TimerHandle m_Handle;
};

inline longlong Now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    return (longlong)(tv.tv_sec * 1000 + tv.tv_usec /1000);
}

inline long NowToSec()
{
    return (long)(Now() / 1000);
}

#endif
