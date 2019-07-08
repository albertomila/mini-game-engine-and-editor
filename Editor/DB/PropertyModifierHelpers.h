#pragma once

#include "Editor/DB/DB.h"
#include "Game/Core/Math.h"
#include "Game/Core/TypeToString.h"
#include "Game/DB/DBManager.h"
#include "Game/DB/DBObject.h"
#include "Game/DB/DBObjectDescriptor.h"
#include "Game/Serialization/Member.h"
#include "Game\Core\Path.h"
#include "Game\Entity\EntityComponent.h"
#include <string>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/propgrid/propgrid.h>
#include <wx/sizer.h>
#include "wx/propgrid/property.h"
#include "wx/propgrid/editors.h"
#include "wx/string.h"

template< class T >
class CGenericVisitor : public ITypeVisitor
{
public:
    CGenericVisitor(){}
    virtual ~CGenericVisitor(){}
    virtual void Visit( void* value )
    {
        m_value = static_cast<T*>( value );
    }

    T* m_value;
};

class SPropertyIDHelpers
{
public:
    static U32 GetNextPropertyID();
    static std::string GetPropertyName( U32 propertyID );
    static std::string GetNextPropertyName();

private:
    static U32 ms_nextPropertyID;
};

class IPropertyModifier
{
public:
    IPropertyModifier();
    ~IPropertyModifier();

    virtual ITypeVisitor*   GetTypeVisitor() const = 0;
    virtual void ApplyPropertyToMember() = 0;
    virtual void ApplyMemberToProperty() = 0;
    virtual wxPGProperty* GetProperty() const = 0;
    U32 GetPropertyID() const{ return m_propertyID; } 
    std::string GetPropertyName() const;

private:
    U32 m_propertyID;
};
