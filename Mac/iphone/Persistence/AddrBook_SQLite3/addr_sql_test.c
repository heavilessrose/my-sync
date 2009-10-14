//
//  main.m
//  AddrBook_SQLite3
//
//  Created by wang luke on 09-10-14.
//  Copyright luke 2009. All rights reserved.
//

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

#include <stdio.h>
#include <string.h>
#include "/usr/include/sqlite3.h"

#pragma mark -
// Phone number max len
#define WINKS_FRIEND_NO_LEN 40
#define WINKS_MAX_NUMBER_COUNT 8
typedef struct tag_Winks_PhoneBookItem_s
	{
		char number[WINKS_MAX_NUMBER_COUNT][WINKS_FRIEND_NO_LEN];
	}Winks_PhoneBookItem_s;

#define kDbName "/var/mobile/Library/AddressBook/AddressBook.sqlitedb"
int Winks_GetPhonebookCount(unsigned long *phone_cnt, unsigned long *sim_cnt);

/*取电话本中指定的一条记录（一条记录可能含有多个电话号码）*/
//type，1表示电话本机，2表示sim卡
//index，表示第几条，从0开始 
//numberItme，返回的电话号码放在此参数中
//返回值，0表示成功，-1表示失败
int Winks_ReadPhonebook(unsigned int type, unsigned int index, Winks_PhoneBookItem_s* numberItem);

// 根据号码得到名字
void Winks_GetPhonebookName(const char* friend_no, char *friend_name, unsigned long len);


#pragma mark -
#pragma mark debug founctions
void CallLog(const char* str) {
    //NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
#if !TARGET_IPHONE_SIMULATOR
	char* path = "/var/mobile/qqq";
#else
	char* path = "/Users/wangluke/qqq";
#endif
	
	FILE *myLog = NULL;
	myLog = fopen(path,"a+");
	
	if (myLog != NULL) 
	{		
//		NSDate *time = [NSDate date];
//		const char *tmp = [[time description] cStringUsingEncoding:NSASCIIStringEncoding];
//		
//		fputs(tmp,myLog);
//		fputs("  ",myLog);
		fputs(str,myLog);
		fputs("\n",myLog);
		//
		printf("%s\n",str);
	}
	fclose(myLog);
	//[pool release];
}

void Winks_printf(const char* fmt, ... )
{
    const int MAX_DBG_STR = 1024;
	
    int written = 0;
    char buffer[MAX_DBG_STR];
	memset(buffer,0,MAX_DBG_STR);
	
    va_list va;
    va_start( va, fmt );
    written = vsprintf( &buffer[0], fmt, va);
    va_end(va);
	
	if(strlen(buffer))
		CallLog(buffer);
}


#pragma mark -

void print_row(int row, char *rowData)
{
	printf("%d  %s\n", row, rowData);
}

int openDB(char *dbFile, sqlite3 *db)
{
	// 打开数据库
	if(sqlite3_open(dbFile, &db) != SQLITE_OK)
	{
		sqlite3_close(db);
		return 0;
	}
	return -1;
}

int readData(sqlite3 *db, char *stat)
{
	// 加载数据，请求数据库中的所有行
	sqlite3_stmt *statement;
	if(sqlite3_prepare_v2(db, stat, -1, &statement, NULL) == SQLITE_OK){
		// 每个返回行
		while (sqlite3_step(statement) == SQLITE_ROW) {
			// 得到字段值
			int row = sqlite3_column_int(statement, 0);
			char *rowData = (char *)sqlite3_column_text(statement, 1);
			
			print_row(row, rowData);
		}
		// 清理statement
		sqlite3_finalize(statement);
	}else{
		printf("sql prepare err\n");
	}
}

int main(int argc, char *argv[]) {
	
	unsigned long phone_cnt;
	unsigned long sim_cnt;
	if(Winks_GetPhonebookCount(&phone_cnt, &sim_cnt) == -1){
		Winks_printf("get count err");
	}
	Winks_printf("count = %d", phone_cnt);
}

#pragma mark -
static char *numlist = NULL;
int Winks_GetPhonebookCount(unsigned long *phone_cnt, unsigned long *sim_cnt)
{
	*sim_cnt = 0;
    sqlite3 *database;
	
	// 打开数据库
	if(sqlite3_open(kDbName, &database) != SQLITE_OK)
	{
		sqlite3_close(database);
		Winks_printf("Failed to open DB--\n");
		return 0;
	}
	
	sqlite3_stmt *statement;
	if(sqlite3_prepare_v2(database, "SELECT name,seq FROM sqlite_sequence", -1, &statement, NULL) == SQLITE_OK){
		while(sqlite3_step(statement) == SQLITE_ROW) {
			char *name = (char *)sqlite3_column_text(statement, 0);
			if(strcmp(name, "ABPeople")){
				*phone_cnt = sqlite3_column_int(statement, 1);
				if((*phone_cnt) > 0){
					extern char allNumber[*phone_cnt][WINKS_FRIEND_NO_LEN];
					numlist = allNumber;
				}else{
					numlist = NULL;
					Winks_printf("count == 0");
				}
			}
		}
		sqlite3_finalize(statement);
		
		return 0;
	}
	
	Winks_printf("sql prepare err\n");
	return -1;
}

/*取电话本中指定的一条记录（一条记录可能含有多个电话号码）*/
//type，1表示电话本机，2表示sim卡
//index，表示第几条，从0开始 
//numberItme，返回的电话号码放在此参数中
//返回值，0表示成功，-1表示失败
int Winks_ReadPhonebook(unsigned int type, unsigned int index, Winks_PhoneBookItem_s* numberItem)
{
	if (!numberItem)
		return -1;
	
	if (type == 2){
		Winks_printfInfo("Winks_ReadPhonebook:: sim card phonebook not supported");
		return -1;	
	}
	
	sqlite3 *database;
	// 打开数据库
	if(sqlite3_open(kDbName, &database) != SQLITE_OK)
	{
		sqlite3_close(database);
		Winks_printf("Failed to open DB--\n");
		return 0;
	}
	
	sqlite3_stmt *statement;
	if(sqlite3_prepare_v2(database, "SELECT value FROM ABMultiValue", -1, &statement, NULL) == SQLITE_OK){
		while(sqlite3_step(statement) == SQLITE_ROW) {
			if(numlist != NULL){
				memcpy(numlist + i, (char *)sqlite3_column_int(statement, 0), strlen((char *)sqlite3_column_int(statement, 0)));
			}
		}
		sqlite3_finalize(statement);
		
		
		return 0;
	}
	return -1;
}

// 根据号码得到名字
void Winks_GetPhonebookName(const char* friend_no, char *friend_name, unsigned long len)
{
	
}