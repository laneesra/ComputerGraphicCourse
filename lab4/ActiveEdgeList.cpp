
#include <algorithm>
#include <iostream>
#include "ActiveEdgeList.h"

ActiveEdgeList::ActiveEdgeList() {}

void ActiveEdgeList::add_edge(YGroup ygroup){
    activeEdges.push_back(ygroup);
}

void ActiveEdgeList::delete_edges(int y){
    for (int i=0; i<activeEdges.size(); i++) {
        if (activeEdges[i].edge.begin.y >= y){
            activeEdges.erase(activeEdges.begin()+i--);
        }
    }
}

void ActiveEdgeList::find_intersection(){
    for (int i=0; i<activeEdges.size(); i++){
        intersections.push_back((int)activeEdges[i].x);
    }
    sort(intersections.begin(), intersections.end());
}