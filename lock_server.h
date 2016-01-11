// this is the lock server
// the lock client has a similar interface

#ifndef lock_server_h
#define lock_server_h

#include <string>
#include "lock_protocol.h"
#include "lock_client.h"
#include "rpc/rpc.h"

//*******************************added my mk*********************************
#include <pthread.h>
#include <map>

class lock
{
public:
    enum lock_stat
    {
        lock_free,
        lock_locked
    };
    lock_stat stat;
    pthread_cond_t condLock=PTHREAD_COND_INITIALIZER;
    lock(){};
    ~lock(){pthread_cond_destroy(&condLock);};
};



//*******************************added my mk*********************************


class lock_server {

 protected:
  int nacquire;
  //*******************************added my mk*********************************
  std::map<lock_protocol::lockid_t,lock *> lock_stat_map;//map to record
  pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
  //*******************************added my mk*********************************

 public:
  lock_server();
  ~lock_server() {};
  lock_protocol::status stat(int clt, lock_protocol::lockid_t lid, int &);
  //*******************************added my mk*********************************
  lock_protocol::status acquire(int clt,lock_protocol::lockid_t lid,int &);
  lock_protocol::status release(int clt,lock_protocol::lockid_t lid,int &);
  //*******************************added my mk*********************************
};

#endif
