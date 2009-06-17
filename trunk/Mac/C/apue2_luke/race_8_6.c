/*
 *  race_8_6.c
 *
 *  Created by wang luke on 6/17/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */

#include "apue.h"

static void charatatime(char *);

int main(int argc, const char* argv[]){
	pid_t pid;
	
	if((pid = fork()) < 0){
		err_sys("fork error");
	} else if(pid == 0){
		charatatime("output from child\n");
	} else {
		charatatime("output from parent\n");
	}
	return 0;
}

static void charatatime(char *str){
	char *ptr;
	int c;
	
	setbuf(stdout, NULL);// 设置不带缓冲
	for (ptr = str; (c = *ptr++) !=0;) {
		putc(c, stdout);
	}
}