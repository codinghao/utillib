#include "NetConnector.h"
#include "NetSession.h"

class Client
{
public:
    Client(NetService& service)
        : m_Service(service)
        , m_Connector(service)
    {

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
        NetSession* session = new NetSession(m_Service, connector->GetSocket(), addr);
        SessionHandle readHandle = SessionHandle(this, &Client::OnRead);
        session->BindOnRead(readHandle);
        SessionHandle brokenHandle = SessionHandle(this, &Client::OnConnectBroken);
        session->BindConnectBroken(brokenHandle);
        char* buf = "01234567890123456789012345678901234567890123456789\r\n"; 
        session->Send(buf, strlen(buf));

        std::cout << "connection success!" << std::endl;
    }

    void OnConnectFaild(NetConnector* connector)
    {

    }

    uint OnRead(NetSession* session, char* data, uint len)
    {
        std::cout << std::string(data, len) << std::endl;
        session->Send(data, len);
        return len;
    }

    uint OnConnectBroken(NetSession* session, char* data, uint len)
    {
        delete session;
        return len;
    }

private:
    NetService& m_Service;
    NetConnector m_Connector;
};


int main(int argc, char* argv[])
{
    NetService service;
    Client client(service);
    PeerAddr addr = PeerAddr("127.0.0.1", 5525);
    client.Connect(addr);
    service.Run();
    return 0;
}
