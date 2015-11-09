#ifndef _NET_CLIENT_H_
#define _NET_CLIENT_H_

#include "DataBuffer.h"
#include "WriteBuffer.h"

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
        Send(m_ReadBuffer.m_Data, m_ReadBuffer.m_Length);
    }

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

                Close();
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

                Close();
                break;
            }

            m_ReadBuffer.m_Length += readLen;
            if (readLen == 0 || m_ReadBuffer.m_Length >= MAX_CLIENT_REQ_DATA)
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
        ReadBufferHandle handle = ReadBufferHandle(this, &NetClient::Write);
        m_WriteBuffer.Read(&handle);

        if (!m_WriteBuffer.Empty())
            AddWriteEvent();
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
    }

private:
    Event m_Event;
    PeerAddr m_PeerAddr;
    NetEpoll& m_Service;
    DataBuffer m_ReadBuffer;
    WriteBuffer m_WriteBuffer;
};

#endif
