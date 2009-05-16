/*
File: missile.h
Abstract: Missile model.

Version: 2.0
*/

#import "Utils.h"

@class Texture2D;

// PROTOTYPES:
void OGL_Set_Current_Texture_Map_Missile(char *texture_map_name, float u_scale, float v_scale, float u_offset, float v_offset);
void MissileRender();