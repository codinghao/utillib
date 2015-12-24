#ifndef _NET_SOCKET_H_
#define _NET_SOCKET_H_

#include "Compact.h"

#define INVALID_SOCKET -1

class NetSocket
{
public:
    NetSocket() 
        : m_Handle(INVALID_SOCKET)
    {
    }

    ~NetSocket()
    {

    }

    void Create()
    {
        if ((m_Handle = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) 
            fprintf(stderr, "Create socket faild!, err:%s", strerror(errno));
    }

    int Read(char* data, uint len)
    {
        return read(m_Handle, data, len);
    }

    int Write(char* data, uint len)
    {
        return write(m_Handle, data, len);
    }

    int Connect(PeerAddr& peer)
    {
        return connect(m_Handle, peer.ToSockaddr(), sizeof(peer));
    }

    int GetSockErr()
    {
	int sockErr = 0;
	socklen_t errLen = sizeof(sockErr);
	if (getsockopt(m_Handle, SOL_SOCKET, SO_ERROR, &sockErr, &errLen) == -1)
	    sockErr = errno;

	return sockErr;
    }

    void ShutDown()
    {
        if (!Invalid())
            shutdown(m_Handle);
    }

    void Close()
    {
        if (!Invalid())
            close(m_Handle);

        m_Handle = INVALID_SOCKET;
    }

    int Native() { return m_Handle; }
    int SetNative(int handle) { return m_Handle = handle; }
    bool Invalid() { return m_Handle == INVALID_SOCKET; }

    void SetNonBlock()
    {
        int opt = fcntl(m_Handle, F_GETFL);
        if (opt != -1)
        {
            opt |= O_NONBLOCK;
            fcntl(m_Handle, F_SETFL, opt);
        }
    }

    bool SetReuseAddr()
    {
	int opt = 1;
	if (setsockopt(m_Handle, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	    return false;

	return true;
    }
private:
    int m_Handle;
};

#endif
