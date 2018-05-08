#pragma once

#ifdef _WIN32

	#include <Windows.h>

#else

	#include <unistd.h>
	#include <sys/param.h>

#endif

#include <string>

#include "tinyfiledialogs.h"
#include "../../dependencies/tfd/tinyfiledialogs.h"

namespace EVA
{

	class FileSystem
	{
		static const int PATH_LENGTH = 1000;
	public:


		static std::string OpenFileDialog(const char* title, const char* defaultPathAndFile = "", const int numberOfFilterPatterns = 0, const char * const * filterPatterns = nullptr, const bool multiSelect = false)
		{
			const auto path = tinyfd_openFileDialog(title, defaultPathAndFile, numberOfFilterPatterns, filterPatterns, "", multiSelect);
			return path == nullptr ? "" : path;
		}

		static std::string SaveFileDialog(const char* title, const char* defaultPathAndFile = "", const int numberOfFilterPatterns = 0, const char * const * filterPatterns = nullptr)
		{
			const auto path = tinyfd_saveFileDialog(title, defaultPathAndFile, numberOfFilterPatterns, filterPatterns, "");
			return path == nullptr ? "" : path;
		}

		static std::string GetWorkingDirectory()
		{
			#ifdef _WIN32

			char buffer[PATH_LENGTH];
			GetModuleFileName(nullptr, buffer, PATH_LENGTH);
			return buffer;

			#elif __linux__

			char szTmp[32];
			sprintf(szTmp, "/proc/%d/exe", getpid());

			char buffer[PATH_LENGTH];
			long bytes = MIN(readlink(szTmp, buffer, PATH_LENGTH), PATH_LENGTH - 1);
			if (bytes >= 0)
				buffer[bytes] = '\0';

			return buffer;

			#elif __APPLE__

			return "";

			#endif

		}
	};

}
