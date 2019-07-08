#pragma once
#include <string>
#include <vector>
#include "Game\GameDefs.h"

namespace FileHelpers
{
    GAME_DLL void wchar_t2string(const wchar_t* wchar, std::string& string_out );
    GAME_DLL void string2wchar_t(const std::string& str, wchar_t* wchar_out );
    GAME_DLL void ListFilesInDirectory(std::string& directoryName, std::vector<std::string>& files_out );
    GAME_DLL void GetFilesInDirectory( const char* path, const char* extension, std::vector<std::string>& files_out );
}