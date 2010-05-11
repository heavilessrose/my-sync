/*
 *  Converter.h
 *  SpeakHere
 *
 *  Created by  apple on 08-12-15.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include <stdio.h>

extern "C" void conv65( unsigned char * c, unsigned char * d);
extern "C" void conv66(unsigned char * d, unsigned char * c);
extern "C" int PcmToGsm(const char *pInPut,const char *pOutName);