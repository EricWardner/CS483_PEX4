#include <stdio.h>
#include <stdlib.h>
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
	//printf("i died here");

	if(lrus->size == 0){
		newNode->prev = NULL;
		newNode->next = NULL;
		lrus->head = newNode;
		lrus->tail = newNode;
		lrus->size = lrus->size + 1;

	}else{

		if(lrus->size > lrus->maxsize){
			//reset tail, push tail off stack
			node* newTail = lrus->tail->prev;
			newTail->next = NULL;
			free(lrus->tail);
			lrus->tail = newTail;
			lrus->size = lrus->size - 1;		
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
	//printf("i died in remove");
	node* currNode = lrus->head;
	int i = 0;
	while(currNode != NULL){
		if(currNode->pagenum == pagenum){
			//if its size is 1
			/*
			if(lrus->size == 1){
				free(currNode);
			}*///if its the tail
			if(currNode->next == NULL){
				lrus->tail = currNode->prev;
				lrus->tail->next = NULL;
				free(currNode);
			}//if its the head 
			else if(currNode->prev == NULL){
				lrus->head = currNode->next;
				lrus->head->prev = NULL;
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