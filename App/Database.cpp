#include "Database.h"

//Ð´Èë¼ÇÂ¼
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
			strcpy(g_getkeyName,argv[0] ? argv[0] : "NULL");
		}
		unsync_ret_buf="{";
		unsync_ret_buf+="\"key\":";
		unsync_ret_buf+="\"";
		unsync_ret_buf+= argv[0] ? argv[0] : "NULL";
		unsync_ret_buf+="\"";
		unsync_ret_buf+=",";
		unsync_ret_buf+="\"val\":";
		unsync_ret_buf+="\"";
		unsync_ret_buf+= argv[1] ? argv[1] : "NULL";
		unsync_ret_buf+="\"";
		unsync_ret_buf+="}";
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

void Database::writeDB(char*keyName,char*value)
{
		sqlite3 *db;
		char *zErrMsg = 0;
		int  rc;
		int key=0;
		const char *sql ;
		string temp;

		/* Open database */
		rc = sqlite3_open(_WS2S_CSTR(db_file), &db);
		if (rc){
			cout <<  "Can't open database "<<db_file << endl;
			sqlite3_free(zErrMsg);
		}
		else{
			cout << "open the database successful!" << endl;
			
			temp ="select count(autoid) from table1 where key='";
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
			}
		}
		sqlite3_close(db);
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
		rc = sqlite3_open(_WS2S_CSTR(db_file), &db);
		if (rc){
			cout <<  "Can't open database "<<db_file << endl;
			sqlite3_free(zErrMsg);
		}
		else{
			cout << "open the database successful!" << endl;
 
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
		rc = sqlite3_open(_WS2S_CSTR(db_file), &db);
		if (rc){
			cout <<  "Can't open database "<<db_file << endl;
			sqlite3_free(zErrMsg);
		}
		else{
			cout << "open the database successful!" << endl;
 
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
		rc = sqlite3_open(_WS2S_CSTR(db_file), &db);
		if (rc){
			cout <<  "Can't open database "<<db_file << endl;
			sqlite3_free(zErrMsg);
		}
		else{
			cout << "open the database successful!" << endl;
 
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
