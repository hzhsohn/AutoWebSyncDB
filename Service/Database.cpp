#include "Database.h"

//写入记录
int is_not_exist_key=0;
string unsync_ret_buf="";
char* g_getkeyName;

TCHAR  Database::db_file[1024]={0};

Database::Database(void)
{
}

Database::~Database(void)
{
}

static int write_check(void *NotUsed, int argc, char **argv, char **azColName){
    if(1==argc)
	{
		if(0==atoi(argv[0]))
		{
			is_not_exist_key=1;
		}
		else
		{
			is_not_exist_key=0;
		}
	}
    return 0;
}

static int read_unsync_callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i=0;
	if(2==argc)
	{
		if(g_getkeyName)
		{
			strcpy(g_getkeyName,argv[0] ? argv[0] : "");
		}
		unsync_ret_buf=argv[1];
	}
    return 0;
}

static int read_callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    fprintf(stderr, "%s: ", (const char*)NotUsed);
    for (i = 0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void Database::genDB()
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	int key=0;
	const char *sql ;
	string temp;
		
	/* Open database */
	rc = sqlite3_open(StringUtil::ws2utf8(db_file).c_str(), &db);
	if (rc){
		cout <<  "Can't open database "<< endl;
		sqlite3_free(zErrMsg);
		cout <<  "exit(0) program ..." << endl;
		exit(0);
	}
	else{
		cout << "open the database successful!" << endl;
	}
 
	// Create table statement	
	//字段is_sync 0未处理 ,1提交服务器成功, 2提交服务器失败无效json-key ,3网站接口数据回复异常
	sql = "CREATE TABLE table1(autoid integer PRIMARY KEY autoincrement,key varchar(64) NOT NULL,value varchar(2048),is_sync tinyint default 0)";
	rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		cout << zErrMsg << endl;
		sqlite3_free(zErrMsg);
	}
	else{
		cout << "create table successful!" << endl;
	}
	sqlite3_close(db);
}
BOOL Database::writeDB(char*keyName,char*value)
{
		sqlite3 *db;
		char *zErrMsg = 0;
		int  rc;
		int key=0;
		const char *sql ;
		string temp;
		BOOL ret=FALSE;

		/* Open database */
		rc = sqlite3_open(StringUtil::ws2utf8(db_file).c_str(), &db);
		if (rc){
			cout <<  "Can't open database "<< endl;
			sqlite3_free(zErrMsg);
		}
		else{
			
			temp ="select count(autoid) from table1 where is_sync=0 and key='";
			temp +=keyName;
			temp +="'";
			sql = temp.c_str();	
			rc = sqlite3_exec(db, sql, write_check, 0, &zErrMsg);
			if (rc != SQLITE_OK){
				cout << zErrMsg << endl;  
				sqlite3_free(zErrMsg);
			}
 
			//---------------------------
			if(is_not_exist_key)
			{
				temp ="INSERT INTO table1(key,value) VALUES('";
				temp +=keyName;
				temp +="','";
				temp +=value;
				temp +="');";
				sql = temp.c_str();	
				rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
				if (rc != SQLITE_OK){
					cout << zErrMsg << endl;  
					sqlite3_free(zErrMsg);
				}
				else
				{
					cout << "write database successful!" << endl;
					ret=TRUE;
				}
			}
		}
		sqlite3_close(db);
		return ret;
}

void Database::readDB(int start,int count)
{
		sqlite3 *db;
		char *zErrMsg = 0;
		int  rc;
		int key=0;
		const char *sql ;
		string temp;
		char bufss[32]={0};

		/* Open database */
		rc = sqlite3_open(StringUtil::ws2utf8(db_file).c_str(), &db);
		if (rc){
			cout <<  "Can't open database "<< endl;
			sqlite3_free(zErrMsg);
		}
		else{
			//cout << "open the database successful!" << endl;
 
			temp ="select * from table1 limit ";
			itoa(start,bufss,10);
			temp +=bufss;
			temp +=",";
			itoa(count,bufss,10);
			temp +=bufss;
			sql = temp.c_str();	
			rc = sqlite3_exec(db, sql, read_callback, 0, &zErrMsg);
			if (rc != SQLITE_OK){
				cout << zErrMsg << endl;  
				sqlite3_free(zErrMsg);
			}
		}
		sqlite3_close(db);
}

BOOL Database::setDBUpdateSuccess(char* keyName)
{
		sqlite3 *db;
		char *zErrMsg = 0;
		int  rc;
		int key=0;
		const char *sql ;
		string temp;

		/* Open database */
		rc = sqlite3_open(StringUtil::ws2utf8(db_file).c_str(), &db);
		if (rc){
			cout <<  "Can't open database "<< endl;
			sqlite3_free(zErrMsg);
		}
		else{
			temp ="update table1 set is_sync=1 where key='";
			temp+=keyName;
			temp+="'";
			sql = temp.c_str();	
			rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
			if (rc != SQLITE_OK){
				cout << zErrMsg << endl;  
				sqlite3_free(zErrMsg);
			}
		}
		sqlite3_close(db);
		return rc != SQLITE_OK;
}


BOOL Database::setDBUpdateExceptionKey(char* keyName)
{
		sqlite3 *db;
		char *zErrMsg = 0;
		int  rc;
		int key=0;
		const char *sql ;
		string temp;

		/* Open database */
		rc = sqlite3_open(StringUtil::ws2utf8(db_file).c_str(), &db);
		if (rc){
			cout <<  "Can't open database "<< endl;
			sqlite3_free(zErrMsg);
		}
		else{
			temp ="update table1 set is_sync=3 where key='";
			temp+=keyName;
			temp+="'";
			sql = temp.c_str();	
			rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
			if (rc != SQLITE_OK){
				cout << zErrMsg << endl;  
				sqlite3_free(zErrMsg);
			}
		}
		sqlite3_close(db);
		return rc != SQLITE_OK;
}

BOOL Database::setDBUpdateInvalidKey(char* keyName)
{
		sqlite3 *db;
		char *zErrMsg = 0;
		int  rc;
		int key=0;
		const char *sql ;
		string temp;

		/* Open database */
		rc = sqlite3_open(StringUtil::ws2utf8(db_file).c_str(), &db);
		if (rc){
			cout <<  "Can't open database "<< endl;
			sqlite3_free(zErrMsg);
		}
		else{
			temp ="update table1 set is_sync=2 where key='";
			temp+=keyName;
			temp+="'";
			sql = temp.c_str();	
			rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
			if (rc != SQLITE_OK){
				cout << zErrMsg << endl;  
				sqlite3_free(zErrMsg);
			}
		}
		sqlite3_close(db);
		return rc != SQLITE_OK;
}

string Database::readUnUpdateTop1(char* getkeyName)
{
		sqlite3 *db;
		char *zErrMsg = 0;
		int  rc;
		int key=0;
		const char *sql ;
		string temp;
		char bufss[32]={0};

		g_getkeyName=getkeyName;
		unsync_ret_buf="";
		/* Open database */
		rc = sqlite3_open(StringUtil::ws2utf8(db_file).c_str(), &db);
		if (rc){
			cout <<  "Can't open database "<< endl;
			sqlite3_free(zErrMsg);
		}
		else{
			temp ="select key,value from table1 where is_sync=0 LIMIT 1";
			sql = temp.c_str();	
			rc = sqlite3_exec(db, sql, read_unsync_callback, 0, &zErrMsg);
			if (rc != SQLITE_OK){
				cout << zErrMsg << endl; 
				sqlite3_free(zErrMsg);
			}
		}
		sqlite3_close(db);
		return unsync_ret_buf;
}
