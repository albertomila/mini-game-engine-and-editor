#include "Game_PCH.h"
#include "Game/Utils/FileHelpers.h"

#include <windows.h>
#include <iostream>
namespace FileHelpers
{
    ///////////////////////////////////////////////////////////////////////////
    void wchar_t2string(const wchar_t* wchar, std::string& string_out )
    {
        int index = 0;
        while(wchar[index] != 0)
        {
            string_out += (char)wchar[index];
            ++index;
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    void string2wchar_t(const std::string& str, wchar_t* wchar_out )
    {
        int index = 0;
        while(index < str.size())
        {
            wchar_out[index] = (wchar_t)str[index];
            ++index;
        }
        wchar_out[index] = 0;
    }

    ///////////////////////////////////////////////////////////////////////////
    void ListFilesInDirectory(std::string& directoryName, std::vector<std::string>& files_out )
    {
        WIN32_FIND_DATA FindFileData;

        wchar_t fileName[260];
        string2wchar_t(directoryName, fileName);
        HANDLE hFind = FindFirstFile(fileName, &FindFileData);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			std::string str = "";
			wchar_t2string(FindFileData.cFileName, str);
			files_out.push_back(str);

			while (FindNextFile(hFind, &FindFileData))
			{
				std::string str = "";
				wchar_t2string(FindFileData.cFileName, str);
				files_out.push_back(str);
			}
		}
    }

    ///////////////////////////////////////////////////////////////////////////
    void GetFilesInDirectory( const char* path, const char* extension, std::vector<std::string>& files_out )
    {
        std::string fileSearchPath( path );
        fileSearchPath.append( "*." );
        fileSearchPath.append( extension );

        ListFilesInDirectory( fileSearchPath, files_out);
    }
}