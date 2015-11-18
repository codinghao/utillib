#ifndef _NET_CLIENT_H_
#define _NET_CLIENT_H_

#include "NetBuffer.h"

#define MAX_CLIENT_REQ_DATA 64*1024*1024

class NetSession;
typedef Delegate<uint (NetSession*, char*, uint)> SessionHandle;

class NetSession
{
public:
    NetSession(NetService& service, NetSocket& socket, PeerAddr& addr)
        : m_PeerAddr(addr)
        , m_Service(service)
    {
        m_Event.m_Socket = socket;
        m_Event.m_Socket.SetNonBlock();
        m_Event.m_ReadHandle = EventHandle(this, &NetSession::OnRead);
        m_Event.m_WriteHandle = EventHandle(this, &NetSession::OnWrite);

        AddReadEvent();
    }

    ~NetSession()
    {}

    void Send(char* data, uint len)
    {
        if (m_WriteBuffer.Empty())
        {
            int writeLen = Write(data, len);
            if (writeLen == -1 || (uint)writeLen == len)
                return ;

            len -= writeLen;
        }

        m_WriteBuffer.Write(data, len);
        AddWriteEvent();
    }

    int Write(char* data, uint len)
    {
        for (;;)
        {
            int writeLen = m_Event.m_Socket.Write(data, len);
            if (writeLen == -1)
            {
                if (errno == EAGAIN || errno == EINTR)
                    continue;

                OnConnectBroken();
                break;
            }

            return writeLen;
        }

        return -1;
    }

    void OnRead(Event* ev)
    {
        for(;;)
        {
            int readLen = m_Event.m_Socket.Read(m_ReadBuffer.m_Data, m_ReadBuffer.Remain());
            if (readLen == -1)
            {
                if (errno == EAGAIN || errno == EINTR)
                    continue;

                OnConnectBroken();
                break;
            }

            m_ReadBuffer.m_Length += readLen;
            if (readLen == 0 || m_ReadBuffer.m_Length >= MAX_CLIENT_REQ_DATA)
            {
                OnConnectBroken();
                break;
            }

            if (m_ReadBuffer.Remain() > 0)
            {
                m_ReadBuffer.m_Off += this->m_OnRead(this, m_ReadBuffer.m_Data, m_ReadBuffer.m_Length);
                m_ReadBuffer.ResetMem();
                AddReadEvent();
                break;
            }

            if (m_ReadBuffer.Remain() == 0)
                m_ReadBuffer.Resize();

        }
    }

    void OnWrite(Event* ev)
    {
        ReadBufferHandle handle = ReadBufferHandle(this, &NetSession::Write);
        m_WriteBuffer.Read(&handle);

        if (!m_WriteBuffer.Empty())
            AddWriteEvent();

        this->m_OnWrite(this, NULL, 0);
    }

    void AddReadEvent()
    {
        m_Service.AddEvent(m_Event, EVENT_READABLE);
    }

    void AddWriteEvent()
    {
        m_Service.AddEvent(m_Event, EVENT_WRITEABLE);
    }

    void OnConnectBroken()
    {
        Close();
        this->m_OnConnectBroken(this, NULL, 0);
    }

    void Close()
    {
        m_Service.DelEvent(m_Event, EVENT_READABLE);
        m_Service.DelEvent(m_Event, EVENT_WRITEABLE);
        m_Event.m_Socket.Close();
    }

    void BindConnectBroken(SessionHandle& handle) { m_OnConnectBroken = handle; }
    void BindOnRead(SessionHandle& handle) { m_OnRead = handle; }
    void BindOnWrite(SessionHandle& handle) { m_OnWrite = handle; }

private:
    Event m_Event;
    PeerAddr m_PeerAddr;
    NetService& m_Service;
    ReadBuffer m_ReadBuffer;
    WriteBuffer m_WriteBuffer;
    SessionHandle m_OnConnectBroken;
    SessionHandle m_OnRead;
    SessionHandle m_OnWrite;
};

#endif
