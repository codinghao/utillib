#include "NetConnector.h"

int main(int argc, char* argv[])
{
    NetService service;
    NetConnector connector(service);
    PeerAddr addr = PeerAddr("127.0.0.1", 5525);
    connector.Connect(addr);
    service.Run();
    return 0;
}
