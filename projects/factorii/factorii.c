#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"

#include "devices/timer.h"
#include "threads/interrupt.h"

struct data {
  int initFe, initCo, Fe, Co, FeNeed, CoNeed, FeOre, CoOre, inputFe, inputCo, CoFe, Target;
  int *blt0, *blt1, *blt2, *blt3;
  struct semaphore *b0,*b1,*b2_left, *b2_right,*b3_left, *b3_right,*fn0,*fn1,*fn2_Fe,*fn2_Co, *fn0_left, *fn1_left;
  struct semaphore *delay0, *delay1, *delay2, *delay3, *delay4, *delay5, *delay6, *delay7, *delay8, *delay9, *delay10, *delay11;
  bool fn0_task, fn1_task, fn2_task, fn0_done, fn1_done, fn2_done;
};

int64_t ticks = TIMER_FREQ;

void arm0(struct data* d)
{
	int64_t start;
	ASSERT(intr_get_level() == INTR_ON);

	int deadcount=0;
  timer_msleep(1000);
  //arm function
  printf("arm0 thread start!\n");
  sema_down(d->delay11);
  while(1)
  {
	start = timer_ticks();
  thread_sleep(start+ticks/2);
    if(d->CoFe == d->Target) break;
    sema_down(d->b0);
    sema_down(d->fn0_left);
    printf("Arm0 works!\n");
    d->blt0[2] = 0;
	deadcount++;
    d->fn0_task = true;
    if(d->fn0_task == true) sema_up(d->delay9);
    sema_up(d->fn0_left);
    sema_up(d->b0);
    if(d->blt0[2]==0) sema_down(d->delay11);
	if(deadcount == d->initFe){
		sema_up(d->delay9);
		sema_up(d->fn0_left);
		printf("**Arm0 DEAD.\n");
		break;
	}
    //timer_msleep(1000);
	thread_sleep(start+ticks);
  }

}

void arm1(struct data* d)
{
  int64_t start;
  ASSERT(intr_get_level() == INTR_ON);
  int deadcount = 0;
  timer_msleep(1000);

  //arm function
  printf("arm0 thread start!\n");
  sema_down(d->delay10);
  while(1)
  {
	start = timer_ticks();
  thread_sleep(start+ticks/2);
    if(d->CoFe == d->Target) break;
    sema_down(d->b1);
    sema_down(d->fn1_left);
    printf("Arm1 works!\n");
    d->blt1[2] = 0;
	deadcount++;
    d->fn1_task = true;
    if(d->fn1_task == true) sema_up(d->delay8);
    sema_up(d->fn1_left);
    sema_up(d->b1);
    if(d->blt1[2]==0) sema_down(d->delay10);
	if(deadcount == d->initCo){
		sema_up(d->delay8);
		sema_up(d->fn1_left);
		printf("**Arm1 DEAD.\n");
		break;
	}
    thread_sleep(start+ticks);
  }

}


void arm2(struct data* d)
{
  int64_t start;
  ASSERT(intr_get_level() == INTR_ON);
  int deadcount = 0;
  bool m;
  timer_msleep(1000);

  printf("arm2 thread start!\n");
  sema_down(d->delay7);
  while(1)
  {
	start = timer_ticks();
  thread_sleep(start+ticks/2);
    if(d->CoFe == d->Target) break;
    sema_down(d->fn0);
    //sema_down(d->b2_left);
	if(d->blt2[0] == 0){
    d->blt2[0] = 1;
	deadcount++;
    printf("Arm2 works!\n");
    d->fn0_done = false;
    m = d->fn0_done;
    //if(d->blt2[0]==1) sema_up(d->delay5);
	}
    //sema_up(d->b2_left);
    sema_up(d->fn0);
    if(m == false) sema_down(d->delay7);
	if(deadcount == d->initFe){
		//sema_up(d->delay5);
		//sema_up(d->b2_left);
		printf("**Arm2 DEAD.\n");
		break;
	}
	thread_sleep(start+ticks);
  }
}

void arm3(struct data* d)
{
  int64_t start;
  ASSERT(intr_get_level() == INTR_ON);
  int deadcount = 0;
  bool m;
  timer_msleep(1000);

  printf("arm3 thread start!\n");
  sema_down(d->delay6);
  while(1)
  {
    start = timer_ticks();
    thread_sleep(start+ticks/2);
    if(d->CoFe == d->Target) break;
    sema_down(d->fn1);
    //sema_down(d->b3_left);
	if(d->blt3[0] == 0){
    d->blt3[0] = 1;
    printf("Arm3 works!\n");
    d->fn1_done = false;
    m = d->fn1_done;
	deadcount++;
    //if(d->blt3[0] == 1) sema_up(d->delay4);
	}
    //sema_up(d->b3_left);
    sema_up(d->fn1);
    if(m == false) sema_down(d->delay6);
	if(deadcount == d->initCo){
		//sema_up(d->delay4);
		//sema_up(d->b3_left);
		printf("**Arm3 DEAD.\n");
		break;
	}
    thread_sleep(start+ticks);
  }

}


void arm4(struct data* d)
{
  int64_t start;
  ASSERT(intr_get_level() == INTR_ON);
  int m;
  timer_msleep(1000);
  //arm function
  printf("arm4 thread start!\n");
  sema_down(d->delay3);
  while(1)
  {
    start = timer_ticks();
    thread_sleep(start+ticks/2);
    if(d->CoFe == d->Target) break;
    sema_down(d->b2_right);
    sema_down(d->fn2_Fe);
    if(d->CoFe == d->Target) break;
	printf("Arm4 works!\n");
    d->blt2[2] = 0;
	d->FeOre--;
    m=d->blt2[2];
    d->inputFe++;
    //(d->fn0_task) = true;
    if(d->inputFe == d->FeNeed) sema_up(d->delay1);
    sema_up(d->fn2_Fe);
    sema_up(d->b2_right);
    if(m==0) sema_down(d->delay3);
    thread_sleep(start+ticks);

  }

}

void arm5(struct data* d)
{
  int64_t start;
  ASSERT(intr_get_level() == INTR_ON);
  int m;
  timer_msleep(1000);
  //arm function
  printf("arm5 thread start!\n");
  sema_down(d->delay2);
  while(1)
  {
    start = timer_ticks();
    thread_sleep(start+ticks/2);
    if(d->CoFe == d->Target) break;
    sema_down(d->b3_right);
    sema_down(d->fn2_Co);
    if(d->CoFe == d->Target) break;
    printf("Arm5 works!\n");
    d->blt3[2] = 0;
	d->CoOre--;
    m=d->blt3[2];
    d->inputCo++;
    if(d->inputCo == d->CoNeed) sema_up(d->delay0);
    sema_up(d->fn2_Co);
    sema_up(d->b3_right);
    if(m==0) sema_down(d->delay2);
    thread_sleep(start+ticks);

  }

}

void furnace0(struct data* d)
{
  int64_t start;
  ASSERT(intr_get_level() == INTR_ON);
  int deadcount = 0;
  bool m;
  timer_msleep(1000);
  printf("furnace0 thread start!\n");
  //furnace0
  sema_down(d->delay9);
  while(1)
  {
    start = timer_ticks();
    thread_sleep(start+ticks/2);
    if(d->CoFe == d->Target) break;
    sema_down(d->fn0_left);
    sema_down(d->fn0);
    printf("furnace0 works!\n");
    timer_msleep(1000);
    (d->FeOre)++;
	deadcount++;
    d->fn0_done = true;
    d->fn0_task = false;
    m=d->fn0_task;
    printf("furnace0 work complete!\n");
    if(d->fn0_done == true) sema_up(d->delay7);
    sema_up(d->fn0);
    sema_up(d->fn0_left);
    if(m == false) sema_down(d->delay9);
	if(deadcount == d->initFe){
		sema_up(d->delay7);
		sema_up(d->fn0);
		printf("**Furnance0 DEAD.\n");
		break;
	}
    thread_sleep(start+ticks);

  }
}

void furnace1(struct data* d)
{
  int64_t start;
  ASSERT(intr_get_level() == INTR_ON);
  int deadcount = 0;
  bool m;
  timer_msleep(1000);
  printf("furnace1 thread start!\n");
  //furnace0
  sema_down(d->delay8);
  while(1)
  {
    start = timer_ticks();
    thread_sleep(start+ticks/2);
    if(d->CoFe == d->Target) break;
    sema_down(d->fn1_left);
    sema_down(d->fn1);
    printf("furnace1 works!\n");
    timer_msleep(1000);
    (d->CoOre)++;
	deadcount++;
    d->fn1_done = true;
    d->fn1_task = false;
    m=d->fn1_task;
    printf("furnace1 work complete!\n");
    if(d->fn1_done == true) sema_up(d->delay6);
    sema_up(d->fn1);
    sema_up(d->fn1_left);
    if(m == false) sema_down(d->delay8);
	if(deadcount == d->initCo){
		sema_up(d->delay6);
		sema_up(d->fn1);
		printf("**Furnance1 DEAD.\n");
		break;
	}
    thread_sleep(start+ticks);
  }
}

void furnace2(struct data* d)
{
  int64_t start;
  ASSERT(intr_get_level() == INTR_ON);
  int m1,m2;
  timer_msleep(1000);
  //furnace2
  printf("furnace2 thread start!\n");
  sema_down(d->delay0);
  sema_down(d->delay1);
  while(1)
  {
    start = timer_ticks();
    thread_sleep(start+ticks/2);
    if(d->CoFe == d->Target) break;
      sema_down(d->fn2_Fe);
      sema_down(d->fn2_Co);

  	  if(d->inputFe == d->FeNeed && d->inputCo == d->CoNeed)
  	  {
        printf("Furnace2 Works!\n");
        timer_msleep(3000);
        (d->CoFe)++;
        d->inputFe = 0;
        d->inputCo = 0;
        m1 = d->inputFe;
        m2 = d->inputCo;
        printf("furnace2 work complete!\n");
		printf("TARGET :%d, CURRENT : %d\n",d->Target,d->CoFe);
	  }
        sema_up(d->fn2_Fe);
        sema_up(d->fn2_Co);
    if(d->CoFe == d->Target) break;
        if(m1 == 0) sema_down(d->delay1);
        if(m2 == 0) sema_down(d->delay0);
      thread_sleep(start+ticks);
  }
}

void belt0(struct data* d)
{
  int64_t start, start2;
  ASSERT(intr_get_level() == INTR_ON);
  timer_msleep(1000);
  int deadcount = d->Fe + 3;

  //belt function
  printf("belt thread start!\n");
  while(1)
  {
    start = timer_ticks();
    thread_sleep(start+ticks/2);
    if(d->CoFe == d->Target) break;
    //belt0
    sema_down(d->b0);
    while(1)
    {
      start2 = timer_ticks();
      thread_sleep(start2+ticks/2);
      //printf("BELT0 : [%d] [%d] [%d]\n",d->blt0[0],d->blt0[1],d->blt0[2]);
      if(d->blt0[2] == 0)
      {
		deadcount--;
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
      if(d->blt0[2] == 1) break;
      else{
        if(deadcount >-1) printf("BELT0 : [%d] [%d] [%d]\n",d->blt0[0],d->blt0[1],d->blt0[2]);
		if(deadcount <= 0){
			sema_up(d->delay11);
			sema_up(d->b0);
			printf("**belt0 DEAD.\n");
			break;
		}
    if(d->CoFe == d->Target) break;
        thread_sleep(start2+ticks);
      }
    }
	if(deadcount <= 0) break;
    printf("BELT0 : [%d] [%d] [%d]\n",d->blt0[0],d->blt0[1],d->blt0[2]);
    if(d->blt0[2] == 1){
		sema_up(d->delay11);
	}
    sema_up(d->b0);


    thread_sleep(start+ticks);
  }
}

void belt1(struct data* d)
{
  int64_t start, start2;
  ASSERT(intr_get_level() == INTR_ON);
  int deadcount = d->Co + 3;
  timer_msleep(1000);

  //belt function
  printf("belt thread start!\n");
  while(1)
  {
    start = timer_ticks();
    thread_sleep(start+ticks/2);
    if(d->CoFe == d->Target) break;
    //belt0
    sema_down(d->b1);
    while(1)
    {
      start2 = timer_ticks();
      thread_sleep(start2+ticks/2);
      //printf("BELT0 : [%d] [%d] [%d]\n",d->blt0[0],d->blt0[1],d->blt0[2]);
      if(d->blt1[2] == 0)
      {
		deadcount--;
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
      if(d->blt1[2] == 1) break;
      else{
        if(deadcount > -1) printf("BELT1 : [%d] [%d] [%d]\n",d->blt1[0],d->blt1[1],d->blt1[2]);
		if(deadcount <= 0){
			sema_up(d->delay10);
			sema_up(d->b1);
			printf("**belt1 DEAD.\n");
			break;
		}
    if(d->CoFe == d->Target) break;
        thread_sleep(start2+ticks);
      }
    }
	if(deadcount<=0) break;
    printf("BELT1 : [%d] [%d] [%d]\n",d->blt1[0],d->blt1[1],d->blt1[2]);
    if(d->blt1[2] == 1){
		sema_up(d->delay10);
	}
    sema_up(d->b1);

    thread_sleep(start+ticks);
  }
}

void belt2(struct data* d)
{
  int64_t start;
  ASSERT(intr_get_level() == INTR_ON);
  int m;
  bool deadflag = false;
  timer_msleep(1000);

  printf("blet2 thread start!\n");
  //sema_down(d->delay5);
  while(1)
  {
    start = timer_ticks();
    thread_sleep(start+ticks/2);
    if(d->CoFe == d->Target) break;
    //sema_down(d->b2_left);
    sema_down(d->b2_right);
    if(d->CoFe == d->Target) break;
	if(d->blt2[2] == 0)
    {
      d->blt2[2] = d->blt2[1];
      d->blt2[1] = d->blt2[0];
      d->blt2[0] = 0;
      m = d->blt2[0];
	  if(d->blt2[2] == 1 && d->blt2[1] + d->blt2[0] == 0) deadflag = true;
	}
    printf("BELT2 : [%d] [%d] [%d]\n",d->blt2[0],d->blt2[1],d->blt2[2]);
    if(d->blt2[2] == 1 && d->inputFe < d->FeNeed)
	{
		sema_up(d->delay3);
	}

	sema_up(d->b2_right);
    //sema_up(d->b2_left);
    //if(m==0) sema_up(d->b2_left);
    //if(m==0) sema_down(d->delay5);
    thread_sleep(start+ticks);
  }
}

void belt3(struct data* d)
{
  int64_t start;
  ASSERT(intr_get_level() == INTR_ON);
  int m;
  bool deadflag = false;
  timer_msleep(1000);

  printf("blet3 thread start!\n");
  //sema_down(d->delay4);
  while(1)
  {
    start = timer_ticks();
    thread_sleep(start+ticks/2);
    if(d->CoFe == d->Target) break;
    //sema_down(d->b3_left);
    sema_down(d->b3_right);
    if(d->CoFe == d->Target) break;
	if(d->blt3[2] == 0)
    {
      d->blt3[2] = d->blt3[1];
      d->blt3[1] = d->blt3[0];
      d->blt3[0] = 0;
      m = d->blt3[0];
	  if(d->blt3[2] == 1 && d->blt3[1] + d->blt3[0] == 0) deadflag = true;
	}
    printf("BELT3 : [%d] [%d] [%d]\n",d->blt3[0],d->blt3[1],d->blt3[2]);
    if(d->blt3[2] == 1 && d->inputCo < d->CoNeed) sema_up(d->delay2);
    sema_up(d->b3_right);

    //sema_up(d->b3_left);
    //if(m==0) sema_down(d->delay4);
    thread_sleep(start+ticks);
  }

}



void print(struct data* d)
{
  int64_t start;
  ASSERT(intr_get_level() == INTR_ON);
  timer_msleep(1000);

  int i=0;
  while(1)
  {

    start = timer_ticks();
    if(d->CoFe == d->Target) break;
    printf("X X X X X X X X X X\n");
    for(int i=0; i<3; i++)
    {
      if(d->blt0[i] == 1) printf("o");
      else printf(" ");
      printf(" ");
    }
    printf("@ ");
	if(d->fn0_task) printf("o");
	else printf("-");
	printf(" @ ");
    for(int i=0; i<3; i++)
    {
      if(d->blt2[i] == 1) printf("i");
      else printf(" ");
      printf(" ");
    }
    printf("@\n");
    printf("X X X X X X X X X -\n");
    for(int i=0; i<3; i++)
    {
      if(d->blt1[i] == 1) printf("o");
      else printf(" ");
      printf(" ");
    }
    printf("@ ");
	if(d->fn1_task) printf("o");
	else printf("-");
	printf(" @ ");
    for(int i=0; i<3; i++)
    {
      if(d->blt3[i] == 1) printf("i");
      else printf(" ");
      printf(" ");
    }
    printf("@\n");
    printf("X X X X X X X X X X\n");
    printf("\n");
    printf("###################\n");
    printf("## T = %d   ##\n",i++);
    printf("###################\n");
    printf("FeOre=%d, CoOre=%d, FeIng=%d, CoIng=%d, CoFe=%d\n",d->Fe,d->Co,d->FeOre,d->CoOre,d->CoFe);
    printf("\n");
    thread_sleep(start+ticks);
  }
  printf("@@@@@@@@@ Result @@@@@@@@@@@@\n");
  printf("FeOre=%d, CoOre=%d, FeIng=%d, CoIng=%d, CoFe=%d\n",d->Fe,d->Co,d->FeOre,d->CoOre,d->CoFe);
}







void run_factorii(char **argv)
{
  struct data* d;
  d = (struct data*) malloc(sizeof(struct data));
  int Fe, Co, FeNeed, CoNeed, FeOre, CoOre, inputFe, inputCo, CoFe, Target, initFe, initCo;
  int *blt0, *blt1, *blt2, *blt3;
  struct semaphore *b0,*b1,*b2_left, *b2_right,*b3_left, *b3_right,*fn0,*fn1,*fn2_Fe,*fn2_Co,*fn0_left, *fn1_left;
  struct semaphore *delay0, *delay1, *delay2, *delay3, *delay4, *delay5, *delay6, *delay7, *delay8, *delay9, *delay10, *delay11;
  bool fn0_task, fn1_task, fn2_task, fn0_done, fn1_done, fn2_done;

  printf("%d %d",sizeof(struct data),sizeof(struct semaphore));

  Fe = atoi(argv[1]);
  Co = atoi(argv[2]);
  FeNeed = atoi(argv[3]);
  CoNeed = atoi(argv[4]);
  FeOre = 0;
  CoOre = 0;
  CoFe = 0;
  initFe = Fe;
  initCo = Co;
  if(Fe/FeNeed > Co/CoNeed)
	  Target = Co/CoNeed;
  else
	  Target = Fe/FeNeed;


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
  fn0_left = (struct semaphore *) malloc(sizeof(struct semaphore));
  fn1_left = (struct semaphore *) malloc(sizeof(struct semaphore));
  fn2_Fe = (struct semaphore *) malloc(sizeof(struct semaphore));
  fn2_Co = (struct semaphore *) malloc(sizeof(struct semaphore));
  delay0 = (struct semaphore *) malloc(sizeof(struct semaphore));
  delay1 = (struct semaphore *) malloc(sizeof(struct semaphore));
  delay2 = (struct semaphore *) malloc(sizeof(struct semaphore));
  delay3 = (struct semaphore *) malloc(sizeof(struct semaphore));
  delay4 = (struct semaphore *) malloc(sizeof(struct semaphore));
  delay5 = (struct semaphore *) malloc(sizeof(struct semaphore));
  delay6 = (struct semaphore *) malloc(sizeof(struct semaphore));
  delay7 = (struct semaphore *) malloc(sizeof(struct semaphore));
  delay8 = (struct semaphore *) malloc(sizeof(struct semaphore));
  delay9 = (struct semaphore *) malloc(sizeof(struct semaphore));
  delay10 = (struct semaphore *) malloc(sizeof(struct semaphore));
  delay11 = (struct semaphore *) malloc(sizeof(struct semaphore));


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
  sema_init(fn0_left,1);
  sema_init(fn1,1);
  sema_init(fn1_left,1);
  sema_init(fn2_Fe,1);
  sema_init(fn2_Co,1);
  sema_init(delay0,0);
  sema_init(delay1,0);
  sema_init(delay2,0);
  sema_init(delay3,0);
  sema_init(delay4,0);
  sema_init(delay5,0);
  sema_init(delay6,0);
  sema_init(delay7,0);
  sema_init(delay8,0);
  sema_init(delay9,0);
  sema_init(delay10,0);
  sema_init(delay11,0);

  for(int i=0; i<3; i++)
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
  d->inputFe = 0;
  d->inputCo = 0;
  d->initFe = initFe;
  d->initCo = initCo;

  d->b0 = b0;
  d->b1 = b1;
  d->b2_left = b2_left;
  d->b2_right = b2_right;
  d->b3_left = b3_left;
  d->b3_right = b3_right;
  d->fn0 = fn0;
  d->fn1 = fn1;
  d->fn0_left = fn0_left;
  d->fn1_left = fn1_left;
  d->fn2_Fe = fn2_Fe;
  d->fn2_Co = fn2_Co;
  d->delay0 = delay0;
  d->delay1 = delay1;
  d->delay2 = delay2;
  d->delay3 = delay3;
  d->delay4 = delay4;
  d->delay5 = delay5;
  d->delay6 = delay6;
  d->delay7 = delay7;
  d->delay8 = delay8;
  d->delay9 = delay9;
  d->delay10 = delay10;
  d->delay11 = delay11;


  d->fn0_task = fn0_task;
  d->fn1_task = fn1_task;
  d->fn2_task = fn2_task;

  d->fn0_done = fn0_done;
  d->fn1_done = fn1_done;
  d->fn2_done = fn2_done;

  d->CoFe = CoFe;
  d->Target = Target;


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
  thread_create("print",0,&print,d);




}
