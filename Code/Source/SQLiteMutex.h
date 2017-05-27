#pragma once

#include <AzCore\RTTI\TypeInfo.h>
#include <AzCore/RTTI/BehaviorContext.h>

namespace SQLite3 {
	////////////////////////////////////////////////////////////////////////
	//SQLiteMutex Class
	class SQLiteMutex {
	public:
		AZ_TYPE_INFO(SQLiteMutex, "{F5EB85B2-E094-4E13-AE1A-71812CD62139}");
	public:
		SQLiteMutex(sqlite3_mutex * mutex) { this->m_pMutex = mutex; }
	public:
		void Enter() { sqlite3_mutex_enter(this->m_pMutex); }
		int Try() { return sqlite3_mutex_try(this->m_pMutex); }
		void Leave() { sqlite3_mutex_leave(this->m_pMutex); }
	public:
		static void RegisterBehaviorContext(AZ::BehaviorContext* bc);
	public:
		sqlite3_mutex * m_pMutex;
	};
	////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////
//SQLiteMutex Class
/*
sqlite3_mutex *sqlite3_mutex_alloc(int); //dont implement
void sqlite3_mutex_free(sqlite3_mutex*); //dont implement?
void sqlite3_mutex_enter(sqlite3_mutex*);
int sqlite3_mutex_try(sqlite3_mutex*);
void sqlite3_mutex_leave(sqlite3_mutex*);
int sqlite3_mutex_held(sqlite3_mutex*); //dont implement?
int sqlite3_mutex_notheld(sqlite3_mutex*); //dont implement?
*/
////////////////////////////////////////////////////////////////////////