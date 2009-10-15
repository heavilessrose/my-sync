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
#include <stdlib.h>
#include "/usr/include/sqlite3.h"

#pragma mark -
// Phone number max len
#define WINKS_FRIEND_NO_LEN 40
#define WINKS_MAX_NUMBER_COUNT 8
typedef struct tag_Winks_PhoneBookItem_s
	{
		char number[WINKS_MAX_NUMBER_COUNT][WINKS_FRIEND_NO_LEN];
	}Winks_PhoneBookItem_s;

// iphone电话号码与一般电话号码匹配结构
typedef struct tag_phoneNumItem_s {
	char pNumber[WINKS_FRIEND_NO_LEN];
	char fNumber[WINKS_FRIEND_NO_LEN];
} phoneNumItem_s;

// 号码对照表
static phoneNumItem_s *phoneNumItems;
static _allCount = 0;

#define kDbName "/var/mobile/Library/AddressBook/AddressBook.sqlitedb"
static void numberFilter(const char *number, char *fixedNumber);
static int getPhoneNumberCount();

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
		return -1;
	}
	return 0;
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
		printf("[contact]sql prepare err\n");
	}
}

int main(int argc, char *argv[]) {
	
//	unsigned long phone_cnt;
//	unsigned long sim_cnt;
//	if(Winks_GetPhonebookCount(&phone_cnt, &sim_cnt) == -1){
//		Winks_printf("[contact]get count err");
//	}
//	Winks_printf("[contact]count = %d", phone_cnt);
//	
//	
//	Winks_PhoneBookItem_s numberItem;
//	Winks_ReadPhonebook(1, 3, &numberItem);
//	
//	char fname[20];
//	Winks_GetPhonebookName("158-0163-0382", fname, 20);
//	
//	char fixedNumber[20];
//	numberFilter("158-0163-0382", fixedNumber);
//	Winks_printf(fixedNumber);
	
	int count = getPhoneNumberCount();
	Winks_printf("count = %d", count);
}

#pragma mark -
// 过滤掉电话号码中的非数字位
static void numberFilter(const char *number, char *fixedNumber)
{
	memset(fixedNumber, 0, strlen(fixedNumber));
	int oldLen = strlen(number);
	
	int i;
	int j = 0;
	for(i = 0; i < oldLen; i++){
		if(*(number + i) == '-' || *(number + i) == ' ' || *(number + i) == '(' || *(number + i) == ')')
		{
			continue;
		}
		*(fixedNumber + j) = *(number + i);
		j++;
	}
}

// 得到电话本中目前所有号码个数
static int getPhoneNumberCount()
{
    sqlite3 *database;
	
	// 打开数据库
	if(sqlite3_open(kDbName, &database) != SQLITE_OK)
	{
		sqlite3_close(database);
		Winks_printf("[contact]Failed to open DB--\n");
		return -1;
	}
	
	sqlite3_stmt *statementCount;
	if(sqlite3_prepare_v2(database, "SELECT count(*) FROM ABMultiValue WHERE property=3", -1, &statementCount, NULL) == SQLITE_OK){
		int allNumberCount = 0;
		while(sqlite3_step(statementCount) == SQLITE_ROW) {
			allNumberCount = sqlite3_column_int(statementCount, 0);
		}
		
		sqlite3_finalize(statementCount);
		sqlite3_close(database);
		return allNumberCount;
	}else{
		Winks_printf("[contact]Failed to prepare statementCount\n");
		sqlite3_close(database);
		return -1;
	}
}

void getIphoneStyleNumber(char *fixedNumber)
{
	for(){
		
	}
}

int Winks_GetPhonebookCount(unsigned long *phone_cnt, unsigned long *sim_cnt)
{
	*sim_cnt = 0;
	
	// 初始化普通电话号码与iphone电话号码对照表
	_allCount = getPhoneNumberCount();
	phoneNumItems = calloc(pnumbersCount, sizeof(phoneNumItem_s));
	
	// 打开数据库
    sqlite3 *database;
	if(sqlite3_open(kDbName, &database) != SQLITE_OK)
	{
		sqlite3_close(database);
		Winks_printf("[contact]Failed to open DB--\n");
		return -1;
	}
	
	// 得到电话本中联系人个数
	sqlite3_stmt *statement;
	if(sqlite3_prepare_v2(database, "SELECT name,seq FROM sqlite_sequence", -1, &statement, NULL) == SQLITE_OK){
		while(sqlite3_step(statement) == SQLITE_ROW) {
			char *name = (char *)sqlite3_column_text(statement, 0);
			if(strcmp(name, "ABPeople")){
				*phone_cnt = sqlite3_column_int(statement, 1);
//				if((*phone_cnt) > 0){
//					extern char allNumber[*phone_cnt][WINKS_FRIEND_NO_LEN];
//					numlist = allNumber;
//				}else{
//					numlist = NULL;
//					Winks_printf("count == 0");
//				}
			}
		}
		sqlite3_finalize(statement);
		
		sqlite3_close(database);
		return 0;
	}
	
	Winks_printf("[contact]sql prepare err\n");
	sqlite3_close(database);
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
		Winks_printf("[contact]Winks_ReadPhonebook:: sim card phonebook not supported");
		return -1;	
	}
	
	// 打开数据库
	sqlite3 *database;
	if(sqlite3_open(kDbName, &database) != SQLITE_OK)
	{
		sqlite3_close(database);
		Winks_printf("[contact]Failed to open DB--\n");
		return -1;
	}
	Winks_printf("db opened");
	
	// 得到index对应的联系人id
	sqlite3_stmt *statementPerson;
	int record_id = 0;
	if(sqlite3_prepare_v2(database, "SELECT ROWID FROM ABPerson ORDER BY ROWID", -1, &statementPerson, NULL) == SQLITE_OK){
		int i = 0;
		while(sqlite3_step(statementPerson) == SQLITE_ROW){
			if((i++) == index)
				record_id = sqlite3_column_int(statementPerson, 0);
		}
		Winks_printf("[contact]record_id: %d",record_id);
		sqlite3_finalize(statementPerson);
	}else{
		Winks_printf("[contact]Failed to prepare person\n");
		sqlite3_close(database);
		return -1;
	}
	
	// 得到联系人的所有号码
	sqlite3_stmt *statementNum;
	char sql[256];
	sprintf(sql, "SELECT value FROM ABMultiValue WHERE property=3 AND record_id=%d", record_id);
	Winks_printf("[contact] %s", sql);
	if(sqlite3_prepare_v2(database, sql, -1, &statementNum, NULL) == SQLITE_OK){
		int i = 0;
		char *fixedNumber;
		while(sqlite3_step(statementNum) == SQLITE_ROW) {
			unsigned char *oldnum = sqlite3_column_text(statementNum, 0);
			fixedNumber = malloc(strlen(oldnum));
			numberFilter(oldnum, fixedNumber);
			memcpy(numberItem->number[i], fixedNumber, strlen(fixedNumber));
			i++;
		}
		free(fixedNumber);
		// debug
		Winks_printf("[contact]person: %d\n", record_id);
		for(i = i-1;i >= 0; i--){
			Winks_printf("		%s\n", numberItem->number[i]);
		}
		
		sqlite3_finalize(statementNum);
		sqlite3_close(database);
		return 0;
	}else{
		Winks_printf("[contact]Failed to prepare number\n");
		sqlite3_close(database);
		return -1;
	}
	Winks_printf("[contact]Winks_ReadPhonebook failed\n");
	return -1;
}

// 根据号码得到名字
void Winks_GetPhonebookName(const char* friend_no, char *friend_name, unsigned long len)
{
	if (!friend_no || !friend_name || len <= 0 || !*friend_no){
		Winks_printf("[contact]friend_no, friend_name or friend_no is NULL");
		return;
	}
	
	// 打开数据库
	sqlite3 *database;
	if(sqlite3_open(kDbName, &database) != SQLITE_OK)
	{
		sqlite3_close(database);
		Winks_printf("[contact]Failed to open DB--\n");
		return;
	}
	
	// 得到号码对应的联系人id
	sqlite3_stmt *statementNum;
	int record_id = 0;
	char sql[256];
	sprintf(sql, "SELECT record_id FROM ABMultiValue WHERE value='%s'", friend_no);
	Winks_printf("[contact] %s", sql);
	if(sqlite3_prepare_v2(database, sql, -1, &statementNum, NULL) == SQLITE_OK){
		while(sqlite3_step(statementNum) == SQLITE_ROW) {
			record_id = sqlite3_column_int(statementNum, 0);
		}
		
		sqlite3_finalize(statementNum);
	}else{
		Winks_printf("[contact]Failed to prepare number\n");
		sqlite3_close(database);
		return;
	}
	
	// 根据联系人id得到联系人姓名
	sqlite3_stmt *statementPerson;
	memset(sql, 0, 256);
	sprintf(sql, "SELECT First, Last FROM ABPerson WHERE ROWID='%d'", record_id);
	Winks_printf("[contact] %s", sql);
	if(sqlite3_prepare_v2(database, sql, -1, &statementPerson, NULL) == SQLITE_OK){
		int i = 0;
		if(sqlite3_step(statementPerson) == SQLITE_ROW){
			char name[len];
			memset(name, 0, len);
			char* first = (char*)sqlite3_column_text(statementPerson, 0);
			char* last = (char*)sqlite3_column_text(statementPerson, 1);
			char* middle = (char*)sqlite3_column_text(statementPerson, 2);
			
			int empty = len;
			if(first != NULL){
				Winks_printf("[contact]first = %s", first);
				if(strlen(first) > len){
					strncpy(name, first, len);
					empty = 0;
				}else{
					strcpy(name, first);
					empty = len - strlen(first);
				}
			}
			if(last != NULL){
				Winks_printf("[contact]last = %s", last);
				if(empty > 0){
					if(strlen(last) > empty){
						strncat(name, last, empty);
						empty = 0;
					}else{
						strcat(name, last);
						empty = empty - strlen(last);
					}
				}
			}
			if(middle != NULL){
				Winks_printf("[contact]middle = %s", middle);
				if(empty > 0){
					strncat(name, middle, empty);
				}
			}
			
			strcpy(friend_name, name);
			Winks_printf("[contact]name = %s", friend_name);
		}
		else{
			Winks_printf("[contact]record_id %d not exist",record_id);
		}
		sqlite3_finalize(statementPerson);
		sqlite3_close(database);
		return;
	}else{
		Winks_printf("[contact]Failed to prepare person\n");
		sqlite3_close(database);
		return;
	}
}