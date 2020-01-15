#ifndef WET2_DS_DCManager_H
#define WET2_DS_DCManager_H

#include "UnionFind.h"
#include "AVLRankTree.h"
#include "DataCenter.h"
#include "ServerHashTable.h"

class DCManager {
    const int numOfDataCenters;
    UnionFind groups;
    ServerHashTable h_table;
    AvlRankTree <server,int> *servers_tree;
    DataCenter* data_centers;
public:
    DCManager(int n);
    ~DCManager();
    TreeStatusType mergeDC(int dc1,int dc2);
    TreeStatusType addServer(int data_center_id,int server_id);
    TreeStatusType removeServer(int server_id);
    TreeStatusType setTraffic(int server_id,int traffic);
    TreeStatusType sumHighestTraffic(int data_cenetr_id, int k,int *traffic_sum);
};


#endif //WET2_DS_DCManager_H
