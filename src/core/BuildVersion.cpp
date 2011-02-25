/*
 * BuildVersion.cpp
 * Dominicus
 *
 * Copyright 2010-2011, Joshua Bodine
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 	* Redistributions of source code must retain the above copyright
 *	notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	notice, this list of conditions and the following disclaimer in the
 *	documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "core/BuildVersion.h"

BuildVersion::BuildVersion() :
		classification(BV_CLASSIFICATION),architecture(PROGRAM_ARCH_STR) {
	std::stringstream versionStream;
	versionStream << BV_VERSION << "." << BV_REVISION;
	version = versionStream.str();

	const char* dateString = __DATE__;

	std::string monthString = std::string(dateString).substr(0, 3);
	unsigned int month = 0;
	if(strcmp(monthString.c_str(), "Jan") == 0) month = 1;
	else if(strcmp(monthString.c_str(), "Feb") == 0) month = 2;
	else if(strcmp(monthString.c_str(), "Mar") == 0) month = 3;
	else if(strcmp(monthString.c_str(), "Apr") == 0) month = 4;
	else if(strcmp(monthString.c_str(), "May") == 0) month = 5;
	else if(strcmp(monthString.c_str(), "Jun") == 0) month = 6;
	else if(strcmp(monthString.c_str(), "Jul") == 0) month = 7;
	else if(strcmp(monthString.c_str(), "Aug") == 0) month = 8;
	else if(strcmp(monthString.c_str(), "Sep") == 0) month = 9;
	else if(strcmp(monthString.c_str(), "Oct") == 0) month = 10;
	else if(strcmp(monthString.c_str(), "Nov") == 0) month = 11;
	else if(strcmp(monthString.c_str(), "Dec") == 0) month = 12;
	else {
		std::stringstream err;
		err << "Unable to determine build month from string '" << std::string(dateString) << "'.";
		programLog->report(ProgramLog::LOG_INTERNALERROR, err.str().c_str());

	}
	unsigned int day = atoi(std::string(dateString).substr(4, 2).c_str());
	unsigned int year = atoi(std::string(dateString).substr(7, 4).c_str());

	char fullDateString[11];
	sprintf(fullDateString, "%04u-%02u-%02u", year, month, day);
	buildDate = fullDateString;
}
