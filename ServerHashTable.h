#ifndef WET2_serverHashTable_H
#define WET2_serverHashTable_H

#define START_SIZE 10
#define EMPTY 0
#define DELETED 1
#define FULL 2

#include "Server.h"
#include "AVLRankTree.h"

class serverHashTable {
    int serversNum;
    int tableSize;
    //to check when to refresh table
    int deletedCounter;
    //to check if we need to make a bigger table
    int addCounter;

    Server* table;
    // to fill a place when its data was Deleted
    Server deletedServer;

    /**
     * checks if the cell is empty, was deleted or full
     * @param k - the place in the hash table table
     * @return macros EMPTY, DELETED, FULL
     */
    int CheckCell(int k);
    /**
     * swaps the hash table to a table without deleted cells
     */
    void CleanTable();
    /**
     * hash function to place the info
     * @param serverID - the server ID number of the new server we want to save in the table
     * @param times - the number of times we checked a cell to fill and it was full
     * @return the place in the array to fill
     */
    int HashFunc(int serverID, int times);
    /**
     * hash function to place the info for the first time
     * @param serverID - the server ID number of the new server we want to save in the table
     * @return the place in the array to fill
     */
    int HFunc(int serverID);
    /**
     * the step func to change the filling place if the prev cell is filled
     * @param serverID - the server ID number of the new server we want to save in the table
     * @return the offset of the place to fill
     */
    int Step(int serverID);
    /**
     * finds a node by its serverID
     * @param serverID - the serverID of the new server we want to save in the table
     * @return the place of the found node in the hash table
     */
    int Find(int serverID);
    /**
     * called if the hash table is full, duplicates the size of the table
     * @return ALLOCATION_ERROR - In case of an allocation error
     *            FAILURE - If the server does not exist
     *            SUCCESS - Otherwise.
     */
    TreeStatusType MakeBiggerTable();
    /**
     * adds the given node to the hash table
     * @param im - the node to add
     * @return ALLOCATION_ERROR - In case of an allocation error
     *            FAILURE - If the server does not exist
     *            SUCCESS - Otherwise.
     */
    TreeStatusType AddNode(Server server);

public:
    serverHashTable();
    ~serverHashTable();
    serverHashTable(serverHashTable& ht);

    TreeStatusType AddServer(int serverID, int dataCenterID);
    TreeStatusType DeleteServer(int serverID);
    Server GetServer(int serverID);
    TreeStatusType SetTraffic(int serverID, int traffic);
    int GetDataCenter(int serverID);
    int GetTraffic(int serverID);
};

typedef serverHashTable *ServerHashTable;

#endif //WET2_serverHashTable_H
