#pragma once
#include "Game/DB/DBObject.h"
#include <string>

class GAME_DLL CLevelDBObject : public CDbObject
{
    DECLARE_DB_OBJECT( CLevelDBObject, CDbObject );

public:
    CLevelDBObject() 
    {
    }

    virtual ~CLevelDBObject(){}

    std::string m_xmlLevelName;
};