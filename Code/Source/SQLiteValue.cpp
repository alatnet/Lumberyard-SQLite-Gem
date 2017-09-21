#include "StdAfx.h"
#include "SQLite\SQLiteValue.h"

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

	SQLiteValue::SQLiteValue() {
		this->m_avail = true;
		SQLiteValueBus::Handler::BusConnect(this);
	}
	SQLiteValue::SQLiteValue(sqlite3_value * val) {
		this->m_avail = true;
		this->m_pVal = val;
		SQLiteValueBus::Handler::BusConnect(this);
	}
	SQLiteValue::~SQLiteValue() {
		SQLiteValueBus::Handler::BusDisconnect();
		if (this->m_avail) this->Free();
	}
	const void* SQLiteValue::Blob() { return sqlite3_value_blob(this->m_pVal); }
	int SQLiteValue::Bytes() { return sqlite3_value_bytes(this->m_pVal); }
	int SQLiteValue::Bytes16() { return sqlite3_value_bytes16(this->m_pVal); }
	double SQLiteValue::Double() { return sqlite3_value_double(this->m_pVal); }
	int SQLiteValue::Int() { return sqlite3_value_int(this->m_pVal); }
	__int64 SQLiteValue::Int64() { return (__int64)sqlite3_value_int64(this->m_pVal); }
	AZStd::string SQLiteValue::Text() {
		const unsigned char * str = sqlite3_value_text(this->m_pVal);

		return (str) ?
			AZStd::string(reinterpret_cast<const char *>(str)) : //true
			AZStd::string(""); //false
	}
	const void* SQLiteValue::Text16() { return sqlite3_value_text16(this->m_pVal); }
	const void* SQLiteValue::Text16LE() { return sqlite3_value_text16le(this->m_pVal); }
	const void* SQLiteValue::Text16BE() { return sqlite3_value_text16be(this->m_pVal); }
	int SQLiteValue::Type() { return sqlite3_value_type(this->m_pVal); }
	int SQLiteValue::Numeric_Type() { return sqlite3_value_numeric_type(this->m_pVal); }
	SQLiteValue * SQLiteValue::Value_Dup() { return new SQLiteValue(sqlite3_value_dup(this->m_pVal)); }
	void SQLiteValue::Free() {
		this->m_avail = false;
		return sqlite3_value_free(this->m_pVal);
	}
	unsigned int SQLiteValue::Subtype() { return sqlite3_value_subtype(this->m_pVal); }
}