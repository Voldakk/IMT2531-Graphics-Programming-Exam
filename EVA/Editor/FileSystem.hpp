#pragma once

#ifdef _WIN32

	#include <Windows.h>

#else

	#include <unistd.h>
	#include <sys/param.h>

#endif

#include <string>
#include <experimental/filesystem>

#include "tinyfiledialogs.h"
#include "../../dependencies/tfd/tinyfiledialogs.h"

namespace EVA
{
	class FileSystem
	{

		static const int PATH_LENGTH = 1024;

	public:

		typedef std::experimental::filesystem::path path;

		static path OpenFileDialog(const char* title, const char* defaultPathAndFile = "", const int numberOfFilterPatterns = 0, const char * const * filterPatterns = nullptr, const bool multiSelect = false)
		{
			const auto path = tinyfd_openFileDialog(title, defaultPathAndFile, numberOfFilterPatterns, filterPatterns, "", multiSelect);
			return path == nullptr ? "" : path;
		}

		static path SaveFileDialog(const char* title, const char* defaultPathAndFile = "", const int numberOfFilterPatterns = 0, const char * const * filterPatterns = nullptr)
		{
			const auto path = tinyfd_saveFileDialog(title, defaultPathAndFile, numberOfFilterPatterns, filterPatterns, "");
			return path == nullptr ? "" : path;
		}

		static path GetProgramPath()
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

			char path[PATH_LENGTH];
			uint32_t size = sizeof(path);
			if (_NSGetExecutablePath(path, &size) == 0)
				return path;
			else
				return "";

			#endif
		}

		static path GetAssetsFolder()
		{
			const auto programPath = GetProgramPath();

			const auto programDirectory = programPath.parent_path();

			return programDirectory / "/assets/";
		}
	};
}