#include "StdAfx.h"
#include "SQLiteMutex.h"

namespace SQLite3 {
	void SQLiteMutex::RegisterBehaviorContext(AZ::BehaviorContext* bc) {
	#define SQLITEMUTEX_METHOD(name, defaultval, dbgDesc) ->Method(#name,&SQLiteMutex::##name##,##defaultval##,##dbgDesc##)
		bc->Class<SQLiteMutex>("SQLite3Mutex")
			SQLITEMUTEX_METHOD(Enter, nullptr, "")
			SQLITEMUTEX_METHOD(Try, nullptr, "")
			SQLITEMUTEX_METHOD(Leave, nullptr, "")
			;
	#undef SQLITEMUTEX_METHOD
	}
}