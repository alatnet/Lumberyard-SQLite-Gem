#pragma once

#include <AzCore\RTTI\TypeInfo.h>
#include <AzCore/RTTI/BehaviorContext.h>

#include "SQLiteValue.h"
#include "InternalFunctions.h"

namespace SQLite3 {
	////////////////////////////////////////////////////////////////////////
	//SQLiteStmt Class
	class SQLiteStmt {
	public:
		AZ_TYPE_INFO(SQLiteStmt, "{9625FDAC-FE7F-4B03-AF5F-3A549E069129}");
	public:
		SQLiteStmt() {}
		SQLiteStmt(sqlite3_stmt * stmt) { this->m_pStmt = stmt; }
	public:
		int Step() { return sqlite3_step(this->m_pStmt); }
		int Finalize() { return sqlite3_finalize(this->m_pStmt); }
		int Reset() { return sqlite3_reset(this->m_pStmt); }
	public:
		const void * Column_Blob(int iCol) { return sqlite3_column_blob(this->m_pStmt, iCol); }
		int Column_Bytes(int iCol) { return sqlite3_column_bytes(this->m_pStmt, iCol); }
		int Column_Bytes16(int iCol) { return sqlite3_column_bytes16(this->m_pStmt, iCol); }
		double Column_Double(int iCol) { return sqlite3_column_double(this->m_pStmt, iCol); }
		int Column_Int(int iCol) { return sqlite3_column_int(this->m_pStmt, iCol); }
		__int64 Column_Int64(int iCol) { return sqlite3_column_int64(this->m_pStmt, iCol); }
		const char * Column_Text(int iCol) { return (const char *)sqlite3_column_text(this->m_pStmt, iCol); }
		const void * Column_Text16(int iCol) { return sqlite3_column_text16(this->m_pStmt, iCol); } //convert to wstring?
		int Column_Type(int iCol) { return sqlite3_column_type(this->m_pStmt, iCol); }
		SQLiteValue * Column_Value(int iCol) { return new SQLiteValue(sqlite3_column_value(this->m_pStmt, iCol)); }
		const char *Column_Name(int N) { return sqlite3_column_name(this->m_pStmt, N); }
		const void *Column_Name16(int N) { return sqlite3_column_name16(this->m_pStmt, N); } //convert to wstring?
		int Column_Count() { return sqlite3_column_count(this->m_pStmt); }
	#ifdef SQLITE_ENABLE_COLUMN_METADATA
	public:
		const char * Column_Database_Name(int N) { return sqlite3_column_database_name(this->m_pStmt, N); }
		const void * Column_Database_Name16(int N) { return sqlite3_column_database_name16(this->m_pStmt, N); }
		const char * Column_Database_Table_Name(int N) { return sqlite3_column_table_name(this->m_pStmt, N); }
		const void * Column_Database_Table_Name16(int N) { return sqlite3_column_table_name16(this->m_pStmt, N); }
		const char * Column_Database_Origin_Name(int N) { return sqlite3_column_origin_name(this->m_pStmt, N); }
		const void * Column_Database_Origin_Name16(int N) { return sqlite3_column_origin_name16(this->m_pStmt, N); }
	#endif
	public:
		int Data_Count() { return sqlite3_data_count(this->m_pStmt); }
		SQLiteDB *DB_Handle() { return new SQLiteDB(sqlite3_db_handle(this->m_pStmt)); }
		//sqlite3* DB_Handle() { return sqlite3_db_handle(this->m_pStmt); } //override
	public:
		const char *SQL() { return sqlite3_sql(this->m_pStmt); }
		char *Expanded_SQL() { return sqlite3_expanded_sql(this->m_pStmt); }
	public:
		SQLiteStmt * Next_Stmt(SQLiteDB * db) { return new SQLiteStmt(sqlite3_next_stmt(db->m_pDB, this->m_pStmt)); }
		//sqlite3_stmt *sqlite3_next_stmt(sqlite3 *pDb, sqlite3_stmt *pStmt);
		//sqlite3_stmt * Next_Stmt(sqlite3 * db) { return sqlite3_next_stmt(db, this->m_pStmt); } //override
	public:
		int Busy() { return sqlite3_stmt_busy(this->m_pStmt); }
		int ReadOnly() { return sqlite3_stmt_readonly(this->m_pStmt); }
	public:
		//int ScanStatus(int idx, int iScanStatusOp);
		//int sqlite3_stmt_scanstatus(
		//	sqlite3_stmt *pStmt,      /* Prepared statement for which info desired */
		//	int idx,                  /* Index of loop to report on */
		//	int iScanStatusOp,        /* Information desired.  SQLITE_SCANSTAT_* */
		//	void *pOut                /* Result written here */
		//);
		//void sqlite3_stmt_scanstatus_reset(sqlite3_stmt*);
		//int sqlite3_stmt_status(sqlite3_stmt*, int op,int resetFlg);
	public:
		sqlite3_stmt * m_pStmt;
		int m_err;
	public:
		static void RegisterBehaviorContext(AZ::BehaviorContext* bc);
	};
	////////////////////////////////////////////////////////////////////////
}