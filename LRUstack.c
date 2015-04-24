#include "LRUstack.h"

void intialize(lrustack* lrus, unsigned int maxsize){	
	//allocate memory for head and tail
	lrus->head = NULL;
	lrus->tail = NULL;
	lrus->maxsize = maxsize;
	lrus->size = 0;
}

void push(lrustack* lrus, unsigned long pagenum){
	node* newNode = (node*)malloc(sizeof(node));
	newNode->pagenum = pagenum;
	if(lrus->size == 0){
		newNode->prev = NULL;
		newNode->next = NULL;
		lrus->head = newNode;
		lrus->tail = newNode;
		lrus->size = lrus->size + 1;
	}else{
		if(lrus->size == lrus->maxsize){
			//reset tail, push tail off stack
			node* tmpTail = lrus->tail->prev;
			tmpTail->next = NULL;
			free(lrus->tail);
			lrus->tail = tmpTail;		
		}
		newNode->prev = NULL;
		newNode->next = lrus->head;
		lrus->head->prev = newNode;
		lrus->head = newNode;
		//update total size
		lrus->size = lrus->size + 1;
	}	
	return;	
}

int seek_and_remove(lrustack* lrus, unsigned long pagenum){
	
	node* currNode = lrus->head;
	int i = 0;
	while(currNode != NULL){
		if(currNode->pagenum == pagenum){
			//if its size is 1
			if(lrus->size == 1){
				free(currNode);
			}//if its the tail
			else if(currNode->next == NULL){
				lrus->tail = currNode->prev;
				free(currNode);
			}//if its the head 
			else if(currNode->prev == NULL){
				lrus->head = currNode->next;
				free(currNode);
			}else{
				//link over the current node
				currNode->prev->next = currNode->next;
				currNode->next->prev = currNode->prev;
				free(currNode);
			}
			lrus->size = lrus->size - 1;
			return i;
		}
		currNode = currNode->next;
		i = i+1;
	}
	return -1;
}