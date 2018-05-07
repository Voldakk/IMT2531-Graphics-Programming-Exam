#pragma once
/*
#ifdef _WIN32

	#define NOC_FILE_DIALOG_WIN32

	#include <Windows.h>

#elif __linux__

	#define NOC_FILE_DIALOG_GTK

	#include <unistd.h>
	#include <sys/param.h>

#elif __APPLE__

	#define NOC_FILE_DIALOG_OSX

#endif

#define NOC_FILE_DIALOG_IMPLEMENTATION

#include <string>

#include "noc_file_dialog.h"

namespace EVA
{

	class FileSystem
	{
		static const int PATH_LENGTH = 1000;
	public:

		enum DialogType
		{
			Open = NOC_FILE_DIALOG_OPEN, 
			Save = NOC_FILE_DIALOG_SAVE, 
			Directory = NOC_FILE_DIALOG_DIR,
			OverwriteConfirmation = NOC_FILE_DIALOG_OVERWRITE_CONFIRMATION
		};

		static std::string OpenDialog(const DialogType flags, const char *filters, const char *defaultPath, const char *defaultName)
		{
			const auto path = noc_file_dialog_open(flags, filters, defaultPath, defaultName);
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
*/