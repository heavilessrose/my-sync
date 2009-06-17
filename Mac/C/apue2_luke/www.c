/*
 *  www.c
 *  apue2_luke
 *
 *  Created by luke on 6/13/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */
#include "apue.h"

extern int glob;

extern void pppp(){
	printf("pid:%d = %d\n", getpid(), glob);
}