#pragma once

#include <AzCore\RTTI\TypeInfo.h>
#include <AzCore/RTTI/BehaviorContext.h>

namespace SQLite {
	class SQLiteMutex;

	class SQLiteMutexRequests
		: public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;

		using BusIdType = SQLiteMutex *;
	public:
		virtual void Enter() = 0;
		virtual int Try() = 0;
		virtual void Leave() = 0;
	};
	using SQLiteMutexBus = AZ::EBus<SQLiteMutexRequests>;

	////////////////////////////////////////////////////////////////////////
	//SQLiteMutex Class
	class SQLiteMutex : SQLiteMutexBus::Handler {
	public:
		AZ_TYPE_INFO(SQLiteMutex, "{F5EB85B2-E094-4E13-AE1A-71812CD62139}");
	public: //not implemented in lua
		SQLiteMutex(int N);
		~SQLiteMutex();
	public:
		SQLiteMutex(sqlite3_mutex * mutex);
	public:
		void Enter();
		int Try();
		void Leave();
	/*#ifndef NDEBUG
	public: //not implemented in lua
		int Held();
		int NotHeld();
	#endif*/
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