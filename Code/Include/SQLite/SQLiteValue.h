#pragma once

#include <AzCore\RTTI\TypeInfo.h>
#include <AzCore/RTTI/BehaviorContext.h>

namespace SQLite3 {
	class SQLiteValue;

	class SQLiteValueRequests : public AZ::EBusTraits {
	public:
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;

		using BusIdType = SQLiteValue *;
	public:
		virtual const void *Blob() = 0;
		virtual int Bytes() = 0;
		virtual int Bytes16() = 0;
		virtual double Double() = 0;
		virtual int Int() = 0;
		virtual __int64 Int64() = 0;
		virtual const char* Text() = 0;
		virtual const void* Text16() = 0;
		virtual const void* Text16LE() = 0;
		virtual const void* Text16BE() = 0;
		virtual int Type() = 0;
		virtual int Numeric_Type() = 0;
		virtual SQLiteValue * Value_Dup() = 0;
		virtual void Free() = 0;
		virtual unsigned int Subtype() = 0;
	};
	using SQLiteValueBus = AZ::EBus<SQLiteValueRequests>;

	////////////////////////////////////////////////////////////////////////
	//SQLiteValue Class
	class SQLiteValue : public SQLiteValueBus::Handler {
	public:
		AZ_TYPE_INFO(SQLiteValue, "{757E4464-5909-406E-BD10-430D2B18BEA0}");
	public:
		SQLiteValue();
		SQLiteValue(sqlite3_value * val);
		~SQLiteValue();
	public:
		const void *Blob();
		int Bytes();
		int Bytes16();
		double Double();
		int Int();
		__int64 Int64();
		const char* Text();
		const void* Text16();
		const void* Text16LE();
		const void* Text16BE();
		int Type();
		int Numeric_Type();
		SQLiteValue * Value_Dup();
		void Free();
		unsigned int Subtype();
	public:
		sqlite3_value * m_pVal;
	public:
		static void RegisterBehaviorContext(AZ::BehaviorContext* bc);
	private:
		bool m_avail;
	};
	////////////////////////////////////////////////////////////////////////
}