
#pragma once

#include <AzCore/Component/Component.h>

#include <SQLite/SQLiteBus.h>

#include <AzCore/Script/ScriptContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>

#include <AzCore\RTTI\TypeInfo.h>

#include "SQLiteClasses.h"
#include "InternalFunctions.h"

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
		SQLiteSystemComponent();
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
		SQLite::SQLiteDB * GetConnection();/* { return new SQLite::SQLiteDB(this->m_pDB, this->GetEntityId()); }*/
		SQLite::SQLiteDB * NewConnection() { return new SQLite::SQLiteDB(); }
		SQLite::SQLiteBackup * NewBackup(SQLite::SQLiteDB * dest, const char * dname, SQLite::SQLiteDB * src, const char *sname) { return new SQLite::SQLiteBackup(dest, dname, src, sname); }
		SQLite::SQLiteMutex * NewMutex(int N) { return new SQLite::SQLiteMutex(N); }
		SQLite::SQLiteVFS * NewVFS(const char * vfsName) { return new SQLite::SQLiteVFS(vfsName); }
		SQLite::SQLiteVFS * NewVFS(sqlite3_vfs * vfs) { return new SQLite::SQLiteVFS(vfs); }
        ////////////////////////////////////////////////////////////////////////
	protected:
		////////////////////////////////////////////////////////////////////////
		// Lua Interface Implementation
		int ExecLua(const char *sql, void * cbarg);
		int ExecToLua(AZ::EntityId id, const char *sql, void * cbarg);
		////////////////////////////////////////////////////////////////////////

		static SQLite::SQLiteDB * GetSysConnectionLua();
	protected:
        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////
	protected:
		bool IsOpenTypeV2() { return this->m_OpenType == SQLiteSystemComponent::OpenType::OPENV2; }
	private:
		AZStd::string m_dbPath;
	protected:
		SQLite::SQLiteDB *m_pDB;
	private:
		enum OpenType {
			OPEN,
			OPEN16,
			OPENV2
		};
		OpenType m_OpenType;
		int m_openv2_flags;
		AZStd::string m_openv2_zvfs;

		/*struct {
			const char * m_dbPath; //database path
			bool m_lockConn; //lock the database connection (disable open/close)
		} m_Settings;*/
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
