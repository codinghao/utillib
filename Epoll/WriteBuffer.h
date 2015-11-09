#ifndef _WRITE_BUFFER_H_
#define _WRITE_BUFFER_H_

#define WRITE_BUFFER_UNIT 4*1024

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

        uint Remain() { return m_Size - m_Len; }

        char* m_Data;
        uint m_Size;
        uint m_Len;
        uint m_Off;
    };

    WriteBuffer() 
        : m_Tail(NULL)
    {
        m_Tail = new Buffer();
        m_BufferList.push_back(m_Tail);
    }

    ~WriteBuffer()
    {
        for (std::deque<Buffer*>::iterator it = m_BufferList.begin(); it != m_BufferList.end(); ++ it)
            delete *it;
    }

    void Write(const char* data, uint len)
    {
        assert(m_Tail != NULL);
    }

private:
    Buffer* m_Tail;
    std::deque<Buffer*> m_BufferList;
};

#endif
