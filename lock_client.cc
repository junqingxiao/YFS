// RPC stubs for clients to talk to lock_server

#include "lock_client.h"
#include "rpc/rpc.h"
#include <arpa/inet.h>

#include <sstream>
#include <iostream>
#include <stdio.h>

lock_client::lock_client(std::string dst)
{
  sockaddr_in dstsock;
  make_sockaddr(dst.c_str(), &dstsock);
  cl = new rpcc(dstsock);
  if (cl->bind() < 0) {
    printf("lock_client: call bind\n");
  }
}

int
lock_client::stat(lock_protocol::lockid_t lid)
{
  int r;
  int ret = cl->call(lock_protocol::stat, cl->id(), lid, r);
  assert (ret == lock_protocol::OK);
  return r;
}

lock_protocol::status
lock_client::acquire(lock_protocol::lockid_t lid)
{
    //*******************************added my mk*********************************
    pthread_mutex_lock(&acquireMutex);
    int r;
    int ret=cl->call(lock_protocol::acquire,cl->id(),lid,r);
    assert(ret == lock_protocol::OK);

    pthread_mutex_unlock(&acquireMutex);
    return ret;
    //*******************************added my mk*********************************
}

lock_protocol::status
lock_client::release(lock_protocol::lockid_t lid)
{
    //*******************************added my mk*********************************
    pthread_mutex_lock(&releaseMutex);
    int r;
    int ret=cl->call(lock_protocol::release,cl->id(),lid,r);
    assert(ret == lock_protocol::OK);

    pthread_mutex_unlock(&releaseMutex);
    return ret;
    //*******************************added my mk*********************************
}
