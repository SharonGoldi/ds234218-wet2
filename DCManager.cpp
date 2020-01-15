
#include "DCManager.h"

DCManager::DCManager(int n) : numOfDataCenters(n) {
    h_table = new serverHashTable();
    this->servers_tree = new AvlRankTree<server,int>();
    this->data_centers = new DataCenter[n];
    for (int j = 0; j < numOfDataCenters; ++j) {
        this->data_centers[j] = new dataCenter(j);
    }
    this->groups = new unionFind(numOfDataCenters,data_centers);
    for (int i = 0; i < numOfDataCenters ; ++i) {
        this->groups->Makeset(i);
    }
}

DCManager::~DCManager() {
    int* id_arr = new int[numOfDataCenters];
    bool* is_deleted_by_id = new bool[numOfDataCenters];
    for (int i = 0; i < numOfDataCenters; ++i) {
        id_arr[i] = this->data_centers[i]->GetID();
        is_deleted_by_id[i] = false;
    }

    delete this->h_table;
    delete servers_tree;
    for (int j = 0; j < numOfDataCenters; ++j) {
        int data_center_id = id_arr[j];
//        if (!is_deleted_by_id[data_center_id]) {
            delete this->data_centers[j];
//            is_deleted_by_id[data_center_id] = true;
//        }
    }
    delete[] is_deleted_by_id;
    delete[] id_arr;
    delete[] this->data_centers;
    delete this->groups;
}

TreeStatusType DCManager::mergeDC(int dc1, int dc2) {
    if (dc1 < 0 || dc2 < 0 || dc1 >= numOfDataCenters || dc2 >= numOfDataCenters){
        return TREE_INVALID_INPUT;
    }
    int root1 = this->groups->Find(dc1);
    int root2 = this->groups->Find(dc2);

    this->groups->Union(root1, root2);
    return TREE_SUCCESS;
}

TreeStatusType DCManager::addServer(int data_center_id, int server_id) {
    if (data_center_id < 0  || data_center_id >= numOfDataCenters || server_id <= 0) {
        return TREE_INVALID_INPUT;
    }
    return this->h_table->AddServer(server_id,data_center_id);
}

TreeStatusType DCManager::removeServer(int server_id) {
    if ( server_id <= 0){
        return TREE_INVALID_INPUT;
    }
    int dc_id = this->h_table->GetDataCenter(server_id);
    if (dc_id < 0) {
        return TREE_FAILURE;
    }
    if (this->h_table->GetTraffic(server_id) > 0 ) {
        int root = this->groups->Find(dc_id);
        this->data_centers[root]->RemoveServer(server_id,this->h_table->GetTraffic(server_id));
    }
    this->h_table->DeleteServer(server_id);
    return TREE_SUCCESS;
}

TreeStatusType DCManager::setTraffic(int server_id, int traffic) {
    if ( server_id <= 0 || traffic < 0){
        return TREE_INVALID_INPUT;
    }
    // if the new traffic is zero. do nothing
    if (traffic == 0) {
        return TREE_SUCCESS;
    }
    // if there is no server with this id return failure
    int dc_id = this->h_table->GetDataCenter(server_id);
    if(dc_id < 0) {
        return TREE_FAILURE;
    }

    Server new_server = this->h_table->GetServer(server_id);
    int old_traffic = new_server->GetTraffic();
    int root = this->groups->Find(dc_id);

    // if its the first update of traffic (there is no node for this server in the rank trees
    if (old_traffic == 0) {
        new_server->SetTraffic(traffic);
        this->data_centers[root]->SetTraffic(server_id, traffic);
        this->servers_tree->Add(*new_server, traffic);
    } // else, we already have a node in the trees which needs to be updated
    else {
        // updating the node in the data center tree
        this->data_centers[root]->UpdateTraffic(server_id, dc_id, old_traffic, traffic);
        // removing the old node
        this->servers_tree->Delete(*new_server);
        // updating the traffic value
        new_server->SetTraffic(traffic);
        // re adding the node to the tree
        this->servers_tree->Add(*new_server, traffic);
    }
    return TREE_SUCCESS;
}

TreeStatusType DCManager::sumHighestTraffic(int data_cenetr_id, int k, int *traffic_sum) {
    if (data_cenetr_id < -1 || k < 0 || data_cenetr_id >= numOfDataCenters || traffic_sum == NULL){
        return  TREE_INVALID_INPUT;
    }
    if (k == 0) {
        *traffic_sum = 0;
        return TREE_SUCCESS;
    }
    if (data_cenetr_id == -1){
        return this->servers_tree->GetSumHighestRanks(k, traffic_sum);
    }
    else {
        int root = this->groups->Find(data_cenetr_id);
        return this->data_centers[root]->SumHighestTrafficServers(k, traffic_sum);
    }
}