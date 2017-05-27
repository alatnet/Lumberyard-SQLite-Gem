#include "StdAfx.h"
#include "SQLiteStmt.h"

#include "SQLiteDB.h"

namespace SQLite3 {
	////////////////////////////////////////////////////////////////////////
	/*int SQLiteStmt::ScanStatus(int idx, int iScanStatusOp) {
		return 0;
	}*/

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
			SQLITESTMT_METHOD(DB_Handle, nullptr, "") //causes build errors
			//	SQLITE_METHOD_OVERRIDE(DB_HandleScript)
			SQLITESTMT_METHOD(SQL, nullptr, "")
			SQLITESTMT_METHOD(Expanded_SQL, nullptr, "")
			SQLITESTMT_METHOD(Next_Stmt, nullptr, "") //causes build errors
			//	SQLITE_METHOD_OVERRIDE(Next_StmtScript)
			SQLITESTMT_METHOD(Busy, nullptr, "")
			SQLITESTMT_METHOD(ReadOnly, nullptr, "")
			;
		#undef SQLITESTMT_METHOD_ALT
		#undef SQLITESTMT_METHOD
		////////////////////////////////////////////////////////////////////////
	}
	////////////////////////////////////////////////////////////////////////
}