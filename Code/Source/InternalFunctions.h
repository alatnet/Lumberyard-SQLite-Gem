#pragma once

#include <AzCore/Script/ScriptContext.h>

namespace SQLite {
	#define SQLITE_METHOD_OVERRIDE(name) ->Attribute(AZ::Script::Attributes::MethodOverride, &Internal::##name##)

	class SQLiteStmt;
	class SQLiteDB;

	////////////////////////////////////////////////////////////////////////
	//Method Overrides
	namespace Internal {
		void SQLitePrepareScript(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc);
		void SQLitePrepare_v2Script(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc);
	#ifdef SQLITE_PREPARE_PERSISTENT
		void SQLitePrepare_v3Script(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc);
	#endif
		/*void SQLitePrepare16Script(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc);
		void SQLitePrepare16_v2Script(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc);*/
		void DB_StatusScript(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc);
		void Load_ExtensionScript(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc);
		void Wal_Checkpoint_v2Script(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc);

		void SQLiteURI_Int64Script(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc);
		void SQLiteStatusScript(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc);
		void SQLiteStatus64Script(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc);

		__int64 SQLiteMemoryUsed();
		__int64 SQLiteMemoryHighWater(int resetFlag);
		__int64 SQLiteSoftHeapLimit64(__int64 N);

		void SQLiteStmtTextScript(SQLiteStmt* thisPtr, AZ::ScriptDataContext& dc);
		void SQLiteStmtText64Script(SQLiteStmt* thisPtr, AZ::ScriptDataContext& dc);
	}
	////////////////////////////////////////////////////////////////////////
}