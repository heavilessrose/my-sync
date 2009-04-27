/*
 *  Functions.h
 *  practice
 *
 *  Created by wang luke on 4/24/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */

#include <stdio.h>
#include <string.h>

void changeArray(int, int[]);
void showArray(int, int[]);

/////
typedef struct song{
	int lengthInSeconds;
	int yearRecorded;
} Song;
Song make_song(int seconds, int year);
void display_song(Song theSong);
/////

void swap(int *, int *);

/////
int arrayPointerTest();
void strSort(const char * [], int n);
void printPointerArray(const char *str[], int count);
