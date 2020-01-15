#include "library2.h"
#include "DCManager.h"

StatusType ReplaceEnums(TreeStatusType treeStatusType) {
    switch(treeStatusType) {
        case TREE_ALLOCATION_ERROR:
            return ALLOCATION_ERROR;
        case TREE_FAILURE:
            return FAILURE;
        case TREE_INVALID_INPUT:
            return INVALID_INPUT;
        default: //TREE_SUCCESS
            return SUCCESS;
    }
}


void* Init(int n){
    if (n <= 0) {
        return NULL;
    }
    DCManager *ds;
    try {
        ds = new DCManager(n);
    }
    catch (std::bad_alloc& ba) {
        return NULL;
    }
    return ds;
}

StatusType MergeDataCenters(void *DS, int dataCenter1, int dataCenter2){
    if (DS == NULL || dataCenter1 <= 0 || dataCenter2 <= 0){
        return INVALID_INPUT;
    }
    DCManager *ds = (DCManager*) DS;
    return ReplaceEnums(ds->mergeDC(dataCenter1 - 1, dataCenter2 - 1));
}

StatusType AddServer(void *DS, int dataCenterID, int serverID){
    if (DS == NULL || dataCenterID <= 0 || serverID <= 0){
        return INVALID_INPUT;
    }
    DCManager *ds = (DCManager*) DS;
    return ReplaceEnums(ds->addServer(dataCenterID - 1, serverID));

}

StatusType RemoveServer(void *DS, int serverID){
    if (DS == NULL || serverID <= 0){
        return INVALID_INPUT;
    }
    DCManager *ds = (DCManager*) DS;
    return ReplaceEnums(ds->removeServer(serverID));
}

StatusType SetTraffic(void *DS, int serverID, int traffic){
    if (DS == NULL || traffic <= 0 || serverID <= 0){
        return INVALID_INPUT;
    }
    DCManager *ds = (DCManager*) DS;
    return ReplaceEnums(ds->setTraffic(serverID, traffic));
}

StatusType SumHighestTrafficServers(void *DS, int dataCenterID,  int k, int *traffic){
    if (DS == NULL || dataCenterID < 0 || k < 0 || traffic == NULL){
        return INVALID_INPUT;
    }
    DCManager *ds = (DCManager*) DS;
    return ReplaceEnums(ds->sumHighestTraffic(dataCenterID - 1, k , traffic));
}

void Quit(void** DS){
    if (DS == NULL || *DS == NULL) {
        return;
    }
    DCManager* ds = (DCManager*) *DS;
    delete ds;
    *DS = NULL;
}