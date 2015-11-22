#include "NetConnector.h"
#include "NetSession.h"

class Client
{
public:
    Client(NetService& service)
        : m_Service(service)
        , m_Connector(service)
	, m_Session(NULL)
    {
	TimerEvent ev;
	ev.m_Handle = TimerHandle(this, &Client::OnTimer);
	m_Service.AddEvent(ev, 1000);
    }

    ~Client()
    {
	if (m_Session != NULL)
	    delete m_Session;
    }

    void OnTimer(TimerEvent* ev)
    {
	const char* buf = "01234567890123456789012345678901234567890123456789\r\n"; 
        m_Session->Send(buf, strlen(buf));

	m_Service.AddEvent(*ev, 1000);
    }

    void Connect(PeerAddr& addr)
    {
        ConnectorHandle connectHandle(this, &Client::OnConnected);
        m_Connector.BindConnected(connectHandle);
        ConnectorHandle faildHandle(this, &Client::OnConnectFaild);
        m_Connector.BindConnectFaild(faildHandle);
        m_Connector.Connect(addr);
    }

    void OnConnected(NetConnector* connector)
    {
        PeerAddr addr("", 0);
        m_Session = new NetSession(m_Service, connector->GetSocket(), addr);
        SessionHandle readHandle = SessionHandle(this, &Client::OnRead);
        m_Session->BindOnRead(readHandle);
        SessionHandle brokenHandle = SessionHandle(this, &Client::OnConnectBroken);
        m_Session->BindConnectBroken(brokenHandle);
        
    }

    void OnConnectFaild(NetConnector* connector)
    {

    }

    uint OnRead(NetSession* session, char* data, uint len)
    {
        std::cout << std::string(data, len) << std::endl;
        return len;
    }

    uint OnConnectBroken(NetSession* session, char* data, uint len)
    {
        delete m_Session;
	m_Session = NULL;
        return len;
    }

private:
    NetService& m_Service;
    NetConnector m_Connector;
    NetSession* m_Session;
};


int main(int argc, char* argv[])
{
    NetService service;
    //for (int i = 0; i < 100000; ++i)
    //{
	Client *client = new Client(service);
    	PeerAddr addr = PeerAddr("127.0.0.1", 5525);
    	client->Connect(addr);
    //}
    service.Run();
    return 0;
}
