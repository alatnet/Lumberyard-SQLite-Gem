#pragma once

#include <AzCore\RTTI\TypeInfo.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include "SQLiteDB.h"

namespace SQLite3 {
	////////////////////////////////////////////////////////////////////////
	//SQLiteBackup Class
	class SQLiteBackup {
	public:
		AZ_TYPE_INFO(SQLiteBackup, "{28AF58EE-9459-41C9-A88B-ADC6CBC05657}");
	public:
		SQLiteBackup(SQLiteDB * dest, const char * dname, SQLiteDB * src, const char *sname) { this->m_pBak = sqlite3_backup_init(dest->m_pDB, dname, src->m_pDB, sname); }
	public:
		int Step(int nPage) { return sqlite3_backup_step(this->m_pBak, nPage); }
		int Finish() { return sqlite3_backup_finish(this->m_pBak); }
		int Remaining() { return sqlite3_backup_remaining(this->m_pBak); }
		int PageCount() { return sqlite3_backup_pagecount(this->m_pBak); }
	public:
		sqlite3_backup* m_pBak;
	public:
		static void RegisterBehaviorContext(AZ::BehaviorContext* bc);
	};
	////////////////////////////////////////////////////////////////////////
}