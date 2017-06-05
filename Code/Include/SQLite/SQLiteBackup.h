#pragma once

#include <AzCore\RTTI\TypeInfo.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include "SQLiteDB.h"

namespace SQLite3 {
	class SQLiteBackup;

	class SQLiteBackupRequests : public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;

		using BusIdType = SQLiteBackup *;
	public:
		virtual int Step(int nPage) = 0;
		virtual int Finish() = 0;
		virtual int Remaining() = 0;
		virtual int PageCount() = 0;
	};
	using SQLiteBackupBus = AZ::EBus<SQLiteBackupRequests>;

	////////////////////////////////////////////////////////////////////////
	//SQLiteBackup Class
	class SQLiteBackup : public SQLiteBackupBus::Handler {
	public:
		AZ_TYPE_INFO(SQLiteBackup, "{28AF58EE-9459-41C9-A88B-ADC6CBC05657}");
	public:
		SQLiteBackup(SQLiteDB * dest, const char * dname, SQLiteDB * src, const char *sname);
		~SQLiteBackup();
	public:
		int Step(int nPage);
		int Finish();
		int Remaining();
		int PageCount();
	public:
		sqlite3_backup* m_pBak;
	public:
		static void RegisterBehaviorContext(AZ::BehaviorContext* bc);
	private:
		bool m_finished;
	};
	////////////////////////////////////////////////////////////////////////
}