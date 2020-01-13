#include <cstdio>
#include <cmath>
#include <new>
#include <cassert>
#include "serverHashTable.h"

// basic class funcs

serverHashTable::serverHashTable() : serversNum(0), tableSize(START_SIZE), deletedCounter(0), addCounter(0) {
    this->table = new Server [START_SIZE];

    for (int i = 0; i< this->tableSize; i++) {
        this->table[i] = NULL;
    }

    this->deletedServer = new server(0,0);
}

serverHashTable::~serverHashTable() {
    for (int i = 0; i< this->tableSize; i++) {
        if (this->table[i] != NULL && this->table[i] != this->deletedServer) {
            delete this->table[i];
        }
    }
    delete deletedServer;
    delete[] this->table;
}

serverHashTable::serverHashTable(serverHashTable &ht) : serversNum(ht.serversNum), deletedCounter(ht.deletedCounter)
        ,tableSize(ht.tableSize), deletedServer(ht.deletedServer), addCounter(ht.addCounter){
    this->table = new Server [ht.tableSize];
    for (int i = 0; i< this->tableSize; i++) {
        this->table[i] = ht.table[i];
    }
}

// heavy funcs of the hashtable

void serverHashTable::CleanTable() {
    Server* newTable = new Server [this->tableSize];
    Server* oldTable = this->table;
    this->table = newTable;

    for (int i = 0; i < this->tableSize; i++) {
        this->table[i] = NULL;
    }

    for (int i = 0; i < this->tableSize; i++) {
        if (oldTable[i] != NULL && oldTable[i] != this->deletedServer) {
            this->AddNode(oldTable[i]);
        }
    }
    delete[] oldTable;
}

TreeStatusType serverHashTable::MakeBiggerTable() {
    int oldSize = this->tableSize;
    this->tableSize = this->tableSize + START_SIZE;
    Server* newTable = new Server [this->tableSize];
    Server* oldTable = this->table;
    this->table = newTable;


    for (int i = 0; i < this->tableSize; i++) {
        this->table[i] = NULL;
    }
    for (int i = 0; i < oldSize; i++) {
        if (oldTable[i] != NULL && oldTable[i] != this->deletedServer) {
            this->AddNode(oldTable[i]);
        }
    }
    delete[] oldTable;
    return TREE_SUCCESS;
}

// basic funcs of th hashtable

int serverHashTable::CheckCell(int k) {
    if (this->table[k] == NULL) {
        return EMPTY;
    } else if (this->table[k] == this->deletedServer) {
        return DELETED;
    }
    return FULL;
}

int serverHashTable::Find(int serverID) {
    for (int i = 0; i < this->tableSize; i++) {
        if (this->table[i] != NULL && this->table[i] != this->deletedServer &&
            this->table[i]->GetServerID() == serverID) {
            return i;
        }
    }
    return -1;
}

int serverHashTable::HashFunc(int serverID, int times) {
    return (this->HFunc(serverID) + times)%(this->tableSize);
}

int serverHashTable::HFunc(int serverID) {
    double phi = (sqrt(5)-1)/2;
    double a = phi*serverID;
    int b = (int) a;
    double offset = (a - b)*this->tableSize;
    return (int) offset;
}

int serverHashTable::Step(int serverID) {
    int a = 5;
    while (this->tableSize%a == 0) {
        a = a+2;
    }
    int step = (serverID)%a;
    if (step == 0) {
        step++;
    }
    return step;
}

TreeStatusType serverHashTable::AddNode(Server server) {
    assert (server != NULL);
    if(this->addCounter == this->tableSize) {
        try {
            this->MakeBiggerTable();
        }
        catch (std::bad_alloc& ba) {
            return TREE_ALLOCATION_ERROR;
        }
    }
    for (int i = 0; i < this->tableSize; i++) {
        int plc = this->HashFunc(server->GetServerID(),i);
        if (this->CheckCell(plc) != FULL) {
            this->table[plc] = server;
            return TREE_SUCCESS;
        }
    }
    return TREE_FAILURE;
}

// basic funcs of th hashtable - public

TreeStatusType serverHashTable::AddServer(int serverID, int dataCenterID) {
    //assumes the data center ID is proper
    // if the ID number is bad, return
    if(serverID <= 0) {
        return TREE_INVALID_INPUT;
     // if a server with this ID is already in the table, return   
    } else if(this->Find(serverID) != -1) {
        return TREE_FAILURE;
    }
    
    // make a new server node
    Server toAdd;
    try{ toAdd = new server(serverID, dataCenterID);
    }
    catch (std::bad_alloc& ba) {
        return TREE_ALLOCATION_ERROR;
    }
    // make the table bigger if needed
    if(this->addCounter == this->tableSize) {
        try{ this->MakeBiggerTable();
        }
        catch (std::bad_alloc& ba) {
            return TREE_ALLOCATION_ERROR;
        }
    }

    //place the server node in the table
    for (int i = 0; i < this->tableSize; i++) {
        int plc = this->HashFunc(serverID,i);
        if (this->CheckCell(plc) != FULL) {
            this->table[plc] = toAdd;
            (this->addCounter)++;
            return TREE_SUCCESS;
        }
    }
    return TREE_FAILURE;
}

TreeStatusType serverHashTable::DeleteServer(int serverID) {
    // check if the server is in the table, if not, return
    int plc = this->Find(serverID);
    if(serverID <= 0) {
        return TREE_INVALID_INPUT;
    } else if(plc == -1) {
        return TREE_FAILURE;
    }
    // update the table cell and counters
    this->deletedCounter ++;
    this->addCounter --;
    Server temp = this->table[plc];
    this->table[plc] = this->deletedServer;
    delete temp;
    return TREE_SUCCESS;
}

TreeStatusType serverHashTable::SetTraffic(int serverID, int traffic) {
    if (traffic <= 0 || serverID <= 0) {
        return TREE_INVALID_INPUT;
    }
    int plc = this->Find(serverID);
    if (plc == -1) {
        return TREE_FAILURE;
    }
    this->table[plc]->SetTraffic(traffic);
    return TREE_SUCCESS;
}

int serverHashTable::GetDataCenter(int serverID) {
    return this->table[this->Find(serverID)]->GetDataCenterID();
}

int serverHashTable::GetTraffic(int serverID) {
    return this->table[this->Find(serverID)]->GetTraffic();
}
