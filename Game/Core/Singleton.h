#pragma once

#include "Game/GameDefs.h"
#include <assert.h>

template<typename T>
class CSingleton
{
public:
	static void CreateInstance()
	{
		if( ms_instance == NULL )
		{
			ms_instance = new T();
		}
		else
		{
			assert(false && "instance already created");
		}
	}
	
	static void DestroyInstance()
	{
		delete ms_instance;
		ms_instance = NULL;
	}

	static inline T* Get()
	{
		return ms_instance;
	}

protected:
	CSingleton(){}
	~CSingleton(){}

private:
	CSingleton(const CSingleton&){}
	void operator=(const CSingleton&){}

private:
	static T* ms_instance;
};

template< typename T >
T* CSingleton<T>::ms_instance = NULL;