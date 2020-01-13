#include "Server.h"

server::server(int serverID, int dataCenterID):
        serverID(serverID), dataCenterID(dataCenterID), traffic(0) {}

server::server(server &server) {
    this->serverID = server.serverID;
    this->dataCenterID = server.dataCenterID;
    this->traffic = server.dataCenterID;
}

int server::GetDataCenterID() {
    return this->dataCenterID;
}

int server::GetServerID() {
    return this->serverID;
}

int server::GetTraffic() {
    return this->traffic;
}

int server::SetTraffic(int traffic) {
    int old_traffic = this->traffic;
    this->traffic = traffic;
    return old_traffic;
}

bool server::operator==(const server &a) {
    return this->serverID == a.serverID;
}

bool server::operator!=(const server &a) {
    return !(*this == a);
}

bool operator<(const server& a, const server& b){
    return (a.traffic < b.traffic || (a.traffic == b.traffic && a.serverID < b.serverID));
}

bool operator>(const server& a, const server& b) {
    return (a.traffic > b.traffic || (a.traffic == b.traffic && a.serverID > b.serverID));
}
