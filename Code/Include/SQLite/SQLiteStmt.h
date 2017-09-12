#pragma once

#include <AzCore\RTTI\TypeInfo.h>
#include <AzCore/RTTI/BehaviorContext.h>

#include "SQLiteValue.h"

namespace SQLite3 {
	class SQLiteStmt;
	class SQLiteDB;

	class SQLiteStmtRequests : public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;

		using BusIdType = SQLiteStmt *;
	public:
		virtual int Step() = 0;
		virtual int Finalize() = 0;
		virtual int Reset() = 0;
		//------------------
		virtual const void * Column_Blob(int iCol) = 0;
		virtual int Column_Bytes(int iCol) = 0;
		virtual int Column_Bytes16(int iCol) = 0;
		virtual double Column_Double(int iCol) = 0;
		virtual int Column_Int(int iCol) = 0;
		virtual __int64 Column_Int64(int iCol) = 0;
		virtual const char * Column_Text(int iCol) = 0;
		virtual const void * Column_Text16(int iCol) = 0;
		virtual int Column_Type(int iCol) = 0;
		virtual SQLiteValue * Column_Value(int iCol) = 0;
		virtual const char *Column_Name(int N) = 0;
		virtual const void *Column_Name16(int N) = 0;
		virtual int Column_Count() = 0;
		//------------------
		#ifdef SQLITE_ENABLE_COLUMN_METADATA
		virtual const char * Column_Database_Name(int N) = 0;
		virtual const void * Column_Database_Name16(int N) = 0;
		virtual const char * Column_Database_Table_Name(int N) = 0;
		virtual const void * Column_Database_Table_Name16(int N) = 0;
		virtual const char * Column_Database_Origin_Name(int N) = 0;
		virtual const void * Column_Database_Origin_Name16(int N) = 0;
		#endif
		//------------------
		virtual int Data_Count() = 0;
		virtual SQLiteDB *DB_Handle() = 0;
		virtual const char *SQL() = 0;
		virtual char *Expanded_SQL() = 0;
		virtual SQLiteStmt * Next_Stmt(SQLiteDB * db) = 0;
		virtual int Busy() = 0;
		virtual int ReadOnly() = 0;
		//------------------
		#ifdef SQLITE_ENABLE_STMT_SCANSTATUS
		virtual int ScanStatus(int idx, int iScanStatusOp, void *pOut) = 0;
		virtual void ScanStatus_Reset() = 0;
		virtual int Status(int op, int resetFlg) = 0;
		#endif
		//------------------
		virtual int Bind_Blob(int index, const void* val, int length, void(*deconstructor)(void*)) = 0; //fifth arg SQLITE_STATIC
		virtual int Bind_Blob64(int index, const void* val, unsigned __int64 length, void(*deconstructor)(void*)) = 0; //fifth arg SQLITE_STATIC
		virtual int Bind_Double(int index, double val) = 0;
		virtual int Bind_Int(int index, int val) = 0;
		virtual int Bind_Int64(int index, __int64 val) = 0;
		virtual int Bind_Null(int index) = 0;
		virtual int Bind_Text(int index, const char* val, int length, void(*deconstructor)(void*)) = 0; //fifth arg SQLITE_STATIC //lua overrider
		virtual int Bind_Text16(int index, const void* val, int length, void(*deconstructor)(void*)) = 0; //fifth arg SQLITE_STATIC //lua overrider
		virtual int Bind_Text64(int index, const char* val, unsigned __int64 length, void(*deconstructor)(void*), unsigned char encoding) = 0; //fifth arg SQLITE_STATIC //6th - SQLITE_UTF8, SQLITE_UTF16, SQLITE_UTF16BE, or SQLITE_UTF16LE //lua overrider
		virtual int Bind_Value(int index, const SQLiteValue* val) = 0;
		//virtual int Bind_Pointer(int index, void* val, const char* type, void(*deconstructor)(void*)) = 0; //fifth arg SQLITE_STATIC
		virtual int Bind_ZeroBlob(int index, int length) = 0;
		virtual int Bind_ZeroBlob64(int index, unsigned __int64 length) = 0;
		virtual int Bind_Parameter_Index(const char *zName) = 0;
		virtual int Bind_Parameter_Count() = 0;
		virtual const char * Bind_Parameter_Name(int index) = 0;
		//------------------
	};

	using SQLiteStmtBus = AZ::EBus<SQLiteStmtRequests>;

	////////////////////////////////////////////////////////////////////////
	//SQLiteStmt Class
	class SQLiteStmt : public SQLiteStmtBus::Handler {
	public:
		AZ_TYPE_INFO(SQLiteStmt, "{9625FDAC-FE7F-4B03-AF5F-3A549E069129}");
	public:
		SQLiteStmt();
		SQLiteStmt(sqlite3_stmt * stmt);
		~SQLiteStmt();
	public:
		int Step();
		int Finalize();
		int Reset();
	public:
		const void * Column_Blob(int iCol);
		int Column_Bytes(int iCol);
		int Column_Bytes16(int iCol);
		double Column_Double(int iCol);
		int Column_Int(int iCol);
		__int64 Column_Int64(int iCol);
		const char * Column_Text(int iCol);
		const void * Column_Text16(int iCol); //convert to wstring?
		int Column_Type(int iCol);
		SQLiteValue * Column_Value(int iCol);
		const char *Column_Name(int N);
		const void *Column_Name16(int N); //convert to wstring?
		int Column_Count();
	#ifdef SQLITE_ENABLE_COLUMN_METADATA
	public:
		const char * Column_Database_Name(int N);
		const void * Column_Database_Name16(int N);
		const char * Column_Database_Table_Name(int N);
		const void * Column_Database_Table_Name16(int N);
		const char * Column_Database_Origin_Name(int N);
		const void * Column_Database_Origin_Name16(int N);
	#endif
	public:
		int Data_Count();
		SQLiteDB *DB_Handle();
	public:
		const char *SQL();
		char *Expanded_SQL();
	public:
		SQLiteStmt * Next_Stmt(SQLiteDB * db);
	public:
		int Busy();
		int ReadOnly();
	public: //not currently implemented in lua because of void*
		//int ScanStatus(int idx, int iScanStatusOp);
		//int sqlite3_stmt_scanstatus(
		//	sqlite3_stmt *pStmt,      /* Prepared statement for which info desired */
		//	int idx,                  /* Index of loop to report on */
		//	int iScanStatusOp,        /* Information desired.  SQLITE_SCANSTAT_* */
		//	void *pOut                /* Result written here */
		//);
		//void sqlite3_stmt_scanstatus_reset(sqlite3_stmt*);
		//int sqlite3_stmt_status(sqlite3_stmt*, int op,int resetFlg);

		#ifdef SQLITE_ENABLE_STMT_SCANSTATUS
		int ScanStatus(int idx, int iScanStatusOp, void *pOut);
		void ScanStatus_Reset();
		int Status(int op, int resetFlg);
		#endif
	public:
		int Bind_Blob(int index, const void* val, int length, void(*deconstructor)(void*)); //fifth arg SQLITE_STATIC
		int Bind_Blob64(int index, const void* val, unsigned __int64 length, void(*deconstructor)(void*)); //fifth arg SQLITE_STATIC
		int Bind_Double(int index, double val);
		int Bind_Int(int index, int val);
		int Bind_Int64(int index, __int64 val);
		int Bind_Null(int index);
		int Bind_Text(int index, const char* val, int length, void(*deconstructor)(void*)); //fifth arg SQLITE_STATIC //lua overrider
		int Bind_Text16(int index, const void* val, int length, void(*deconstructor)(void*)); //fifth arg SQLITE_STATIC //lua overrider
		int Bind_Text64(int index, const char* val, unsigned __int64 length, void(*deconstructor)(void*), unsigned char encoding); //fifth arg SQLITE_STATIC //6th - SQLITE_UTF8, SQLITE_UTF16, SQLITE_UTF16BE, or SQLITE_UTF16LE //lua overrider
		int Bind_Value(int index, const SQLiteValue* val);
		//int Bind_Pointer(int index, void* val, const char* type, void(*deconstructor)(void*)); //fifth arg SQLITE_STATIC
		int Bind_ZeroBlob(int index, int length);
		int Bind_ZeroBlob64(int index, unsigned __int64 length);
		int Bind_Parameter_Index(const char *zName);
		int Bind_Parameter_Count();
		const char * Bind_Parameter_Name(int index);
	public:
		sqlite3_stmt * m_pStmt;
		int m_err;
	public:
		static void RegisterBehaviorContext(AZ::BehaviorContext* bc);
	private:
		bool m_running;
	};
	////////////////////////////////////////////////////////////////////////
}