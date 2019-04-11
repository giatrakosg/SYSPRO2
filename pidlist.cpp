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
    if (first == NULL) {
        first = m;
        last = first ;
        return 0 ;
    }
    else {
        last->next = m ;
        last = m ;
        return 0 ;
    }
    return 1 ;
}
pidlist::~pidlist() {
    if (first != NULL) {
        delete first->next ;
    }
}
int pidentry::free(void) {
    int status ;
    pid_t rc ;
    rc = waitpid(reader,&status,WNOHANG);
    if (rc > 0) {
        printf("Reader proc %ld ended with return status %d \n",
        (long) reader,status);
        reader_done = true ;
    } else if (rc == 0) {
    //    printf("No change \n");
    //    fflush(stdout);
    }

    rc = waitpid(writer,&status,WNOHANG);
    if (rc > 0) {
        printf("Writer proc %ld ended with return status %d \n",
        (long) writer,status);
        writer_done = true ;
    } else if (rc == 0) {
        //printf("No change \n");
        //fflush(stdout);
    }
    if (reader_done && writer_done) {
        return id ;
    }
    else {
        if (next != NULL) {
            return next->free();
        } else {
            return -1 ;
        }
    }

}
int pidlist::remove(int rid) {
    struct pidentry *ind = first;
    struct pidentry *prev = NULL ;
    while ((ind->id != rid) && (ind != NULL)) {
        prev = ind ;
        ind = ind->next ;
    }
    // It wasnt found
    if (ind == NULL) {
        return -1 ;
    }
    // It is the first node of the list
    if (prev == NULL) {
        delete ind ;
        first = NULL ;
        last = NULL ;
    } else {
        prev->next = ind->next ;
        delete ind ;
    }
    return 0 ;
}

int pidlist::free(void) {
    int rc ;
    while( (first != NULL) && ((rc = first->free()) > 0)) {
        remove(rc);
    }
}
