#include "Game_PCH.h"
#include "Game/Gameplay/LevelDBObject.h"

#include "Game/DB/DBObject.h"
#include "Game/Serialization/Member.h"
#include "Game/Serialization/MemberHandler.h"

BEGIN_DEFINE_DB_OBJECT( CLevelDBObject )
    SERIALIZE_MEMBER( "LevelName", m_xmlLevelName, BasicTypeHandler )
END_DEFINE_DB_OBJECT()