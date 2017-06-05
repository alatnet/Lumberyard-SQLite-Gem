
#pragma once

#include <AzCore/Component/Component.h>

#include <AzCore/Script/ScriptContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>

#include <AzCore\RTTI\TypeInfo.h>

#include "SQLite\SQLiteClasses.h"
#include "InternalFunctions.h"

namespace SQLite3 {
	////////////////////////////////////////////////////////////////////////
	//SQLiteComponent class
	class SQLiteComponent
		: public AZ::Component
	{
	public:
		AZ_COMPONENT(SQLiteComponent, "{110B7042-C6F2-48B8-9EAE-CF46A0F2A4FB}");

		static void Reflect(AZ::ReflectContext* context);

		static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
		static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
		static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
		static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);
	public:
	protected:
		////////////////////////////////////////////////////////////////////////
		// AZ::Component interface implementation
		void Init() override;
		void Activate() override;
		void Deactivate() override;
		////////////////////////////////////////////////////////////////////////
	};
	////////////////////////////////////////////////////////////////////////
}