#ifndef _NET_EPOLL_H_
#define _NET_EPOLL_H_

#include "Net.h"

#define MAX_EVENTS 1024

class NetEpoll
{
public:
    NetEpoll()
        : m_EpollFd(-1)
        , m_EventNum(MAX_EVENTS)
        , m_EpollEvent(NULL)
    {}
    ~NetEpoll()
    {
        Close();
    }

    void Create()
    {
        if ((m_EpollFd = epoll_create(MAX_EVENTS)) == -1)
        {
            fprintf(stderr, "epoll create error, err : %d", errno);
            return ;
        }

        m_EpollEvent = (epoll_event*)malloc(m_EventNum*sizeof(epoll_event));
    }

    void Close()
    {
        if (m_EpollFd != -1)
            close(m_EpollFd);
        m_EpollFd = -1;
    }

    void AddEvent(Event& ev, int mask)
    {
        epoll_event epollEvent = {0, {&ev}};

        int op = ev.m_Mask == EVENT_NONE ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;

        mask |= ev.m_Mask;
        ev.m_Mask = mask;

        if (mask & EVENT_READABLE) epollEvent.events |= EPOLLIN;
        if (mask & EVENT_WRITEABLE) epollEvent.events |= EPOLLOUT;

        epoll_ctl(m_EpollFd, op, ev.m_Socket.Native(), &epollEvent);
    }

    void DelEvent(Event& ev, int mask)
    {
        epoll_event epollEvent = {0, {&ev}};

        mask = ev.m_Mask & (~mask);
        ev.m_Mask = mask;

        if (mask & EVENT_READABLE) epollEvent.events |= EPOLLIN;
        if (mask & EVENT_WRITEABLE) epollEvent.events |= EPOLLOUT;

        if (mask == EVENT_NONE)
            epoll_ctl(m_EpollFd, EPOLL_CTL_DEL, ev.m_Socket.Native(), &epollEvent);
        else
            epoll_ctl(m_EpollFd, EPOLL_CTL_MOD, ev.m_Socket.Native(), &epollEvent);
    }

    int ProcessEvent()
    {
        for(;;)
        {
            int evNum = epoll_wait(m_EpollFd, m_EpollEvent, m_EventNum, 500);
            if (evNum < 0)
            {
                return 0;
            }

            for (int i = 0; i < evNum; ++ i)
            {
                Event* ev = (Event*)m_EpollEvent[i].data.ptr;
                int mask = EVENT_NONE;

                if (m_EpollEvent[i].events & EPOLLIN)  mask |= EVENT_READABLE;
                if (m_EpollEvent[i].events & EPOLLOUT) mask |= EVENT_WRITEABLE;
                if (m_EpollEvent[i].events & EPOLLERR) mask |= EVENT_WRITEABLE;
                if (m_EpollEvent[i].events & EPOLLHUP) mask |= EVENT_WRITEABLE;

                if (mask & EVENT_READABLE & ev->m_Mask)
                {
                    ev->m_ReadHandle(ev);
                }
                if (mask & EVENT_WRITEABLE & ev->m_Mask)
                {
                    ev->m_WriteHandle(ev);
                }
            }
        }
    }
private:
    int m_EpollFd;
    int m_EventNum;    
    epoll_event* m_EpollEvent;
};

#endif
