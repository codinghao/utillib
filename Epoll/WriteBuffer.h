#ifndef _WRITE_BUFFER_H_
#define _WRITE_BUFFER_H_

#include "Delegate.h"

#define WRITE_BUFFER_UNIT 4*1024

typedef Delegate<int (char*, uint len)> ReadBufferHandle;

class WriteBuffer
{
public:
    struct Buffer
    {
        Buffer() 
            : m_Data(new char[WRITE_BUFFER_UNIT])
            , m_Size(WRITE_BUFFER_UNIT)
        {}

        ~Buffer()
        {
            if (m_Data != NULL)
                delete [] m_Data;
        }

        uint LeftBuffSize() { return m_Size - m_Len; }
        uint LeftDataLen() { return m_Len - m_Off; }

        void Write(const char* data, uint len)
        {
            if (len > LeftBuffSize())
                return;

            memcpy(m_Data, data, len);
            m_Len += len;
        }

        char* m_Data;
        uint m_Size;
        uint m_Len;
        uint m_Off;
    };

    WriteBuffer() 
    {
    }

    ~WriteBuffer()
    {
        for (std::deque<Buffer*>::iterator it = m_BufferList.begin(); it != m_BufferList.end(); ++ it)
            delete *it;
    }

    void Write(const char* data, uint len)
    {
        Buffer* buffer = NULL;
        
        if (m_BufferList.empty())
            buffer = m_BufferList.front();
        else
            m_BufferList.push_back(new Buffer());

        uint left = buffer->LeftBuffSize();
        if (left > len)
        {
            buffer->Write(data, len);
        }
        else
        {
            buffer->Write(data, left);
            Write(data, len - left);
        }
    }

    bool Empty() { return m_BufferList.empty(); }

    void Read(ReadBufferHandle* readHandle)
    {
        while(!m_BufferList.empty())
        {
            Buffer* buffer = m_BufferList.front();
            if (buffer->LeftDataLen() > 0)
            {
                int readLen = (*readHandle)(buffer->m_Data + buffer->m_Off, buffer->LeftDataLen());
                if (readLen <= 0)
                    break;

                buffer->m_Off += readLen;
                if (buffer->LeftDataLen() > 0)
                    break;
            }

            m_BufferList.pop_front();
            delete buffer;
        }
    }

private:
    std::deque<Buffer*> m_BufferList;
};

#endif
