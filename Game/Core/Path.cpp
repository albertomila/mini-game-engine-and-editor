#include "Game_PCH.h"
#include "Game/Core/Path.h"
#include <direct.h>

void PathHelpers::GetMainPath(std::string& path_out)
{
	char the_path[256];
	getcwd(the_path, 255);
	path_out = std::string(the_path);
	path_out.append("/../");
}

void PathHelpers::GetRelativePath(const char* folder, std::string& path_out)
{
	path_out = std::string(folder);

	std::string dataPath;
	GetMainPath(dataPath);

	int pos = path_out.find(dataPath.c_str());
	if (pos != -1)
	{
		path_out.replace(0, dataPath.length(), "");
	}
}

void PathHelpers::GetSpecificPath(const char* folder, std::string& path_out)
{
	GetMainPath(path_out);
	path_out.append(folder);
}

void PathHelpers::GetDataBasePath(std::string& path_out) { return GetSpecificPath(RELATIVE_DATA_PATH_DB, path_out); }
void PathHelpers::GetEntitiesPath(std::string& path_out) { return GetSpecificPath(RELATIVE_DATA_PATH_ENTITY, path_out); }
void PathHelpers::GetLevelsPath(std::string& path_out) { return GetSpecificPath(RELATIVE_DATA_PATH_LEVELS, path_out); }
void PathHelpers::GetEditorPath(std::string& path_out) { return GetSpecificPath(RELATIVE_DATA_PATH_EDITOR, path_out); }


/*
void PathHelpers::GetRelativePath(const char* folder, std::string& path_out)
{
	std::string dataPath;
	GetMainPath(dataPath);

	int pos = path_out.find(dataPath.c_str());
	if (pos != -1)
	{
		path_out.replace(0, dataPath.length(), "");
	}
}*/