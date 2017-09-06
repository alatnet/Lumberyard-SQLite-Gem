#include "StdAfx.h"
#include "SQLite\SQLiteStmt.h"
#include "SQLite\SQLiteDB.h"
#include "InternalFunctions.h"

namespace SQLite3 {
	////////////////////////////////////////////////////////////////////////
	void SQLiteStmt::RegisterBehaviorContext(AZ::BehaviorContext* bc) {
		////////////////////////////////////////////////////////////////////////
		// Statement Class
		#define SQLITESTMT_METHOD(name, defaultval, dbgDesc) ->Method(#name,&SQLiteStmt::##name##,##defaultval##,##dbgDesc##)
		#define SQLITESTMT_METHOD_ALT(name, func, defaultval, dbgDesc) ->Method(##name##,&SQLiteStmt::##func##,##defaultval##,##dbgDesc##)
		bc->Class<SQLiteStmt>("SQLiteStmt")
			SQLITESTMT_METHOD(Step, nullptr, "")
			SQLITESTMT_METHOD(Finalize, nullptr, "")
			SQLITESTMT_METHOD(Reset, nullptr, "")

			SQLITESTMT_METHOD(Column_Blob, nullptr, "")
			SQLITESTMT_METHOD(Column_Bytes, nullptr, "")
			SQLITESTMT_METHOD(Column_Bytes16, nullptr, "")
			SQLITESTMT_METHOD(Column_Double, nullptr, "")
			SQLITESTMT_METHOD(Column_Int, nullptr, "")
			SQLITESTMT_METHOD(Column_Int64, nullptr, "")
			SQLITESTMT_METHOD(Column_Text, nullptr, "")
			//SQLITESTMT_METHOD(Column_Text16, nullptr, "")
			SQLITESTMT_METHOD(Column_Type, nullptr, "")
			SQLITESTMT_METHOD(Column_Value, nullptr, "")
			SQLITESTMT_METHOD(Column_Name, nullptr, "")
			//SQLITESTMT_METHOD(Column_Name16, nullptr, "")
			SQLITESTMT_METHOD(Column_Count, nullptr, "")
			#ifdef SQLITE_ENABLE_COLUMN_METADATA
			SQLITESTMT_METHOD(Column_Database_Name, nullptr, "")
			SQLITESTMT_METHOD(Column_Database_Table_Name, nullptr, "")
			SQLITESTMT_METHOD(Column_Database_Origin_Name, nullptr, "")
			//SQLITESTMT_METHOD(Column_Database_Name16, nullptr, "")
			//SQLITESTMT_METHOD(Column_Database_Table_Name16, nullptr, "")
			//SQLITESTMT_METHOD(Column_Database_Origin_Name16, nullptr, "")
			#endif
			SQLITESTMT_METHOD(Data_Count, nullptr, "")
			SQLITESTMT_METHOD(DB_Handle, nullptr, "")
			SQLITESTMT_METHOD(SQL, nullptr, "")
			SQLITESTMT_METHOD(Expanded_SQL, nullptr, "")
			SQLITESTMT_METHOD(Next_Stmt, nullptr, "")
			SQLITESTMT_METHOD(Busy, nullptr, "")
			SQLITESTMT_METHOD(ReadOnly, nullptr, "")
			;
		#undef SQLITESTMT_METHOD_ALT
		#undef SQLITESTMT_METHOD
		////////////////////////////////////////////////////////////////////////
	}
	////////////////////////////////////////////////////////////////////////

	SQLiteStmt::SQLiteStmt() {
		this->m_running = true;
		SQLiteStmtBus::Handler::BusConnect(this);
	}
	SQLiteStmt::SQLiteStmt(sqlite3_stmt * stmt) {
		this->m_running = true;
		this->m_pStmt = stmt;
		SQLiteStmtBus::Handler::BusConnect(this);
	}
	SQLiteStmt::~SQLiteStmt() {
		SQLiteStmtBus::Handler::BusDisconnect();
		if (this->m_running) this->Finalize();
	}
	int SQLiteStmt::Step() { return sqlite3_step(this->m_pStmt); }
	int SQLiteStmt::Finalize() {
		this->m_running = false;
		return sqlite3_finalize(this->m_pStmt);
	}
	int SQLiteStmt::Reset() { return sqlite3_reset(this->m_pStmt); }
	const void * SQLiteStmt::Column_Blob(int iCol) { return sqlite3_column_blob(this->m_pStmt, iCol); }
	int SQLiteStmt::Column_Bytes(int iCol) { return sqlite3_column_bytes(this->m_pStmt, iCol); }
	int SQLiteStmt::Column_Bytes16(int iCol) { return sqlite3_column_bytes16(this->m_pStmt, iCol); }
	double SQLiteStmt::Column_Double(int iCol) { return sqlite3_column_double(this->m_pStmt, iCol); }
	int SQLiteStmt::Column_Int(int iCol) { return sqlite3_column_int(this->m_pStmt, iCol); }
	__int64 SQLiteStmt::Column_Int64(int iCol) { return sqlite3_column_int64(this->m_pStmt, iCol); }
	const char * SQLiteStmt::Column_Text(int iCol) { return (const char *)sqlite3_column_text(this->m_pStmt, iCol); }
	const void * SQLiteStmt::Column_Text16(int iCol) { return sqlite3_column_text16(this->m_pStmt, iCol); } //convert to wstring?
	int SQLiteStmt::Column_Type(int iCol) { return sqlite3_column_type(this->m_pStmt, iCol); }
	SQLiteValue * SQLiteStmt::Column_Value(int iCol) { return new SQLiteValue(sqlite3_column_value(this->m_pStmt, iCol)); }
	const char *SQLiteStmt::Column_Name(int N) { return sqlite3_column_name(this->m_pStmt, N); }
	const void *SQLiteStmt::Column_Name16(int N) { return sqlite3_column_name16(this->m_pStmt, N); } //convert to wstring?
	int SQLiteStmt::Column_Count() { return sqlite3_column_count(this->m_pStmt); }
	#ifdef SQLITE_ENABLE_COLUMN_METADATA
	const char * SQLiteStmt::Column_Database_Name(int N) { return sqlite3_column_database_name(this->m_pStmt, N); }
	const void * SQLiteStmt::Column_Database_Name16(int N) { return sqlite3_column_database_name16(this->m_pStmt, N); }
	const char * SQLiteStmt::Column_Database_Table_Name(int N) { return sqlite3_column_table_name(this->m_pStmt, N); }
	const void * SQLiteStmt::Column_Database_Table_Name16(int N) { return sqlite3_column_table_name16(this->m_pStmt, N); }
	const char * SQLiteStmt::Column_Database_Origin_Name(int N) { return sqlite3_column_origin_name(this->m_pStmt, N); }
	const void * SQLiteStmt::Column_Database_Origin_Name16(int N) { return sqlite3_column_origin_name16(this->m_pStmt, N); }
	#endif
	int SQLiteStmt::Data_Count() { return sqlite3_data_count(this->m_pStmt); }
	SQLiteDB *SQLiteStmt::DB_Handle() { return new SQLiteDB(sqlite3_db_handle(this->m_pStmt)); }
	const char *SQLiteStmt::SQL() { return sqlite3_sql(this->m_pStmt); }
	char *SQLiteStmt::Expanded_SQL() { return sqlite3_expanded_sql(this->m_pStmt); }
	SQLiteStmt * SQLiteStmt::Next_Stmt(SQLiteDB * db) { return new SQLiteStmt(sqlite3_next_stmt(db->m_pDB, this->m_pStmt)); }
	int SQLiteStmt::Busy() { return sqlite3_stmt_busy(this->m_pStmt); }
	int SQLiteStmt::ReadOnly() { return sqlite3_stmt_readonly(this->m_pStmt); }

	#ifdef SQLITE_ENABLE_STMT_SCANSTATUS
	int SQLiteStmt::ScanStatus(int idx, int iScanStatusOp, void *pOut) { return sqlite3_stmt_scanstatus(this->m_pStmt, idx, iScanStatusOp, pOut); }
	void SQLiteStmt::ScanStatus_Reset() { sqlite3_stmt_scanstatus_reset(this->m_pStmt); }
	int SQLiteStmt::Status(int op, int resetFlg) { return sqlite3_stmt_status(this->m_pStmt, op, resetFlg); }
	#endif
}