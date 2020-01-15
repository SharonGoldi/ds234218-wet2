#include "DataCenter.h"

dataCenter::dataCenter(int dataCenterID): dataCenterID(dataCenterID) {
    this->trafficSumTree = new AvlRankTree<server,int>();
}

dataCenter::dataCenter(dataCenter &ds): dataCenterID(ds.dataCenterID) {
    AvlRankTree<server, int>* tree = ds.trafficSumTree;
    this->trafficSumTree = tree;
}

dataCenter::~dataCenter() {
    delete this->trafficSumTree;
}

void dataCenter::RemoveServer(const Server server) {
    this->trafficSumTree->Delete(*server);
}

void dataCenter::RemoveServer(const int serverID, const int traffic) {
    Server temp = new server(serverID,0);
    temp->SetTraffic(traffic);
    this->RemoveServer(temp);
    delete temp;
}

TreeStatusType dataCenter::SetTraffic(int serverID, int traffic) {
    Server s = new server(serverID, this->dataCenterID);
    s->SetTraffic(traffic);
    TreeStatusType status = this->trafficSumTree->Add(*s,traffic);
    delete s;
    return status;
}

TreeStatusType dataCenter::UpdateTraffic(int serverID, int dataCenterID, int old_traffic, int new_traffic){
    Server old_server_node = new server(serverID, dataCenterID);
    old_server_node->SetTraffic(old_traffic);
    this->trafficSumTree->Delete(*old_server_node);
    old_server_node->SetTraffic(new_traffic);
    TreeStatusType status = this->trafficSumTree->Add(*old_server_node, new_traffic);
    delete old_server_node;
    return status;
}

TreeStatusType dataCenter::SumHighestTrafficServers(int k, int *traffic) {
    return this->trafficSumTree->GetSumHighestRanks(k, traffic);
}

TreeStatusType MergeDataCenters (dataCenter& a, const dataCenter& b){
    int b_size;
    b.trafficSumTree->Size(&b_size);
    Node<server, int>** array = new Node<server, int>*[b_size];
    b.trafficSumTree->GetTreeInOrder(array);

    for(int i = 0; i < b_size; i++) {
        TreeStatusType status = a.trafficSumTree->Add(*(array[i]->key),*(array[i]->value));
        if (status != TREE_SUCCESS) {
            for (int j = 0; j < i ; j++) {
                a.trafficSumTree->Delete(*(array[i]->key));
            }
            return status;
        }
    }
    // delete the b tree node
    for(int i = 0; i < b_size; i++) {
        b.trafficSumTree->Delete(*(array[i]->key));

    }
    return TREE_SUCCESS;
}
