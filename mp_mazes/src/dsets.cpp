#include <vector>


#include "dsets.h"

void DisjointSets::addelements(int num) {
    for (int i = 0; i < num; i++) {
        elmnts.push_back(-1);
    }
}

int DisjointSets::find(int elem) {
    if (elmnts[elem] < 0) {
        return elem;
    }
    int root = find(elmnts[elem]);
    elmnts[elem] = root;
    return root;
}

void DisjointSets::setunion(int a, int b) {
    int root_a = find(a);
    int root_b = find(b);
   
    if (root_a == root_b) {
        return;
    }
int height =  elmnts[root_a] + elmnts[root_b];


    if (elmnts[root_a] < elmnts[root_b]) {
        elmnts[root_b] = root_a;
        elmnts[root_a] = height ;
    } else {
        elmnts[root_a] = root_b;
        elmnts[root_b] = height ;
    }
}

int DisjointSets::size(int elem) {
    int root = find(elem);
    return (-1)*elmnts[root];
}





//Ali 