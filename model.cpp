#include "model.hpp"

BlockQueue::BlockQueue(int cap = MAX_QUEUE):capacity_(cap){
    pthread_mutex_init(&mutex_,NULL);
    pthread_cond_init(&cond_consumer,NULL);
    pthread_cond_init(&cond_produce,NULL);
}

BlockQueue::~BlockQueue(){
    pthread_mutex_destroy(&mutex_);
    pthread_cond_destroy(&cond_consumer);
    pthread_cond_destroy(&cond_produce);
}

bool BlockQueue::QueuePush(int data){
    pthread_mutex_lock(&mutex_);
    while(queue_.size() == capacity_){
        pthread_cond_wait(&cond_produce,&mutex_);
    }
    queue_.push(data);
    pthread_cond_signal(&cond_consumer);
    pthread_mutex_unlock(&mutex_);
}

bool BlockQueue::QueuePop(int &data){
    pthread_mutex_lock(&mutex_);
    while(queue_.empty()){
        pthread_cond_wait(&cond_consumer,&mutex_);
    }
    data = queue_.front();
    queue_.pop();
    pthread_cond_signal(&cond_produce);
    pthread_mutex_unlock(&mutex_);
}

void *thr_conmser(void *arg){
    BlockQueue *q = (BlockQueue*)arg;
    while (1)    {
        int data;
        q->QueuePop(data);
        std::cout << "conmser:" << pthread_self() << "  get data:" << data << "\n";
    }
    return NULL;
}

void *thr_produce(void *arg){
    BlockQueue *q = (BlockQueue*)arg;
    int i = 0;
    while (1){
        int data;
        q->QueuePush(i++);
        std::cout << "produce:" << pthread_self() << "  put data:" << i << "\n";
    }
    return NULL;    
}

int main(){
    pthread_t con_tid[4],pro_tid[4];
    BlockQueue q;
    int ret;
    for(int i = 0;i < 4;i++){
        ret = pthread_create(&con_tid[i],NULL,thr_conmser,(void*)&q);
        if(ret != 0){
            printf("pthread create error!\n");
            return -1;
        }
    }

    for(int i = 0;i < 4;i++){
        ret = pthread_create(&pro_tid[i],NULL,thr_produce,(void*)&q);
        if(ret != 0){
            printf("pthread create error!\n");
            return -1;
        }
    }

    for(int i = 0;i < 4;i++){
        pthread_join(pro_tid[i],NULL);
    }

    for(int i = 0;i < 4;i++){
        pthread_join(con_tid[i],NULL);
    }

    return 0;
}