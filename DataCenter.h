#ifndef WET2_DATACENTER_H
#define WET2_DATACENTER_H

#include "Server.h"
#include "AVLRankTree.h"

class dataCenter {
    int dataCenterID;
    AvlRankTree<server,int>* trafficSumTree;

    // assumes the traffic of the server is bigger than zero
    void RemoveServer(const Server server);
public:
    dataCenter(int dataCenterID);
    dataCenter(dataCenter& ds);
    ~dataCenter();
    // assumes there is no node in the tree for this server ID
    TreeStatusType SetTraffic(int serverID, int traffic);
    TreeStatusType UpdateTraffic(int serverID, int dataCenterID, int old_traffic, int new_traffic);
    // assumes the traffic of the server is bigger than zero
    void RemoveServer(int serverID, int traffic);
    TreeStatusType SumHighestTrafficServers(int k, int* traffic);
    // merges the b tree to the a tree, and erase every node in the b tree
    friend TreeStatusType MergeDataCenters (dataCenter a, const dataCenter b);
};
 typedef dataCenter *DataCenter;

#endif //WET2_DATACENTER_H
