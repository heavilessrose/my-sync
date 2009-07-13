/*
 *  wk_sem.c
 *  TabiNavi
 *
 *  Created by wang luke on 7/13/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */

#include "wk_sem.h"

// �½�һ���ź���,�����ź����Ѵ�����ʧ�ܡ��ɹ�����semid�����󷵻أ�1
// ������ initval���ź�����ʼֵ
int sem_create(int initval){
	int semID;
	if((semID = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT | IPC_EXCL)) == -1){
		perror("sem_create");
		return -1;
	}
	
	// �ź�������ֵ
	if(semctl(semID, 0, GETVAL) == 0){
		union semun semctl_arg;
		semctl_arg.val = initval;
		if (semctl(semID, 0, SETVAL, semctl_arg) < 0){
			perror("sem_create");
			return -1;
		}
	}
	return semID;
}

// �ź�����һ
int sem_set(int id){
	if(semop(id, &op_set, sizeof(struct sembuf)) == -1){
		perror("sem_set");
		return -1;
	}
	return 0;
}

// �ź�����һ
int sem_get(int id){
	if(semop(id, &op_get, sizeof(struct sembuf)) == -1){
		perror("sem_get");
		return -1;
	}
	return 0;
}

// ɾ���ź���
int sem_rm(int id){
	if (semctl(id, 0, IPC_RMID) < 0){
		perror("sem_rm");
		return -1;
	}
	return 0;
}