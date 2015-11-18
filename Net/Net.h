#ifndef _NET_H_
#define _NET_H_

#include "Compact.h"
#include "PeerAddr.h"
#include "NetSocket.h"
#include "Delegate.h"

#define EVENT_NONE 0
#define EVENT_READABLE 1
#define EVENT_WRITEABLE 2

struct Event;
typedef Delegate<void (Event*)> EventHandle;

struct Event
{
    Event()
	: m_Mask(0)
    {}
    int m_Mask;
    NetSocket m_Socket;
    EventHandle m_ReadHandle;
    EventHandle m_WriteHandle;
};

#endif
