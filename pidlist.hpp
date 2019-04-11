//
//  pidlist.hpp
//  SYSPRO2
//
//  Created by <author> on 11/04/2019.
//
//

#ifndef pidlist_hpp
#define pidlist_hpp
#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <unistd.h>

#include <stdio.h>
struct pidentry {
    int id ;
    pid_t reader ;
    pid_t writer ;
    bool reader_done ;
    bool writer_done ;
    struct pidentry *next ;
    pidentry(int id,pid_t reader,pid_t writer) :
    id(id) , reader(reader) , writer(writer) ,
    next(NULL) , reader_done(false) , writer_done(false){}
    int free(void) ;
};

class pidlist {
private:
    struct pidentry *first ;
    struct pidentry *last ;
public:
    pidlist();
    int add(int ,pid_t ,pid_t );
    int remove(int ); // Removes the entry with id from list
    int free(void);
    ~pidlist();
protected:

};


#endif /* pidlist_hpp */
