#include "StdAfx.h"
#include "SQLite\SQLiteBackup.h"

namespace SQLite {
	void SQLiteBackup::RegisterBehaviorContext(AZ::BehaviorContext* bc) {
		////////////////////////////////////////////////////////////////////////
		// Backup Class
		#define SQLITEBAK_METHOD(name, defaultval, dbgDesc) ->Method(#name,&SQLiteBackup::##name##,##defaultval##,##dbgDesc##)
		bc->Class<SQLiteBackup>("SQLiteBackup")
			->Attribute(AZ::Script::Attributes::Category, "SQLite")
			->Constructor<SQLiteDB *, const char *, SQLiteDB *, const char *>()
			SQLITEBAK_METHOD(Step, nullptr, "")
			SQLITEBAK_METHOD(Finish, nullptr, "")
			SQLITEBAK_METHOD(Remaining, nullptr, "")
			SQLITEBAK_METHOD(PageCount, nullptr, "")
			;
		#undef SQLITEBAK_METHOD
		////////////////////////////////////////////////////////////////////////
	}

	SQLiteBackup::SQLiteBackup(SQLiteDB * dest, const char * dname, SQLiteDB * src, const char *sname) {
		this->m_finished = false;
		this->m_pBak = sqlite3_backup_init(dest->m_pDB, dname, src->m_pDB, sname);
		SQLiteBackupBus::Handler::BusConnect(this);
	}
	SQLiteBackup::~SQLiteBackup() {
		SQLiteBackupBus::Handler::BusDisconnect();
		if (!this->m_finished) this->Finish();
	}
	int SQLiteBackup::Step(int nPage) { return sqlite3_backup_step(this->m_pBak, nPage); }
	int SQLiteBackup::Finish() {
		this->m_finished = true;
		return sqlite3_backup_finish(this->m_pBak);
	}
	int SQLiteBackup::Remaining() { return sqlite3_backup_remaining(this->m_pBak); }
	int SQLiteBackup::PageCount() { return sqlite3_backup_pagecount(this->m_pBak); }
}