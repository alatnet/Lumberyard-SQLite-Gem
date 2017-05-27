#pragma once

#include "SQLiteDB.h"
#include "SQLiteComponent.h"

namespace SQLite3 {
	#define SQLITE_METHOD_OVERRIDE(name) ->Attribute(AZ::Script::Attributes::MethodOverride, &Internal::##name##)

	class SQLiteComponent;

	////////////////////////////////////////////////////////////////////////
	//Method Overrides
	namespace Internal {
		void SQLitePrepareScript(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc);
		void SQLitePrepare_v2Script(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc);
		/*void SQLitePrepare16Script(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc);
		void SQLitePrepare16_v2Script(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc);*/
		void DB_StatusScript(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc);
		void Load_ExtensionScript(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc);
		void Wal_Checkpoint_v2Script(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc);

		void SQLiteURI_Int64Script(SQLiteComponent* thisPtr, AZ::ScriptDataContext& dc);
		void SQLiteStatusScript(SQLiteComponent* thisPtr, AZ::ScriptDataContext& dc);
		int SQLiteStatus64(int op, __int64 *pCurrent, __int64 *pHighwater, int resetFlag);
		void SQLiteStatus64Script(SQLiteComponent* thisPtr, AZ::ScriptDataContext& dc);

		__int64 SQLiteMemoryUsed();
		__int64 SQLiteMemoryHighWater(int resetFlag);
		__int64 SQLiteSoftHeapLimit64(__int64 N);
	}
	////////////////////////////////////////////////////////////////////////
}