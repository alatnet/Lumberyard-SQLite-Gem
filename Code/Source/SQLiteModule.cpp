
#include "StdAfx.h"
#include <platform_impl.h>

#include "SQLiteSystemComponent.h"
//#include "SQLiteComponent.h"

#include <IGem.h>

namespace SQLite
{
    class SQLiteModule
        : public CryHooksModule
    {
    public:
        AZ_RTTI(SQLiteModule, "{916735A1-D2F1-4AFF-84A0-5FCAA2F67B33}", CryHooksModule);

        SQLiteModule()
            : CryHooksModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            m_descriptors.insert(m_descriptors.end(), {
				SQLiteSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
				//azrtti_typeid<SQLite::SQLiteComponent>(),
				azrtti_typeid<SQLiteSystemComponent>(),
            };
        }
    };
}

// DO NOT MODIFY THIS LINE UNLESS YOU RENAME THE GEM
// The first parameter should be GemName_GemIdLower
// The second should be the fully qualified name of the class above
AZ_DECLARE_MODULE_CLASS(SQLite_4b76424b22de476497488bc53b6c88b5, SQLite::SQLiteModule)
