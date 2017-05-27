
#pragma once

#include <AzCore/EBus/EBus.h>
#include <SQLite\sqlite3.h>

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
	public:
        // Public functions
		virtual int Open(const char * path) = 0;
		virtual int Open16(const char * path) = 0;
		virtual int Open_v2(const char * path, int flags, const char *zVfs) = 0;

		virtual int Exec(const char *sql, int(*callback)(void*, int, char**, char**), void * cbarg, char **errmsg) = 0;

		virtual int Prepare(const char * sql, int nByte, sqlite3_stmt **ppStmt, const char **pzTail) = 0;
		virtual int Prepare_v2(const char * sql, int nByte, sqlite3_stmt **ppStmt, const char **pzTail) = 0;
		virtual int Prepare16(const char * sql, int nByte, sqlite3_stmt **ppStmt, const void **pzTail) = 0;
		virtual int Prepare16_v2(const char * sql, int nByte, sqlite3_stmt **ppStmt, const void **pzTail) = 0;

		virtual sqlite3 * GetConnection() = 0;

		virtual int Step(sqlite3_stmt* pStmt) = 0;
		virtual int Finalize(sqlite3_stmt *pStmt) = 0;
		virtual int Reset(sqlite3_stmt *pStmt) = 0;

		virtual int ErrCode() = 0;
		virtual int ExtErrCode() = 0;
		virtual const char * ErrMsg() = 0;
		virtual const void * ErrMsg16() = 0;
		virtual const char * ErrStr(int err) = 0;
	protected:
		/*struct PrepareReturn {
			AZ_TYPE_INFO(PrepareReturn, "{DCA04754-B847-4117-B736-68E1CD242A3C}");
			int ret;
			sqlite3_stmt *ppStmt;
			const char *pzTail;
		};

		struct Prepare16Return {
			AZ_TYPE_INFO(Prepare16Return, "{0143434C-14F7-44EC-99DC-F1159C0DF991}");
			int ret;
			sqlite3_stmt *ppStmt;
			const void *pzTail;
		};*/

		struct PrepareReturn {
			AZ_TYPE_INFO(PrepareReturn, "{0143434C-14F7-44EC-99DC-F1159C0DF991}");
			int ret;
			sqlite3_stmt *ppStmt;
			union {
				const char *c;
				const void *v;
			} pzTail;
		};

		// Protected Lua Functions
		virtual int ExecLua(const char *sql, void * cbarg) = 0;
		virtual int ExecToLua(AZ::EntityId id, const char *sql, void * cbarg) = 0;

		virtual PrepareReturn PrepareLua(const char* sql, int nByte) = 0;
		virtual PrepareReturn Prepare_v2Lua(const char* sql, int nByte) = 0;
		virtual PrepareReturn Prepare16Lua(const char* sql, int nByte) = 0;
		virtual PrepareReturn Prepare16_v2Lua(const char* sql, int nByte) = 0;

		virtual int StepLua(PrepareReturn pr) = 0;
		virtual int FinalizeLua(PrepareReturn pr) = 0;
		virtual int ResetLua(PrepareReturn pr) = 0;
		virtual const char * PrepareErrStrLua(PrepareReturn pr) = 0;
		virtual int PrepareErrIntLua(PrepareReturn r) = 0;

		virtual const char * GetPrepareTailLua(PrepareReturn pr) = 0;
		virtual const void * GetPrepare16TailLua(PrepareReturn pr) = 0;
	};
    using SQLiteRequestBus = AZ::EBus<SQLiteRequests>;

	class SQLiteLuaRequests : public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;

		using BusIdType = AZ::EntityId;
	public:
		//virtual int SqlCallback(void * cbargs, int argc, char **argv, char **azColName) = 0;
		virtual int SqlCallback(void * cbargs, int argc, AZStd::vector<AZStd::string> argv, AZStd::vector<AZStd::string> azColName) = 0;
	};
	using SQLiteLuaBus = AZ::EBus<SQLiteLuaRequests>;
} // namespace SQLite
