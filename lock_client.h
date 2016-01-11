// lock client interface.

#ifndef lock_client_h
#define lock_client_h

#include <string>
#include "lock_protocol.h"
#include "rpc/rpc.h"
#include <vector>

//*******************************added my mk*********************************
#include <pthread.h>
//*******************************added my mk*********************************

// Client interface to the lock server
class lock_client {
 protected:
  rpcc *cl;

  //*******************************added my mk*********************************
  //have to use different mutex to lock acquire and release
  //or can not get mutex when a client want to release but mutex is blocked by acquire
  pthread_mutex_t acquireMutex=PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t releaseMutex=PTHREAD_MUTEX_INITIALIZER;
  //*******************************added my mk*********************************

 public:
  lock_client(std::string d);
  virtual ~lock_client() {};
  virtual lock_protocol::status acquire(lock_protocol::lockid_t);
  virtual lock_protocol::status release(lock_protocol::lockid_t);
  virtual lock_protocol::status stat(lock_protocol::lockid_t);
};


#endif
