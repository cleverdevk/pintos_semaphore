#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"

#include "devices/timer.h"

struct param {
                int Fe, Co, FeNeed, CoNeed;
                int BLT[4][3];
        };
struct param_arm {
	int BLT[4][3];
	struct lock l;
};

//arm1
void arm0(struct param* p)
{
	//semwait(if0);
	if(p->BLT[0][2] == 1)
		p->IF[0]++;
	//semsignal(

//check if the belt is full
bool isBltFull(int* blt)
{
	int sum = 0;
	for(int i=0;i<3;i++)
		sum+=blt[i];
	return (sum==3);
}
//belt shift
void shiftbelt(int* blt, int* item)
{
	blt[2]=blt[1];
	blt[1]=blt[0];
	if((*item) > 0)
	{
		blt[0] = 1;
		(*item)--;
	}
	else
		blt[0] = 0;
}
	

void printbelt(int* blt)
{
	for(int i=0;i<3;i++)
	{
		printf("[%d]",blt[i]);
	}
	printf("\n");
}
//belt move
void beltmove(struct param* p)
{
	for(int i=0;;i++)
	{
		if(i%50000000==0)
		{
			printbelt(p->BLT[0]);
			//belt0
			if(isBltFull(p->BLT[0]))
			{
				//idle
			}
			else{
				shiftbelt(p->BLT[0],&(p->Fe));
			}
			printbelt(p->BLT[1]);
			//belt1
			if(isBltFull(p->BLT[1]))
			{	
				//idle
			}
			else{
				shiftbelt(p->BLT[1],&(p->Co));
			}
			printf("Fe:%d, Co:%d\n",p->Fe,p->Co);
		}
	}
}

void armwork0(struct param* p)
{
	//semwait(blt0)
	//semwait(if0)
	//
	//critical section
	
	//
	//semsignal(if0)
	//semsignal(blt0)
	

		
}

void run_factorii(char **argv)
{

	printf("%d",sizeof(struct param_arm)); 	
	int Fe, Co, FeNeed, CoNeed;


	Fe = atoi(argv[1]);
	Co = atoi(argv[2]);
	FeNeed = atoi(argv[3]);
	CoNeed = atoi(argv[4]);

	int BLT[4][3]={0,}; // conveyer belt
	
	struct param *p = (struct param *) malloc(64);

	p->Fe = Fe;
	p->Co = Co;
	p->FeNeed = FeNeed;
	p->CoNeed = CoNeed;
	
	for(int i=0;i<4;i++)
		for(int j=0;j<3;j++)
			p->BLT[i][j]= 0;
	

	printf("Fe = %d, Co = %d, FeNeed = %d, CoNeed = %d",Fe,Co,FeNeed,CoNeed);
        printf("implement factorii !\n");
	
	//tid_t thread1 = thread_create("inbae",0,&dummy_func,0);
	//tid_t thread2 = thread_create("inbae2",0,&dummy_func2,0);
	//
	
	//tid_t th_blt = thread_create("blt",0,&beltmove,p);
//	tid_t th_ma1 = thread_create("machine_arm1",&armwork0,p);
}
