#pragma once

#include <AzCore\RTTI\TypeInfo.h>
#include <AzCore/RTTI/BehaviorContext.h>

namespace SQLite {
	class SQLiteVFS;

	class SQLiteVFSRequests : public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;

		using BusIdType = SQLiteVFS *;
	public:
		virtual SQLiteVFS * Find(const char * vfsName) = 0;
		virtual int Register(int makeDflt) = 0;
		virtual int Unregister() = 0;
	};
	using SQLiteVFSBus = AZ::EBus<SQLiteVFSRequests>;

	////////////////////////////////////////////////////////////////////////
	//SQLiteVFS Class
	class SQLiteVFS : public SQLiteVFSBus::Handler {
	public:
		AZ_TYPE_INFO(SQLiteVFS, "{657B0155-BA5B-4FDF-91EC-A967051577EC}");
	public:
		SQLiteVFS(const char * vfsName);
		SQLiteVFS(sqlite3_vfs * vfs);
		~SQLiteVFS();
	public:
		SQLiteVFS * Find(const char * vfsName);
		int Register(int makeDflt);
		int Unregister();
	public:
		sqlite3_vfs * m_pVFS;
	public:
		static void RegisterBehaviorContext(AZ::BehaviorContext* bc);
	};
	////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////
//SQLiteVFS Class
/*
sqlite3_vfs *sqlite3_vfs_find(const char *zVfsName);
int sqlite3_vfs_register(sqlite3_vfs*, int makeDflt);
int sqlite3_vfs_unregister(sqlite3_vfs*);
*/
////////////////////////////////////////////////////////////////////////