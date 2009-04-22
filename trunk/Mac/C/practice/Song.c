/*
 *  Song.c
 *  practice
 *
 *  Created by wang luke on 4/22/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */

#include <stdio.h>
#include "Song.h"

Song make_song(int seconds, int year)
{
	Song newSong;
	
	newSong.lengthInSeconds = seconds;
	newSong.yearRecorded = year;
	display_song(newSong);
	
	return newSong;
}

void display_song(Song theSong)
{
	printf ("the song is %i seconds long ", theSong.lengthInSeconds);
	printf ("and was made in %i\n", theSong.yearRecorded);
}
