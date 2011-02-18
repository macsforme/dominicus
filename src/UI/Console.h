/*
 *  Console.h
 *  ModelViewer
 *
 *  Created by Joshua Bodine on 10/7/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef CONSOLE_H
#define CONSOLE_H

// program headers
#include "core/ProgramLog.h"

// library headers
#include <map>
#include <string>
#include <vector>

class Console {
private:
	typedef std::pair< std::string,std::vector<std::string> > TabStorageEntry;
	std::vector<TabStorageEntry> tabContents;

public:
	// tab manipulation
	void addTab(std::string tabName);
	void deleteTab(std::string tabName);
	void changeTabName(std::string newName, std::string oldName);
	void changeTabPosition(unsigned int position, std::string tabName);

	// content manipulation
	void addLine(std::string lineContents, std::string tabName);
	void deleteLine(unsigned int lineNumber, std::string tabName);
	void changeLineContents(std::string newContents, unsigned int lineNumber, std::string tabName);
};

#endif
