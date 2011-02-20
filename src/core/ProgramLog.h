/*
 *  ProgramLog.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef PROGRAMLOG_H
#define PROGRAMLOG_H

// library headers
#include <cstdlib>
#include <iostream>
#include <vector>

class ProgramLog {
public:
	enum LogDetail {
			LOG_INFO,	// verbose information that users generally don't need to see
			LOG_DEBUG,	// problems that shouldn't normally happen
			LOG_FATAL,	// errors created by outside means that we can't survive
			LOG_INTERNALERROR	// safety mechanisms in case of internal bugs (no external libs or files)
		};

	std::vector<std::string> logStrings;

	void report(LogDetail detail, const char* report);
};

#endif
