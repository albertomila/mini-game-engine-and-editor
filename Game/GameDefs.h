#pragma once

#ifdef GAME_EXPORT_DLL
	#define GAME_DLL __declspec(dllexport)
#else
	#define GAME_DLL __declspec(dllimport)
#endif // #ifdef GAME_EXPORT_DLL

#define NULL 0
