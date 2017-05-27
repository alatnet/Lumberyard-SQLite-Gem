#include "StdAfx.h"
#include "SQLiteVFS.h"

namespace SQLite3 {
	void SQLiteVFS::RegisterBehaviorContext(AZ::BehaviorContext* bc) {
		#define SQLITEVFS_METHOD(name, defaultval, dbgDesc) ->Method(#name,&SQLiteVFS::##name##,##defaultval##,##dbgDesc##)
		bc->Class<SQLiteVFS>("SQLite3VFS")
			SQLITEVFS_METHOD(Find, nullptr, "")
			SQLITEVFS_METHOD(Register, nullptr, "")
			SQLITEVFS_METHOD(Unregister, nullptr, "")
			;
		#undef SQLITEVFS_METHOD
	}
}