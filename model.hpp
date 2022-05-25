#include <queue>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <cstdio>

#define MAX_QUEUE 10

class BlockQueue{
private:
    int capacity_;
    std::queue<int> queue_;

    pthread_mutex_t mutex_;
    pthread_cond_t cond_consumer;
    pthread_cond_t cond_produce;

public:
    BlockQueue(int cap);

    ~BlockQueue();

    bool QueuePush(int data);
    
    bool QueuePop(int &data);
};
