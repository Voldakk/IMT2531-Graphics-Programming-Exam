#pragma once

#include <string>
#include <experimental/filesystem>

#include "tinyfiledialogs.h"

namespace FS = std::experimental::filesystem;

namespace EVA
{
	class FileSystem
	{

	public:

		static FS::path OpenFileDialog(const char* title, const char* defaultPathAndFile = "", const int numberOfFilterPatterns = 0, const char * const * filterPatterns = nullptr, const bool multiSelect = false)
		{
			const auto path = tinyfd_openFileDialog(title, defaultPathAndFile, numberOfFilterPatterns, filterPatterns, "", multiSelect);
			return path == nullptr ? "" : path;
		}

		static FS::path SaveFileDialog(const char* title, const char* defaultPathAndFile = "", const int numberOfFilterPatterns = 0, const char * const * filterPatterns = nullptr)
		{
			const auto path = tinyfd_saveFileDialog(title, defaultPathAndFile, numberOfFilterPatterns, filterPatterns, "");
			return path == nullptr ? "" : path;
		}

		static std::string ToString(const FS::path& path)
		{
			auto pathString = path.string();
			std::replace(pathString.begin(), pathString.end(), '\\', '/');
			return pathString;
		}
	};
}