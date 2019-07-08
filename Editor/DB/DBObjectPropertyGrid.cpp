#include "Editor_PCH.h"
#include "Editor/DB/DBObjectPropertyGrid.h"

#include "Editor/DB/PropertyModifierHelpers.h"
#include "Editor/DB/PropertyModifierBool.h"
#include "Editor/DB/PropertyModifierComponent.h"
#include "Editor/DB/PropertyModifierDBObject.h"
#include "Editor/DB/PropertyModifierDBPointer.h"
#include "Editor/DB/PropertyModifierEnum.h"
#include "Editor/DB/PropertyModifierFile.h"
#include "Editor/DB/PropertyModifierFloatGeneric.h"
#include "Editor/DB/PropertyModifierGroup.h"
#include "Editor/DB/PropertyModifierIntGeneric.h"
#include "Editor/DB/PropertyModifierString.h"
#include "Editor/DB/PropertyModifierVector.h"
#include "Editor/DB/PropertyModifierVectorElement.h"
#include "Editor/DB/PropertyModifierVector2.h"
#include "Editor/DB/PropertyModifierVector3.h"
#include "Editor/Main/GUITypes.h"
#include "Game/Core/Math.h"
#include "Game/DB/DBManager.h"
#include "Game/entity/Entity.h"
#include "wx/string.h"
#include <winbase.h>
#include <wx/panel.h>
#include <wx/propgrid/propgrid.h>
#include <wx/sizer.h>
#include "wx/propgrid/property.h"

CCallback2<DBObjectPtrPropertyModifier*,CStringID> DbPropertyGrid::ms_onLibrarySelected;

BEGIN_EVENT_TABLE(DbPropertyGrid, wxDialog )
    EVT_PG_SELECTED(ListboxPage_Listbox, DbPropertyGrid::OnPropertySelected)
    EVT_PG_RIGHT_CLICK(ListboxPage_Listbox, DbPropertyGrid::OnPropertyRightButton)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////
DbPropertyGrid::DbPropertyGrid()
: m_parentPanel( NULL )
, m_parentSizer( NULL )
, m_objectsPropertyGrid( NULL )
{
}

///////////////////////////////////////////////////////////////////////////
DbPropertyGrid::~DbPropertyGrid()
{
    // Disconnect Events
   // m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbPropertyGrid::OnPropertySaveButtonClick ), NULL, this );
   // m_objectsPropertyGrid->Disconnect( wxEVT_PG_SELECTED, wxPropertyGridEventHandler( DbPropertyGrid::OnPropertySelected ), NULL, this );
}

struct STreeData : public wxObjectRefData
{
    STreeData()
        : m_property( NULL )
    {
    }

    IPropertyModifier* m_property;
};

struct VectorPropertyRefData : public STreeData
{
    VectorPropertyRefData()
        : m_vectorPropertyModifier( NULL )
    {

    }
    VectorPropertyModifier* m_vectorPropertyModifier;
};

struct VectorElementPropertyRefData : public STreeData
{
    VectorElementPropertyRefData()
        : m_vectorPropertyModifier( NULL )
        , m_vectorElement( NULL )
    {

    }

    VectorPropertyModifier* m_vectorPropertyModifier;
    CDbObject* m_vectorElement;
};


///////////////////////////////////////////////////////////////////////////
void DbPropertyGrid::OnPropertySelected(wxPropertyGridEvent& event)
{
    wxPGProperty* propertyWx = event.GetProperty();

    if (propertyWx)
    {
        wxObjectRefData* refData = propertyWx->GetRefData();
        STreeData* data = static_cast< STreeData* >( refData );
        if( data )
        {
            DBObjectPtrPropertyModifier* stringProperty = dynamic_cast< DBObjectPtrPropertyModifier*>( data->m_property );
            if( stringProperty )
            {
                CStringID libraryID = stringProperty->GetTypeVisitor()->m_libraryID;
                ms_onLibrarySelected.Call( stringProperty, libraryID );
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////
void DbPropertyGrid::OnPropertyRightButton(wxPropertyGridEvent& event)
{
    wxPGProperty* propertyGridValue = event.GetProperty();
    if( propertyGridValue )
    {
        wxPropertyCategory* propertyCategory = dynamic_cast< wxPropertyCategory* >( propertyGridValue );
        if( propertyCategory )
        {
            wxObjectRefData* refData =  propertyCategory->GetRefData();

            //delete entry
            VectorElementPropertyRefData* vectorElementPropertyRefData = dynamic_cast<VectorElementPropertyRefData*>( refData );
            if( vectorElementPropertyRefData )
            {   
                CDbObject* dbObject = vectorElementPropertyRefData->m_vectorElement;
                VectorPropertyModifier* vectorPropertyModifier = vectorElementPropertyRefData->m_vectorPropertyModifier;
                S32 index = vectorPropertyModifier->RemoveEntry( dbObject );

                vectorPropertyModifier->GetProperty()->DeleteChildren();

                std::vector<CDbObject*>& dbObjectList = vectorPropertyModifier->GetDBObjectList();
                FOR_EACH( std::vector<CDbObject*>, it, dbObjectList )
                {
                    CDbObject* dbObject = *it;
                    VectorElementPropertyModifier* vectorElementPropertyCategory = new VectorElementPropertyModifier( "element" );
                    vectorPropertyModifier->GetProperty()->AppendChild( vectorElementPropertyCategory->GetProperty() );

                    VectorElementPropertyRefData* data = new VectorElementPropertyRefData();
                    data->m_property = vectorElementPropertyCategory;
                    data->m_vectorElement = dbObject;
                    data->m_vectorPropertyModifier = vectorPropertyModifier;
                    vectorElementPropertyCategory->GetProperty()->SetRefData( data );

                    ComposePropertyPanel( dbObject, vectorElementPropertyCategory->GetProperty() );
                }

                return;
            }

            //add entry
            VectorPropertyRefData* vectorPropertyRefData = dynamic_cast<VectorPropertyRefData*>( refData );
            if( vectorPropertyRefData )
            {
                VectorPropertyModifier* vectorPropertyModifier = vectorPropertyRefData->m_vectorPropertyModifier;
                if( vectorPropertyModifier )
                {
                    vectorPropertyModifier->AddNewEntry();
                    std::vector<CDbObject*>& dbObjectList = vectorPropertyModifier->GetDBObjectList();
                    CDbObject* dbObject = *(dbObjectList.end() - 1 );
                    if( dbObject )
                    {
                        VectorElementPropertyModifier* vectorElementPropertyCategory = new VectorElementPropertyModifier( "element" );
                        vectorPropertyModifier->GetProperty()->AppendChild( vectorElementPropertyCategory->GetProperty() );

                        VectorElementPropertyRefData* data = new VectorElementPropertyRefData();
                        data->m_property = vectorElementPropertyCategory;
                        data->m_vectorElement = dbObject;
                        data->m_vectorPropertyModifier = vectorPropertyModifier;
                        vectorElementPropertyCategory->GetProperty()->SetRefData( data );

                        ComposePropertyPanel( dbObject, vectorElementPropertyCategory->GetProperty() );
                    }
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////
void DbPropertyGrid::Init_Begin( wxPanel* parentPanel, wxBoxSizer* parentSizer )
{
    m_parentSizer = parentSizer;
    m_parentPanel = parentPanel;

    m_currentPanel = new wxPanel( parentPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    m_currentSizer = new wxBoxSizer( wxVERTICAL );

    //objects property grid
    // Construct wxPropertyGrid control
    m_objectsPropertyGrid = new wxPropertyGrid(   m_currentPanel// parent
                                                , wxID_ANY // id
                                                , wxDefaultPosition // position
                                                , wxSize( 550,250 ) // size
                                                , wxPG_AUTO_SORT  // Automatic sorting after items added
                                                | wxPG_SPLITTER_AUTO_CENTER  // Automatically center splitter until user manually adjusts it
                                                | wxPG_DEFAULT_STYLE );// Default style

    m_objectsPropertyGrid->Connect( wxEVT_PG_SELECTED, wxPropertyGridEventHandler( DbPropertyGrid::OnPropertySelected ), NULL, this );
    m_objectsPropertyGrid->Connect( wxEVT_PG_RIGHT_CLICK, wxPropertyGridEventHandler( DbPropertyGrid::OnPropertyRightButton ), NULL, this );

    m_currentSizer->Add( m_objectsPropertyGrid, 0, wxALL, 5 );

}

///////////////////////////////////////////////////////////////////////////
void DbPropertyGrid::Init_End( wxPanel* parentPanel, wxBoxSizer* parentSizer )
{
    //change bool dropdown list by checkbox
    m_objectsPropertyGrid->SetPropertyAttributeAll(wxPG_BOOL_USE_CHECKBOX,true);
    m_currentPanel->SetSizer( m_currentSizer );
    m_currentPanel->Layout();
    m_currentSizer->Fit( m_currentPanel );

    m_parentSizer->Add( m_currentPanel, 1, wxEXPAND | wxALL, 5 );
}

///////////////////////////////////////////////////////////////////////////
void DbPropertyGrid::Init( wxPanel* parentPanel, wxBoxSizer* parentSizer, CDbObject* dbObject )
{
    Init_Begin( parentPanel, parentSizer );
    ComposePropertyPanel( dbObject );
    Init_End( parentPanel, parentSizer );
}

///////////////////////////////////////////////////////////////////////////
void DbPropertyGrid::Init( wxPanel* parentPanel, wxBoxSizer* parentSizer, CEntity* entity )
{
    Init_Begin( parentPanel, parentSizer );
    ComposePropertyPanel( entity );
    Init_End( parentPanel, parentSizer );
}

///////////////////////////////////////////////////////////////////////////
void DbPropertyGrid::Shutdown()
{
    //Disconnect Events
    m_objectsPropertyGrid->Disconnect( wxEVT_PG_SELECTED, wxPropertyGridEventHandler( DbPropertyGrid::OnPropertySelected ), NULL, this );
    m_objectsPropertyGrid->Disconnect( wxEVT_PG_SELECTED, wxPropertyGridEventHandler( DbPropertyGrid::OnPropertyRightButton ), NULL, this );

    m_currentSizer->Clear( true );
    m_currentPanel->SetSizer( NULL, true );
    m_currentPanel->DestroyChildren();
    m_parentPanel->RemoveChild( m_currentPanel );

    delete m_currentPanel;
    m_currentPanel = NULL;
    m_currentSizer = NULL;
}


///////////////////////////////////////////////////////////////////////////
void DbPropertyGrid::ComposePropertyPanel( CDbObject* dbObjectNonConst, wxPropertyCategory* parentCategory )
{
    const CDbObjectDescriptor& dbDescriptor = dbObjectNonConst->GetDescriptor();

    const CDbObjectDescriptor::TMemberList& memberList = dbDescriptor.GetMemberList();

    ComposePropertyPanel( dbObjectNonConst, memberList, parentCategory );
}

///////////////////////////////////////////////////////////////////////////
void DbPropertyGrid::ComposePropertyPanel( CEntity* entity )
{
	//Special case for entity name
	StringPropertyModifier* namePropertyModifier = new StringPropertyModifier(" Entity name");
	STreeData* data = new STreeData();
	data->m_property = namePropertyModifier;
	namePropertyModifier->GetProperty()->SetRefData(data);
	namePropertyModifier->GetTypeVisitor()->Visit(&entity->GetNameRef());
	namePropertyModifier->ApplyMemberToProperty();
	m_objectsPropertyGrid->Append(namePropertyModifier->GetProperty());

	//Components
    const CEntity::TComponentMap& components = entity->GetComponents();
    FOR_EACH_CONST( CEntity::TComponentMap, it, components )
    {
        const IEntityComponent* component = it->second;
        const CDbObject* dbObjectConst = component;
        CDbObject* dbObject = const_cast< CDbObject* >( dbObjectConst );

        ComponentPropertyModifier* groupPropertyModifier = new ComponentPropertyModifier( component,  wxString(component->GetName()) );

        STreeData* data = new STreeData();
        data->m_property = groupPropertyModifier;
        groupPropertyModifier->GetProperty()->SetRefData( data );

        m_objectsPropertyGrid->Append( groupPropertyModifier->GetProperty() );

        wxPropertyCategory* propertyCategory = groupPropertyModifier->GetProperty();
        ComposePropertyPanel( dbObject, groupPropertyModifier->GetMemberList(), propertyCategory );
        groupPropertyModifier->ApplyMemberToProperty();
    }
}

///////////////////////////////////////////////////////////////////////////
void DbPropertyGrid::Save()
{
    wxPGProperty* mainProperty = m_objectsPropertyGrid->GetRoot();
    for( U32 i = 0; i < mainProperty->GetChildCount(); ++i )
    {
        wxPGProperty* currentProperty = mainProperty->Item( i );
        if( currentProperty )
        {
            //OutputDebugString( L"main" );
            //OutputDebugString( currentProperty->GetLabel().c_str() );
            //OutputDebugString( L"\n" );
            wxObjectRefData* refData = currentProperty->GetRefData();
            if( STreeData* data = static_cast< STreeData* >( refData ) )
            {
                IPropertyModifier* propertyModifier = data->m_property;
                ParsePropertyChildren( propertyModifier );
                propertyModifier->ApplyPropertyToMember();
                //No need to iterate children of this property, wxPG_ITERATE_ALL has all the children.
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////
void DbPropertyGrid::ComposePropertyPanel( CDbObject* dbObjectNonConst, const CDbObjectDescriptor::TMemberList& memberList, wxPropertyCategory* parentCategory )
{
    FOR_EACH_CONST( CDbObjectDescriptor::TMemberList, it, memberList )
    {
        IPropertyModifier* propertyModifier = NULL;
        CStringID::IDType memberTypeID = (*it)->GetMemberTypeID();
        
        bool added( false );

        switch( memberTypeID )
        {
            case MemberToTypeID<U8>::value :         { propertyModifier = new IntPropertyModifier<U8>( wxString((*it)->GetName()) ); } break; 
            case MemberToTypeID<U16>::value :        { propertyModifier = new IntPropertyModifier<U16>(  wxString((*it)->GetName()) ); } break;  
            case MemberToTypeID<U32>::value :        { propertyModifier = new IntPropertyModifier<U32>(  wxString((*it)->GetName()) ); } break; 
            case MemberToTypeID<U64>::value :        { propertyModifier = new IntPropertyModifier<U64>(  wxString((*it)->GetName()) ); } break; 
            case MemberToTypeID<S8>::value :         { propertyModifier = new IntPropertyModifier<S8>(  wxString((*it)->GetName()) ); } break; 
            case MemberToTypeID<S16>::value :        { propertyModifier = new IntPropertyModifier<S16>(  wxString((*it)->GetName()) ); } break;  
            case MemberToTypeID<S32>::value :        { propertyModifier = new IntPropertyModifier<S32>(  wxString((*it)->GetName()) ); } break; 
            case MemberToTypeID<S64>::value :        { propertyModifier = new IntPropertyModifier<S64>(  wxString((*it)->GetName()) ); } break; 

            case MemberToTypeID<bool>::value:        { propertyModifier = new BoolPropertyModifier(  wxString((*it)->GetName()) ); } break; 
            case MemberToTypeID<float>::value :      { propertyModifier = new FloatPropertyModifier<float>(  wxString((*it)->GetName()) ); } break; 
            case MemberToTypeID<double>::value :     { propertyModifier = new FloatPropertyModifier<double>(  wxString((*it)->GetName()) ); } break; 

            case MemberToTypeID<CDBFile>::value :   { propertyModifier = new FilePropertyModifier(  wxString((*it)->GetName()) ); } break; 

            case MemberToTypeID<std::string>::value :
				{
					propertyModifier = new StringPropertyModifier(  wxString((*it)->GetName()) ); 
				} 
				break; 
            case MemberToTypeID<MemberTypeIDEnum>::value :{ propertyModifier = new EnumPropertyModifier( memberList, *it,  wxString((*it)->GetName()) ); } break; 

            case MemberToTypeID<vector3>::value :
                { 
                    Vector3PropertyModifier* vect3PropertyModifier = new Vector3PropertyModifier( wxString((*it)->GetName()) );
                    propertyModifier = vect3PropertyModifier;

                    STreeData* data = new STreeData();
                    data->m_property = propertyModifier;
                    propertyModifier->GetProperty()->SetRefData( data );

                    if( parentCategory )
                    {
                        parentCategory->AppendChild( propertyModifier->GetProperty() );
                    }
                    else
                    {
                        m_objectsPropertyGrid->Append( propertyModifier->GetProperty() );
                    }
                    
                    wxPropertyCategory* propertyCategory = vect3PropertyModifier->GetProperty();
                    propertyCategory->AppendChild( vect3PropertyModifier->GetPropertyX() );
                    propertyCategory->AppendChild( vect3PropertyModifier->GetPropertyY() );
                    propertyCategory->AppendChild( vect3PropertyModifier->GetPropertyZ() );
                    
                    //m_memberReferenceList.push_back( propertyModifier );
                    (*it)->Visit( dbObjectNonConst, propertyModifier->GetTypeVisitor() );
                    propertyModifier->ApplyMemberToProperty();
                    added = true;
                } 
                break; 

            case MemberToTypeID<vector2>::value :
                { 
                    Vector3PropertyModifier* vect3PropertyModifier = new Vector3PropertyModifier( wxString((*it)->GetName()) );
                    propertyModifier = vect3PropertyModifier;

                    STreeData* data = new STreeData();
                    data->m_property = propertyModifier;
                    propertyModifier->GetProperty()->SetRefData( data );

                    if( parentCategory )
                    {
                        parentCategory->AppendChild( propertyModifier->GetProperty() );
                    }
                    else
                    {
                        m_objectsPropertyGrid->Append( propertyModifier->GetProperty() );
                    }

                    wxPropertyCategory* propertyCategory = vect3PropertyModifier->GetProperty();
                    propertyCategory->AppendChild( vect3PropertyModifier->GetPropertyX() );
                    propertyCategory->AppendChild( vect3PropertyModifier->GetPropertyY() );

                    //m_memberReferenceList.push_back( propertyModifier );
                    (*it)->Visit( dbObjectNonConst, propertyModifier->GetTypeVisitor() );
                    propertyModifier->ApplyMemberToProperty();
                    added = true;
                } 
                break; 

            case MemberToTypeID<MemberTypeIDGroup>::value :
                { 
                    GroupPropertyModifier* groupPropertyModifier = new GroupPropertyModifier( *it,  wxString((*it)->GetName()) );

                    propertyModifier = groupPropertyModifier;

                    STreeData* data = new STreeData();
                    data->m_property = propertyModifier;
                    propertyModifier->GetProperty()->SetRefData( data );

                    if( parentCategory )
                    {
                        parentCategory->AppendChild( propertyModifier->GetProperty() );
                    }
                    else
                    {
                        m_objectsPropertyGrid->Append( propertyModifier->GetProperty() );
                    }

                    wxPropertyCategory* propertyCategory = groupPropertyModifier->GetProperty();
                    //m_memberReferenceList.push_back( propertyModifier );
                    (*it)->Visit( dbObjectNonConst, propertyModifier->GetTypeVisitor() );
                    ComposePropertyPanel( dbObjectNonConst, groupPropertyModifier->GetMemberList(), propertyCategory );
                    propertyModifier->ApplyMemberToProperty();
                    
                    added = true;
                } 
                break; 
            case MemberToTypeID<MemberTypeDBObject>::value :
                { 
                    DBObjectPropertyModifier* dbObjectPropertyModifier = new DBObjectPropertyModifier( wxString((*it)->GetName()) );

                    propertyModifier = dbObjectPropertyModifier;

                    STreeData* data = new STreeData();
                    data->m_property = propertyModifier;
                    propertyModifier->GetProperty()->SetRefData( data );

                    if( parentCategory )
                    {
                        parentCategory->AppendChild( propertyModifier->GetProperty() );
                    }
                    else
                    {
                        m_objectsPropertyGrid->Append( propertyModifier->GetProperty() );
                    }

                    wxPropertyCategory* propertyCategory = dbObjectPropertyModifier->GetProperty();
                    //m_memberReferenceList.push_back( propertyModifier );
                    (*it)->Visit( dbObjectNonConst, propertyModifier->GetTypeVisitor() );
                    ComposePropertyPanel( dbObjectPropertyModifier->GetDBObject(), propertyCategory );
                    propertyModifier->ApplyMemberToProperty();

                    added = true;
                } 
                break;  
            case MemberToTypeID<MemberTypeDBObjectPtr>::value :
                { 
                    propertyModifier =  new DBObjectPtrPropertyModifier( wxString((*it)->GetName()) );
                } 
                    break;     
            case MemberToTypeID<MemberTypeVector>::value :
                { 
                    VectorPropertyModifier* vectorPropertyModifier =  new VectorPropertyModifier( wxString((*it)->GetName()) );

                    propertyModifier = vectorPropertyModifier;

                    VectorPropertyRefData* data = new VectorPropertyRefData();
                    data->m_property = vectorPropertyModifier;
                    data->m_vectorPropertyModifier = vectorPropertyModifier;
                    propertyModifier->GetProperty()->SetRefData( data );

                    if( parentCategory )
                    {
                        parentCategory->AppendChild( propertyModifier->GetProperty() );
                    }
                    else
                    {
                        m_objectsPropertyGrid->Append( propertyModifier->GetProperty());
                    }


                    //wxPropertyCategory* propertyCategory = vectorPropertyModifier->GetProperty();
                   // m_memberReferenceList.push_back( vectorPropertyModifier );
                    (*it)->Visit( dbObjectNonConst, vectorPropertyModifier->GetTypeVisitor() );
                    vectorPropertyModifier->ApplyMemberToProperty();
                    std::vector<CDbObject*>& dbObjectList = vectorPropertyModifier->GetDBObjectList();
                    FOR_EACH( std::vector<CDbObject*>, it, dbObjectList )
                    {
                        CDbObject* dbObject = *it;
                        VectorElementPropertyModifier* vectorElementPropertyCategory = new VectorElementPropertyModifier( "element" );
                        
                        VectorElementPropertyRefData* data = new VectorElementPropertyRefData();
                        data->m_property = vectorElementPropertyCategory;
                        data->m_vectorPropertyModifier = vectorPropertyModifier;
                        data->m_vectorElement = dbObject;
                        vectorElementPropertyCategory->GetProperty()->SetRefData( data );
                        
                        propertyModifier->GetProperty()->AppendChild( vectorElementPropertyCategory->GetProperty() );

                        ComposePropertyPanel( dbObject, vectorElementPropertyCategory->GetProperty() );
                    }

                    added = true;
                }
                    break;         
            default:
                continue;
                break;
        }
        
        if( propertyModifier )
        {
            if( !added )
            {
                STreeData* data = new STreeData();
                data->m_property = propertyModifier;
                propertyModifier->GetProperty()->SetRefData( data );

                if( parentCategory )
                {
                    parentCategory->AppendChild( propertyModifier->GetProperty() );
                }
                else
                {
                    m_objectsPropertyGrid->Append( propertyModifier->GetProperty() );
                }

                //m_memberReferenceList.push_back( propertyModifier );
                (*it)->Visit( dbObjectNonConst, propertyModifier->GetTypeVisitor() );
                propertyModifier->ApplyMemberToProperty();
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////
void DbPropertyGrid::ParsePropertyChildren( IPropertyModifier* parentProperty )
{
    if( wxPropertyCategory* groupPropertyModifier = dynamic_cast<wxPropertyCategory*>( parentProperty->GetProperty() ) )
    {
        for( U32 i = 0; i < groupPropertyModifier->GetChildCount(); ++i )
        {
            wxPGProperty* currentProperty = groupPropertyModifier->Item( i );
            if( currentProperty )
            {
                //OutputDebugString( currentProperty->GetLabel().c_str() );
                //OutputDebugString( L"\n" );
                wxObjectRefData* refData = currentProperty->GetRefData();
                if( STreeData* data = static_cast< STreeData* >( refData ) )
                {
                    IPropertyModifier* propertyModifier = data->m_property;
                    if( propertyModifier )
                    {
                        ParsePropertyChildren( propertyModifier );
                        propertyModifier->ApplyPropertyToMember();
                    }
                }
            }
        }
    }
}