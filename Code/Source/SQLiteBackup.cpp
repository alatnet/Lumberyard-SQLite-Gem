#include "StdAfx.h"
#include "SQLiteBackup.h"

namespace SQLite3 {
	void SQLiteBackup::RegisterBehaviorContext(AZ::BehaviorContext* bc) {
		////////////////////////////////////////////////////////////////////////
		// Backup Class
		#define SQLITEBAK_METHOD(name, defaultval, dbgDesc) ->Method(#name,&SQLiteBackup::##name##,##defaultval##,##dbgDesc##)
		bc->Class<SQLiteBackup>("SQLiteBackup")
			->Constructor<SQLiteDB *, const char *, SQLiteDB *, const char *>()
			SQLITEBAK_METHOD(Step, nullptr, "")
			SQLITEBAK_METHOD(Finish, nullptr, "")
			SQLITEBAK_METHOD(Remaining, nullptr, "")
			SQLITEBAK_METHOD(PageCount, nullptr, "")
			;
		#undef SQLITEBAK_METHOD
		////////////////////////////////////////////////////////////////////////
	}
}