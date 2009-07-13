/*
 *  wk_sem.h
 *  TabiNavi
 *
 *  Created by wang luke on 7/13/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */

#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdio.h>

// 获取资源
struct sembuf op_get = {0, -1, SEM_UNDO};
// 释放资源
struct sembuf op_set = {0, 1, SEM_UNDO};

// 等待该信号量值变为0
struct sembuf op_end = {0, 0, SEM_UNDO};

int sem_create(int initval);
int sem_set(int id);
int sem_get(int id);
int sem_rm(int id);