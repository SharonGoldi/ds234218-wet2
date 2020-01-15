
#include "UnionFind.h"

unionFind::unionFind(int size, DataCenter *dc_array):size(size),dc_array(dc_array){
    size_ = new int [size];
    parents = new int [size];

    for (int i = 0; i < size; ++i) {
        parents[i] = -1;
        size_[i] = 0;
    }
}

unionFind::~unionFind() {
    this->dc_array = NULL;
    delete [] size_;
    delete [] parents;
}

void unionFind::Makeset(int x) {
    this->parents[x] = -1;
    this->size_[x] = 1;
}

int unionFind::Find(int x) {
    int parent = this->parents[x];
    int old_p = x;
    while (parent > -1){
        old_p = parent;
        parent = this->parents[parent];
    }
    parent = old_p;
    int temp = x;
    int new_temp;
    while(temp != parent){
        new_temp = parents[temp];
        parents[temp] = parent;
        temp = new_temp;
    }
    return parent;
}

void unionFind::Union(int p, int q) {
    if ( size_[p] > size_[q]) {
        size_[p] += size_[q];
        size_[q]=0;
        parents[q] = p;
        MergeDataCenters(*dc_array[p], *dc_array[q]);

//        merged_dc_to_delete[q] = dc_array[q];
//        dc_array[q] = dc_array[p];
    }
    else if (size_[p] < size_[q]) {
        size_[q] += size_[p];
        size_[p] = 0;
        parents[p] = q;
        MergeDataCenters(*dc_array[q], *dc_array[p]);

//        merged_dc_to_delete[p] = dc_array[p];
//        dc_array[p] = dc_array[q];
    }
    else {
        if (p > q) {
            size_[p] += size_[q];
            size_[q]=0;
            parents[q] = p;
            MergeDataCenters(*dc_array[p], *dc_array[q]);

//            merged_dc_to_delete[q] = dc_array[q];
//            dc_array[q] = dc_array[p];
        }
        else {
            size_[q] += size_[p];
            size_[p] = 0;
            parents[p] = q;
            MergeDataCenters(*dc_array[q],*dc_array[p]);

//            merged_dc_to_delete[p] = dc_array[p];
//            dc_array[p] = dc_array[q];
        }
    }
}
void unionFind::print() {
    printf("parents : [");
    for (int i = 0; i < size ; ++i) {
        if (i == size-1){
            printf("%d",this->parents[i]);
        }
        else {
            printf("%d ,",this->parents[i]);
        }
    }
    printf("]\n");
    printf("size : [");
    for (int j = 0; j < size; ++j) {
        if (j == size-1){
            printf("%d",this->size_[j]);
        }
        else {
            printf("%d ,",this->size_[j]);
        }
    }
    printf("]\n");
}
