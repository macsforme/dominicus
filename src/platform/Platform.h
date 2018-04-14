// Platform.h
// Dominicus

#ifndef PLATFORM_H
#define PLATFORM_H

// this should be the one and only compile-time determination system-wide
// of what architecture the program is being compiled for (generally the
// code should not have platform-specific behavior outside of Platform,
// but we acknowledge that there must be exceptions
#if defined(__APPLE__) && defined(__MACH__)
	#define PROGRAM_ARCH_MACOSX
	#define PROGRAM_ARCH_STR "MacOSX"
#elif defined(_WIN32)
	#define PROGRAM_ARCH_WINNT
	#define PROGRAM_ARCH_STR "WinNT"
#elif defined(linux) || defined(__linux)
	#define PROGRAM_ARCH_LINUX
	#define PROGRAM_ARCH_STR "Linux"
#else
	#define PROGRAM_ARCH_UNKNOWN
	#define PROGRAM_ARCH_STR "Unknown"
#endif

#include <string>

class Platform {
public:
	std::string dataPath;

	Platform();

	// static system console output function
	static void consoleOut(std::string output);

	// time functions
	unsigned int getExecMills();
	void sleepMills(unsigned int mills);

	// application preferences
	std::string getPreferenceString(const char* key);
	float getPreferenceFloat(const char* key);
	void setPreference(const char* key, const char* value);
	void setPreference(const char* key, float value);
};

#endif // PLATFORM_H
