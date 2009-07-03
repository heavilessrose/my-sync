//
//  wk_fileapi.h
//  TabiNavi
//
//  Created by wang luke on 7/1/09.
//  Copyright 2009 luke. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <syslimits.h>


@interface wk_fileapi : NSObject {

}

// osal
/*打开指定的文件*/
FILE* Winks_fopen(const char *filename,const char *type);
/*打开指定的文件(unicode)*/
FILE* Winks_fopenUni(const unsigned short *filename,const char *type);
/*关闭一个文件*/
int Winks_fclose(FILE *stream);
/*测试是否到达指定文件的末尾*/
int Winks_feof(FILE *stream);
/*测试指定文件是否存在,如果存在返回0，否则-1*/
int Winks_fexist(char *filename);
/*从所给文件stream中读取的count个数据数据单元，每一个数据单元的长度为size字节，到由data所指的块中*/
size_t Winks_fread(void *data, size_t size, size_t count, FILE *stream);
/*从指针data开始把count个数据单元项添加到给定文件stream中，每个数据单元的长度为size个字节*/
size_t Winks_fwrite(const void *data, size_t size, size_t count, FILE *stream);
/*本函数返回文件fp 的指针偏移位(offset) 值。成功偏移量，失败返回-1。*/
int Winks_ftell(FILE *stream);

/*将文件指针移到指定位*/
#define WINKS_SEEK_SET 0 /* Seek from beginning of file.  */
#define WINKS_SEEK_CUR 1 /* Seek from current position.  */
#define WINKS_SEEK_END 2 /* Set file pointer to EOF plus "offset" */
int Winks_fseek(FILE *stream, long offset, int fromwhere);

/*删除指定的文件。成功返回0，失败返回-1。*/
int Winks_fdelete(char *file_name);
/*更新指定文件的文件名。成功返回0，失败返回-1。*/
int Winks_frename(char *old_file_name, char *new_file_name);
/* 获取文件大小, 返回负值表示失败 */
int Winks_fgetsize(const char *filename, unsigned int is_ascii, unsigned int *size);
/*取文件大小，成功返回0*/
int Winks_fGetFileSize(FILE *stream, unsigned long *size);
/*获取当前路径。成功返回0，失败返回-1。*/
int Winks_getcwd(char *path_name_ptr, size_t size);
/*设置当前路径。成功返回0，失败返回-1。*/
int Winks_chdir(char *dir_name);
/*创建目录。成功返回0，失败返回-1。*/
int Winks_mkdir(char *dir_name);
/*删除指定的空目录。成功返回0，失败返回-1。*/
int Winks_rmdir(char *dir_name);
/*修改目录名。成功返回0，失败返回-1。*/
int Winks_rndir(char *old_dir_name, char *new_dir_name);
/* 删除一个目录下所有文件, 不递归子目录 */
int Winks_XdeleteDir(char *dir_name);
/* 获取文件夹中所有文件大小,调用前*RetSize必须置0 */
int Winks_GetFolderSize(
						const char * Path,      /* Src path string */
						int          IsAscii,   /* Src path is ascii ? */
						unsigned long * RetSize,  /* return folder size */
						unsigned long CmpSize);  /* CmpSize==0 获取整个文件夹的文件大小，否则只判断是否大于CmpSize */
@end
