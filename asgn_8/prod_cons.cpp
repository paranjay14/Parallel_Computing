#include"conc_queue.h"
#include <numeric>
#include <iomanip> 

queue_t *Q;
long long Ops_per_prod_thread, Ops_per_cons_thread;
int FULLSIZE, num_Prod;
atomic<int> prod_end_cnt;
atomic<bool> prod_ended;

void* producer(void* enq_arg){
	int val=1, mod=2;
    // long long enq_success_cnt=0;
    for(long long i=0; i<Ops_per_prod_thread; val++,i++){
        if(val>mod) val = 1;
        while(!enqueue(Q,val)) this_thread::sleep_for(chrono::microseconds(2));
        // enq_success_cnt++;
    }
    // *((long long*)enq_arg) = enq_success_cnt;

    ++prod_end_cnt;
    if(prod_end_cnt==num_Prod) prod_ended=1;

    return NULL;
}

void* consumer(void* deq_arg){
    int val;
    // long long deq_success_cnt=0;
    bool prod_flg;
    for(long long i=0; i<Ops_per_cons_thread; i++){
        prod_flg=prod_ended;
        if(!prod_flg) while(!dequeue(Q,val) && !prod_flg) prod_flg=prod_ended;
        else          while(!dequeue(Q,val) && !prod_flg);

        // deq_success_cnt++;
        this_thread::sleep_for(chrono::microseconds(val));
    }
    // *((long long*)deq_arg) = deq_success_cnt;
    return NULL;
}

int main(int argc,char** argv){
    prod_end_cnt=0;
    prod_ended=0;
    int num_prod = atoi(argv[1]);
    num_Prod=num_prod;
    int num_cons = atoi(argv[2]);
    FULLSIZE = atoi(argv[3]);
    long long Total_Ops = atoll(argv[4]);
    Ops_per_prod_thread = Total_Ops/(long long)num_prod;
    Ops_per_cons_thread = Total_Ops/(long long)num_cons;
    pthread_t prod_handles[num_prod], cons_handles[num_cons];
    long long enq_cnt_arg[num_prod], deq_cnt_arg[num_cons];
    int num_max = max(num_prod,num_cons);

    Q = new queue_t();
    initialize(Q);

    auto start = chrono::high_resolution_clock::now();

    for(int id=0;id<num_max;id++){
        if(id<num_prod) pthread_create(&prod_handles[id],NULL,producer,(void*)&enq_cnt_arg[id]);
        if(id<num_cons) pthread_create(&cons_handles[id],NULL,consumer,(void*)&deq_cnt_arg[id]);
    }

    for(int id=0;id<num_max;id++){
        if(id<num_prod) pthread_join(prod_handles[id],NULL);
        if(id<num_cons) pthread_join(cons_handles[id],NULL);
    }

    auto end = chrono::high_resolution_clock::now(); 
  
    double time_taken =  chrono::duration_cast<chrono::nanoseconds>(end - start).count(); 
    time_taken *= 1e-9;

    // long long total_enq_success_cnt = accumulate(enq_cnt_arg,enq_cnt_arg+num_prod,0LL);
    // long long total_deq_success_cnt = accumulate(deq_cnt_arg,deq_cnt_arg+num_cons,0LL);
    
    // for (int i = 0; i < num_prod; ++i)
    //     cout<<enq_cnt_arg[i]<<" ";
    // cout<<endl;
    // for (int i = 0; i < num_cons; ++i)
    //     cout<<deq_cnt_arg[i]<<" ";
    // cout<<endl;
    
    // pointer_t tail, head;
    // head = Q->Head.load();
    // tail = Q->Tail.load();
    // cout<<tail.queueSize - head.queueSize<<endl;
    // cout<<num_prod<<","<<num_cons<<","<<FULLSIZE<<","<<Ops_per_prod_thread<<","<<Ops_per_cons_thread<<",";
    // cout<<","<<total_enq_success_cnt<<","<<total_deq_success_cnt<<endl;
    
    cout<<fixed<<time_taken<<setprecision(12)<<endl;

    return 0;
}   
