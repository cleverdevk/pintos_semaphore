#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"

#include "devices/timer.h"

struct data {
	int Fe, Co, FeNeed, CoNeed, FeOre, CoOre, inputFe, inputCo;
	int *blt0, *blt1, *blt2, *blt3;
	struct semaphore *b0,*b1,*b2_left, *b2_right,*b3_left, *b3_right,*fn0,*fn1,*fn2_Fe,*fn2_Co,*fn0_complete,*fn1_complete;
	bool fn0_task, fn1_task, fn2_task, fn0_done, fn1_done, fn2_done;
};

void arm0(struct data* d)
{
	timer_msleep(1000);
	//arm function
	printf("arm0 thread start!\n");
        while(1)
        {
			sema_down(d->fn0);
			sema_down(d->b0);
	 	    printf("Arm0 works!\n");
			d->blt0[2] = 0;
	        //(d->fn0_task) = true;
			sema_up(d->b0);
			sema_up(d->fn0);
			timer_msleep(1000);

        }

}

void arm1(struct data* d)
{
        timer_msleep(1000);

        //arm function
        printf("arm0 thread start!\n");
        while(1)
        {
                sema_down(d->fn1);
                sema_down(d->b1);
                printf("Arm1 works!\n");
                d->blt1[2] = 0;
				//(d->fn1_task) = true;
                sema_up(d->b1);
                sema_up(d->fn1);
                timer_msleep(1000);
        }

}

void arm2(struct data* d)
{
    timer_msleep(1000);

	printf("arm2 thread start!\n");
        while(1)
        {
			sema_down(d->b2_left);
			if(d->blt2[0]==0 && d->fn0_done == true)
			{	
				d->blt2[0] = 1;
				printf("Arm2 works!\n");
				d->fn0_done = false;
				sema_up(d->b2_left);
			}
			timer_msleep(1000);
        }
}

void arm3(struct data* d)
{
        timer_msleep(1000);

        printf("arm3 thread start!\n");
        while(1)
        {
                sema_down(d->b3_left);
                if(d->blt3[0]==0 && d->fn1_done == true)
                {
                        d->blt3[0] = 1;
                        printf("Arm3 works!\n");
                        d->fn1_done = false;
                        sema_up(d->b3_left);
                }
                timer_msleep(1000);
        }

}


void arm4(struct data* d)
{
	int current = 0;
	timer_msleep(1000);
	//arm function
	printf("arm4 thread start!\n");
        while(1)
        {
			sema_down(d->fn2_Fe);
			sema_down(d->b2_right);
	 	    printf("Arm4 works!\n");
			d->blt2[2] = 0;
			current++;
	        //(d->fn0_task) = true;
			sema_up(d->b2_right);
			if(current % d->FeNeed == 0) sema_up(d->fn2_Fe);
			timer_msleep(1000);

        }

}

void arm5(struct data* d)
{
	int current = 0;
	timer_msleep(1000);
	//arm function
	printf("arm5 thread start!\n");
        while(1)
        {
			sema_down(d->fn2_Co);
			sema_down(d->b3_right);
	 	    printf("Arm5 works!\n");
			d->blt2[2] = 0;
			current++;
	        //(d->fn0_task) = true;
			sema_up(d->b3_right);
			if(current % d->CoNeed == 0) sema_up(d->fn2_Co);
			timer_msleep(1000);

        }

}

void furnace0(struct data* d)
{
    timer_msleep(1000);
	printf("furnace0 thread start!\n");
	//furnace0
	while(1)
	{
		if(d->fn0_done==false)
		{
			//sema_down(d->fn0_complete);			
			sema_down(d->fn0);
			printf("furnace0 works!\n");
			timer_msleep(1000);
			(d->FeOre)++;
			d->fn0_done = true;
			printf("furnace0 work complete!\n");
			(d->fn0_task) = false;
			//sema_up(d->fn0_complete);
			sema_up(d->fn0);
			timer_msleep(1000);

		}
	}
}

void furnace1(struct data* d)
{
    timer_msleep(1000);
    printf("furnace1 thread start!\n");
    //furnace0
    while(1)
    {
        if(d->fn1_done==false)
        {
            //sema_down(d->fn1_complete);                   
            sema_down(d->fn1);
            printf("furnace1 works!\n");
            timer_msleep(1000);
			(d->CoOre)++;
            d->fn1_done = true;
            printf("furnace1 work complete!\n");
            (d->fn1_task) = false;
            //sema_up(d->fn1_complete);
            sema_up(d->fn1);
            timer_msleep(1000);
        }
    }
}

void furnace2(struct data* d)
{
    timer_msleep(1000);
    //furnace2
	printf("furnace2 thread start!\n");
	while(1)
	{
		sema_down(d->fn2_Fe);
		sema_down(d->fn2_Co);
		printf("Furnace2 Works!\n");
		timer_msleep(3000);
		//(d->CoFe)++;
		d->fn2_done = true;
		printf("furnace2 work complete!\n");
		sema_up(d->fn2_Co);
		sema_up(d->fn2_Fe);
		timer_msleep(1000);
	}
}

void belt0(struct data* d)
{
    timer_msleep(1000);

	//belt function
	printf("belt thread start!\n");
	while(1)
	{
		//belt0
		sema_down(d->b0);
		while(1)
		{
			printf("BELT0 : [%d] [%d] [%d]\n",d->blt0[0],d->blt0[1],d->blt0[2]);
			if(d->blt0[2] == 0)
			{
				d->blt0[2] = d->blt0[1];
				d->blt0[1] = d->blt0[0];
				if((d->Fe) > 0)
				{
					d->blt0[0] = 1;
					(d->Fe)--;
				}
				else
				{
					d->blt0[0] = 0;
				}
			}
			if(d->blt0[2]==1){
				sema_up(d->b0);
				break;
			}
			else
				timer_msleep(1000);
		}
		timer_msleep(1000);
	}

}

void belt1(struct data* d)
{
    timer_msleep(1000);
    //belt function
    printf("belt1 thread start!\n");
    while(1)
    {
        //belt0
        sema_down(d->b1);
        while(1)
        {
            printf("BELT1 : [%d] [%d] [%d]\n",d->blt1[0],d->blt1[1],d->blt1[2]);
            if(d->blt1[2] == 0)
            {
	            d->blt1[2] = d->blt1[1];
				d->blt1[1] = d->blt1[0];
				if((d->Co) > 0)
                {
					d->blt1[0] = 1;
					(d->Co)--;
				}
				else
				{
					d->blt1[0] = 0;
				}
			}
			if(d->blt1[2]==1){
				sema_up(d->b1);
				break;
			}
			else
				timer_msleep(1000);
		}
		timer_msleep(1000);
	}
}

void belt2(struct data* d)
{
	timer_msleep(1000);

	printf("blet2 thread start!\n");
	while(1)
	{
		sema_down(d->b2_left);
		sema_down(d->b2_right);
		if(d->blt2[2] == 0)
		{
			d->blt2[2] = d->blt2[1];
			d->blt2[1] = d->blt2[0];
			d->blt2[0] = 0;
		}
		if(d->blt2[0]==0) sema_up(d->b2_left);
		if(d->blt2[2]==1) sema_up(d->b2_right);
		printf("BELT2 : [%d] [%d] [%d]\n",d->blt2[0],d->blt2[1],d->blt2[2]);
		sema_up(d->b2_left);
		timer_msleep(1000);
	}


}

void belt3(struct data* d)
{
	timer_msleep(1000);

	printf("blet3 thread start!\n");
	while(1)
	{
		sema_down(d->b3_left);
		sema_down(d->b3_right);
		if(d->blt3[2] == 0)
		{
			d->blt3[2] = d->blt3[1];
			d->blt3[1] = d->blt3[0];
			d->blt3[0] = 0;
		}
		if(d->blt3[0]==0) sema_up(d->b3_left);
		if(d->blt3[2]==1) sema_up(d->b3_right);
		printf("BELT3 : [%d] [%d] [%d]\n",d->blt3[0],d->blt3[1],d->blt3[2]);
		sema_up(d->b3_left);
		timer_msleep(1000);
	}
	
}



void print(struct data* d)
{
        timer_msleep(1000);

	int i=0;
	while(1)
	{
		printf("X X X X X X X X X X\n");
		for(int i=0;i<3;i++)
		{
			if(d->blt0[i] == 1) printf("o");
			else printf(" ");
			printf(" ");
		}
		printf("@ - @ ");
		for(int i=0;i<3;i++)
	        {
	                if(d->blt2[i] == 1) printf("o");
	                else printf(" ");
	                printf(" ");
	        }
		printf("@\n");
		printf("X X X X X X X X X -\n");
		for(int i=0;i<3;i++)
	        {
        	        if(d->blt1[i] == 1) printf("o");
                	else printf(" ");
                	printf(" ");
	        }
	        printf("@ - @ ");
	        for(int i=0;i<3;i++)
	        {
	                if(d->blt3[i] == 1) printf("o");
	                else printf(" ");
	                printf(" ");
	        }
		printf("@\n");
		printf("X X X X X X X X X X\n");
		printf("\n");	
		printf("###################\n");
		printf("##    T = %d      ##\n",i++);
		printf("###################\n");
		printf("FeOre=%d, CoOre=%d, FeIng=%d, CoIng=%d, CoFe=%d\n",d->Fe,d->Co,0,0,0);
		printf("\n");
		timer_msleep(1100);
	}
}



	



void run_factorii(char **argv)
{

	int Fe, Co, FeNeed, CoNeed, FeOre, CoOre;
	struct data* d;
	d = (struct data*) malloc(sizeof(struct data));
	int *blt0, *blt1, *blt2, *blt3;
	struct semaphore *b0, *b1, *b2_left,*b2_right, *b3_left,*b3_right, *fn0, *fn1, *fn2_Fe, *fn2_Co, *fn0_complete, *fn1_complete;
	bool fn0_task, fn1_task, fn2_task, fn0_done, fn1_done, fn2_done;

	//printf("%d",sizeof(struct data));

	Fe = atoi(argv[1]);
	Co = atoi(argv[2]);
	FeNeed = atoi(argv[3]);
	CoNeed = atoi(argv[4]);
	FeOre = 0;
	CoOre = 0;


	blt0 = (int *)malloc(12);
	blt1 = (int *)malloc(12);
	blt2 = (int *)malloc(12);
	blt3 = (int *)malloc(12);

	b0 = (struct semaphore *) malloc(sizeof(struct semaphore));
    b1 = (struct semaphore *) malloc(sizeof(struct semaphore));
    b2_left = (struct semaphore *) malloc(sizeof(struct semaphore));
	b2_right = (struct semaphore *) malloc(sizeof(struct semaphore));
	b3_left = (struct semaphore *) malloc(sizeof(struct semaphore));
    b3_right = (struct semaphore *) malloc(sizeof(struct semaphore));
    fn0 = (struct semaphore *) malloc(sizeof(struct semaphore));
    fn1 = (struct semaphore *) malloc(sizeof(struct semaphore));
    fn2_Fe = (struct semaphore *) malloc(sizeof(struct semaphore));
    fn2_Co = (struct semaphore *) malloc(sizeof(struct semaphore));
	fn0_complete = (struct semaphore *) malloc(sizeof(struct semaphore));
	fn1_complete = (struct semaphore *) malloc(sizeof(struct semaphore));


	fn0_task = false;
	fn1_task = false;
	fn2_task = false;
	fn0_done = false;
	fn1_done = false;
	fn2_done = false;

	sema_init(b0,1);
	sema_init(b1,1);
	sema_init(b2_left,1);
	sema_init(b2_right,1);
	sema_init(b3_left,1);
	sema_init(b3_right,1);
	sema_init(fn0,1);
	sema_init(fn1,1);
	sema_init(fn2_Fe,1);
	sema_init(fn2_Co,1);
	sema_init(fn0_complete,1);
	sema_init(fn1_complete,1);

	for(int i=0;i<3;i++)
	{
		blt0[i] = 0;
		blt1[i] = 0;
		blt2[i] = 0;
		blt3[i] = 0;
	}

	d->blt0 = blt0;
	d->blt1 = blt1;
	d->blt2 = blt2;
	d->blt3 = blt3;

	d->Fe = Fe;
	d->Co = Co;
	d->FeNeed = FeNeed;
	d->CoNeed = CoNeed;
	d->FeOre = FeOre;
	d->CoOre = CoOre;
	d->inputFe = Fe;
	d->inputCo = Co;

	d->b0 = b0;
	d->b1 = b1;
	d->b2_left = b2_left;
	d->b2_right = b2_right;
	d->b3_left = b3_left;
	d->b3_right = b3_right;
	d->fn0 = fn0;
	d->fn1 = fn1;
	d->fn2_Fe = fn2_Fe;
	d->fn2_Co = fn2_Co;
	d->fn0_complete = fn0_complete;
	d->fn1_complete = fn1_complete;

	d->fn0_task = fn0_task;
	d->fn1_task = fn1_task;
	d->fn2_task = fn2_task;

	d->fn0_done = fn0_done;
	d->fn1_done = fn1_done;
	d->fn2_done = fn2_done;


	thread_create("belt0",0,&belt0,d);
	thread_create("belt1",0,&belt1,d);
	thread_create("belt2",0,&belt2,d);
	thread_create("belt3",0,&belt3,d);
	thread_create("arm0",0,&arm0,d);
	thread_create("arm1",0,&arm1,d);
	thread_create("arm2",0,&arm2,d);
	thread_create("arm3",0,&arm3,d);
	thread_create("arm4",0,&arm4,d);
	thread_create("arm5",0,&arm5,d);
	thread_create("furnace0",0,&furnace0,d);
	thread_create("furnace1",0,&furnace1,d);
	thread_create("furnace2",0,&furnace2,d);
	//thread_create("print",0,&print,d);
	

}
