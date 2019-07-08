#pragma once

#ifdef MDGAME_EXPORT_DLL
	#define MDGAME_DLL __declspec(dllexport)
#else
	#define MDGAME_DLL __declspec(dllimport)
#endif // #ifdef MDGAME_EXPORT_DLL
