#pragma once
#include "Game/GameDefs.h"
#include <vector>
#include <string>
#include <algorithm>

///////////////////////////////////////////////////////////////////////////
class IResource
{
public:
    virtual const std::string& GetResource() const = 0;
    virtual void Load() = 0;
};

///////////////////////////////////////////////////////////////////////////
class CSoundResource : public IResource
{
public:
    CSoundResource( const std::string& resourcePath )
        : m_resource( resourcePath )
    {
    }

	virtual const std::string& GetResource() const override;
	virtual void Load() override;
private:
    std::string m_resource;
};

///////////////////////////////////////////////////////////////////////////
class CImageResource : public IResource
{
public:
    CImageResource( const std::string& resourcePath )
        : m_resource( resourcePath )
    {
    }

	virtual const std::string& GetResource() const override;
	virtual void Load() override;
	bool operator()(const IResource * resource);
private:
    std::string m_resource;
};

///////////////////////////////////////////////////////////////////////////
struct SFindPath
{
    SFindPath( const std::string& resourcePath )
        : m_resource( resourcePath )
    {
    }

	bool operator () (const IResource* resource);

    std::string m_resource;
};

///////////////////////////////////////////////////////////////////////////
class GAME_DLL CResourceContainer
{
public:
    typedef std::vector< IResource* > TRecourceList;

    CResourceContainer();
    ~CResourceContainer();

    void Clear();
    void AddImageResource( const std::string& resourcePath );
    void AddSoundResource( const std::string& resourcePath );
    const TRecourceList& GetResources() const;
    
    U32 GetResourcesCount() const;
    U32 GetResourcesLoadedCount() const;
    U32 IsLoaded() const;

	void LoadResources();

private:
    TRecourceList m_resourceVector;
    U32           m_loadCountIndex;
};