#ifndef WET2_SERVER_H
#define WET2_SERVER_H

class server {
    int serverID;
    int dataCenterID;
    int traffic;

public:

    server(int serverID, int dataCenterID);
    ~server() = default;
    server(server& server);

    int GetDataCenterID();
    int GetServerID();
    int GetTraffic();

    // returns old traffic value
    int SetTraffic(int traffic);

    bool operator==(const server& a);
    bool operator!=(const server& a);

    friend bool operator<(const server& a, const server& b);
    friend bool operator>(const server& a, const server& b);
};

typedef server *Server;

#endif //WET2_SERVER_H
