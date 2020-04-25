#include"conc_queue.h"

queue_t *Q;
int Ops_per_thread;
int FULLSIZE;

void* producer(void*){
	int val;
    for(int i=0; i<Ops_per_thread; i++){
        val = i%4 + 1;
        if(enqueue(Q,val)) cout<<"ENQ val:"<<val<<endl;
        else cout<<"###########ENQ failed###########"<<endl;
    }
    return NULL;
}

void* consumer(void*){
    int val;
    while(1){
        if(!dequeue(Q,val)){
        	cout<<"*********DQ failed*********"<<endl;
            break;
        }
        cout<<val<<": DQ"<<endl;
        this_thread::sleep_for(chrono::microseconds(val));
    }
    return NULL;
}

int main(int argc,char** argv){
    int num_prod = atoi(argv[1]);
    int num_cons = atoi(argv[2]);
    FULLSIZE = atoi(argv[3]);
    Ops_per_thread = atoi(argv[4]);
    pthread_t prod_handles[num_prod], cons_handles[num_cons];
    bool flg=0;
    int num_max = max(num_prod,num_cons);
    int num_min = min(num_prod,num_cons);
    if(num_max==num_prod) flg=1;

    Q = new queue_t();
    initialize(Q);

    for(int id=0;id<num_max;id++){
        if(flg || id<num_min) pthread_create(&prod_handles[id],NULL,producer,NULL);
        if(!flg || id<num_min) pthread_create(&cons_handles[id],NULL,consumer,NULL);
    }
    for(int id=0;id<num_max;id++){
        if(flg || id<num_min) pthread_join(prod_handles[id],NULL);
        if(!flg || id<num_min) pthread_join(cons_handles[id],NULL);
    }


    // pointer_t tail, head;
    // head = Q->Head.load();
    // tail = Q->Tail.load();

    // cout<<tail.queueSize - head.queueSize<<endl;
    cout<<"FINISHED"<<endl;
    return 0;
}   
