# Lumberyard-SQLite-Gem
An Amazon Lumberyard Gem that adds in SQLite functionality.

## Requirements
To compile the gem, it requires that you have downloaded SQLite from [sqlite.org](sqlite.org).  
Download the amalgamation version to be able to compile the gem.  
sqlite3.c needs to be placed in Code/Source.  
sqlite3.h needs to be placed in Code/Include/SQLite.

## What is not currently implemented in Lua
Any function that has a parameter that is for a function, i.e. "void foo(void(\*)(void\*))".  
Any function that uses argument variation, i.e. "void foo(...)".  
Any function that returns or has parameters of void \*. (UTF-16 base functions use these.)  
Any function that has multidimentional array returns or parameters, i.e. "void foo(int \*\*mdarray)"  
SQLite contexts (i.e. sqlite3_context *) are not implemented.  
String functions.  
[Malloc Functions.](https://sqlite.org/c3ref/free.html) (For obvious reasons)

## Lua Syntax
The lua syntax is basicly a slight variation of the C++ syntax.  
For example, sqlite3_errcode becomes SQLite.ErrCode.  
Do note that some functions may have shorter names than the C++ version.  
Any C++ function that has Out parameters are multi returned in Lua starting with the C++ return then Out parameters.  
Constants and Enums are provided with variation.  SQLITE_*name* becomes SQLite.*name*.

## Lumberyard Specific Lua
The lua for lumberyard is the same as the regular lua syntax.  
Entities that use the SQLiteSystemComponent, such as the system entity, will have a function to retrieve the current database connection that is associated with it.  
To retrieve the database in the entity you use SQLiteLY.Event.GetConnection().  
Afterwards, any and all lua database functions will work with the sqlite database object.  
NOTE! DO NOT call Close on a database connection that is associated with an entity.  
The entity will automatically close it\'s database connection apon destruction.  
Calling Close or Close_v2 on an entity database connection will return a SQLITE_MISUSE error.  
Calling any of the Open functions on an entity will automatically close and open the database connection with the appropriate sqlite open function that was requested.

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
	sqliteEntdb = SQLiteLY.Event.GetConnection(self.entityId)

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
end

function Example:OnDeactivate()
end

return Example
```
