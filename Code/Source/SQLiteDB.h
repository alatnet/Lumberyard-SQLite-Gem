#pragma once

#include <AzCore\RTTI\TypeInfo.h>
#include <AzCore/RTTI/BehaviorContext.h>

#include "SQLiteMutex.h"

namespace SQLite3 {
	class SQLiteStmt;

	////////////////////////////////////////////////////////////////////////
	//SQLiteDB Class
	class SQLiteDB {
	public:
		AZ_TYPE_INFO(SQLiteDB, "{36DD1FB6-66E5-42D0-86B4-6C4072009A16}");
	public:
		SQLiteDB() {}
		SQLiteDB(sqlite3* db) { m_pDB = db; }
	public:
		int Open(const char * path);
		int Open16(const char * path);
		int Open_v2(const char * path, int flags, const char *zVfs);
		int Close();
		int Close_v2();
	public:
		SQLiteStmt * Prepare(const char * sql, int nByte);
		SQLiteStmt * Prepare_v2(const char * sql, int nByte);
		SQLiteStmt * Prepare16(const char * sql, int nByte);
		SQLiteStmt * Prepare16_v2(const char * sql, int nByte);
	public:
		int ErrCode() { return sqlite3_errcode(this->m_pDB); }
		int ExtErrCode() { return sqlite3_extended_errcode(this->m_pDB); }
		const char * ErrMsg() { return sqlite3_errmsg(this->m_pDB); }
		const void * ErrMsg16() { return sqlite3_errmsg16(this->m_pDB); }
	public:
		int Limit(int id, int newVal) { return sqlite3_limit(this->m_pDB, id, newVal); }
	public:
		SQLiteMutex * DB_Mutex() { return new SQLiteMutex(sqlite3_db_mutex(this->m_pDB)); }
	public:
		int DB_CacheFlush() { return sqlite3_db_cacheflush(this->m_pDB); }
		//int DB_Config(int op, ...);
		const char * DB_Filename(const char * zDbName) { return sqlite3_db_filename(this->m_pDB, zDbName); }
		int DB_ReadOnly(const char *zDbName) { return sqlite3_db_readonly(this->m_pDB, zDbName); }
		int DB_Release_Memory() { return sqlite3_db_release_memory(this->m_pDB); }
		int DB_Status(int op, int *pCur, int *pHiwtr, int resetFlg) { return sqlite3_db_status(this->m_pDB, op, pCur, pHiwtr, resetFlg); } //override
		int Declare_VTab(const char * zSQL) { return sqlite3_declare_vtab(this->m_pDB, zSQL); }
		int Enable_Load_Extension(int onoff) { return sqlite3_enable_load_extension(this->m_pDB, onoff); }
		//int File_Control(const char * zDbName, int op, void * x) { sqlite3_file_control(this->m_pDB, zDbName, op, x); }

		//int sqlite3_db_config(sqlite3*, int op, ...);
		//int sqlite3_file_control(sqlite3*, const char *zDbName, int op, void*);

		//requires tables
		//int sqlite3_get_table(
		//  sqlite3 *db,          /* An open database */
		//  const char *zSql,     /* SQL to be evaluated */
		//	char ***pazResult,    /* Results of the query */
		//	int *pnRow,           /* Number of result rows written here */
		//	int *pnColumn,        /* Number of result columns written here */
		//	char **pzErrmsg       /* Error msg written here */
		//);
		//void sqlite3_free_table(char **result); 

	public:
		int Get_AutoCommit() { return sqlite3_get_autocommit(this->m_pDB); }
		void Interrupt() { sqlite3_interrupt(this->m_pDB); }
		__int64 Last_Insert_RowId() { return (__int64)sqlite3_last_insert_rowid(this->m_pDB); }

		int Load_Extension(const char * zFile, const char * zProc, char ** pzErrMsg) { return sqlite3_load_extension(this->m_pDB, zFile,zProc, pzErrMsg); }; //override

		int Overload_Function(const char * zFuncName, int nArgs) { return sqlite3_overload_function(this->m_pDB, zFuncName, nArgs); }
		void Set_Last_Insert_RowId(__int64 N) { sqlite3_set_last_insert_rowid(this->m_pDB, N); }
		int System_ErrNo() { return sqlite3_system_errno(this->m_pDB); }

		//requires tables
		//int sqlite3_table_column_metadata(
		//	sqlite3 *db,                /* Connection handle */
		//	const char *zDbName,        /* Database name or NULL */
		//	const char *zTableName,     /* Table name */
		//	const char *zColumnName,    /* Column name */
		//	char const **pzDataType,    /* OUTPUT: Declared data type */
		//	char const **pzCollSeq,     /* OUTPUT: Collation sequence name */
		//	int *pNotNull,              /* OUTPUT: True if NOT NULL constraint exists */
		//	int *pPrimaryKey,           /* OUTPUT: True if column part of PK */
		//	int *pAutoinc               /* OUTPUT: True if column is auto-increment */
		//);

		int Total_Changes() { return sqlite3_total_changes(this->m_pDB); }
		//int VTab_Config(int op, ...) { return sqlite3_vtab_config(this->m_pDB); }
		int VTab_On_Conflict() { return sqlite3_vtab_on_conflict(this->m_pDB); }
		int Wal_AutoCheckpoint(int N) { return sqlite3_wal_autocheckpoint(this->m_pDB, N); }
		int Wal_Checkpoint(const char * zDB) { return sqlite3_wal_checkpoint(this->m_pDB, zDB); }
		//int sqlite3_vtab_config(sqlite3*, int op, ...);

		int Wal_Checkpoint_v2(const char *zDb, int eMode, int *pnLog, int *pnCkpt) { return sqlite3_wal_checkpoint_v2(this->m_pDB, zDb, eMode, pnLog, pnCkpt); } //override
	public:
		sqlite3 *m_pDB;
	public:
		static void RegisterBehaviorContext(AZ::BehaviorContext* bc);
	};
	////////////////////////////////////////////////////////////////////////
}