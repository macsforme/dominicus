/*
 *  ProgramLog.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "ProgramLog.h"

void ProgramLog::report(LogDetail detail, const char* report) {
	switch(detail) {
		case LOG_INTERNALERROR:
		case LOG_FATAL:
			std::cout << "FATAL: " << report << std::endl;
			exit(1);

			break;
		case LOG_DEBUG:
			std::cout << "DEBUG: " << report << std::endl;

			break;
		case LOG_INFO:
			std::cout << "INFO: " << report << std::endl;

			break;
	}
}

