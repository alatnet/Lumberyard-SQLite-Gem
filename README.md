# Lumberyard-SQLite-Gem
An Amazon Lumberyard Gem that adds in SQLite functionality.

## Requirements
To compile the gem, it requires that you have downloaded SQLite from [sqlite.org](sqlite.org).  
Download the amalgamation version to be able to compile the gem.  
sqlite3.c needs to be placed in Code/Source.  
sqlite3.h needs to be placed in Code/Include/SQLite.

## Build Requirements
Due to the nature of how Lumberyard builds its files, it will force enable C2220 - Warning treated as error (/WX).  
To overcome this, it is required that you add "__pragma(warning(disable : 4267));" and "__pragma(warning(default : 4267));" for the section of code that triggers this error.  

## C++ Syntax
The C++ Syntax is generally the same as the C syntax but due to the way Lumberyard loads Components you will not be able to directly use the C++ classes themselves.  
To use the C++ classes functions you have to go through EBuses.  
Almost every function in the classes has an EBus equivalent.  
Each EBus requires an address which is the pointer to the class that is going to have it\'s function called.  
In addition, macros are available to make it easier to deal with the buses.

## Lumberyard Specific C++
Due to the way EBuses function, there is no current way to create a new pointer (i.e. new <class>).  
To get around this, the System Entity can create new pointers using it\'s SQLiteBus EBus interface.  
DO NOT under ANY circumstances use the Broadcast EBus function.  
Use Event or EventResult instead to use a specific database\'s function and a class pointer as the address.  
NOTE! DO NOT call Close on a database connection that is associated with an entity.  
The entity will automatically close it\'s database connection apon destruction.  
Calling Close on an entity database connection will return a SQLITE_MISUSE error.  
Calling any of the Open functions on an entity will automatically close and open the database connection with the appropriate sqlite open function that was requested.

## C++ Classes
SQLiteDB - SQLiteDBBus - SQLite/SQLiteDB.h - sqlite3 \*  
SQLiteStmt - SQLiteStmtBus - SQLite/SQLiteStmt.h - sqlite3_stmt \*  
SQLiteBackup - SQLiteBackupBus - SQLite/SQLiteBackup.h - sqlite3_backup \*  
SQLiteValue - SQLiteValueBus - SQLite/SQLiteValue.h - sqlite3_value \*  
SQLiteVFS - SQLiteVFSBus - SQLite/SQLiteVFS.h - sqlite3_vfs \*  
SQLiteMutex - SQLiteMutexBus - SQLite/SQLiteMutex.h - sqlite3_mutex \*  

## C++ Example Using Direct Bus Functions
```C++
#include <SQLite/SQLiteBus.h>
#include <SQLite\sqlite3.h>

int main(){
  int ret;
  //System Entity Database
  ///////////////////////////////////////////////////
  SQLite3::SQLiteDB * sysDb;
  SQLite::SQLiteRequestBus::EventResult(sysDb, AZ::EntityId(0), &SQLite::SQLiteRequestBus::Events::GetConnection); //Get the system entity database connection
  AZ_Assert(sysDb, "sysDb is null.");

  //create a table
  SQLite3::SQLiteDBBus::EventResult(ret, sysDb, &SQLite3::SQLiteDBBus::Events::Exec, "CREATE TABLE System (Col1 int, Col2 varchar(255));", nullptr, nullptr, nullptr);
  AZ_Assert(ret == SQLITE_OK, "Create Table Failed.");

  //insert a row
  SQLite3::SQLiteDBBus::EventResult(ret, sysDb, &SQLite3::SQLiteDBBus::Events::Exec, "INSERT INTO System (Col1, Col2) VALUES(0,'FIRST');", nullptr, nullptr, nullptr);
  AZ_Assert(ret == SQLITE_OK, "Insert Failed.");

  //insert a row
  SQLite3::SQLiteDBBus::EventResult(ret, sysDb, &SQLite3::SQLiteDBBus::Events::Exec, "INSERT INTO System (Col1, Col2) VALUES(1,'SECOND');", nullptr, nullptr, nullptr);
  AZ_Assert(ret == SQLITE_OK, "Insert Failed.");

  //insert a row
  SQLite3::SQLiteDBBus::EventResult(ret, sysDb, &SQLite3::SQLiteDBBus::Events::Exec, "INSERT INTO System (Col1, Col2) VALUES(2,'THIRD');", nullptr, nullptr, nullptr);
  AZ_Assert(ret == SQLITE_OK, "Insert Failed.");
  
  //select all from the table
  SQLite3::SQLiteDBBus::EventResult(
    ret,
    sysDb,
    &SQLite3::SQLiteDBBus::Events::Exec,
    "SELECT * FROM System;",
    [](void* cbarg, int argc, char **argv, char **azColName) -> int {
      for (int i = 0; i < argc; i++) CryLog("%s = %s", azColName[i], argv[i] ? argv[i] : "NULL");
      return 0;
    },
    nullptr,
    nullptr
  );
  AZ_Assert(ret == SQLITE_OK, "Select Failed");
  ///////////////////////////////////////////////////

  //Individual Database
  ///////////////////////////////////////////////////
  SQLite3::SQLiteDB * indDb;
  SQLite::SQLiteRequestBus::EventResult(indDb, AZ::EntityId(0), &SQLite::SQLiteRequestBus::Events::NewConnection); //create a new database connection (separate from entity)
  AZ_Assert(indDb, "indDb is null.");

  //open the database connection
  SQLite3::SQLiteDBBus::EventResult(ret, indDb, &SQLite3::SQliteDBBus::Events::Open, ":memory:");
  AZ_Assert(ret == SQLITE_OK, "Open Failed.");

  //create a table
  SQLite3::SQLiteDBBus::EventResult(ret, indDb, &SQLite3::SQLiteDBBus::Events::Exec, "CREATE TABLE Individual (Col3 int, Col4 varchar(255));", nullptr, nullptr, nullptr);
  AZ_Assert(ret == SQLITE_OK, "Create Table Failed.");

  //insert a row
  SQLite3::SQLiteDBBus::EventResult(ret, indDb, &SQLite3::SQLiteDBBus::Events::Exec, "INSERT INTO Individual (Col3, Col4) VALUES(3,'FOURTH');", nullptr, nullptr, nullptr);
  AZ_Assert(ret == SQLITE_OK, "Insert Failed.");

  //insert a row
  SQLite3::SQLiteDBBus::EventResult(ret, indDb, &SQLite3::SQLiteDBBus::Events::Exec, "INSERT INTO Individual (Col3, Col4) VALUES(4,'FIFTH');", nullptr, nullptr, nullptr);
  AZ_Assert(ret == SQLITE_OK, "Insert Failed.");

  //insert a row
  SQLite3::SQLiteDBBus::EventResult(ret, indDb, &SQLite3::SQLiteDBBus::Events::Exec, "INSERT INTO Individual (Col3, Col4) VALUES(5,'SIXTH');", nullptr, nullptr, nullptr);
  AZ_Assert(ret == SQLITE_OK, "Insert Failed.");
  
  //select all from the table
  SQLite3::SQLiteDBBus::EventResult(
    ret,
    sysDb,
    &SQLite3::SQLiteDBBus::Events::Exec,
    "SELECT * FROM Individual;",
    [](void* cbarg, int argc, char **argv, char **azColName) -> int {
      for (int i = 0; i < argc; i++) CryLog("%s = %s", azColName[i], argv[i] ? argv[i] : "NULL");
      return 0;
    },
    nullptr,
    nullptr
  );
  AZ_Assert(ret == SQLITE_OK, "Select Failed");
  
  //close the database connection
  SQLite3::SQLiteDBBus::EventResult(ret, indDb, &SQLite3::SQliteDBBus::Events::Close);
  AZ_Assert(ret == SQLITE_OK, "Close Failed.");

  //free up memory;
  delete indDb;
  ///////////////////////////////////////////////////

  return 0;
}
```

## C++ Example Using Bus Macros
```C++
#include <SQLite/SQLiteBus.h>
#include <SQLite\sqlite3.h>

int main(){
  int ret;
  //System Entity Database
  ///////////////////////////////////////////////////
  SQLite3::SQLiteDB * sysDb;
  SQLITE_BUS(sysDb, AZ::EntityId(0), GetConnection); //Get the system entity database connection
  AZ_Assert(sysDb, "sysDb is null.");

  //create a table
  SQLITEDB_BUS(ret, sysDb, Exec, "CREATE TABLE System (Col1 int, Col2 varchar(255));", nullptr, nullptr, nullptr);
  AZ_Assert(ret == SQLITE_OK, "Create Table Failed.");

  //insert a row
  SQLITEDB_BUS(ret, sysDb, Exec, "INSERT INTO System (Col1, Col2) VALUES(0,'FIRST');", nullptr, nullptr, nullptr);
  AZ_Assert(ret == SQLITE_OK, "Insert Failed.");

  //insert a row
  SQLITEDB_BUS(ret, sysDb, Exec, "INSERT INTO System (Col1, Col2) VALUES(1,'SECOND');", nullptr, nullptr, nullptr);
  AZ_Assert(ret == SQLITE_OK, "Insert Failed.");

  //insert a row
  SQLITEDB_BUS(ret, sysDb, Exec, "INSERT INTO System (Col1, Col2) VALUES(2,'THIRD');", nullptr, nullptr, nullptr);
  AZ_Assert(ret == SQLITE_OK, "Insert Failed.");
  
  //select all from the table
  SQLITEDB_BUS(
	ret,
	sysDb,
	Exec,
	"SELECT * FROM System;",
	[](void* cbarg, int argc, char **argv, char **azColName) -> int {
		for (int i = 0; i < argc; i++) CryLog("%s = %s", azColName[i], argv[i] ? argv[i] : "NULL");
		return 0;
	},
	nullptr,
	nullptr
  );
  AZ_Assert(ret == SQLITE_OK, "Select Failed");
  ///////////////////////////////////////////////////

  //Individual Database
  ///////////////////////////////////////////////////
  SQLite3::SQLiteDB * indDb;
  SQLITE_BUS(indDb, AZ::EntityId(0), NewConnection); //create a new database connection (separate from entity)
  AZ_Assert(indDb, "indDb is null.");

  //open the database connection
  SQLITEDB_BUS(ret, indDb, Open, ":memory:");
  AZ_Assert(ret == SQLITE_OK, "Open Failed.");

  //create a table
  SQLITEDB_BUS(ret, indDb, Exec, "CREATE TABLE Individual (Col3 int, Col4 varchar(255));", nullptr, nullptr, nullptr);
  AZ_Assert(ret == SQLITE_OK, "Create Table Failed.");

  //insert a row
  SQLITEDB_BUS(ret, indDb, Exec, "INSERT INTO Individual (Col3, Col4) VALUES(3,'FOURTH');", nullptr, nullptr, nullptr);
  AZ_Assert(ret == SQLITE_OK, "Insert Failed.");

  //insert a row
  SQLITEDB_BUS(ret, indDb, Exec, "INSERT INTO Individual (Col3, Col4) VALUES(4,'FIFTH');", nullptr, nullptr, nullptr);
  AZ_Assert(ret == SQLITE_OK, "Insert Failed.");

  //insert a row
  SQLITEDB_BUS(ret, indDb, Exec, "INSERT INTO Individual (Col3, Col4) VALUES(5,'SIXTH');", nullptr, nullptr, nullptr);
  AZ_Assert(ret == SQLITE_OK, "Insert Failed.");
  
  //select all from the table
  SQLITEDB_BUS(
    ret,
    sysDb,
    Exec,
    "SELECT * FROM Individual;",
    [](void* cbarg, int argc, char **argv, char **azColName) -> int {
      for (int i = 0; i < argc; i++) CryLog("%s = %s", azColName[i], argv[i] ? argv[i] : "NULL");
      return 0;
    },
    nullptr,
    nullptr
  );
  AZ_Assert(ret == SQLITE_OK, "Select Failed");
  
  //close the database connection
  SQLITEDB_BUS(ret, indDb, Close);
  AZ_Assert(ret == SQLITE_OK, "Close Failed.");

  //free up memory;
  delete indDb;
  ///////////////////////////////////////////////////

  return 0;
}
```

## Lua Syntax
The lua syntax is basicly a slight variation of the C syntax.  
For example, sqlite3_errcode becomes SQLite.ErrCode.  
Do note that some functions may have shorter names than the C++ version.  
Any C++ function that has Out parameters are multi returned in Lua starting with the C++ return then Out parameters.  
Constants and Enums are provided with variation.  SQLITE_*name* becomes SQLite.*name*.

## Lumberyard Specific Lua
The lua for lumberyard is the same as the regular lua syntax.  
Entities that use the SQLiteSystemComponent, such as the system entity, will have a function to retrieve the current database connection that is associated with it.  
To retrieve the database in the entity you use SQLiteBus.Event.GetConnection().  
Afterwards, any and all lua database functions will work with the sqlite database object.  
NOTE! DO NOT call Close on a database connection that is associated with an entity.  
The entity will automatically close it\'s database connection apon destruction.  
Calling Close on an entity database connection will return a SQLITE_MISUSE error.  
Calling any of the Open functions on an entity will automatically close and open the database connection with the appropriate sqlite open function that was requested.

## What is not currently implemented in Lua
Any function that has a parameter that is for a function, i.e. "void foo(void(\*)(void\*))".  
Any function that uses argument variation, i.e. "void foo(...)".  
Any function that returns or has parameters of void \*. (UTF-16 base functions use these.)  
Any function that has multidimentional array returns or parameters, i.e. "void foo(int \*\*mdarray)"  
SQLite contexts (i.e. sqlite3_context *) are not implemented.  
String functions.  
[Malloc Functions.](https://sqlite.org/c3ref/free.html) (For obvious reasons)

## Lua Classes
SQLite - sqlite3 \*  
SQLiteStmt - sqlite3_stmt \*  
SQLiteBackup - sqlite3_backup \*  
SQLiteValue - sqlite3_value \*  
SQLiteVFS - sqlite3_vfs \*  
SQLiteMutex - sqlite3_mutex \*  

## Lua Example
```Lua
Example = {
	Properties = {
	}
}

function Example:OnActivate()
	Debug.Log("Hello, world!")

	Debug.Log("SQLite LibVersion: "..SQLite.LibVersion())
	
	--test individual database connection
	Debug.Log("Individual DB Connection")
	sqlitedb = SQLite()
	sqlitedb:Open(":memory:")

	Debug.Log("Type: " .. type(sqlitedb))

	err, stmt, tail = sqlitedb:Prepare_v2("CREATE TABLE Individual (Col1 int, Col2 varchar(255));")
	stmt:Step()
	stmt:Finalize()
	err, stmt, tail = sqlitedb:Prepare_v2("INSERT INTO Individual (Col1, Col2) VALUES(0,'FIRST');")
	stmt:Step()
	stmt:Finalize()
	err, stmt, tail = sqlitedb:Prepare_v2("INSERT INTO Individual (Col1, Col2) VALUES(1,'SECOND');")
	stmt:Step()
	stmt:Finalize()
	err, stmt, tail = sqlitedb:Prepare_v2("INSERT INTO Individual (Col1, Col2) VALUES(2,'THIRD');")
	stmt:Step()
	stmt:Finalize()

	err, stmt, tail = sqlitedb:Prepare_v2("SELECT * FROM Individual;")
	while stmt:Step() == SQLite.ROW do
		first = stmt:Column_Int(0)
		second = stmt:Column_Text(1)

		firstCol = stmt:Column_Name(0)
		secondCol = stmt:Column_Name(1)

		Debug.Log(firstCol ..": " .. first .. " = " .. secondCol ..": " .. second)
	end
	stmt:Finalize()
	sqlitedb:Close()

	--test entity database connection
	Debug.Log("Entity DB Connection")
	sqliteEntdb = SQLiteBus.Event.GetConnection(self.entityId)

	Debug.Log("Type: " .. type(sqliteEntdb))

	if sqliteEntdb ~= nil then
		err, stmt, tail = sqliteEntdb:Prepare_v2("CREATE TABLE Entity (Col3 int, Col4 varchar(255));")
		stmt:Step()
		stmt:Finalize()
		err, stmt, tail = sqliteEntdb:Prepare_v2("INSERT INTO Entity (Col3, Col4) VALUES(0,'FIRST');")
		stmt:Step()
		stmt:Finalize()
		err, stmt, tail = sqliteEntdb:Prepare_v2("INSERT INTO Entity (Col3, Col4) VALUES(1,'SECOND');")
		stmt:Step()
		stmt:Finalize()
		err, stmt, tail = sqliteEntdb:Prepare_v2("INSERT INTO Entity (Col3, Col4) VALUES(2,'THIRD');")
		stmt:Step()
		stmt:Finalize()

		err, stmt, tail = sqliteEntdb:Prepare_v2("SELECT * FROM Entity;")
		while stmt:Step() == SQLite.ROW do
			first = stmt:Column_Int(0)
			second = stmt:Column_Text(1)

			firstCol = stmt:Column_Name(0)
			secondCol = stmt:Column_Name(1)

			Debug.Log(firstCol ..": " .. first .. " = " .. secondCol ..": " .. second)
		end
		stmt:Finalize()
	end

	--test system entity database connection
	Debug.Log("System DB Connection")
	sqliteSysdb = SQLiteLY.GetSysConnection(self.entityId)

	Debug.Log("Type: " .. type(sqliteSysdb))

	if sqliteSysdb ~= nil then
		err, stmt, tail = sqliteSysdb:Prepare_v2("CREATE TABLE System (Col5 int, Col6 varchar(255));")
		stmt:Step()
		stmt:Finalize()
		err, stmt, tail = sqliteSysdb:Prepare_v2("INSERT INTO System (Col5, Col6) VALUES(0,'FIRST');")
		stmt:Step()
		stmt:Finalize()
		err, stmt, tail = sqliteSysdb:Prepare_v2("INSERT INTO System (Col5, Col6) VALUES(1,'SECOND');")
		stmt:Step()
		stmt:Finalize()
		err, stmt, tail = sqliteSysdb:Prepare_v2("INSERT INTO System (Col5, Col6) VALUES(2,'THIRD');")
		stmt:Step()
		stmt:Finalize()

		err, stmt, tail = sqliteSysdb:Prepare_v2("SELECT * FROM System;")
		while stmt:Step() == SQLite.ROW do
			first = stmt:Column_Int(0)
			second = stmt:Column_Text(1)

			firstCol = stmt:Column_Name(0)
			secondCol = stmt:Column_Name(1)

			Debug.Log(firstCol ..": " .. first .. " = " .. secondCol ..": " .. second)
		end
		stmt:Finalize()
	end
end

function Example:OnDeactivate()
end

return Example
```
