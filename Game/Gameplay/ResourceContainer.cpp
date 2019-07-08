#include "Game_PCH.h"
#include "Game/Gameplay/ResourceContainer.h"


///////////////////////////////////////////////////////////////////////////
const std::string& CSoundResource::GetResource() const 
{ 
	return m_resource; 
}

void CSoundResource::Load()
{
}

///////////////////////////////////////////////////////////////////////////
const std::string& CImageResource::GetResource() const
{
	return m_resource;
}

void CImageResource::Load()
{
}

///////////////////////////////////////////////////////////////////////////
bool CImageResource::operator () (const IResource* resource)
{
	return m_resource == resource->GetResource();
}


///////////////////////////////////////////////////////////////////////////
bool SFindPath::operator () (const IResource* resource)
{
	return m_resource == resource->GetResource();
}



///////////////////////////////////////////////////////////////////////////
CResourceContainer::CResourceContainer()
	: m_loadCountIndex(0)
{
}

CResourceContainer::~CResourceContainer()
{
	Clear();
}

void CResourceContainer::Clear()
{
	m_loadCountIndex = 0;
	DeleteVector(m_resourceVector);
}

void CResourceContainer::AddImageResource(const std::string& resourcePath)
{
	if (resourcePath.empty())
	{
		return;
	}

	TRecourceList::iterator it = std::find_if(m_resourceVector.begin(), m_resourceVector.end(), SFindPath(resourcePath));
	if (it == m_resourceVector.end())
	{
		m_resourceVector.push_back(new CImageResource(resourcePath));
	}
}

void CResourceContainer::AddSoundResource(const std::string& resourcePath)
{
	if (resourcePath.empty())
	{
		return;
	}

	TRecourceList::iterator it = std::find_if(m_resourceVector.begin(), m_resourceVector.end(), SFindPath(resourcePath));
	if (it == m_resourceVector.end())
	{
		m_resourceVector.push_back(new CSoundResource(resourcePath));
	}
}

const CResourceContainer::TRecourceList& CResourceContainer::GetResources() const
{
	return m_resourceVector;
}

U32 CResourceContainer::GetResourcesCount() const
{
	return m_resourceVector.size();
}
U32 CResourceContainer::GetResourcesLoadedCount() const
{
	return m_loadCountIndex;
}
U32 CResourceContainer::IsLoaded() const
{
	return m_loadCountIndex == m_resourceVector.size();
}

void CResourceContainer::LoadResources()
{
	m_loadCountIndex = 0;
	FOR_EACH(TRecourceList, it, m_resourceVector)
	{
		(*it)->Load();
		++m_loadCountIndex;
	}
}
