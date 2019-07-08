#pragma once
#include "Game\DB\DBObject.h"
#include "Game\Core\Singleton.h"
#include "Game\Core\StringID.h"
#include <unordered_map>

#define DECLARE_SERVICE( TServiceName )                         \
static CStringID GetClassType()                                 \
{                                                               \
    static CStringID ms_serviceID( STRINGIZE(TServiceName) );   \
    return ms_serviceID;                                        \
}                                                               \

/////////////////////////////////////////////////////////////////////////////////
//IGameService
/////////////////////////////////////////////////////////////////////////////////
class GAME_DLL IGameService /*: public CDbObject*/
{
    //DECLARE_DB_OBJECT( IGameService, CDbObject );
public:
    IGameService(){}
    virtual ~IGameService(){}

    virtual void Init() = 0;
    virtual void ShutDown() = 0;
    virtual void OnGameStart() = 0;
    virtual void OnGameEnd() = 0;
    virtual void Update() = 0;
};

/////////////////////////////////////////////////////////////////////////////////
//CServiceManager
/////////////////////////////////////////////////////////////////////////////////
class GAME_DLL CServiceManager : public CSingleton<CServiceManager>
{
    typedef std::unordered_map< CStringID::IDType, IGameService* > TServiceMap;
public:
    CServiceManager(){}
    ~CServiceManager(){}

    template<class T>
	void RegisterService();

    template<class T>
	void UnregisterService();

    template< class T >
	T* GetService();

	void Init();
	void ShutDown();
	void OnGameStart();
	void OnGameEnd();
	void Update();

private:
    TServiceMap m_services;
};

template<class T>
void CServiceManager::RegisterService()
{
	T* gameService = new T();
	m_services[T::GetClassType().GetUniqueID()] = gameService;
}

template<class T>
void CServiceManager::UnregisterService()
{
	TServiceMap::iterator it = m_services.find(T::GetClassType().GetUniqueID());
	if (it != m_services.end())
	{
		delete it->second;
		m_services.erase(it);
	}
}

template< class T >
T* CServiceManager::GetService()
{
	return m_services[T::GetClassType().GetUniqueID()];
}
