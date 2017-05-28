
#pragma once

#include <AzCore/Component/Component.h>

#include <SQLite/SQLiteBus.h>

#include <AzCore/Script/ScriptContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>

#include <AzCore\RTTI\TypeInfo.h>

#include "SQLiteDB.h"

namespace SQLite
{
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

		sqlite3 * GetConnection() { return this->m_pDB; }

		int ErrCode() { return sqlite3_errcode(this->m_pDB); }
		int ExtErrCode() { return sqlite3_extended_errcode(this->m_pDB); }
		const char * ErrMsg() { return sqlite3_errmsg(this->m_pDB); }
		const void * ErrMsg16() { return sqlite3_errmsg16(this->m_pDB); }
        ////////////////////////////////////////////////////////////////////////
	protected:
		////////////////////////////////////////////////////////////////////////
		// Lua Interface Implementation
		int ExecLua(const char *sql, void * cbarg);
		int ExecToLua(AZ::EntityId id, const char *sql, void * cbarg);

		SQLite3::SQLiteDB * GetConnectionLua() { return new SQLite3::SQLiteDB(this->m_pDB); }
		////////////////////////////////////////////////////////////////////////
	protected:
        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////
	protected:
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
	protected:
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
}
