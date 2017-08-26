#include "StdAfx.h"
#include "SQLite\SQLiteDB.h"
#include "InternalFunctions.h"

#include "SQLite\SQLiteStmt.h"

#include "SQLite\SQLiteBus.h"

#include <AzCore\IO\SystemFile.h>

namespace SQLite3 {
	void SQLiteDB::RegisterBehaviorContext(AZ::BehaviorContext* bc) {
		////////////////////////////////////////////////////////////////////////
		// SQLite class
		#define SQLITE_ENUM(name) ->Enum<SQLITE_##name##>(#name)
		#define SQLITE_METHOD(name, func, defaultval,dbgDesc) ->Method(#name,&sqlite3_##func##,##defaultval##,##dbgDesc##)
		#define SQLITEDB_METHOD(name, defaultval, dbgDesc) ->Method(#name,&SQLiteDB::##name##,##defaultval##,##dbgDesc##)
		//#define SQLITEDB_METHOD_OVERRIDE(name) ->Attribute(AZ::Script::Attributes::MethodOverride, &Internal::##name##)
		#define SQLITE_CONSTANT(name) ->Constant(#name, []() -> int { return SQLITE_##name##; })
		#define SQLITE_CONSTANT_TYPE(name, type) ->Constant(#name, []() -> type { return SQLITE_##name##; })
		bc->Class<SQLiteDB>("SQLite")
			->Constructor<>()

			//Methods
			SQLITEDB_METHOD(Open, nullptr, "")
			SQLITEDB_METHOD(Open16, nullptr, "")
			SQLITEDB_METHOD(Open_v2, nullptr, "")
			SQLITEDB_METHOD(Close, nullptr, "")
			//SQLITEDB_METHOD(Close_v2, nullptr, "")
			SQLITEDB_METHOD(Prepare, nullptr, "")
				SQLITE_METHOD_OVERRIDE(SQLitePrepareScript)
			SQLITEDB_METHOD(Prepare_v2, nullptr, "")
				SQLITE_METHOD_OVERRIDE(SQLitePrepare_v2Script)
			/*SQLITEDB_METHOD(Prepare16, nullptr, "")
			SQLITEDB_METHOD_OVERRIDE(SQLitePrepare16Script)
			SQLITEDB_METHOD(Prepare16_v2, nullptr, "")
			SQLITEDB_METHOD_OVERRIDE(SQLitePrepare16_v2Script)*/
			SQLITEDB_METHOD(ErrCode, nullptr, "")
			SQLITEDB_METHOD(ExtErrCode, nullptr, "")
			SQLITEDB_METHOD(ErrMsg, nullptr, "")
			SQLITEDB_METHOD(ErrMsg16, nullptr, "")
			SQLITEDB_METHOD(Limit, nullptr, "")
			SQLITEDB_METHOD(DB_CacheFlush, nullptr, "")
			SQLITEDB_METHOD(DB_Filename, nullptr, "")
			SQLITEDB_METHOD(DB_ReadOnly, nullptr, "")
			SQLITEDB_METHOD(DB_Release_Memory, nullptr, "")
			SQLITEDB_METHOD(DB_Status, nullptr, "") //override
				SQLITE_METHOD_OVERRIDE(DB_StatusScript)
			SQLITEDB_METHOD(Declare_VTab, nullptr, "")
			SQLITEDB_METHOD(Enable_Load_Extension, nullptr, "")
			SQLITEDB_METHOD(Get_AutoCommit, nullptr, "")
			SQLITEDB_METHOD(Interrupt, nullptr, "")
			SQLITEDB_METHOD(Last_Insert_RowId, nullptr, "")
			SQLITEDB_METHOD(Load_Extension, nullptr, "") //override
				SQLITE_METHOD_OVERRIDE(Load_ExtensionScript)
			SQLITEDB_METHOD(Overload_Function, nullptr, "")
			SQLITEDB_METHOD(Set_Last_Insert_RowId, nullptr, "")
			SQLITEDB_METHOD(System_ErrNo, nullptr, "")
			SQLITEDB_METHOD(Total_Changes, nullptr, "")
			SQLITEDB_METHOD(VTab_On_Conflict, nullptr, "")
			SQLITEDB_METHOD(Wal_AutoCheckpoint, nullptr, "")
			SQLITEDB_METHOD(Wal_Checkpoint, nullptr, "")
			SQLITEDB_METHOD(Wal_Checkpoint_v2, nullptr, "") //override
				SQLITE_METHOD_OVERRIDE(Wal_Checkpoint_v2Script)

			//SQLite3 Raw Methods
			SQLITE_METHOD("ErrStr", errstr, nullptr, "")
			SQLITE_METHOD("LibVersion", libversion, nullptr, "")
			SQLITE_METHOD("LibVersion_Number", libversion_number, nullptr, "")
			SQLITE_METHOD("SourceID", sourceid, nullptr, "")
			SQLITE_METHOD("Enable_Shared_Cache", enable_shared_cache, nullptr, "")
			SQLITE_METHOD("Reset_Auto_Ext", reset_auto_extension, nullptr, "")
			SQLITE_METHOD("ThreadSafe", threadsafe, nullptr, "")
			SQLITE_METHOD("URI_Parameter", uri_parameter, nullptr, "")
			SQLITE_METHOD("URI_Boolean", uri_boolean, nullptr, "")
			SQLITE_METHOD("URI_Int64", uri_int64, nullptr, "")
				SQLITE_METHOD_OVERRIDE(SQLiteURI_Int64Script)
			SQLITE_METHOD("Status", status, nullptr, "")
				SQLITE_METHOD_OVERRIDE(SQLiteStatusScript)
			SQLITE_METHOD("Status64", status64, nullptr, "")
				SQLITE_METHOD_OVERRIDE(SQLiteStatus64Script)
			SQLITE_METHOD("Complete", complete, nullptr, "")
			//SQLITEDB_METHOD(Memory_Used, nullptr, "")
			//SQLITEDB_METHOD(Memory_HighWater, nullptr, "")
			//SQLITEDB_METHOD(Soft_Heap_Limit64, nullptr, "")

			->Method("Memory_Used", &Internal::SQLiteMemoryUsed, nullptr, "")
			->Method("Memory_HighWater", &Internal::SQLiteMemoryHighWater, nullptr, "")
			->Method("Soft_Heap_Limit64", &Internal::SQLiteSoftHeapLimit64, nullptr, "")
			//SQLITE_METHOD("Complete16", complete16, nullptr, "")

			//Constants
			SQLITE_CONSTANT_TYPE(VERSION, const char *)
			SQLITE_CONSTANT(VERSION_NUMBER)
			SQLITE_CONSTANT_TYPE(SOURCE_ID, const char *)
			SQLITE_CONSTANT(INTEGER)
			SQLITE_CONSTANT(FLOAT)
			SQLITE_CONSTANT(BLOB)
			->Constant("NULL", []() -> int { return SQLITE_NULL; })
			SQLITE_CONSTANT(TEXT)
			SQLITE_CONSTANT(LIMIT_LENGTH)
			SQLITE_CONSTANT(LIMIT_SQL_LENGTH)
			SQLITE_CONSTANT(LIMIT_COLUMN)
			SQLITE_CONSTANT(LIMIT_EXPR_DEPTH)
			SQLITE_CONSTANT(LIMIT_COMPOUND_SELECT)
			SQLITE_CONSTANT(LIMIT_VDBE_OP)
			SQLITE_CONSTANT(LIMIT_FUNCTION_ARG)
			SQLITE_CONSTANT(LIMIT_ATTACHED)
			SQLITE_CONSTANT(LIMIT_LIKE_PATTERN_LENGTH)
			SQLITE_CONSTANT(LIMIT_VARIABLE_NUMBER)
			SQLITE_CONSTANT(LIMIT_TRIGGER_DEPTH)
			SQLITE_CONSTANT(LIMIT_WORKER_THREADS)
			/*SQLITE_CONSTANT(MAX_LENGTH)
			SQLITE_CONSTANT(MAX_SQL_LENGTH)
			SQLITE_CONSTANT(MAX_COLUMN)
			SQLITE_CONSTANT(MAX_EXPR_DEPTH)
			SQLITE_CONSTANT(MAX_COMPOUND_SELECT)
			SQLITE_CONSTANT(MAX_VDBE_OP)
			SQLITE_CONSTANT(MAX_FUNCTION_ARG)
			SQLITE_CONSTANT(MAX_ATTACHED)
			SQLITE_CONSTANT(MAX_LIKE_PATTERN_LENGTH)
			SQLITE_CONSTANT(MAX_VARIABLE_NUMBER)
			SQLITE_CONSTANT(MAX_TRIGGER_DEPTH)
			SQLITE_CONSTANT(MAX_WORKER_THREADS)*/
			SQLITE_CONSTANT(ACCESS_EXISTS)
			SQLITE_CONSTANT(ACCESS_READ)
			SQLITE_CONSTANT(ACCESS_READWRITE)
			SQLITE_CONSTANT(ALTER_TABLE)
			SQLITE_CONSTANT(ANALYZE)
			SQLITE_CONSTANT(ANY)
			SQLITE_CONSTANT(ATTACH)
			SQLITE_CONSTANT(CHECKPOINT_FULL)
			SQLITE_CONSTANT(CHECKPOINT_PASSIVE)
			SQLITE_CONSTANT(CHECKPOINT_RESTART)
			SQLITE_CONSTANT(CHECKPOINT_TRUNCATE)
			SQLITE_CONSTANT(CONFIG_COVERING_INDEX_SCAN)
			SQLITE_CONSTANT(CONFIG_GETMALLOC)
			SQLITE_CONSTANT(CONFIG_GETMUTEX)
			SQLITE_CONSTANT(CONFIG_GETPCACHE)
			SQLITE_CONSTANT(CONFIG_GETPCACHE2)
			SQLITE_CONSTANT(CONFIG_HEAP)
			SQLITE_CONSTANT(CONFIG_LOG)
			SQLITE_CONSTANT(CONFIG_LOOKASIDE)
			SQLITE_CONSTANT(CONFIG_MALLOC)
			SQLITE_CONSTANT(CONFIG_MEMSTATUS)
			SQLITE_CONSTANT(CONFIG_MMAP_SIZE)
			SQLITE_CONSTANT(CONFIG_MULTITHREAD)
			SQLITE_CONSTANT(CONFIG_MUTEX)
			SQLITE_CONSTANT(CONFIG_PAGECACHE)
			SQLITE_CONSTANT(CONFIG_PCACHE)
			SQLITE_CONSTANT(CONFIG_PCACHE2)
			SQLITE_CONSTANT(CONFIG_PCACHE_HDRSZ)
			SQLITE_CONSTANT(CONFIG_PMASZ)
			SQLITE_CONSTANT(CONFIG_SCRATCH)
			SQLITE_CONSTANT(CONFIG_SERIALIZED)
			SQLITE_CONSTANT(CONFIG_SINGLETHREAD)
			SQLITE_CONSTANT(CONFIG_SQLLOG)
			SQLITE_CONSTANT(CONFIG_STMTJRNL_SPILL)
			SQLITE_CONSTANT(CONFIG_URI)
			SQLITE_CONSTANT(CONFIG_WIN32_HEAPSIZE)
			SQLITE_CONSTANT(COPY)
			SQLITE_CONSTANT(CREATE_INDEX)
			SQLITE_CONSTANT(CREATE_TABLE)
			SQLITE_CONSTANT(CREATE_TEMP_INDEX)
			SQLITE_CONSTANT(CREATE_TEMP_TABLE)
			SQLITE_CONSTANT(CREATE_TEMP_TRIGGER)
			SQLITE_CONSTANT(CREATE_TEMP_VIEW)
			SQLITE_CONSTANT(CREATE_TRIGGER)
			SQLITE_CONSTANT(CREATE_VIEW)
			SQLITE_CONSTANT(CREATE_VTABLE)
			SQLITE_CONSTANT(DBCONFIG_ENABLE_FKEY)
			SQLITE_CONSTANT(DBCONFIG_ENABLE_FTS3_TOKENIZER)
			SQLITE_CONSTANT(DBCONFIG_ENABLE_LOAD_EXTENSION)
			SQLITE_CONSTANT(DBCONFIG_ENABLE_TRIGGER)
			SQLITE_CONSTANT(DBCONFIG_LOOKASIDE)
			SQLITE_CONSTANT(DBCONFIG_MAINDBNAME)
			SQLITE_CONSTANT(DBCONFIG_NO_CKPT_ON_CLOSE)
			SQLITE_CONSTANT(DBSTATUS_CACHE_HIT)
			SQLITE_CONSTANT(DBSTATUS_CACHE_MISS)
			SQLITE_CONSTANT(DBSTATUS_CACHE_USED)
			SQLITE_CONSTANT(DBSTATUS_CACHE_USED_SHARED)
			SQLITE_CONSTANT(DBSTATUS_CACHE_WRITE)
			SQLITE_CONSTANT(DBSTATUS_DEFERRED_FKS)
			SQLITE_CONSTANT(DBSTATUS_LOOKASIDE_HIT)
			SQLITE_CONSTANT(DBSTATUS_LOOKASIDE_MISS_FULL)
			SQLITE_CONSTANT(DBSTATUS_LOOKASIDE_MISS_SIZE)
			SQLITE_CONSTANT(DBSTATUS_LOOKASIDE_USED)
			SQLITE_CONSTANT(DBSTATUS_MAX)
			SQLITE_CONSTANT(DBSTATUS_SCHEMA_USED)
			SQLITE_CONSTANT(DBSTATUS_STMT_USED)
			SQLITE_CONSTANT(DELETE)
			SQLITE_CONSTANT(DENY)
			SQLITE_CONSTANT(DETACH)
			SQLITE_CONSTANT(DETERMINISTIC)
			SQLITE_CONSTANT(DROP_INDEX)
			SQLITE_CONSTANT(DROP_TABLE)
			SQLITE_CONSTANT(DROP_TEMP_INDEX)
			SQLITE_CONSTANT(DROP_TEMP_TABLE)
			SQLITE_CONSTANT(DROP_TEMP_TRIGGER)
			SQLITE_CONSTANT(DROP_TEMP_VIEW)
			SQLITE_CONSTANT(DROP_TRIGGER)
			SQLITE_CONSTANT(DROP_VIEW)
			SQLITE_CONSTANT(DROP_VTABLE)
			SQLITE_CONSTANT(FAIL)
			SQLITE_CONSTANT(FCNTL_BUSYHANDLER)
			SQLITE_CONSTANT(FCNTL_CHUNK_SIZE)
			SQLITE_CONSTANT(FCNTL_COMMIT_PHASETWO)
			SQLITE_CONSTANT(FCNTL_FILE_POINTER)
			SQLITE_CONSTANT(FCNTL_GET_LOCKPROXYFILE)
			SQLITE_CONSTANT(FCNTL_HAS_MOVED)
			SQLITE_CONSTANT(FCNTL_JOURNAL_POINTER)
			SQLITE_CONSTANT(FCNTL_LAST_ERRNO)
			SQLITE_CONSTANT(FCNTL_LOCKSTATE)
			SQLITE_CONSTANT(FCNTL_MMAP_SIZE)
			SQLITE_CONSTANT(FCNTL_OVERWRITE)
			SQLITE_CONSTANT(FCNTL_PDB)
			SQLITE_CONSTANT(FCNTL_PERSIST_WAL)
			SQLITE_CONSTANT(FCNTL_POWERSAFE_OVERWRITE)
			SQLITE_CONSTANT(FCNTL_PRAGMA)
			SQLITE_CONSTANT(FCNTL_RBU)
			SQLITE_CONSTANT(FCNTL_SET_LOCKPROXYFILE)
			SQLITE_CONSTANT(FCNTL_SIZE_HINT)
			SQLITE_CONSTANT(FCNTL_SYNC)
			SQLITE_CONSTANT(FCNTL_SYNC_OMITTED)
			SQLITE_CONSTANT(FCNTL_TEMPFILENAME)
			SQLITE_CONSTANT(FCNTL_TRACE)
			SQLITE_CONSTANT(FCNTL_VFSNAME)
			SQLITE_CONSTANT(FCNTL_VFS_POINTER)
			SQLITE_CONSTANT(FCNTL_WAL_BLOCK)
			SQLITE_CONSTANT(FCNTL_WIN32_AV_RETRY)
			SQLITE_CONSTANT(FCNTL_WIN32_GET_HANDLE)
			SQLITE_CONSTANT(FCNTL_WIN32_SET_HANDLE)
			SQLITE_CONSTANT(FCNTL_ZIPVFS)
			SQLITE_CONSTANT(FUNCTION)
			SQLITE_CONSTANT(IGNORE)
			SQLITE_CONSTANT(INDEX_CONSTRAINT_EQ)
			SQLITE_CONSTANT(INDEX_CONSTRAINT_GE)
			SQLITE_CONSTANT(INDEX_CONSTRAINT_GLOB)
			SQLITE_CONSTANT(INDEX_CONSTRAINT_GT)
			SQLITE_CONSTANT(INDEX_CONSTRAINT_LE)
			SQLITE_CONSTANT(INDEX_CONSTRAINT_LIKE)
			SQLITE_CONSTANT(INDEX_CONSTRAINT_LT)
			SQLITE_CONSTANT(INDEX_CONSTRAINT_MATCH)
			SQLITE_CONSTANT(INDEX_CONSTRAINT_REGEXP)
			SQLITE_CONSTANT(INDEX_SCAN_UNIQUE)
			SQLITE_CONSTANT(INSERT)
			SQLITE_CONSTANT(IOCAP_ATOMIC)
			SQLITE_CONSTANT(IOCAP_ATOMIC16K)
			SQLITE_CONSTANT(IOCAP_ATOMIC1K)
			SQLITE_CONSTANT(IOCAP_ATOMIC2K)
			SQLITE_CONSTANT(IOCAP_ATOMIC32K)
			SQLITE_CONSTANT(IOCAP_ATOMIC4K)
			SQLITE_CONSTANT(IOCAP_ATOMIC512)
			SQLITE_CONSTANT(IOCAP_ATOMIC64K)
			SQLITE_CONSTANT(IOCAP_ATOMIC8K)
			SQLITE_CONSTANT(IOCAP_IMMUTABLE)
			SQLITE_CONSTANT(IOCAP_POWERSAFE_OVERWRITE)
			SQLITE_CONSTANT(IOCAP_SAFE_APPEND)
			SQLITE_CONSTANT(IOCAP_SEQUENTIAL)
			SQLITE_CONSTANT(IOCAP_UNDELETABLE_WHEN_OPEN)
			SQLITE_CONSTANT(IOERR_SHMSIZE)
			SQLITE_CONSTANT(LOCK_EXCLUSIVE)
			SQLITE_CONSTANT(LOCK_NONE)
			SQLITE_CONSTANT(LOCK_PENDING)
			SQLITE_CONSTANT(LOCK_RESERVED)
			SQLITE_CONSTANT(LOCK_SHARED)
			SQLITE_CONSTANT(MUTEX_FAST)
			SQLITE_CONSTANT(MUTEX_RECURSIVE)
			SQLITE_CONSTANT(MUTEX_STATIC_APP1)
			SQLITE_CONSTANT(MUTEX_STATIC_APP2)
			SQLITE_CONSTANT(MUTEX_STATIC_APP3)
			SQLITE_CONSTANT(MUTEX_STATIC_LRU)
			SQLITE_CONSTANT(MUTEX_STATIC_LRU2)
			SQLITE_CONSTANT(MUTEX_STATIC_MASTER)
			SQLITE_CONSTANT(MUTEX_STATIC_MEM)
			SQLITE_CONSTANT(MUTEX_STATIC_MEM2)
			SQLITE_CONSTANT(MUTEX_STATIC_OPEN)
			SQLITE_CONSTANT(MUTEX_STATIC_PMEM)
			SQLITE_CONSTANT(MUTEX_STATIC_PRNG)
			SQLITE_CONSTANT(MUTEX_STATIC_VFS1)
			SQLITE_CONSTANT(MUTEX_STATIC_VFS2)
			SQLITE_CONSTANT(MUTEX_STATIC_VFS3)
			SQLITE_CONSTANT(PRAGMA)
			SQLITE_CONSTANT(READ)
			SQLITE_CONSTANT(RECURSIVE)
			SQLITE_CONSTANT(REINDEX)
			SQLITE_CONSTANT(REPLACE)
			SQLITE_CONSTANT(ROLLBACK)
			SQLITE_CONSTANT(SAVEPOINT)
			SQLITE_CONSTANT(SCANSTAT_EST)
			SQLITE_CONSTANT(SCANSTAT_EXPLAIN)
			SQLITE_CONSTANT(SCANSTAT_NAME)
			SQLITE_CONSTANT(SCANSTAT_NLOOP)
			SQLITE_CONSTANT(SCANSTAT_NVISIT)
			SQLITE_CONSTANT(SCANSTAT_SELECTID)
			SQLITE_CONSTANT(SELECT)
			SQLITE_CONSTANT(SHM_EXCLUSIVE)
			SQLITE_CONSTANT(SHM_LOCK)
			SQLITE_CONSTANT(SHM_NLOCK)
			SQLITE_CONSTANT(SHM_SHARED)
			SQLITE_CONSTANT(SHM_UNLOCK)
			//SQLITE_CONSTANT(STATIC)
			SQLITE_CONSTANT(STATUS_MALLOC_COUNT)
			SQLITE_CONSTANT(STATUS_MALLOC_SIZE)
			SQLITE_CONSTANT(STATUS_MEMORY_USED)
			SQLITE_CONSTANT(STATUS_PAGECACHE_OVERFLOW)
			SQLITE_CONSTANT(STATUS_PAGECACHE_SIZE)
			SQLITE_CONSTANT(STATUS_PAGECACHE_USED)
			SQLITE_CONSTANT(STATUS_PARSER_STACK)
			SQLITE_CONSTANT(STATUS_SCRATCH_OVERFLOW)
			SQLITE_CONSTANT(STATUS_SCRATCH_SIZE)
			SQLITE_CONSTANT(STATUS_SCRATCH_USED)
			SQLITE_CONSTANT(STMTSTATUS_AUTOINDEX)
			SQLITE_CONSTANT(STMTSTATUS_FULLSCAN_STEP)
			SQLITE_CONSTANT(STMTSTATUS_SORT)
			SQLITE_CONSTANT(STMTSTATUS_VM_STEP)
			SQLITE_CONSTANT(SYNC_DATAONLY)
			SQLITE_CONSTANT(SYNC_FULL)
			SQLITE_CONSTANT(SYNC_NORMAL)
			SQLITE_CONSTANT(TESTCTRL_ALWAYS)
			SQLITE_CONSTANT(TESTCTRL_ASSERT)
			SQLITE_CONSTANT(TESTCTRL_BENIGN_MALLOC_HOOKS)
			SQLITE_CONSTANT(TESTCTRL_BITVEC_TEST)
			SQLITE_CONSTANT(TESTCTRL_BYTEORDER)
			SQLITE_CONSTANT(TESTCTRL_EXPLAIN_STMT)
			SQLITE_CONSTANT(TESTCTRL_FAULT_INSTALL)
			SQLITE_CONSTANT(TESTCTRL_FIRST)
			SQLITE_CONSTANT(TESTCTRL_IMPOSTER)
			SQLITE_CONSTANT(TESTCTRL_ISINIT)
			SQLITE_CONSTANT(TESTCTRL_ISKEYWORD)
			SQLITE_CONSTANT(TESTCTRL_LAST)
			SQLITE_CONSTANT(TESTCTRL_LOCALTIME_FAULT)
			SQLITE_CONSTANT(TESTCTRL_NEVER_CORRUPT)
			SQLITE_CONSTANT(TESTCTRL_ONCE_RESET_THRESHOLD)
			SQLITE_CONSTANT(TESTCTRL_OPTIMIZATIONS)
			SQLITE_CONSTANT(TESTCTRL_PENDING_BYTE)
			SQLITE_CONSTANT(TESTCTRL_PRNG_RESET)
			SQLITE_CONSTANT(TESTCTRL_PRNG_RESTORE)
			SQLITE_CONSTANT(TESTCTRL_PRNG_SAVE)
			SQLITE_CONSTANT(TESTCTRL_RESERVE)
			SQLITE_CONSTANT(TESTCTRL_SCRATCHMALLOC)
			SQLITE_CONSTANT(TESTCTRL_SORTER_MMAP)
			SQLITE_CONSTANT(TESTCTRL_VDBE_COVERAGE)
			SQLITE_CONSTANT(TRACE_CLOSE)
			SQLITE_CONSTANT(TRACE_PROFILE)
			SQLITE_CONSTANT(TRACE_ROW)
			SQLITE_CONSTANT(TRACE_STMT)
			SQLITE_CONSTANT(TRANSACTION)
			//SQLITE_CONSTANT(TRANSIENT)
			SQLITE_CONSTANT(UPDATE)
			SQLITE_CONSTANT(UTF16)
			SQLITE_CONSTANT(UTF16BE)
			SQLITE_CONSTANT(UTF16LE)
			SQLITE_CONSTANT(UTF16_ALIGNED)
			SQLITE_CONSTANT(UTF8)
			SQLITE_CONSTANT(VTAB_CONSTRAINT_SUPPORT)

			//Enums
			//Result Codes
			SQLITE_ENUM(OK)
			SQLITE_ENUM(ERROR)
			SQLITE_ENUM(INTERNAL)
			SQLITE_ENUM(PERM)
			SQLITE_ENUM(ABORT)
			SQLITE_ENUM(BUSY)
			SQLITE_ENUM(LOCKED)
			SQLITE_ENUM(NOMEM)
			SQLITE_ENUM(READONLY)
			SQLITE_ENUM(INTERRUPT)
			SQLITE_ENUM(IOERR)
			SQLITE_ENUM(CORRUPT)
			SQLITE_ENUM(NOTFOUND)
			SQLITE_ENUM(FULL)
			SQLITE_ENUM(CANTOPEN)
			SQLITE_ENUM(PROTOCOL)
			SQLITE_ENUM(EMPTY)
			SQLITE_ENUM(SCHEMA)
			SQLITE_ENUM(TOOBIG)
			SQLITE_ENUM(CONSTRAINT)
			SQLITE_ENUM(MISMATCH)
			SQLITE_ENUM(MISUSE)
			SQLITE_ENUM(NOLFS)
			SQLITE_ENUM(AUTH)
			SQLITE_ENUM(FORMAT)
			SQLITE_ENUM(RANGE)
			SQLITE_ENUM(NOTADB)
			SQLITE_ENUM(NOTICE)
			SQLITE_ENUM(WARNING)
			SQLITE_ENUM(ROW)
			SQLITE_ENUM(DONE)
			//Extended Codes
			SQLITE_ENUM(IOERR_READ)
			SQLITE_ENUM(IOERR_SHORT_READ)
			SQLITE_ENUM(IOERR_WRITE)
			SQLITE_ENUM(IOERR_FSYNC)
			SQLITE_ENUM(IOERR_DIR_FSYNC)
			SQLITE_ENUM(IOERR_TRUNCATE)
			SQLITE_ENUM(IOERR_FSTAT)
			SQLITE_ENUM(IOERR_UNLOCK)
			SQLITE_ENUM(IOERR_RDLOCK)
			SQLITE_ENUM(IOERR_DELETE)
			SQLITE_ENUM(IOERR_BLOCKED)
			SQLITE_ENUM(IOERR_NOMEM)
			SQLITE_ENUM(IOERR_ACCESS)
			SQLITE_ENUM(IOERR_CHECKRESERVEDLOCK)
			SQLITE_ENUM(IOERR_LOCK)
			SQLITE_ENUM(IOERR_CLOSE)
			SQLITE_ENUM(IOERR_DIR_CLOSE)
			SQLITE_ENUM(IOERR_SHMOPEN)
			SQLITE_ENUM(IOERR_SHMLOCK)
			SQLITE_ENUM(IOERR_SHMMAP)
			SQLITE_ENUM(IOERR_SEEK)
			SQLITE_ENUM(IOERR_DELETE_NOENT)
			SQLITE_ENUM(IOERR_MMAP)
			SQLITE_ENUM(IOERR_GETTEMPPATH)
			SQLITE_ENUM(IOERR_CONVPATH)
			SQLITE_ENUM(IOERR_VNODE)
			SQLITE_ENUM(IOERR_AUTH)
			SQLITE_ENUM(LOCKED_SHAREDCACHE)
			SQLITE_ENUM(BUSY_RECOVERY)
			SQLITE_ENUM(BUSY_SNAPSHOT)
			SQLITE_ENUM(CANTOPEN_NOTEMPDIR)
			SQLITE_ENUM(CANTOPEN_ISDIR)
			SQLITE_ENUM(CANTOPEN_FULLPATH)
			SQLITE_ENUM(CANTOPEN_CONVPATH)
			SQLITE_ENUM(CORRUPT_VTAB)
			SQLITE_ENUM(READONLY_RECOVERY)
			SQLITE_ENUM(READONLY_CANTLOCK)
			SQLITE_ENUM(READONLY_ROLLBACK)
			SQLITE_ENUM(READONLY_DBMOVED)
			SQLITE_ENUM(ABORT_ROLLBACK)
			SQLITE_ENUM(CONSTRAINT_CHECK)
			SQLITE_ENUM(CONSTRAINT_COMMITHOOK)
			SQLITE_ENUM(CONSTRAINT_FOREIGNKEY)
			SQLITE_ENUM(CONSTRAINT_FUNCTION)
			SQLITE_ENUM(CONSTRAINT_NOTNULL)
			SQLITE_ENUM(CONSTRAINT_PRIMARYKEY)
			SQLITE_ENUM(CONSTRAINT_TRIGGER)
			SQLITE_ENUM(CONSTRAINT_UNIQUE)
			SQLITE_ENUM(CONSTRAINT_VTAB)
			SQLITE_ENUM(CONSTRAINT_ROWID)
			SQLITE_ENUM(NOTICE_RECOVER_WAL)
			SQLITE_ENUM(NOTICE_RECOVER_ROLLBACK)
			SQLITE_ENUM(WARNING_AUTOINDEX)
			SQLITE_ENUM(AUTH_USER)
			SQLITE_ENUM(OK_LOAD_PERMANENTLY)
			//Flags for file open operations
			SQLITE_ENUM(OPEN_READONLY)
			SQLITE_ENUM(OPEN_READWRITE)
			SQLITE_ENUM(OPEN_CREATE)
			SQLITE_ENUM(OPEN_DELETEONCLOSE)
			SQLITE_ENUM(OPEN_EXCLUSIVE)
			SQLITE_ENUM(OPEN_AUTOPROXY)
			SQLITE_ENUM(OPEN_URI)
			SQLITE_ENUM(OPEN_MEMORY)
			SQLITE_ENUM(OPEN_MAIN_DB)
			SQLITE_ENUM(OPEN_TEMP_DB)
			SQLITE_ENUM(OPEN_TRANSIENT_DB)
			SQLITE_ENUM(OPEN_MAIN_JOURNAL)
			SQLITE_ENUM(OPEN_TEMP_JOURNAL)
			SQLITE_ENUM(OPEN_SUBJOURNAL)
			SQLITE_ENUM(OPEN_MASTER_JOURNAL)
			SQLITE_ENUM(OPEN_NOMUTEX)
			SQLITE_ENUM(OPEN_FULLMUTEX)
			SQLITE_ENUM(OPEN_SHAREDCACHE)
			SQLITE_ENUM(OPEN_PRIVATECACHE)
			SQLITE_ENUM(OPEN_WAL);
		#undef SQLITE_CONSTANT_TYPE
		#undef SQLITE_CONSTANT
		//#undef SQLITEDB_METHOD_OVERRIDE
		#undef SQLITEDB_METHOD
		#undef SQLITE_METHOD
		#undef SQLITE_ENUM
		////////////////////////////////////////////////////////////////////////
	}

	////////////////////////////////////////////////////////////////////////
	//SQLite DB

	SQLiteDB::SQLiteDB() {
		SQLiteDBBus::Handler::BusConnect(this);
		this->m_OpenType = CLOSED;
	}

	SQLiteDB::SQLiteDB(sqlite3* db) {
		m_pDB = db;
		SQLiteDBBus::Handler::BusConnect(this);
		this->m_OpenType = CLOSED;
	}

	SQLiteDB::SQLiteDB(sqlite3* db, AZ::EntityId id) {
		this->m_pDB = db;
		this->m_entityid = id;
		SQLiteDBBus::Handler::BusConnect(this);
		this->m_OpenType = CLOSED;
	}

	SQLiteDB::SQLiteDB(SQLiteDB * db) {
		this->m_pDB = db->m_pDB;
		SQLiteDBBus::Handler::BusConnect(this);
		this->m_OpenType = CLOSED;
	}

	SQLiteDB::SQLiteDB(SQLiteDB * db, AZ::EntityId id) {
		this->m_pDB = db->m_pDB;
		this->m_entityid = id;
		SQLiteDBBus::Handler::BusConnect(this);
		this->m_OpenType = db->m_OpenType;
	}

	SQLiteDB::~SQLiteDB() {
		SQLiteDBBus::Handler::BusDisconnect();
		this->Close();
	}

	int SQLiteDB::Open(const char * path) {
		if (this->m_entityid.IsValid()) {
			SQLiteDB * db;
			SQLITE_BUS(db, this->m_entityid, GetConnection);
			
			if (db != nullptr) {
				int ret = db->Open(path);
				this->m_OpenType = db->m_OpenType;
				return ret;
			}
		}

		if (this->m_OpenType != CLOSED) {
			int err = this->Close2Open();
			if (err != SQLITE_OK) return err;
		}

		if (path == nullptr) path = ":memory:";
		AZ_Printf("SQLite3", "Opening Database - %s", path);
		this->m_OpenType = OPEN;

		AZStd::string sPath = path;

		if (gEnv) {
			if (sPath.compare(":memory:") == 0 || sPath.find("file:") != AZStd::string::npos) return sqlite3_open(path, &this->m_pDB);
			else if (sPath.size() == 0) return sqlite3_open("", &this->m_pDB);
			else {
				char * resolvedPath = new char[AZ_MAX_PATH_LEN];
				gEnv->pFileIO->ResolvePath(path, resolvedPath, AZ_MAX_PATH_LEN);
				AZ_Printf("SQLite3", "Resolved DB Path - %s", resolvedPath);
				int ret = sqlite3_open(resolvedPath, &this->m_pDB);
				delete resolvedPath;
				return ret;
			}
		} else {
			return sqlite3_open(path, &this->m_pDB);
		}
	}

	int SQLiteDB::Open16(const char * path) {
		if (this->m_entityid.IsValid()) {
			SQLiteDB * db;
			SQLITE_BUS(db, this->m_entityid, GetConnection);

			if (db != nullptr) {
				int ret = db->Open16(path);
				this->m_OpenType = db->m_OpenType;
				return ret;
			}
		}

		if (this->m_OpenType != CLOSED) {
			int err = this->Close2Open();
			if (err != SQLITE_OK) return err;
		}

		if (path == nullptr) path = ":memory:";
		AZ_Printf("SQLite3", "Opening Database - %s", path);
		this->m_OpenType = OPEN16;

		AZStd::string sPath = path;

		if (gEnv) {
			if (sPath.compare(":memory:") == 0 || sPath.find("file:") != AZStd::string::npos) return sqlite3_open16(path, &this->m_pDB);
			else if (sPath.size() == 0) return sqlite3_open("", &this->m_pDB);
			else {
				char * resolvedPath = new char[AZ_MAX_PATH_LEN];
				gEnv->pFileIO->ResolvePath(path, resolvedPath, AZ_MAX_PATH_LEN);
				AZ_Printf("SQLite3", "Resolved DB Path - %s", resolvedPath);
				int ret = sqlite3_open16(resolvedPath, &this->m_pDB);
				delete resolvedPath;
				return ret;
			}
		} else {
			return sqlite3_open16(path, &this->m_pDB);
		}
	}

	int SQLiteDB::Open_v2(const char * path, int flags, const char *zVfs) {
		if (this->m_entityid.IsValid()) {
			SQLiteDB * db;
			SQLITE_BUS(db, this->m_entityid, GetConnection);

			if (db != nullptr) {
				int ret = db->Open_v2(path, flags, zVfs);
				this->m_OpenType = db->m_OpenType;
				return ret;
			}
		}

		if (this->m_OpenType != CLOSED) {
			int err = this->Close2Open();
			if (err != SQLITE_OK) return err;
		}

		if (path == nullptr) path = ":memory:";
		AZ_Printf("SQLite3", "Opening Database - %s", path);
		this->m_OpenType = OPENV2;

		AZStd::string sPath = path;

		if (gEnv) {
			if (sPath.compare(":memory:") == 0 || sPath.find("file:") != AZStd::string::npos) return sqlite3_open_v2(path, &this->m_pDB, flags, zVfs);
			else if (sPath.size() == 0) return sqlite3_open("", &this->m_pDB);
			else {
				char * resolvedPath = new char[AZ_MAX_PATH_LEN];
				gEnv->pFileIO->ResolvePath(path, resolvedPath, AZ_MAX_PATH_LEN);
				AZ_Printf("SQLite3", "Resolved DB Path - %s", resolvedPath);
				int ret = sqlite3_open_v2(resolvedPath, &this->m_pDB, flags, zVfs);
				delete resolvedPath;
				return ret;
			}
		} else {
			return sqlite3_open_v2(path, &this->m_pDB, flags, zVfs);
		}
	}

	//visible close
	//if entity id is valid allows ONLY the entity to close it.
	int SQLiteDB::Close() {
		if (this->m_entityid.IsValid()) return SQLITE_MISUSE;
		return this->Close2Open();
		//return sqlite3_close(this->m_pDB);
	}

	//hidden close
	//used to be able to close a connection for a follow up to an open.
	int SQLiteDB::Close2Open() {
		AZ_Printf("SQLite3", "Closing Database");
		switch (this->m_OpenType) {
		case OPEN:
		case OPEN16:
			if (this->m_pDB) return sqlite3_close(this->m_pDB);
			break;
		case OPENV2:
			if (this->m_pDB) return sqlite3_close_v2(this->m_pDB);
			break;
		}
		return SQLITE_OK;
	}

	SQLiteStmt * SQLiteDB::Prepare(const char * sql, int nByte, const char **pzTail) {
		SQLiteStmt *ret = new SQLiteStmt();
		ret->m_err = sqlite3_prepare(this->m_pDB, sql, nByte, &ret->m_pStmt, pzTail);
		return ret;
	}

	SQLiteStmt * SQLiteDB::Prepare_v2(const char * sql, int nByte, const char **pzTail) {
		SQLiteStmt *ret = new SQLiteStmt();
		ret->m_err = sqlite3_prepare_v2(this->m_pDB, sql, nByte, &ret->m_pStmt, pzTail);
		return ret;
	}

	SQLiteStmt * SQLiteDB::Prepare16(const char * sql, int nByte, const void **pzTail) {
		SQLiteStmt *ret = new SQLiteStmt();
		ret->m_err = sqlite3_prepare16(this->m_pDB, sql, nByte, &ret->m_pStmt, pzTail);
		return ret;
	}

	SQLiteStmt * SQLiteDB::Prepare16_v2(const char * sql, int nByte, const void **pzTail) {
		SQLiteStmt *ret = new SQLiteStmt();
		ret->m_err = sqlite3_prepare16_v2(this->m_pDB, sql, nByte, &ret->m_pStmt, pzTail);
		return ret;
	}
	////////////////////////////////////////////////////////////////////////


	/*int SQLiteDB::Exec(const char *sql) { this->Exec(sql, nullptr, nullptr, nullptr); }
	int SQLiteDB::Exec(const char *sql, char **errmsg) { this->Exec(sql, nullptr, nullptr, errmsg); }
	int SQLiteDB::Exec(const char *sql, int(*callback)(void*, int, char**, char**)) { this->Exec(sql, callback, nullptr, nullptr); }
	int SQLiteDB::Exec(const char *sql, int(*callback)(void*, int, char**, char**), char **errmsg) { this->Exec(sql, callback, nullptr, errmsg); }*/
	int SQLiteDB::Exec(const char *sql, int(*callback)(void*, int, char**, char**), void *cbarg, char **errmsg) { return sqlite3_exec(this->m_pDB,sql, callback, cbarg, errmsg); }

	int SQLiteDB::ErrCode() { return sqlite3_errcode(this->m_pDB); }
	int SQLiteDB::ExtErrCode() { return sqlite3_extended_errcode(this->m_pDB); }
	const char * SQLiteDB::ErrMsg() { return sqlite3_errmsg(this->m_pDB); }
	const void * SQLiteDB::ErrMsg16() { return sqlite3_errmsg16(this->m_pDB); }
	int SQLiteDB::Limit(int id, int newVal) { return sqlite3_limit(this->m_pDB, id, newVal); }
	SQLiteMutex * SQLiteDB::DB_Mutex() { return new SQLiteMutex(sqlite3_db_mutex(this->m_pDB)); }
	int SQLiteDB::DB_CacheFlush() { return sqlite3_db_cacheflush(this->m_pDB); }
	const char * SQLiteDB::DB_Filename(const char * zDbName) { return sqlite3_db_filename(this->m_pDB, zDbName); }
	int SQLiteDB::DB_ReadOnly(const char *zDbName) { return sqlite3_db_readonly(this->m_pDB, zDbName); }
	int SQLiteDB::DB_Release_Memory() { return sqlite3_db_release_memory(this->m_pDB); }
	int SQLiteDB::DB_Status(int op, int *pCur, int *pHiwtr, int resetFlg) { return sqlite3_db_status(this->m_pDB, op, pCur, pHiwtr, resetFlg); } //override
	int SQLiteDB::Declare_VTab(const char * zSQL) { return sqlite3_declare_vtab(this->m_pDB, zSQL); }
	int SQLiteDB::Enable_Load_Extension(int onoff) { return sqlite3_enable_load_extension(this->m_pDB, onoff); }
	int SQLiteDB::File_Control(const char * zDbName, int op, void * x) { return sqlite3_file_control(this->m_pDB, zDbName, op, x); }
	int SQLiteDB::Get_Table(const char *zSql, char ***pazResult, int *pnRow, int *pnColumn, char **pzErrmsg) { return sqlite3_get_table(this->m_pDB, zSql, pazResult, pnRow, pnColumn, pzErrmsg); }
	void SQLiteDB::Free_Table(char **result) { sqlite3_free_table(result); }
	int SQLiteDB::Get_AutoCommit() { return sqlite3_get_autocommit(this->m_pDB); }
	void SQLiteDB::Interrupt() { sqlite3_interrupt(this->m_pDB); }
	__int64 SQLiteDB::Last_Insert_RowId() { return (__int64)sqlite3_last_insert_rowid(this->m_pDB); }
	int SQLiteDB::Load_Extension(const char * zFile, const char * zProc, char ** pzErrMsg) { //override
		int ret = SQLITE_ERROR;

		if (gEnv) {
			char * resolvedFile = new char[AZ_MAX_PATH_LEN];
			gEnv->pFileIO->ResolvePath(zFile, resolvedFile, AZ_MAX_PATH_LEN);
			ret = sqlite3_load_extension(this->m_pDB, resolvedFile, zProc, pzErrMsg);
			delete resolvedFile;
		} else
			ret = sqlite3_load_extension(this->m_pDB, zFile, zProc, pzErrMsg);

		return ret;
	};
	int SQLiteDB::Overload_Function(const char * zFuncName, int nArgs) { return sqlite3_overload_function(this->m_pDB, zFuncName, nArgs); }
	void SQLiteDB::Set_Last_Insert_RowId(__int64 N) { sqlite3_set_last_insert_rowid(this->m_pDB, N); }
	int SQLiteDB::System_ErrNo() { return sqlite3_system_errno(this->m_pDB); }
	int SQLiteDB::Total_Changes() { return sqlite3_total_changes(this->m_pDB); }
	int SQLiteDB::VTab_On_Conflict() { return sqlite3_vtab_on_conflict(this->m_pDB); }
	int SQLiteDB::Wal_AutoCheckpoint(int N) { return sqlite3_wal_autocheckpoint(this->m_pDB, N); }
	int SQLiteDB::Wal_Checkpoint(const char * zDB) { return sqlite3_wal_checkpoint(this->m_pDB, zDB); }
	int SQLiteDB::Table_Column_Metadata(const char *zDbName, const char *zTableName, const char *zColumnName, char const **pzDataType, char const ** pzCollSeq, int *pNotNull, int *pPrimaryKey, int *pAutoinc) { return sqlite3_table_column_metadata(this->m_pDB, zDbName, zTableName, zColumnName, pzDataType, pzCollSeq, pNotNull, pPrimaryKey, pAutoinc); }
	int SQLiteDB::Wal_Checkpoint_v2(const char *zDb, int eMode, int *pnLog, int *pnCkpt) { return sqlite3_wal_checkpoint_v2(this->m_pDB, zDb, eMode, pnLog, pnCkpt); } //override
	int SQLiteDB::Status64(int op, __int64 *pCurrent, __int64 *pHighwater, int resetFlag) { return sqlite3_status64(op, (sqlite3_int64*)pCurrent, (sqlite3_int64*)pHighwater, resetFlag); }
	__int64 SQLiteDB::Memory_Used() { return (__int64)sqlite3_memory_used(); }
	__int64 SQLiteDB::Memory_HighWater(int resetFlag) { return (__int64)sqlite3_memory_highwater(resetFlag); }
	__int64 SQLiteDB::Soft_Heap_Limit64(__int64 N) { return (__int64)sqlite3_soft_heap_limit64((sqlite3_int64)N); }
	const char * SQLiteDB::ErrStr(int rc) { return sqlite3_errstr(rc); }
	const char * SQLiteDB::LibVersion() { return sqlite3_libversion(); }
	int SQLiteDB::LibVersion_Number() { return sqlite3_libversion_number(); }
	const char * SQLiteDB::SourceID() { return sqlite3_sourceid(); }
	int SQLiteDB::Enable_Shared_Cache(int enable) { return sqlite3_enable_shared_cache(enable); }
	void SQLiteDB::Reset_Auto_Ext() { sqlite3_reset_auto_extension(); }
	int SQLiteDB::ThreadSafe() { return sqlite3_threadsafe(); }
	const char * SQLiteDB::URI_Parameter(const char *zFilename, const char *zParam) { return sqlite3_uri_parameter(zFilename, zParam); }
	int SQLiteDB::URI_Boolean(const char *zFilename, const char *zParam, int bDflt) { return sqlite3_uri_boolean(zFilename, zParam, bDflt); }
	__int64 SQLiteDB::URI_Int64(const char *zFilename, const char *zParam, int bDflt) { return (__int64)sqlite3_uri_int64(zFilename, zParam, bDflt); }
	int SQLiteDB::Status(int op, int *pCurrent, int *pHighwater, int resetFlag) { return sqlite3_status(op, pCurrent, pHighwater, resetFlag); }
	int SQLiteDB::Complete(const char *zSql) { return sqlite3_complete(zSql); }
	int SQLiteDB::Complete16(const void *zSql) { return sqlite3_complete16(zSql); }

}