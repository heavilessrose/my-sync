//
//  wk_fileapi.m
//  TabiNavi
//
//  Created by wang luke on 7/1/09.
//  Copyright 2009 luke. All rights reserved.
//

#import "wk_fileapi.h"


@implementation wk_fileapi
static int dirsize;
typedef	int	TraversFunc(const char *);
static int traversDir(const char *path);

#pragma mark -
#pragma mark osal
/*打开指定的文件*/
FILE* Winks_fopen(const char *filename,const char *type){
#ifdef WINKS_DEBUG
	
#else
	return fopen(filename, type);
#endif
}

/*打开指定的文件(unicode)*/
FILE* Winks_fopenUni(const unsigned short *filename,const char *type){
	
}

/*关闭一个文件*/
int Winks_fclose(FILE *stream){
	return fclose(stream);
}

/*测试是否到达指定文件的末尾*/
int Winks_feof(FILE *stream){
	return feof(stream);
}

/*测试指定文件是否存在,如果存在返回0，否则-1*/
int Winks_fexist(char *filename){
	FILE *fp;
	if((fp = fopen(filename, "rb")) == NULL){
#ifdef WINKS_DEBUG
		perror("Winks_fexist");
#endif
		return -1;
	}
	fclose(fp);
	return 0;
}

/*从所给文件stream中读取的count个数据数据单元，每一个数据单元的长度为size字节，到由data所指的块中*/
size_t Winks_fread(void *data, size_t size, size_t count, FILE *stream){
	return fread(data, size, count, stream);
}

/*从指针data开始把count个数据单元项添加到给定文件stream中，每个数据单元的长度为size个字节*/
size_t Winks_fwrite(const void *data, size_t size, size_t count, FILE *stream){
	return fwrite(data, size, count, stream);
}

/*本函数返回文件fp 的指针偏移位(offset) 值。成功偏移量，失败返回-1。*/
int Winks_ftell(FILE *stream){
	return ftell(stream);
}

int Winks_fseek(FILE *stream, long offset, int fromwhere){
	return fseek(stream, offset, fromwhere);
}

/*删除指定的文件。成功返回0，失败返回-1。*/
int Winks_fdelete(char *file_name){
	NSString *filePath = [[NSString alloc] initWithCString:file_name];
	
	BOOL success = [[NSFileManager defaultManager] removeItemAtPath:filePath error:NULL];
	[filePath release];
	if(success)
		return 0;
	else
		return -1;
}

/*更新指定文件的文件名。成功返回0，失败返回-1。*/
int Winks_frename(char *old_file_name, char *new_file_name){
#ifdef WINKS_DEBUG
	if(rename(old_file_name, new_file_name) == -1){
		perror("Winks_frename");
		return -1;
	}else {
		return 0;
	}
#else
	return rename(old_file_name, new_file_name);
#endif
}

/* 获取文件大小, 返回负值表示失败 */
int Winks_fgetsize(const char *filename, unsigned int is_ascii, unsigned int *size){
	NSString *filePath = [[NSString alloc] initWithCString:filename];
	NSDictionary *attrs = [[NSFileManager defaultManager] fileAttributesAtPath:filePath traverseLink:NO];
	if(attrs != nil){
		[filePath release];
		NSNumber *fileSize;
		if (fileSize = [attrs objectForKey:NSFileSize]) {
			NSLog(@"File size: %qi\n", [fileSize unsignedLongLongValue]);
			return [fileSize intValue];
		}else{
			return 0;
		}
	}else{
		NSLog(@"Path (%@) is invalid.", filePath);
		[filePath release];
		return -1;
	}
}

/*取文件大小，成功返回0*/
int Winks_fGetFileSize(FILE *stream, unsigned long *size){
	int iRet = -1;	
	long lOldPos = 0;
	
	if ((lOldPos = ftell(stream)) == -1)
		return iRet;
	
	if (fseek(stream, 0, SEEK_END) == 0){
		if ((*size = ftell(stream)) != -1)
			iRet = 0;
		fseek(stream, lOldPos, SEEK_SET);		
	}
	
	return iRet;
}

/*获取当前路径。成功返回0，失败返回-1。*/
int Winks_getcwd(char *path_name_ptr, size_t size){
	if(getcwd(path_name_ptr, size) == NULL)
		return -1;
	else
		return 0;
}

/*设置当前路径。成功返回0，失败返回-1。*/
int Winks_chdir(char *dir_name){
	return chdir(dir_name);
}

/*创建目录。成功返回0，失败返回-1。*/
int Winks_mkdir(char *dir_name){
	return mkdir(dir_name, EFAULT);
}

/*删除指定的空目录。成功返回0，失败返回-1。*/
int Winks_rmdir(char *dir_name){
	return rmdir(dir_name);
}

/*修改目录名。成功返回0，失败返回-1。*/
int Winks_rndir(char *old_dir_name, char *new_dir_name){
	return Winks_frename(old_dir_name, new_dir_name);
}

/* 删除一个目录下所有文件, 不递归子目录 */
int Winks_XdeleteDir(char *dir_name){
	DIR *dirp = opendir(dir_name);
	if(dirp == NULL)
		return -1;
	
	char *fullpath = malloc(PATH_MAX);
	strcpy(fullpath, dir_name);
	char *ptr = fullpath + strlen(fullpath);
	*ptr++ = '/';
	*ptr = 0;
	
	struct stat statbuf;
	struct dirent *entry;
	
	while((entry = readdir(dirp)) != NULL){
		// 忽略.及..
		if(strcmp(entry->d_name, ".") == 0  || strcmp(entry->d_name, "..") == 0)
			continue;
		strcpy(ptr, entry->d_name);
		// stat error
		if(lstat(fullpath, &statbuf) < 0)
			continue;
		// 非目录且为普通文件
		if((S_ISDIR(statbuf.st_mode) == 0) && S_ISREG(statbuf.st_mode)){
			Winks_fdelete(fullpath);
		}
	}
	if (closedir(dirp) < 0)
		perror("Winks_XdeleteDir");
	return 0;
}

/* 获取文件夹中所有文件大小,调用前*RetSize必须置0 */
int Winks_GetFolderSize(
						const char * Path,      /* Src path string */
						int          IsAscii,   /* Src path is ascii ? */
						unsigned long * RetSize,  /* return folder size */
						unsigned long CmpSize)/* CmpSize==0 获取整个文件夹的文件大小，否则只判断是否大于CmpSize */
{
	traversDir(Path);
	*RetSize = dirsize;
	return 0;
}  


#pragma mark -
static int traversDir(const char *path){
	DIR *dirp = opendir(path);
	if(dirp == NULL)
		return -1;
	
	char *fullpath = malloc(1024);
	strcpy(fullpath, path);
	char *ptr = fullpath + strlen(fullpath);
	*ptr++ = '/';
	*ptr = 0;
	
	struct stat statbuf;
	struct dirent *entry;
	
	while((entry = readdir(dirp)) != NULL){
		// 忽略.及..
		if(strcmp(entry->d_name, ".") == 0  || strcmp(entry->d_name, "..") == 0){
			continue;
		}
		strcpy(ptr, entry->d_name);
		// stat error
		if(lstat(fullpath, &statbuf) < 0)
			continue;
		// 非目录且为普通文件
		if((S_ISDIR(statbuf.st_mode) == 0)){
			//printf("unDir: %s\n", fullpath);
			dirsize += statbuf.st_size;
		}else{
			//printf("__Dir: %s\n", fullpath);
			if(traversDir(fullpath) == 0){
				continue;
			}
		}
	}
	if (closedir(dirp) < 0)
		perror("Winks_XdeleteDir");
	//printf("\n\nallsize: %d", dirsize);
	return 0;
}
@end
