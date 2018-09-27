//
// Created by agatha on 07.04.18.
//

#ifndef LAB4_ACTIVEEDGELIST_H
#define LAB4_ACTIVEEDGELIST_H

#include <vector>
#include "YGroup.h"

using namespace std;

class ActiveEdgeList {
public:
    vector<YGroup> activeEdges;
    vector<int> intersections;
    ActiveEdgeList();
    void add_edge(YGroup ygroup);
    void delete_edges(int y);
    void find_intersection();
};


#endif //LAB4_ACTIVEEDGELIST_H
