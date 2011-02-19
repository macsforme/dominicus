/*
 *  ProgramLog.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "core/ProgramLog.h"

std::vector<std::string> logStrings;

void ProgramLog::report(LogDetail detail, const char* report) {
	std::string fullReport = "";
	if(detail == LOG_INFO)
		fullReport += "INFO: ";
	else if(detail == LOG_DEBUG)
		fullReport += "DEBUG: ";
	else if(detail == LOG_FATAL)
		fullReport += "FATAL: ";
	else
		fullReport += "INTERNAL ERROR: ";

	fullReport += report;
	logStrings.push_back(fullReport);

	if(detail != LOG_INFO) {
		std::cout << fullReport << std::endl;

		if(detail != LOG_DEBUG)
			exit(1);
	}
}
