#pragma once
#include <string>

static const char* RELATIVE_DATA_PATH           = "data/";
static const char* RELATIVE_DATA_PATH_DB        = "data/db/";
static const char* RELATIVE_DATA_PATH_ENTITY    = "data/entities/";
static const char* RELATIVE_DATA_PATH_LEVELS    = "data/levels/";
static const char* RELATIVE_DATA_PATH_EDITOR    = "data/editor/";

struct GAME_DLL PathHelpers
{
	static void GetMainPath(std::string& folder);
	static void GetRelativePath(const char* folder, std::string& path_out);
    static void GetSpecificPath( const char* folder, std::string& path_out);

    static void GetDataBasePath(std::string& path_out);
    static void GetEntitiesPath(std::string& path_out);
    static void GetLevelsPath(std::string& path_out);
    static void GetEditorPath(std::string& path_out);
};
