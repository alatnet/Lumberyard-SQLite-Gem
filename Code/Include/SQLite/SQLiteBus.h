
#pragma once

#include <AzCore/EBus/EBus.h>
#include <SQLite\sqlite3.h>

namespace SQLite3 {
	class SQLiteDB;
}

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
		virtual int Open(const char * path) = 0;
		virtual int Open16(const char * path) = 0;
		virtual int Open_v2(const char * path, int flags, const char *zVfs) = 0;

		virtual int Exec(const char *sql, int(*callback)(void*, int, char**, char**), void * cbarg, char **errmsg) = 0;

		virtual sqlite3 * GetConnection() = 0;

		virtual int ErrCode() = 0;
		virtual int ExtErrCode() = 0;
		virtual const char * ErrMsg() = 0;
		virtual const void * ErrMsg16() = 0;
	protected: //Lua specific functions. Not exposed for usage in regular C++.
		virtual int ExecLua(const char *sql, void * cbarg) = 0;
		virtual int ExecToLua(AZ::EntityId id, const char *sql, void * cbarg) = 0;
		virtual SQLite3::SQLiteDB * GetConnectionLua() = 0;
	};
    using SQLiteRequestBus = AZ::EBus<SQLiteRequests>;
} // namespace SQLite
