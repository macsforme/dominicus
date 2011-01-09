/*
 *  BuildVersion.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 1/9/11.
 *  Copyright 2011 Joshua Bodine. All rights reserved.
 *
 */

#include "BuildVersion.h"

BuildVersion::BuildVersion() : classification(BV_CLASSIFICATION), source(BV_SOURCE),
		architecture(platform.getArchitecture()) {
	std::stringstream versionStream;
	versionStream << BV_VERSION << "." << BV_REVISION;
	version = versionStream.str();

	const char* dateString = __DATE__;
	
	const char* monthString = std::string(dateString).substr(0, 3).c_str();
	unsigned int month = 0;
	if(strcmp(monthString, "Jan") == 0) month = 1;
	else if(strcmp(monthString, "Feb") == 0) month = 2;
	else if(strcmp(monthString, "Mar") == 0) month = 3;
	else if(strcmp(monthString, "Apr") == 0) month = 4;
	else if(strcmp(monthString, "May") == 0) month = 5;
	else if(strcmp(monthString, "Jun") == 0) month = 6;
	else if(strcmp(monthString, "Jul") == 0) month = 7;
	else if(strcmp(monthString, "Aug") == 0) month = 8;
	else if(strcmp(monthString, "Sep") == 0) month = 9;
	else if(strcmp(monthString, "Oct") == 0) month = 10;
	else if(strcmp(monthString, "Nov") == 0) month = 11;
	else if(strcmp(monthString, "Dec") == 0) month = 12;
	else {
		std::stringstream err;
		err << "Unable to determine build month from string '" << std::string(dateString) << "'.";
		programLog.report(LOG_INTERNALERROR, err.str().c_str());

	}
	unsigned int day = atoi(std::string(dateString).substr(4, 2).c_str());
	unsigned int year = atoi(std::string(dateString).substr(7, 4).c_str());

	char fullDateString[11];
	sprintf(fullDateString, "%04u-%02u-%02u", year, month, day);
	buildDate = fullDateString;
}
