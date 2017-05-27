#include "StdAfx.h"
#include "SQLiteValue.h"

namespace SQLite3 {
	void SQLiteValue::RegisterBehaviorContext(AZ::BehaviorContext* bc) {
		////////////////////////////////////////////////////////////////////////
		// Value Class
		#define SQLITEVALUE_METHOD(name, defaultval, dbgDesc) ->Method(#name,&SQLiteValue::##name##,##defaultval##,##dbgDesc##)
		bc->Class<SQLiteValue>("SQLiteValue")
			//SQLITEVALUE_METHOD(Blob, nullptr, "")
			SQLITEVALUE_METHOD(Bytes, nullptr, "")
			SQLITEVALUE_METHOD(Bytes16, nullptr, "")
			SQLITEVALUE_METHOD(Double, nullptr, "")
			SQLITEVALUE_METHOD(Int, nullptr, "")
			SQLITEVALUE_METHOD(Int64, nullptr, "")
			SQLITEVALUE_METHOD(Text, nullptr, "")
			//SQLITEVALUE_METHOD(Text16, nullptr, "")
			//SQLITEVALUE_METHOD(Text16LE, nullptr, "")
			//SQLITEVALUE_METHOD(Text16BE, nullptr, "")
			SQLITEVALUE_METHOD(Type, nullptr, "")
			SQLITEVALUE_METHOD(Numeric_Type, nullptr, "")
			SQLITEVALUE_METHOD(Value_Dup, nullptr, "")
			SQLITEVALUE_METHOD(Free, nullptr, "")
			SQLITEVALUE_METHOD(Subtype, nullptr, "")
			;
		#undef SQLITEVALUE_METHOD
		////////////////////////////////////////////////////////////////////////
	}
}