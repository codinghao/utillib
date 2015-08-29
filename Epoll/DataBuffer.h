#ifndef _DATA_BUFFER_H_
#define _DATA_BUFFER_H_

#include "Compact.h"

#define MEM_UNIT 4*1024
#define MEM_ROUND(size) (((size) % MEM_UNIT + 1) * MEM_UNIT)

struct DataBuffer
{
    DataBuffer(uint size = MEM_UNIT)
	: m_Data(NULL)
	, m_Size(0)
	, m_Length(0)
	, m_Off(0)
    {
	if (size == 0)
	    return ;
	
	m_Data = (char*)malloc(size);
	m_Size = size;
    }

    ~DataBuffer()
    {
	Free();
    }

    uint Remain() { return m_Size - m_Length; }

    void Resize(uint size = MEM_UNIT)
    {
	if (size <= m_Size)
	    return ;

	m_Data = (char*)realloc(m_Data, size);
    }

    void ResetMem()
    {
	if (m_Off >= m_Length)
	{
	    m_Length = 0;
	    m_Off = 0;
	    return ;
	}

	memmove(m_Data, m_Data + m_Off, m_Length - m_Off);
	m_Length  -= m_Off;
	m_Off = 0;
    }

    void Clear()
    {
	m_Length = m_Off = 0;
    }

    void Free()
    {
	if (m_Data != NULL)
	    delete m_Data;

	m_Data = NULL;
	m_Size = m_Length = m_Off = 0;
    }
    
    char* m_Data;
    uint m_Size;
    uint m_Length;
    uint m_Off;
};

#endif
