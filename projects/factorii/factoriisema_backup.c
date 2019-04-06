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
                int* blt1;
		int* blt2;
		int* blt3;
		int* blt0;
		struct semaphore* s,*n;
        };
struct param_arm {
	int* blt1;
	int* blt2;
	int* blt3;
	int* blt0;
	struct semaphore* s,*n;
};

void dummy_func()
{
}

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
	printf("ShiftBelt() Called!\n");
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

void arm0(struct param_arm* p)
{
	printf("arm0 Called!");
        for(int i=0;;i++)
        {
		if(p->blt0[2]==1)
                {
			printf("Arm0 Operated!\n");
                	//sema_down(p->n);
                        //sema_down(p->s);
                        p->blt0[2] = 0;
                        //sema_up(p->s);
                }
        }
}


//belt move
void beltmove(struct param* p)
{
	for(int i=0;;i++)
	{
		//printbelt(p->blt0);
		//belt0
		if(p->blt0[2]!=1)
		{
			//sema_down(p->s);
			shiftbelt(p->blt0,&(p->Fe));
			//sema_up(p->s);
			//sema_up(p->n);
		}
		//printf("Fe:%d, Co:%d\n",p->Fe,p->Co);
	}
}

void run_factorii(char **argv)
{

	printf("%d %d %d",sizeof(struct param),sizeof(struct param_arm), sizeof(struct lock)); 	
	int Fe, Co, FeNeed, CoNeed;


	Fe = atoi(argv[1]);
	Co = atoi(argv[2]);
	FeNeed = atoi(argv[3]);
	CoNeed = atoi(argv[4]);

	int* BLT[3]; // conveyer belt

	struct semaphore s,n;
	struct param *p = (struct param *) malloc(40);
	struct param_arm *pa = (struct param_arm *) malloc(24);
	sema_init(&s,1);
	sema_init(&n,0);

	for(int i=0;i<4;i++){
		BLT[i] = (int *) malloc(12);
		for(int j=0;j<3;j++){
			BLT[i][j] = 0;
		}
	}
	

	p->Fe = Fe;
	p->Co = Co;
	p->FeNeed = FeNeed;
	p->CoNeed = CoNeed;

	p->blt0 = BLT[0];
	p->blt1 = BLT[1];
	p->blt2 = BLT[2];
	p->blt3 = BLT[3];

	pa->blt0 = BLT[0];
	pa->blt1 = BLT[1];
	pa->blt2 = BLT[2];
	pa->blt3 = BLT[3];
	
	p->s = &s;
	pa->s = &s;
	p->n = &n;
	pa->n = &n;


	printf("Fe = %d, Co = %d, FeNeed = %d, CoNeed = %d",Fe,Co,FeNeed,CoNeed);
	

	//sema_self_test();

	tid_t thread2 = thread_create("inbae2",PRI_DEFAULT,&arm0,pa);
	tid_t thread1 = thread_create("inbae",PRI_DEFAULT,&beltmove,p);


	//tid_t th_blt = thread_create("blt",0,&beltmove,0);
	//tid_t th_ma1 = thread_create("arm0",0,&arm0,0);
}
