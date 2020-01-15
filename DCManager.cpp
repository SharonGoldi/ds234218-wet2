
#include "DCManager.h"

DCManager::DCManager(int n) : numOfDataCenters(n) {
    h_table = new serverHashTable();
    this->servers_tree = new AvlRankTree<server,int>();
    this->data_centers = new DataCenter[n];
    for (int j = 1; j < numOfDataCenters; ++j) {
        this->data_centers[j] = new dataCenter(j);
    }
    this->groups = new unionFind(numOfDataCenters,data_centers);
    for (int i = 1; i < numOfDataCenters ; ++i) {
        this->groups->Makeset(i);
    }
}

DCManager::~DCManager() {
    delete this->h_table;
    delete servers_tree;
    for (int j = 1; j < numOfDataCenters; ++j) {
        delete this->data_centers[j];
    }
    delete[] this->data_centers;
    delete this->groups;
}

TreeStatusType DCManager::mergeDC(int dc1, int dc2) {
    if (dc1 <= 0 || dc2 <= 0 || dc1 > numOfDataCenters || dc2 > numOfDataCenters){
        return TREE_INVALID_INPUT;
    }
    this->groups->Union(dc1,dc2);
    return TREE_SUCCESS;
}

TreeStatusType DCManager::addServer(int data_center_id, int server_id) {
    if (data_center_id <= 0  || data_center_id > numOfDataCenters || server_id <= 0){
        return TREE_INVALID_INPUT;
    }
    return this->h_table->AddServer(server_id,data_center_id);
}

TreeStatusType DCManager::removeServer(int server_id) {
    if ( server_id <= 0){
        return TREE_INVALID_INPUT;
    }
    int dc_id = this->h_table->GetDataCenter(server_id);
    if (this->h_table->GetTraffic(server_id) > 0 ){
        this->data_centers[dc_id]->RemoveServer(server_id,this->h_table->GetTraffic(server_id));
    }
    this->h_table->DeleteServer(server_id);

}

TreeStatusType DCManager::setTraffic(int server_id, int traffic) {
    if ( server_id <= 0 || traffic < 0){
        return TREE_INVALID_INPUT;
    }
    int dc_id = this->h_table->GetDataCenter(server_id);
    Server new_server = new server(server_id,dc_id);
    this->h_table->SetTraffic(server_id,traffic);
    this->servers_tree->Add(*new_server, traffic);
    this->data_centers[dc_id]->SetTraffic(server_id,traffic);

}

TreeStatusType DCManager::sumHighestTraffic(int data_cenetr_id, int k, int *traffic_sum) {
    if (data_cenetr_id < 0 || k < 0 || data_cenetr_id > numOfDataCenters || traffic_sum == NULL){
        return  TREE_INVALID_INPUT;
    }
    if (k == 0) {
        *traffic_sum = 0;
        return TREE_SUCCESS;
    }
    if ( data_cenetr_id == 0){
        return this->servers_tree->GetSumHighestRanks(k,traffic_sum);
    }
    else {
        return this->data_centers[data_cenetr_id]->SumHighestTrafficServers(k,traffic_sum);
    }
    return TREE_SUCCESS;
}