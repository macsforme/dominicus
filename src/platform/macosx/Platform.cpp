// Platform.cpp
// Crucible Island

#include <CoreFoundation/CoreFoundation.h>
#include <cstdlib>
#include <iostream>
#include <mach/mach_time.h>
#include <sstream>
#include <stdint.h>
#include <string>
#include <time.h>

#include "core/GameSystem.h"
#include "platform/Platform.h"

extern GameSystem* gameSystem;

Platform::Platform() {
	// determine the resource directory path
	CFBundleRef bundle = CFBundleGetMainBundle();
	CFURLRef resources = CFBundleCopyResourcesDirectoryURL(bundle);

	char detectedDataPath[PATH_MAX];
	if(! CFURLGetFileSystemRepresentation(resources, TRUE,
			(UInt8 *)detectedDataPath, PATH_MAX)) {
		// no GameSystem yet to log so just print and exit
		std::cout << "Could not get path for resource directory." << std::endl;
		exit(1);
	}
	CFRelease(resources);

	dataPath = detectedDataPath;

	// initialize the random number generator
	srand(time(NULL));
}

void Platform::consoleOut(std::string output) {
	std::cout << output;
}

unsigned int Platform::getExecMills() {
	static uint64_t beginning = mach_absolute_time();
	uint64_t now = mach_absolute_time();
	mach_timebase_info_data_t timeInfo;
	kern_return_t error = mach_timebase_info(&timeInfo);

	if(error) {
		if(gameSystem != NULL) {
			gameSystem->log(GameSystem::LOG_FATAL,
					"An error occurred when attempting to retrieve the time.");
		} else {
			std::cout << "An error occurred when attempting to retrieve the time." << std::endl;
		}
	}

	return (unsigned int) (
			(0.000001 *
					((double) timeInfo.numer / (double) timeInfo.denom) *
					(double) (now - beginning)
				)
		);
}

void Platform::sleepMills(unsigned int mills) {
	timespec delayTime;
	delayTime.tv_sec = 0;
	delayTime.tv_nsec = mills * 1000000;
	nanosleep(&delayTime, NULL);
}

std::string Platform::getPreferenceString(const char* key) {
	std::string toReturn = std::string("");
	CFStringRef keyRef = CFStringCreateWithCString(NULL, key, kCFStringEncodingASCII);
	CFPropertyListRef value = CFPreferencesCopyAppValue(keyRef, kCFPreferencesCurrentApplication);
	if(value != NULL) {
		if(CFGetTypeID(value) == CFStringGetTypeID()) {
			char buffer[1025];
			buffer[0] = '\0';
			CFStringGetCString((CFStringRef) value, buffer, 1025, kCFStringEncodingASCII);
			toReturn = std::string(buffer);
		}
		CFRelease(value);
	}
	CFRelease(keyRef);
	return toReturn;
}

float Platform::getPreferenceFloat(const char* key) {
	return atof(getPreferenceString(key).c_str());
}

void Platform::setPreference(const char* key, const char* value) {
	CFStringRef keyRef = CFStringCreateWithCString(NULL, key, kCFStringEncodingASCII);
	CFDataRef dataRef = CFDataCreate(NULL, (uint8_t*) value, strlen(value));
	CFPropertyListRef propertyList = CFPropertyListCreateWithData(NULL, dataRef, kCFPropertyListImmutable, NULL, NULL);
	CFPreferencesSetAppValue(keyRef, propertyList, kCFPreferencesCurrentApplication);
	CFPreferencesSynchronize(kCFPreferencesCurrentApplication, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);

	CFRelease(keyRef);
	CFRelease(dataRef);
	if(propertyList != NULL)
		CFRelease(propertyList);
}

void Platform::setPreference(const char* key, float value) {
	std::stringstream stringValue;
	stringValue << value;
	setPreference(key, stringValue.str().c_str());
}
