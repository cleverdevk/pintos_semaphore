#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"

#include "devices/timer.h"



void belt_function(struct param* p)
{
	//belt0
	while(1)
	{


void run_factorii(char **argv)
{

	struct param {
                int* Fe, Co, FeNeed, CoNeed;
                int blt[4][3];
                struct semaphore sema_belt[4];
        };

	struct param* param_belt;

	for(int i=0;i<4;i++){
		param_belt->sema_init(&sema[i],3);
		for(int j=0;j<3;j++){
			param_belt->blt[i][j] = 0;
		}
	}

	*(param_belt->Fe) = atoi(argv[1]);
	*(param_belt->Co) = atoi(argv[2]);
	*(param_belt->FeNeed) = atoi(argv[3]);
	*(param_belt->CoNeed) = atoi(argv[4]);
	


}
