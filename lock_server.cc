// the lock server implementation

#include "lock_server.h"
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

lock_server::lock_server():
  nacquire (0)
{

}

lock_protocol::status
lock_server::stat(int clt, lock_protocol::lockid_t lid, int &r)
{
  lock_protocol::status ret = lock_protocol::OK;
  printf("stat request from clt %d\n", clt);
  r = nacquire;
  return ret;
}

//*******************************added my mk********************************
lock_protocol::status
lock_server::acquire(int clt, lock_protocol::lockid_t lid, int &r)
{
    pthread_mutex_lock(&mutex);
    lock_protocol::status ret=lock_protocol::OK;
    printf("acquire request from clt %d\n", clt);
    std::map<lock_protocol::lockid_t,lock *>::iterator find_one;
    find_one=lock_stat_map.find(lid);
    if (find_one == lock_stat_map.end())
    {//if haven't see this lock before,lock it!
        lock *l=new lock();
        l->stat=lock::lock_locked;
        lock_stat_map[lid]=l;

        //printf("clt %d creates a new lock\n", clt);
    }
    else if(lock_stat_map[lid]->stat == lock::lock_free)
    {//it's free now,lock it!
        lock_stat_map[lid]->stat=lock::lock_locked;

        //printf("clt %d locks a lock\n", clt);
    }
    else if(lock_stat_map[lid]->stat == lock::lock_locked)
    {//it's locked before,block to wait release
        //printf("clt %d blocked\n", clt);
        while(lock_stat_map[lid]->stat == lock::lock_locked)
        {//when the thread woke up,the lock may be locked by a new coming thread!!!!!
            pthread_cond_wait(&(lock_stat_map[lid]->condLock),&mutex);
        }
        //printf("clt %d locks a lock after blocked\n", clt);
        lock_stat_map[lid]->stat=lock::lock_locked;
    }

    pthread_mutex_unlock(&mutex);

    return ret;
}

lock_protocol::status
lock_server::release(int clt,lock_protocol::lockid_t lid,int &r)
{
    pthread_mutex_lock(&mutex);
    lock_protocol::status ret=lock_protocol::OK;
    printf("release request from clt %d\n", clt);
    if (lock_stat_map[lid]->stat ==lock::lock_locked)
    {//just free it and notify others
        lock_stat_map[lid]->stat =lock::lock_free;
        //printf("clt %d releases a lock\n", clt);
        pthread_cond_signal(&(lock_stat_map[lid]->condLock));
    }

    pthread_mutex_unlock(&mutex);

    return ret;
}
//*******************************added my mk*********************************
