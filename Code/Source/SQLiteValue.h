#pragma once

#include <AzCore\RTTI\TypeInfo.h>
#include <AzCore/RTTI/BehaviorContext.h>

namespace SQLite3 {
	////////////////////////////////////////////////////////////////////////
	//SQLiteValue Class
	class SQLiteValue {
	public:
		AZ_TYPE_INFO(SQLiteValue, "{757E4464-5909-406E-BD10-430D2B18BEA0}");
	public:
		SQLiteValue() {}
		SQLiteValue(sqlite3_value * val) { this->m_pVal = val; }
	public:
		const void *Blob() { return sqlite3_value_blob(this->m_pVal); }
		int Bytes() { return sqlite3_value_bytes(this->m_pVal); }
		int Bytes16() { return sqlite3_value_bytes16(this->m_pVal); }
		double Double() { return sqlite3_value_double(this->m_pVal); }
		int Int() { return sqlite3_value_int(this->m_pVal); }
		__int64 Int64() { return (__int64)sqlite3_value_int64(this->m_pVal); }
		const char* Text() { return (const char *)sqlite3_value_text(this->m_pVal); }
		const void* Text16() { return sqlite3_value_text16(this->m_pVal); }
		const void* Text16LE() { return sqlite3_value_text16le(this->m_pVal); }
		const void* Text16BE() { return sqlite3_value_text16be(this->m_pVal); }
		int Type() { return sqlite3_value_type(this->m_pVal); }
		int Numeric_Type() { return sqlite3_value_numeric_type(this->m_pVal); }
		SQLiteValue * Value_Dup() { return new SQLiteValue(sqlite3_value_dup(this->m_pVal)); }
		void Free() { return sqlite3_value_free(this->m_pVal); }
		unsigned int Subtype() { return sqlite3_value_subtype(this->m_pVal); }
	public:
		sqlite3_value * m_pVal;
	public:
		static void RegisterBehaviorContext(AZ::BehaviorContext* bc);
	};
	////////////////////////////////////////////////////////////////////////
}