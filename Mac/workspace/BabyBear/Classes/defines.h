/*
 *  defines.h
 *  BabyBear
 *
 *  Created by luke on 10-8-11.
 *  Copyright 2010 Luke. All rights reserved.
 *
 */

#ifndef _BBEAR_DEFINE_H_
#define _BBEAR_DEFINE_H_


#ifdef __cplusplus
extern "C" {
#endif
	
typedef struct {
	UInt32	id;
	char	*name;
	Float32	rating;
	char	*desc;
	char	*icon_url;
}Review;

typedef struct {
	UInt32	id;
	char	*name;
	char	*addr;
	char	*desc;
	UInt32	reserves;
	double	longitude;
	double	latitude;
}Store;

typedef struct {
	UInt32	id;
	char	*name;
	UInt32	price;
	Float32	rating;
	char	*desc;
	char	*icon_url;
	char	**gallary;
	UInt32	photoCount;
	Review	*reviews;
	UInt32	reviewsCount;
	Store	*stores;
	UInt32	storesCount;
}Product;

typedef struct {
	Product *base;
	
}FavoriteItem;

typedef struct {
	Product *base;
	
}CartItem;
	
#ifdef __cplusplus
}
#endif

#endif /* _BBEAR_DEFINE_H_ */
