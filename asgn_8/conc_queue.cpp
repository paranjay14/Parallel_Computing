#include "conc_queue.h"


void initialize(queue_t* Q){
	Q->Head = {new node_t{0,{}},0,0}; 
	Q->Tail = Q->Head.load();
	return;	
}


bool enqueue(queue_t* Q, int val){
	node_t* Node = new node_t{val,{}}; 																// E1, E2, E3
	pointer_t tail, nxt, head;

	while(1){ 																						// E4
		tail = Q->Tail.load();																		// E5
		head = Q->Head.load();
		if(tail.queueSize - head.queueSize >= FULLSIZE){
			delete Node;
			return false;
		}

		nxt = tail.ptr->next;																		// E6

		if(tail == Q->Tail){																		// E7
			if(nxt.ptr == NULL){																	// E8
				if(tail.ptr->next.compare_exchange_weak(nxt,{Node,nxt.count+1,tail.queueSize+1})){	// E9
					break;																			// E10
				}																					// E11
			}
			else{																					// E12
				Q->Tail.compare_exchange_weak(tail,{nxt.ptr,tail.count+1,tail.queueSize+1});		// E13
			}																						// E14
		}																							// E15
	}																								// E16
	Q->Tail.compare_exchange_weak(tail,{Node,tail.count+1,tail.queueSize+1});						// E17

	return true;
}


bool dequeue(queue_t* Q, int &pvalue){
	pointer_t tail, nxt, head;

	while(1){ 																						// D1
		head = Q->Head.load();																		// D2
		tail = Q->Tail.load();																		// D3
		nxt = head.ptr->next;																		// D4

		if(head == Q->Head){																		// D5
			if(head.ptr == tail.ptr){																// D6
				if(nxt.ptr == NULL){																// D7
					return false;																	// D8
				}																					// D9	
				Q->Tail.compare_exchange_weak(tail,{nxt.ptr,tail.count+1,tail.queueSize+1});		// D10
			}
			else{																					// D11
				pvalue = nxt.ptr->value;															// D12
				if(Q->Head.compare_exchange_weak(head,{nxt.ptr,head.count+1,head.queueSize+1})){	// D13
					break;																			// D14
				}																					// D15
			}																						// D16
		}																							// D17
	}																								// D18
	free(head.ptr);																					// D19
	return true;																					// D20
}