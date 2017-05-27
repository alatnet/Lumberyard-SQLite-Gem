#pragma once

#include <AzCore\RTTI\TypeInfo.h>
#include <AzCore/RTTI/BehaviorContext.h>

namespace SQLite3 {
	////////////////////////////////////////////////////////////////////////
	//SQLiteVFS Class
	class SQLiteVFS {
	public:
		AZ_TYPE_INFO(SQLiteVFS, "{657B0155-BA5B-4FDF-91EC-A967051577EC}");
	public:
		SQLiteVFS(sqlite3_vfs * vfs) { this->m_pVFS = vfs; }
	public:
		SQLiteVFS * Find(const char * vfsName) { return new SQLiteVFS(sqlite3_vfs_find(vfsName)); }
		int Register(int makeDflt) { return sqlite3_vfs_register(this->m_pVFS, makeDflt); }
		int Unregister() { return sqlite3_vfs_unregister(this->m_pVFS); }
	public:
		sqlite3_vfs * m_pVFS;
	public:
		static void RegisterBehaviorContext(AZ::BehaviorContext* bc);
	};
	////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////
//SQLiteVFS Class
/*
sqlite3_vfs *sqlite3_vfs_find(const char *zVfsName);
int sqlite3_vfs_register(sqlite3_vfs*, int makeDflt);
int sqlite3_vfs_unregister(sqlite3_vfs*);
*/
////////////////////////////////////////////////////////////////////////