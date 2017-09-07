
#pragma once

#include <AzCore/EBus/EBus.h>
#include <SQLite\sqlite3.h>
#include <SQLite\SQLiteDB.h>
#include <SQLite\SQLiteMutex.h>
#include <SQLite\SQLiteStmt.h>
#include <SQLite\SQLiteBackup.h>
#include <SQLite\SQLiteValue.h>
#include <SQLite\SQLiteVFS.h>

namespace SQLite
{
	const char * const SQLiteSystemComponentUUID = "{0758FD42-2B80-44E9-A0F9-0DDECC7B99B7}";

    class SQLiteRequests
        : public AZ::EBusTraits
    {
    public:
        static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;

		using BusIdType = AZ::EntityId;
	public:
        // Public functions
		virtual SQLite3::SQLiteDB * GetConnection() = 0;
		virtual SQLite3::SQLiteDB * NewConnection() = 0;
	public:
		virtual SQLite3::SQLiteBackup * NewBackup(SQLite3::SQLiteDB * dest, const char * dname, SQLite3::SQLiteDB * src, const char *sname) = 0;
	public:
		virtual SQLite3::SQLiteMutex * NewMutex(int N) = 0;
	public:
		virtual SQLite3::SQLiteVFS * NewVFS(const char * vfsName) = 0;
		virtual SQLite3::SQLiteVFS * NewVFS(sqlite3_vfs * vfs) = 0;
	protected: //Lua specific functions. Not exposed for usage in regular C++.
		virtual int ExecLua(const char *sql, void * cbarg) = 0;
		virtual int ExecToLua(AZ::EntityId id, const char *sql, void * cbarg) = 0;
	};
    using SQLiteRequestBus = AZ::EBus<SQLiteRequests>;
} // namespace SQLite

//quick and easy defines
#define SQLITE_BUS(ret, var, func, ...) SQLite::SQLiteRequestBus::EventResult(##ret##,##var##, &SQLite::SQLiteRequestBus::Events::##func##,__VA_ARGS__)
#define SQLITE3_BUS(ret, var, class_, func, ...) SQLite3::##class_##::EventResult(##ret##,##var##, &SQLite3::##class_##::Events::##func##,__VA_ARGS__)
#define SQLITEDB_BUS(ret, var, func, ...) SQLITE3_BUS(ret,var, SQLiteDBBus, func, __VA_ARGS__)
#define SQLITESTMT_BUS(ret, var, func, ...) SQLITE3_BUS(ret,var, SQLiteStmtBus, func, __VA_ARGS__)
#define SQLITEVFS_BUS(ret, var, func, ...) SQLITE3_BUS(ret,var, SQLiteVFSBus, func, __VA_ARGS__)
#define SQLITEVALUE_BUS(ret, var, func, ...) SQLITE3_BUS(ret,var, SQLiteValueBus, func, __VA_ARGS__)
#define SQLITEMUTEX_BUS(ret, var, func, ...) SQLITE3_BUS(ret,var, SQLiteMutexBus, func, __VA_ARGS__)
#define SQLITEBACKUP_BUS(ret, var, func, ...) SQLITE3_BUS(ret,var, SQLiteBackupBus, func, __VA_ARGS__)