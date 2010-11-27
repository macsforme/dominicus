/*
 *  ProgramLog.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "ProgramLog.h"

std::vector<std::string> logStrings;

void ProgramLog::report(LogDetail detail, const char* report) {
	switch(detail) {
		case LOG_INTERNALERROR:
		case LOG_FATAL:
			std::cout << "FATAL: " << report << std::endl;
			exit(1);

			break;
		case LOG_DEBUG:
			std::cout << "DEBUG: " << report << std::endl;

			logStrings.push_back(std::string("DEBUG: ") + report);

			break;
		case LOG_INFO:
			logStrings.push_back(std::string("INFO: ") + report);

			break;
	}
}
