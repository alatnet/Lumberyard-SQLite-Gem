#include "StdAfx.h"
#include "InternalFunctions.h"

#include "SQLite\SQLiteDB.h"

namespace SQLite3 {
	////////////////////////////////////////////////////////////////////////
	//Method Overrides
	namespace Internal {
		////////////////////////////////////////////////////////////////////////
		//Prepare Overrides
		///Todo, error checking
		void SQLitePrepareScript(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc) {
			SQLiteStmt * stmt = new SQLiteStmt();

			const char * sql;
			int nByte;
			const char *pzTail = nullptr;

			int numArgs = dc.GetNumArguments();

			switch (numArgs) {
			case 1:
				dc.ReadArg(0, sql);
				stmt->m_err = sqlite3_prepare(thisPtr->m_pDB, sql, -1, &stmt->m_pStmt, &pzTail);
				break;
			case 2:

				dc.ReadArg(0, sql);
				dc.ReadArg(1, nByte);
				stmt->m_err = sqlite3_prepare(thisPtr->m_pDB, sql, nByte, &stmt->m_pStmt, &pzTail);
				break;
			default:
				//error
				break;
			}

			if (numArgs == 1 || numArgs == 2) {
				dc.PushResult(stmt->m_err);
				dc.PushResult(stmt);
				if (pzTail) dc.PushResult(pzTail);
				else dc.PushResult(NULL);
			} else {
				delete stmt;
			}
		}
		void SQLitePrepare_v2Script(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc) {
			SQLiteStmt * stmt = new SQLiteStmt();

			const char * sql;
			int nByte;
			const char *pzTail = nullptr;

			int numArgs = dc.GetNumArguments();

			switch (numArgs) {
			case 1:
				dc.ReadArg(0, sql);
				stmt->m_err = sqlite3_prepare_v2(thisPtr->m_pDB, sql, -1, &stmt->m_pStmt, &pzTail);
				break;
			case 2:

				dc.ReadArg(0, sql);
				dc.ReadArg(1, nByte);
				stmt->m_err = sqlite3_prepare_v2(thisPtr->m_pDB, sql, nByte, &stmt->m_pStmt, &pzTail);
				break;
			default:
				//error
				break;
			}

			if (numArgs == 1 || numArgs == 2) {
				dc.PushResult(stmt->m_err);
				dc.PushResult(stmt);
				if (pzTail) dc.PushResult(pzTail);
				else dc.PushResult(NULL);
			} else {
				delete stmt;
			}
		}
		/*void SQLitePrepare16Script(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc) {
		SQLiteStmt * stmt = new SQLiteStmt();

		const void * sql;
		int nByte;
		const void *pzTail;

		int numArgs = dc.GetNumArguments();

		switch (numArgs) {
		case 1:
		dc.ReadArg(0, sql);
		stmt->m_err = sqlite3_prepare16(thisPtr->m_pDB, sql, -1, &stmt->m_pStmt, &pzTail);
		break;
		case 2:

		dc.ReadArg(0, sql);
		dc.ReadArg(1, nByte);
		stmt->m_err = sqlite3_prepare16(thisPtr->m_pDB, sql, nByte, &stmt->m_pStmt, &pzTail);
		break;
		default:
		//error
		break;
		}

		if (numArgs == 1 || numArgs == 2) {
		dc.PushResult(stmt->m_err);
		dc.PushResult(stmt);
		//dc.PushResult(pzTail);
		} else {
		delete stmt;
		}
		}
		void SQLitePrepare16_v2Script(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc) {
		SQLiteStmt * stmt = new SQLiteStmt();

		const void * sql;
		int nByte;
		const void *pzTail;

		int numArgs = dc.GetNumArguments();

		switch (numArgs) {
		case 1:
		dc.ReadArg(0, sql);
		stmt->m_err = sqlite3_prepare16(thisPtr->m_pDB, sql, -1, &stmt->m_pStmt, &pzTail);
		break;
		case 2:

		dc.ReadArg(0, sql);
		dc.ReadArg(1, nByte);
		stmt->m_err = sqlite3_prepare16_v2(thisPtr->m_pDB, sql, nByte, &stmt->m_pStmt, &pzTail);
		break;
		default:
		//error
		break;
		}

		if (numArgs == 1 || numArgs == 2) {
		dc.PushResult(stmt->m_err);
		dc.PushResult(stmt);
		//dc.PushResult(pzTail);
		} else {
		delete stmt;
		}
		}*/
		////////////////////////////////////////////////////////////////////////

		void DB_StatusScript(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc){
			if (dc.GetNumArguments() == 2) {
				int op, resetFlg;
				int pCur, pHiwtr;

				dc.ReadArg(0, op);
				dc.ReadArg(1, resetFlg);

				int ret = sqlite3_db_status(thisPtr->m_pDB, op, &pCur, &pHiwtr, resetFlg);

				dc.PushResult(ret);
				dc.PushResult(pCur);
				dc.PushResult(pHiwtr);
			}
		}
		
		void Load_ExtensionScript(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc){
			if (dc.GetNumArguments() == 2) {
				const char * zFile;
				const char * zProc;
				char * pzErrMsg;

				dc.ReadArg(0, zFile);
				dc.ReadArg(1, zProc);

				int ret = SQLITE_ERROR;

				if (gEnv) {
					char * resolvedFile = new char[AZ_MAX_PATH_LEN];
					gEnv->pFileIO->ResolvePath(zFile, resolvedFile, AZ_MAX_PATH_LEN);
					ret = sqlite3_load_extension(thisPtr->m_pDB, resolvedFile, zProc, &pzErrMsg);
					delete resolvedFile;
				} else
					ret = sqlite3_load_extension(thisPtr->m_pDB, zFile, zProc, &pzErrMsg);

				dc.PushResult(ret);
				dc.PushResult(pzErrMsg);
			}
		}
		
		void Wal_Checkpoint_v2Script(SQLiteDB* thisPtr, AZ::ScriptDataContext& dc) {
			if (dc.GetNumArguments() == 2) {
				const char * zDb;
				int eMode;

				int pnLog;
				int pnCkpt;

				dc.ReadArg(0, zDb);
				dc.ReadArg(1, eMode);

				int ret = sqlite3_wal_checkpoint_v2(thisPtr->m_pDB, zDb, eMode, &pnLog, &pnCkpt);

				dc.PushResult(ret);
				dc.PushResult(pnLog);
				dc.PushResult(pnCkpt);
			}
		}

		void SQLiteURI_Int64Script(SQLiteComponent* thisPtr, AZ::ScriptDataContext& dc) {
			if (dc.GetNumArguments() == 3) {
				const char * filename;
				const char * param;
				__int64 bDflt;

				dc.ReadArg(0, filename);
				dc.ReadArg(1, param);
				dc.ReadArg(2, bDflt);

				sqlite3_int64 ret = sqlite3_uri_int64(filename, param, (sqlite3_int64)bDflt);

				dc.PushResult((__int64)ret);
			}
		}

		void SQLiteStatusScript(SQLiteComponent* thisPtr, AZ::ScriptDataContext& dc) {
			if (dc.GetNumArguments() == 2) {
				int op, resetFlag;
				dc.ReadArg(0, op);
				dc.ReadArg(1, resetFlag);

				int pCurrent, pHighwater;
				int ret = sqlite3_status(op, &pCurrent, &pHighwater, resetFlag);

				dc.PushResult(ret);
				dc.PushResult(pCurrent);
				dc.PushResult(pHighwater);
			}
		}
		
		void SQLiteStatus64Script(SQLiteComponent* thisPtr, AZ::ScriptDataContext& dc) {
			if (dc.GetNumArguments() == 2) {
				int op, resetFlag;
				dc.ReadArg(0, op);
				dc.ReadArg(1, resetFlag);

				__int64 pCurrent, pHighwater;
				int ret = sqlite3_status64(op, &((sqlite3_int64)pCurrent), &((sqlite3_int64)pHighwater), resetFlag);

				dc.PushResult(ret);
				dc.PushResult(pCurrent);
				dc.PushResult(pHighwater);
			}
		}

		__int64 SQLiteMemoryUsed() { return (__int64)sqlite3_memory_used(); }
		__int64 SQLiteMemoryHighWater(int resetFlag) { return (__int64)sqlite3_memory_highwater(resetFlag); }
		__int64 SQLiteSoftHeapLimit64(__int64 N) { return (__int64)sqlite3_soft_heap_limit64((sqlite3_int64)N); }


		void SQLiteStmtTextScript(SQLiteStmt* thisPtr, AZ::ScriptDataContext& dc) {
			if (dc.GetNumArguments() == 3) {
				int index, length;
				char* value;
				dc.ReadArg(0, index);
				dc.ReadArg(1, value);
				dc.ReadArg(2, length);

				int ret = sqlite3_bind_text(thisPtr->m_pStmt,index, value, length, SQLITE_STATIC);

				dc.PushResult(ret);
			}
		}

		void SQLiteStmtText64Script(SQLiteStmt* thisPtr, AZ::ScriptDataContext& dc) {
			if (dc.GetNumArguments() == 4) {
				int index;
				unsigned __int64 length;
				char* value;
				unsigned char encoding;
				dc.ReadArg(0, index);
				dc.ReadArg(1, value);
				dc.ReadArg(2, length);
				dc.ReadArg(3, encoding);

				int ret = sqlite3_bind_text64(thisPtr->m_pStmt, index, value, length, SQLITE_STATIC, encoding);

				dc.PushResult(ret);
			}
		}
	}
	////////////////////////////////////////////////////////////////////////
}