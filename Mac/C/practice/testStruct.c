/*
 *  testStruct.c
 *  practice
 *
 *  Created by wang luke on 4/22/09.
 *  Copyright 2009 luke. All rights reserved.
 *
 */


#include <stdio.h>
#include "Song.h"

int main(int argc, const char *argv[])
{
	Song firstSong  = make_song (210, 2004);
	Song secondSong = make_song (256, 1992);
	
	Song thirdSong  = { 223, 1997 };
	display_song ( thirdSong );
	
	Song fourthSong = { 199, 2003 };
	return 0;
}