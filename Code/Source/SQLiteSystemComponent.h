
#pragma once

#include <AzCore/Component/Component.h>

#include <SQLite/SQLiteBus.h>

#include <AzCore/Script/ScriptContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>

#include <AzCore\RTTI\TypeInfo.h>

namespace SQLite
{
    class SQLiteSystemComponent
        : public AZ::Component
        , protected SQLiteRequestBus::Handler
		//, public AZ::BehaviorEBusHandler
    {
    public:
        AZ_COMPONENT(SQLiteSystemComponent, "{0758FD42-2B80-44E9-A0F9-0DDECC7B99B7}");

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);
    public:
        ////////////////////////////////////////////////////////////////////////
        // SQLiteRequestBus interface implementation
		int Open(const char *path);
		int Open16(const char * path);
		int Open_v2(const char * path, int flags, const char *zVfs);

		int Exec(const char *sql, int(*callback)(void*, int, char**, char**), void * cbarg, char **errmsg) { return sqlite3_exec(this->m_pDB, sql, callback, cbarg, errmsg); }

		int Prepare(const char * sql, int nByte, sqlite3_stmt **ppStmt, const char **pzTail) { return sqlite3_prepare(this->m_pDB, sql, nByte, ppStmt, pzTail); }
		int Prepare_v2(const char * sql, int nByte, sqlite3_stmt **ppStmt, const char **pzTail) { return sqlite3_prepare_v2(this->m_pDB, sql, nByte, ppStmt, pzTail); }
		int Prepare16(const char * sql, int nByte, sqlite3_stmt **ppStmt, const void **pzTail) { return sqlite3_prepare16(this->m_pDB, sql, nByte, ppStmt, pzTail); }
		int Prepare16_v2(const char * sql, int nByte, sqlite3_stmt **ppStmt, const void **pzTail) { return sqlite3_prepare16_v2(this->m_pDB, sql, nByte, ppStmt, pzTail); }

		sqlite3 * GetConnection() { return this->m_pDB; }

		int Step(sqlite3_stmt* pStmt) { return sqlite3_step(pStmt); }
		int Finalize(sqlite3_stmt *pStmt) { return sqlite3_finalize(pStmt); }
		int Reset(sqlite3_stmt *pStmt) { return sqlite3_reset(pStmt); }

		int ErrCode() { return sqlite3_errcode(this->m_pDB); }
		int ExtErrCode() { return sqlite3_extended_errcode(this->m_pDB); }
		const char * ErrMsg() { return sqlite3_errmsg(this->m_pDB); }
		const void * ErrMsg16() { return sqlite3_errmsg16(this->m_pDB); }
		const char * ErrStr(int err) { return sqlite3_errstr(err); }
        ////////////////////////////////////////////////////////////////////////
	protected:
		////////////////////////////////////////////////////////////////////////
		// Lua Interface Implementation
		int ExecLua(const char *sql, void * cbarg);
		int ExecToLua(AZ::EntityId id, const char *sql, void * cbarg);

		PrepareReturn PrepareLua(const char* sql, int nByte);
		PrepareReturn Prepare_v2Lua(const char* sql, int nByte);
		PrepareReturn Prepare16Lua(const char* sql, int nByte);
		PrepareReturn Prepare16_v2Lua(const char* sql, int nByte);

		int StepLua(PrepareReturn r) { return sqlite3_step(r.ppStmt); }
		int FinalizeLua(PrepareReturn r) { return sqlite3_finalize(r.ppStmt); }
		int ResetLua(PrepareReturn r) { return sqlite3_reset(r.ppStmt); }

		const char * PrepareErrStrLua(PrepareReturn r) { return sqlite3_errstr(r.ret); }
		int PrepareErrIntLua(PrepareReturn r) { return r.ret; }

		const char * GetPrepareTailLua(PrepareReturn pr) { return pr.pzTail.c; }
		const void * GetPrepare16TailLua(PrepareReturn pr) { return pr.pzTail.v; }
		////////////////////////////////////////////////////////////////////////
	protected:
        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////
	private:
		const char * m_dbPath;
		sqlite3 *m_pDB;
	private:
		enum OpenType {
			CLOSED,
			OPEN,
			OPEN16,
			OPENV2
		};
		OpenType m_OpenType;
	public:
		class LuaHandler : public SQLiteLuaBus::Handler, public AZ::BehaviorEBusHandler {
		public:
			AZ_EBUS_BEHAVIOR_BINDER(
				LuaHandler, //Name
				"{72DC2E33-0A1E-48EC-81F2-DD8D8CCAE680}", //TypeId
				AZ::SystemAllocator, //default allocator.
									 // List of event names to handle and support for BehaviorContext.
				SqlCallback
			);
		public:
			//int SqlCallback(void * cbargs, int argc, char **argv, char **azColName);
			//int SqlCallback(void * cbargs, /*AZStd::vector<AZStd::pair<char *, char *>> row,*/ AZStd::unordered_map<char *, char *> rowMap);
			int SqlCallback(void * cbargs, int argc, AZStd::vector<AZStd::string> argv, AZStd::vector<AZStd::string> azColName);
		};
    };

	/*namespace SQLite3 {
		class SQLite {
		public:
			AZ_TYPE_INFO(SQLite, "{A669689D-4938-473C-BE0A-C0B111BB6CF3}")
			sqlite3 * ptr;
		};

		void SQLite3Constructor(SQLite * thisPtr, AZ::ScriptDataContext& dc);
	}*/
}
