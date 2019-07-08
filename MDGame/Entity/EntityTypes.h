#pragma once
#include "Game\Core\traits.h"

class CGraphicComponent;
class CSoundComponent;
class CPhysicsComponent;

typedef STypeList 
<
    CGraphicComponent, STypeList< 
    CSoundComponent, STypeList< 
    CPhysicsComponent, SEmpty
	>>
> 
TCommonEntity;