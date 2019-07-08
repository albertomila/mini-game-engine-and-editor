#include "Game_PCH.h"
#include "Game/Entity/Entity.h"
#include "Game/Entity/EntityComponent.h"

///////////////////////////////////////////////////////////////////////////
CEntity::CEntity()
{
    m_matrix.identity();
}

///////////////////////////////////////////////////////////////////////////
CEntity::~CEntity()
{
    DeleteMap( m_components );
}

///////////////////////////////////////////////////////////////////////////
IEntityComponent* CEntity::GetComponent( CStringID componentID ) const
{
    TComponentMap::const_iterator itFind = m_components.find( componentID.GetUniqueID() );
    if( itFind != m_components.end() )
    {
        return itFind->second;
    }

    return NULL;
}

///////////////////////////////////////////////////////////////////////////
void CEntity::SetPos( const vector3& val )
{ 
    cml::matrix_set_translation( m_matrix, val );
}

///////////////////////////////////////////////////////////////////////////
vector3 CEntity::GetPos() const 
{ 
    return cml::matrix_get_translation( m_matrix );
}

///////////////////////////////////////////////////////////////////////////
void CEntity::SetRotation( float angle )
{ 
  //  cml::matrix_rotate_2D( m_matrix, angle );
}