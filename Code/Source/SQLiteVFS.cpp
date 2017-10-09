#include "StdAfx.h"
#include "SQLite\SQLiteVFS.h"

namespace SQLite {
	void SQLiteVFS::RegisterBehaviorContext(AZ::BehaviorContext* bc) {
		#define SQLITEVFS_METHOD(name, defaultval, dbgDesc) ->Method(#name,&SQLiteVFS::##name##,##defaultval##,##dbgDesc##)
		bc->Class<SQLiteVFS>("SQLite3VFS")
			->Constructor<const char*>()

			SQLITEVFS_METHOD(Find, nullptr, "")
			SQLITEVFS_METHOD(Register, nullptr, "")
			SQLITEVFS_METHOD(Unregister, nullptr, "")
			;
		#undef SQLITEVFS_METHOD
	}

	SQLiteVFS::SQLiteVFS(const char * vfsName) {
		this->m_pVFS = sqlite3_vfs_find(vfsName);
		SQLiteVFSBus::Handler::BusConnect(this);
	}
	SQLiteVFS::SQLiteVFS(sqlite3_vfs * vfs) {
		this->m_pVFS = vfs;
		SQLiteVFSBus::Handler::BusConnect(this);
	}
	SQLiteVFS::~SQLiteVFS() {
		SQLiteVFSBus::Handler::BusDisconnect();
	}

	SQLiteVFS * SQLiteVFS::Find(const char * vfsName) { return new SQLiteVFS(sqlite3_vfs_find(vfsName)); }
	int SQLiteVFS::Register(int makeDflt) { return sqlite3_vfs_register(this->m_pVFS, makeDflt); }
	int SQLiteVFS::Unregister() { return sqlite3_vfs_unregister(this->m_pVFS); }
}