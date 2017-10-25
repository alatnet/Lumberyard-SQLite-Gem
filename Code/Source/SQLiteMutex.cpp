#include "StdAfx.h"
#include "SQLite\SQLiteMutex.h"

namespace SQLite {
	void SQLiteMutex::RegisterBehaviorContext(AZ::BehaviorContext* bc) {
	#define SQLITEMUTEX_METHOD(name, defaultval, dbgDesc) ->Method(#name,&SQLiteMutex::##name##,##defaultval##,##dbgDesc##)
		bc->Class<SQLiteMutex>("SQLiteMutex")
			->Attribute(AZ::Script::Attributes::Category, "SQLite")
			SQLITEMUTEX_METHOD(Enter, nullptr, "")
			SQLITEMUTEX_METHOD(Try, nullptr, "")
			SQLITEMUTEX_METHOD(Leave, nullptr, "")
			;
	#undef SQLITEMUTEX_METHOD
	}

	SQLiteMutex::SQLiteMutex(int N) {
		this->m_pMutex = sqlite3_mutex_alloc(N);
		SQLiteMutexBus::Handler::BusConnect(this);
	}

	SQLiteMutex::SQLiteMutex(sqlite3_mutex * mutex) {
		this->m_pMutex = mutex;
		SQLiteMutexBus::Handler::BusConnect(this);
	}

	SQLiteMutex::~SQLiteMutex() {
		SQLiteMutexBus::Handler::BusDisconnect();
		sqlite3_mutex_free(this->m_pMutex);
	}

	void SQLiteMutex::Enter() { sqlite3_mutex_enter(this->m_pMutex); }
	int SQLiteMutex::Try() { return sqlite3_mutex_try(this->m_pMutex); }
	void SQLiteMutex::Leave() { sqlite3_mutex_leave(this->m_pMutex); }

	/*#ifndef NDEBUG
	int SQLiteMutex::Held() { return sqlite3_mutex_held(this->m_pMutex); }
	int SQLiteMutex::NotHeld(){ return sqlite3_mutex_notheld(this->m_pMutex); }
	#endif*/
}