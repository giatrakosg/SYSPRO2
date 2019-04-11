//
//  pidlist.cpp
//  SYSPRO2
//
//  Created by <author> on 11/04/2019.
//
//

#include "pidlist.hpp"

pidlist::pidlist() : first(NULL) , last(NULL) {}
int pidlist::add(int id ,pid_t rd,pid_t wr) {
    struct pidentry *m = new pidentry(id,rd,wr);
    if (last == NULL) {
        first = m;
        last = first ;
        return 0 ;
    }
    else {
        last->next = m ;
        last = m;
        return 0 ;
    }
    return 1 ;
}
pidlist::~pidlist() {
    if (first != NULL) {
        delete first->next ;
    }
}
