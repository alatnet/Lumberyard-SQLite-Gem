
#include "StdAfx.h"
#include "SQLiteSystemComponent.h"

namespace SQLite {
	////////////////////////////////////////////////////////////////////////
	// Component Implementation
	SQLiteSystemComponent::SQLiteSystemComponent() {
		this->m_pDB = nullptr;
		this->m_dbPath = ":memory:";
		this->m_OpenType = OPEN;
		this->m_openv2_flags = 0;
		this->m_openv2_zvfs = "";
	}

	void SQLiteSystemComponent::Reflect(AZ::ReflectContext* context) {
		if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context)) {
			serialize->Class<SQLiteSystemComponent, AZ::Component>()
				->Version(1)
				->Field("dbPath", &SQLiteSystemComponent::m_dbPath)
				->Field("OpenType", &SQLiteSystemComponent::m_OpenType)
				->Field("Openv2_flags", &SQLiteSystemComponent::m_openv2_flags)
				->Field("Openv2_zvfs", &SQLiteSystemComponent::m_openv2_zvfs);

			if (AZ::EditContext* ec = serialize->GetEditContext()) {
				ec->Class<SQLiteSystemComponent>("SQLite", "SQLite database component.")
					->ClassElement(AZ::Edit::ClassElements::EditorData, "")
					->Attribute(AZ::Edit::Attributes::Category, "Database")
					->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game", 0x232b318c))
					->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System", 0xc94d118b))
					->Attribute(AZ::Edit::Attributes::AutoExpand, true)
					->DataElement(0, &SQLiteSystemComponent::m_dbPath, "Database", "Database Path")
					->DataElement(AZ::Edit::UIHandlers::ComboBox, &SQLiteSystemComponent::m_OpenType, "Open Type", "Which open function to use.")
						->EnumAttribute(OpenType::OPEN, "Open")
						->EnumAttribute(OpenType::OPEN16, "Open16")
						->EnumAttribute(OpenType::OPENV2, "Openv2")
						->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::EntireTree)
					->DataElement(0, &SQLiteSystemComponent::m_openv2_flags, "Open V2 Flags", "Flags to use for Open V2.")
						->Attribute(AZ::Edit::Attributes::Visibility, &SQLiteSystemComponent::IsOpenTypeV2)
					->DataElement(0, &SQLiteSystemComponent::m_openv2_zvfs, "Open V2 VFS", "Virtual filesystem to use for Open V2.")
						->Attribute(AZ::Edit::Attributes::Visibility, &SQLiteSystemComponent::IsOpenTypeV2);
			}
		}

		AZ::BehaviorContext* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context);
		if (behaviorContext) {
			#define SQLITE_EVENT(name) ->Event(#name, &SQLiteRequestBus::Events::##name##)
			#define SQLITE_EVENTLUA(name) ->Event(#name, &SQLiteRequestBus::Events::##name##Lua)
			behaviorContext->EBus<SQLiteRequestBus>("SQLiteBus")
				->Attribute(AZ::Script::Attributes::Category, "SQLite")
				->Handler<LuaHandler>()
				SQLITE_EVENTLUA(Exec)
				SQLITE_EVENTLUA(ExecTo)
				SQLITE_EVENT(GetConnection)
				//SQLITE_EVENTLUA(GetSysConnection)
				;
			behaviorContext->Class<SQLiteSystemComponent>("SQLite3")
				->Attribute(AZ::Script::Attributes::Category, "SQLite")
				->Method("GetSysConnection", &SQLiteSystemComponent::GetSysConnectionLua, nullptr, "")
				->Constant("TEXT", []() -> int { return SQLITE3_TEXT; });
			#undef SQLITE_EVENTLUA
			#undef SQLITE_EVENT

			SQLite::SQLiteDB::RegisterBehaviorContext(behaviorContext);
			SQLite::SQLiteStmt::RegisterBehaviorContext(behaviorContext);
			SQLite::SQLiteBackup::RegisterBehaviorContext(behaviorContext);
			SQLite::SQLiteValue::RegisterBehaviorContext(behaviorContext);
			SQLite::SQLiteVFS::RegisterBehaviorContext(behaviorContext);
			SQLite::SQLiteMutex::RegisterBehaviorContext(behaviorContext);
		}
	}

	void SQLiteSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided) {
		provided.push_back(AZ_CRC("SQLiteService", 0x892cc9bc));
	}

	void SQLiteSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible) {
		incompatible.push_back(AZ_CRC("SQLiteService", 0x892cc9bc));
	}

	void SQLiteSystemComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required) {
		(void)required;
	}

	void SQLiteSystemComponent::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent) {
		(void)dependent;
	}

	void SQLiteSystemComponent::Init() {
		this->m_pDB = new SQLite::SQLiteDB();
		this->m_pDB->m_entityid = this->GetEntityId();
	}

	void SQLiteSystemComponent::Activate() {
		AZ_Printf("SQLite", "[SQLite] %s - Opening Database: %s\n", this->GetEntityId().ToString().c_str(), this->m_dbPath.c_str());

		switch (this->m_OpenType) {
		case OPEN:
			this->m_pDB->Open(this->m_dbPath.c_str());
			break;
		case OPEN16:
			this->m_pDB->Open16(this->m_dbPath.c_str());
			break;
		case OPENV2:
			this->m_pDB->Open_v2(this->m_dbPath.c_str(), m_openv2_flags, m_openv2_zvfs.c_str());
			break;
		}
		SQLiteRequestBus::Handler::BusConnect(this->GetEntityId());
	}

	void SQLiteSystemComponent::Deactivate() {
		SQLiteRequestBus::Handler::BusDisconnect();
		AZ_Printf("SQLite", "[SQLite] %s - Closing Database.\n", this->GetEntityId().ToString().c_str());
		this->m_pDB->m_entityid.SetInvalid();
		this->m_pDB->Close();
		delete this->m_pDB;
	}
	////////////////////////////////////////////////////////////////////////

	SQLite::SQLiteDB * SQLiteSystemComponent::GetConnection() {
		return this->m_pDB;
		//if (this->GetEntityId() == this->m_pDB->m_entityid) return this->m_pDB;
		//if (this->GetEntityId() == AZ::EntityId(0)) return this->m_pDB;
		//if (this->m_pDB) return new SQLite::SQLiteDB(this->m_pDB, this->GetEntityId()); //creates an alias to an entity's db?
		//return nullptr;
	}

	////////////////////////////////////////////////////////////////////////
	// Lua Implementation
	int SQLiteSystemComponent::ExecLua(const char *sql, void * cbarg) {
		typedef struct {
			void* cbarg;
			AZ::EntityId id;
		} ARGS;

		ARGS args;
		args.cbarg = cbarg;
		args.id = this->GetEntityId();

		auto callback = [](void* cbarg, int argc, char **argv, char **azColName) -> int {
			int ret;
			ARGS* args = (ARGS*)cbarg;

			//////////////////////////
			for (int i = 0; i < argc; i++) {
				AZ_Printf("SQLite", "%s = %s", azColName[i], argv[i] ? argv[i] : "NULL");
			}

			AZStd::vector<AZStd::string> argvVec(argv, argc + argv);
			AZStd::vector<AZStd::string> azColNameVec(azColName, azColName+argc);
			//////////////////////////

			//SQLite::SQLiteLuaBus::EventResult(ret, args->id, &SQLite::SQLiteLuaBus::Events::SqlCallback, args->cbarg, argc, argv, azColName);
			SQLite::SQLiteLuaBus::EventResult(ret, args->id, &SQLite::SQLiteLuaBus::Events::SqlCallback, args->cbarg, argc, argvVec, azColNameVec);
			
			return ret;
		};

		//char * errmsg = 0;

		return this->m_pDB->Exec(
			sql,
			callback,
			&args,
			nullptr
		);
	}

	int SQLiteSystemComponent::ExecToLua(AZ::EntityId id, const char *sql, void * cbarg) {
		typedef struct {
			void* cbarg;
			AZ::EntityId id;
		} ARGS;

		ARGS args;
		args.cbarg = cbarg;
		args.id = id;

		auto callback = [](void* cbarg, int argc, char **argv, char **azColName) -> int {
			int ret;
			ARGS* args = (ARGS*)cbarg;

			//////////////////////////
			for (int i = 0; i < argc; i++) {
				AZ_Printf("SQLiteLY", "%s = %s", azColName[i], argv[i] ? argv[i] : "NULL");
			}

			AZStd::vector<AZStd::string> argvVec(argv, argc + argv);
			AZStd::vector<AZStd::string> azColNameVec(azColName, azColName + argc);
			//////////////////////////

			//SQLite::SQLiteLuaBus::EventResult(ret, args->id, &SQLite::SQLiteLuaBus::Events::SqlCallback, args->cbarg, argc, argv, azColName);
			SQLite::SQLiteLuaBus::EventResult(ret, args->id, &SQLite::SQLiteLuaBus::Events::SqlCallback, args->cbarg, argc, argvVec, azColNameVec);

			return ret;
		};

		//char * errmsg = 0;

		return this->m_pDB->Exec(
			sql,
			callback,
			&args,
			nullptr
		);
	}

	int SQLiteSystemComponent::LuaHandler::SqlCallback(void * cbargs, int argc, AZStd::vector<AZStd::string> argv, AZStd::vector<AZStd::string> azColName){
	//int SQLiteSystemComponent::LuaHandler::SqlCallback(void * cbargs, int argc, char **argv, char **azColName) {
		int ret;
		CallResult(ret, FN_SqlCallback, cbargs, argc, argv, azColName);
		return ret;
	}

	/*namespace SQLite {
		void SQLite3Constructor(SQLite * thisPtr, AZ::ScriptDataContext& dc) {
			if (dc.GetNumArguments() == 1) {
				const char * path;
				dc.ReadArg(0, path);
				sqlite3_open(path, &thisPtr->ptr);
			}
		}
	}*/
	////////////////////////////////////////////////////////////////////////

	SQLite::SQLiteDB * SQLiteSystemComponent::GetSysConnectionLua() {
		SQLite::SQLiteDB * ret = nullptr;
		SQLITE_BUS(ret, AZ::EntityId(0), GetConnection);
		return ret;
	}
}
