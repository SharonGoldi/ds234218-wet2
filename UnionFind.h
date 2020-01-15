
#ifndef WET2_DS_unionFind_H
#define WET2_DS_unionFind_H

#include <stdio.h>
#include <stdlib.h>
#include "DataCenter.h"
#include "AVLRankTree.h"

class unionFind {
    int size;
    int *parents;
    int *size_;
    DataCenter *dc_array;
public:
    unionFind(int size,DataCenter *dc_array);
    ~unionFind();
    /**
     *
     * @param x Make the set with the id x.
     */
    void Makeset(int x);
    /**
     *
     * @param p The first group to merge.
     * @param q  The second group to megre.
     */
    void Union(int p, int q);
    /**
     *
     * @param x  the item id that we want to find its group.
     * @return return the group that x belongs to, if x not in the groups returns -1.
     */
    int Find(int x);
    /**
     * prints the size of each group and every item's parent.
     */
    void print();
};
typedef unionFind *UnionFind;


#endif //WET2_DS_unionFind_H
