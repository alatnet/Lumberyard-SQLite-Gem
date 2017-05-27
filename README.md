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
String functions.  
[Malloc Functions.](https://sqlite.org/c3ref/free.html) (For obvious reasons)

## Lua Syntax
The lua syntax is basicly a slight variation of the C++ syntax.  
For example, sqlite3_errcode becomes SQLite.ErrCode.  
Do note that some functions may have shorter names than the C++ version.  
Any C++ function that has Out parameters are multi returned in Lua starting with the C++ return then Out parameters.  
Constants and Enums are provided with variation.  SQLITE_*name* becomes SQLite.*name*.

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

	Debug.Log("LibVersion: "..SQLite.LibVersion())
	
	sqlitedb = SQLite()
	sqlitedb:Open(":memory:")

	err, stmt, tail = sqlitedb:Prepare_v2("CREATE TABLE Test (Col1 int, Col2 varchar(255));")
	stmt:Step()
	stmt:Finalize()
	
	err, stmt, tail = sqlitedb:Prepare_v2("INSERT INTO Test (Col1, Col2) VALUES(0,'FIRST');")
	stmt:Step()
	stmt:Finalize()

	err, stmt, tail = sqlitedb:Prepare_v2("INSERT INTO Test (Col1, Col2) VALUES(1,'SECOND');")
	stmt:Step()
	stmt:Finalize()

	err, stmt, tail = sqlitedb:Prepare_v2("INSERT INTO Test (Col1, Col2) VALUES(2,'THIRD');")
	stmt:Step()
	stmt:Finalize()

	err, stmt, tail = sqlitedb:Prepare_v2("SELECT * FROM Test;")

	while stmt:Step() == SQLite.ROW do
		first = stmt:Column_Int(0)
		second = stmt:Column_Text(1)

		firstCol = stmt:Column_Name(0)
		secondCol = stmt:Column_Name(1)

		Debug.Log(firstCol ..": " .. first .. " = " .. secondCol ..": " .. second)
	end

	stmt:Finalize()

	sqlitedb:Close()
end

function Example:OnDeactivate()
end

return Example
```
