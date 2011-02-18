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
	switch(detail) {
		case LOG_INTERNALERROR:
		case LOG_FATAL:
			std::cout << "FATAL: " << report << std::endl;
			exit(1);

			break;
		case LOG_DEBUG:
			{
			std::cout << "DEBUG: " << report << std::endl;
			std::string msg = "DEBUG: ";
			msg += report;
			logStrings.push_back(msg);
			}
			break;
		case LOG_INFO:
			{
			std::string msg = "INFO: ";
			msg += report;
			logStrings.push_back(msg);
			}
			break;
	}
}
