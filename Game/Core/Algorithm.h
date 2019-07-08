#pragma once

#define FOR_EACH( CONTAINER_TYPE, IT_NAME, CONTAINER ) \
	for( CONTAINER_TYPE::iterator IT_NAME = CONTAINER.begin(), IT_END = CONTAINER.end(); IT_NAME != IT_END; ++IT_NAME)

#define FOR_EACH_CONST( CONTAINER_TYPE, IT_NAME, CONTAINER ) \
	for( CONTAINER_TYPE::const_iterator IT_NAME = CONTAINER.begin(), IT_END = CONTAINER.end(); IT_NAME != IT_END; ++IT_NAME)

#define FOR_EACH_SLOW( CONTAINER_TYPE, IT_NAME, CONTAINER ) \
	for( CONTAINER_TYPE::iterator IT_NAME = CONTAINER.begin(); IT_NAME != CONTAINER.end(); )

///////////////////////////////////////////////////////////////////
template<class T>
void DeleteVector(T& vectorContainer)
{
	FOR_EACH(T, it, vectorContainer)
	{
		delete *it;
	}

	vectorContainer.clear();
}

///////////////////////////////////////////////////////////////////
template<class T>
void DeleteMap(T& hashMap)
{
	FOR_EACH(T, it, hashMap)
	{
		delete it->second;
	}

	hashMap.clear();
}