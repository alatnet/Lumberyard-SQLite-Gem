#pragma once

#include <AzCore/Component/Component.h>

#include <AzCore\RTTI\TypeInfo.h>
#include <AzCore/RTTI/BehaviorContext.h>

#include "SQLiteMutex.h"

namespace SQLite3 {
	class SQLiteStmt;
	class SQLiteDB;

	class SQLiteDBRequests
		: public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;

		using BusIdType = SQLiteDB *;
	public:
		virtual int Open(const char * path) = 0;
		virtual int Open16(const char * path) = 0;
		virtual int Open_v2(const char * path, int flags, const char *zVfs) = 0;
		virtual int Close() = 0;
		//virtual int Close_v2() = 0;
		/*virtual int Exec(const char *sql) = 0;
		virtual int Exec(const char *sql, char **errmsg) = 0;
		virtual int Exec(const char *sql, int(*callback)(void*, int, char**, char**)) = 0;
		virtual int Exec(const char *sql, int(*callback)(void*, int, char**, char**), char **errmsg) = 0;*/
		virtual int Exec(const char *sql, int(*callback)(void*, int, char**, char**), void *, char **errmsg) = 0;
		virtual SQLiteStmt * Prepare(const char * sql, int nByte, const char **pzTail) = 0;
		virtual SQLiteStmt * Prepare_v2(const char * sql, int nByte, const char **pzTail) = 0;
	#ifdef SQLITE_PREPARE_PERSISTENT
		virtual SQLiteStmt * Prepare_v3(const char *zSql, int nByte, unsigned int prepFlags, const char **pzTail) = 0;
	#endif
		virtual SQLiteStmt * Prepare16(const char * sql, int nByte, const void **pzTail) = 0;
		virtual SQLiteStmt * Prepare16_v2(const char * sql, int nByte, const void **pzTail) = 0;
	#ifdef SQLITE_PREPARE_PERSISTENT
		virtual SQLiteStmt * Prepare16_v3(const char *zSql, int nByte, unsigned int prepFlags, const void **pzTail) = 0;
	#endif
		virtual int ErrCode() = 0;
		virtual int ExtErrCode() = 0;
		virtual int ExtResCode(int onoff) = 0;
		virtual const char * ErrMsg() = 0;
		virtual const void * ErrMsg16() = 0;
		virtual int Limit(int id, int newVal) = 0;
		virtual SQLiteMutex * DB_Mutex() = 0;
		virtual int DB_CacheFlush() = 0;
		virtual const char * DB_Filename(const char * zDbName) = 0;
		virtual int DB_ReadOnly(const char *zDbName) = 0;
		virtual int DB_Release_Memory() = 0;
		virtual int DB_Status(int op, int *pCur, int *pHiwtr, int resetFlg) = 0;
		virtual int Declare_VTab(const char * zSQL) = 0;
		virtual int Enable_Load_Extension(int onoff) = 0;
		//virtual int DB_Config(int op, ...) = 0;
		virtual int File_Control(const char * zDbName, int op, void * x) = 0;
		virtual int Get_Table(const char *zSql, char ***pazResult, int *pnRow, int *pnColumn, char **pzErrmsg) = 0;
		virtual void Free_Table(char **result) = 0;
		virtual int Get_AutoCommit() = 0;
		virtual void Interrupt() = 0;
		virtual __int64 Last_Insert_RowId() = 0;
		virtual int Load_Extension(const char * zFile, const char * zProc, char ** pzErrMsg) = 0;
		virtual int Overload_Function(const char * zFuncName, int nArgs) = 0;
		virtual void Set_Last_Insert_RowId(__int64 N) = 0;
		virtual int System_ErrNo() = 0;
		virtual int Total_Changes() = 0;
		virtual int VTab_On_Conflict() = 0;
		virtual int Wal_AutoCheckpoint(int N) = 0;
		virtual int Wal_Checkpoint(const char * zDB) = 0;
		//virtual int VTab_Config(int op, ...) = 0;
		virtual int Table_Column_Metadata(const char *zDbName, const char *zTableName, const char *zColumnName, char const **pzDataType, char const ** pzCollSeq, int *pNotNull, int *pPrimaryKey, int *pAutoinc) = 0;
		virtual int Wal_Checkpoint_v2(const char *zDb, int eMode, int *pnLog, int *pnCkpt) = 0;
		virtual int Status64(int op, __int64 *pCurrent, __int64 *pHighwater, int resetFlag) = 0;
		virtual __int64 Memory_Used() = 0;
		virtual __int64 Memory_HighWater(int resetFlag) = 0;
		virtual __int64 Soft_Heap_Limit64(__int64 N) = 0;
		virtual const char * ErrStr(int rc) = 0;
		virtual const char * LibVersion() = 0;
		virtual int LibVersion_Number() = 0;
		virtual const char * SourceID() = 0;
		virtual int Enable_Shared_Cache(int enable) = 0;
		virtual void Reset_Auto_Ext() = 0;
		virtual int ThreadSafe() = 0;
		virtual const char * URI_Parameter(const char *zFilename, const char *zParam) = 0;
		virtual int URI_Boolean(const char *zFilename, const char *zParam, int bDflt) = 0;
		virtual __int64 URI_Int64(const char *zFilename, const char *zParam, int bDflt) = 0;
		virtual int Status(int op, int *pCurrent, int *pHighwater, int resetFlag) = 0;
		virtual int Complete(const char *zSql) = 0;
		virtual int Complete16(const void *zSql) = 0;
	};
	using SQLiteDBBus = AZ::EBus<SQLiteDBRequests>;

	////////////////////////////////////////////////////////////////////////
	//SQLiteDB Class
	class SQLiteDB : public SQLiteDBBus::Handler {
	public:
		AZ_TYPE_INFO(SQLiteDB, "{36DD1FB6-66E5-42D0-86B4-6C4072009A16}");
	public:
		SQLiteDB();
	public: //not implemented in lua
		SQLiteDB(sqlite3* db);
		SQLiteDB(sqlite3* db, AZ::EntityId id);
	public: //not implemented in lua
		SQLiteDB(SQLiteDB * db);
		SQLiteDB(SQLiteDB * db, AZ::EntityId id);
	public:
		~SQLiteDB();
	public:
		int Open(const char * path);
		int Open16(const char * path);
		int Open_v2(const char * path, int flags, const char *zVfs);
	public:
		int Close();
		//int Close_v2();
	private:
		int Close2Open();
	public: //not implemented in lua
		/*int Exec(const char *sql);
		int Exec(const char *sql, char **errmsg);
		int Exec(const char *sql, int(*callback)(void*, int, char**, char**));
		int Exec(const char *sql, int(*callback)(void*, int, char**, char**), char **errmsg);*/
		int Exec(const char *sql, int(*callback)(void*, int, char**, char**), void *, char **errmsg);
	public:
		SQLiteStmt * Prepare(const char * sql, int nByte, const char **pzTail);
		SQLiteStmt * Prepare_v2(const char * sql, int nByte, const char **pzTail);
	#ifdef SQLITE_PREPARE_PERSISTENT
		SQLiteStmt * Prepare_v3(const char *zSql, int nByte, unsigned int prepFlags, const char **pzTail);
	#endif
		SQLiteStmt * Prepare16(const char * sql, int nByte, const void **pzTail);
		SQLiteStmt * Prepare16_v2(const char * sql, int nByte, const void **pzTail);
	#ifdef SQLITE_PREPARE_PERSISTENT
		SQLiteStmt * Prepare16_v3(const char *zSql, int nByte, unsigned int prepFlags, const void **pzTail);
	#endif
	public:
		int ErrCode();
		int ExtErrCode();
		int ExtResCode(int onoff);
		const char * ErrMsg();
		const void * ErrMsg16();
	public:
		int Limit(int id, int newVal);
	public:
		SQLiteMutex * DB_Mutex();
	public:
		int DB_CacheFlush();
		const char * DB_Filename(const char * zDbName);
		int DB_ReadOnly(const char *zDbName);
		int DB_Release_Memory();
		int DB_Status(int op, int *pCur, int *pHiwtr, int resetFlg); //override
		int Declare_VTab(const char * zSQL);
		int Enable_Load_Extension(int onoff);
	public: //not implemented in lua
		//int DB_Config(int op, ...) { return sqlite3_db_config(this->m_DB, op, ...); }
		int File_Control(const char * zDbName, int op, void * x);

		//requires tables
		int Get_Table(const char *zSql, char ***pazResult, int *pnRow, int *pnColumn, char **pzErrmsg);
		/*static*/ void Free_Table(char **result);
	public:
		int Get_AutoCommit();
		void Interrupt();
		__int64 Last_Insert_RowId();

		int Load_Extension(const char * zFile, const char * zProc, char ** pzErrMsg); //override

		int Overload_Function(const char * zFuncName, int nArgs);
		void Set_Last_Insert_RowId(__int64 N);
		int System_ErrNo();

		int Total_Changes();
		int VTab_On_Conflict();
		int Wal_AutoCheckpoint(int N);
		int Wal_Checkpoint(const char * zDB);
	public: //not implemented in lua
		//int VTab_Config(int op, ...) { return sqlite3_vtab_config(this->m_pDB, op, ...); }

		//requires tables
		int Table_Column_Metadata(const char *zDbName, const char *zTableName, const char *zColumnName, char const **pzDataType, char const ** pzCollSeq, int *pNotNull, int *pPrimaryKey, int *pAutoinc);
	public:
		int Wal_Checkpoint_v2(const char *zDb, int eMode, int *pnLog, int *pnCkpt); //override
	public: //static functions
		/*static*/ int Status64(int op, __int64 *pCurrent, __int64 *pHighwater, int resetFlag); //override
		/*static*/ __int64 Memory_Used();
		/*static*/ __int64 Memory_HighWater(int resetFlag);
		/*static*/ __int64 Soft_Heap_Limit64(__int64 N);
		/*static*/ const char * ErrStr(int rc);
		/*static*/ const char * LibVersion();
		/*static*/ int LibVersion_Number();
		/*static*/ const char * SourceID();
		/*static*/ int Enable_Shared_Cache(int enable);
		/*static*/ void Reset_Auto_Ext();
		/*static*/ int ThreadSafe();
		/*static*/ const char * URI_Parameter(const char *zFilename, const char *zParam);
		/*static*/ int URI_Boolean(const char *zFilename, const char *zParam, int bDflt);
		/*static*/ __int64 URI_Int64(const char *zFilename, const char *zParam, int bDflt); //override
		/*static*/ int Status(int op, int *pCurrent, int *pHighwater, int resetFlag); //override
		/*static*/ int Complete(const char *zSql);
		/*static*/ int Complete16(const void *zSql);
	public:
		sqlite3 *m_pDB;
	public:
		static void RegisterBehaviorContext(AZ::BehaviorContext* bc);
	public:
		AZ::EntityId m_entityid;
	private:
		enum OpenType {
			CLOSED,
			OPEN,
			OPEN16,
			OPENV2
		};
		OpenType m_OpenType;
	};
	////////////////////////////////////////////////////////////////////////
}