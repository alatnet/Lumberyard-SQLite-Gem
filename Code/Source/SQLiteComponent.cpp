
#include "StdAfx.h"
#include "SQLiteComponent.h"

namespace SQLite3 {
	////////////////////////////////////////////////////////////////////////
	// Component Implementation
	void SQLiteComponent::Reflect(AZ::ReflectContext* context) {
		if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context)) {
			serialize->Class<SQLiteComponent, AZ::Component>()
				->SerializerForEmptyClass();

			/*if (AZ::EditContext* ec = serialize->GetEditContext()) {
				ec->Class<SQLiteComponent>("SQLite3", "SQLite Lua database component.")
					->ClassElement(AZ::Edit::ClassElements::EditorData, "")
					->Attribute(AZ::Edit::Attributes::Category, "Database")
					->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System"))
					->Attribute(AZ::Edit::Attributes::AutoExpand, true);
			}*/
		}

		AZ::BehaviorContext* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context);
		if (behaviorContext) {
			behaviorContext->Class<SQLiteComponent>("SQLite3")
				->Constant("TEXT", []() -> int { return SQLITE3_TEXT; });

			SQLiteDB::RegisterBehaviorContext(behaviorContext);
			SQLiteStmt::RegisterBehaviorContext(behaviorContext);
			SQLiteBackup::RegisterBehaviorContext(behaviorContext);
			SQLiteValue::RegisterBehaviorContext(behaviorContext);
			SQLiteVFS::RegisterBehaviorContext(behaviorContext);
			SQLiteMutex::RegisterBehaviorContext(behaviorContext);
		}
	}

	void SQLiteComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided) {
		provided.push_back(AZ_CRC("SQLite3Service"));
	}

	void SQLiteComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible) {
		incompatible.push_back(AZ_CRC("SQLite3Service"));
	}

	void SQLiteComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required) {
		(void)required;
	}

	void SQLiteComponent::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent) {
		//(void)dependent;
		dependent.push_back(AZ_CRC("SQLiteLYService"));
	}

	void SQLiteComponent::Init() {
	}

	void SQLiteComponent::Activate() {
	}

	void SQLiteComponent::Deactivate() {
	}
	////////////////////////////////////////////////////////////////////////
}