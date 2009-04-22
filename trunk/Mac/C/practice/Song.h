/*
 *  Song.h
 *  practice
 *
 *  Created by wang luke on 4/22/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */

typedef struct Song{
	int lengthInSeconds;
	int yearRecorded;
} Song;

Song make_song(int seconds, int year);
void display_song(Song theSong);