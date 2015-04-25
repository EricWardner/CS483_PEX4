#include <stdio.h>
#include <stdlib.h>
#include "byutr.h"
#include "LRUstack.h"

int main(int argc, char **argv)
{
	lrustack* stack = (lrustack*)malloc(sizeof(lrustack));

	FILE *ifp;
	unsigned long num_accesses=0;
	p2AddrTr trace_record;
	
	// If the number of arguments is wrong then quit
	if(argc!=2 && argc!=3)
	{
		fprintf(stderr,"usage: %s input_byutr_file [frame_size]\n",argv[0]);
		fprintf(stderr,"\nwhere frame_size is a digit corresponding to the following menu:\n\t1: 512 bytes\n\t2: 1KB\n\t3: 2KB\n\t4: 4KB\n");
		exit(1);
	}
	
	// If the file cannot be opened, then quit
	if((ifp = fopen(argv[1],"rb")) == NULL)
	{
		fprintf(stderr,"cannot open %s for reading\n",argv[1]);
		exit(1);
	}

	int menu_option=0;
	if(argc==3){
		menu_option=atoi(argv[2]);
	}
	while(menu_option<1 || menu_option>4){
		char input[128];		
		fprintf(stderr,"Select a frame size:\n\t1: 512 bytes\n\t2: 1KB\n\t3: 2KB\n\t4: 4KB\n");
		fgets(input,128,stdin);
		menu_option=atoi(input);
	}
	int OFFSET_BITS;
	int MAX_FRAMES;//THIS WILL BE USED AS THE SIZE OF YOUR ARRAY
	switch(menu_option){
		case 1:
			OFFSET_BITS=9;
			MAX_FRAMES=8192;
			break;
		case 2:
			OFFSET_BITS=10;
			MAX_FRAMES=4096;
			break;
		case 3:
			OFFSET_BITS=11;
			MAX_FRAMES=2048;
			break;
		case 4:
			OFFSET_BITS=12;
			MAX_FRAMES=1024;
			break;
	}

	unsigned int* pageFaults = (unsigned int*) calloc(MAX_FRAMES+1, sizeof(unsigned int));

	intialize(stack, MAX_FRAMES);

	int i = 0;

	while(!feof(ifp))
	//while(!feof(ifp) && i < 100)  //you may want to use this to debug
	{		
		//read next trace record		
		fread(&trace_record, sizeof(p2AddrTr), 1, ifp);
		//to get the page number, we shift the offset bits off of the address
		unsigned long page_num = trace_record.addr >> OFFSET_BITS;//THIS IS THE PAGE NUMBER THAT WAS REQUESTED!!!!!

		// this next line prints the page number that was referenced.
		// Note the use of %lu as it is an unsigned long!  Might be useful when debugging.
		//if(num_accesses<4){
		//	printf("%lu\n",page_num);
		//}
		

		num_accesses++;

		// more code possibly useful for debugging... gives an indication of progress being made
		if((num_accesses % 100000) == 0){
			fprintf(stderr,"%lu samples read\r", num_accesses);
		}

		//TODO: process each page reference
		
		unsigned int stackIndex = seek_and_remove(stack, page_num);

		//printf("passed seek\n");

		push(stack, page_num);
		//printf("passed push\n");

		if(stackIndex == -1){
			stackIndex = MAX_FRAMES;
		}
		for(i = 0; i < stackIndex; i++){
			pageFaults[i] = pageFaults[i] + 1;
		}


	}

	//TODO: find the number of page faults for each number of allocated frames
	printf("num_accesses: %lu\n", num_accesses);
	printf("frames, misses, miss rate");
	for(i = 0; i < MAX_FRAMES; i++){
		printf("%d, %d, %0.5f\n", i, pageFaults[i], pageFaults[i]/(double)num_accesses);
	}

	fclose(ifp);

	return( 0 );
}

