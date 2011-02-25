/*
 * Console.cpp
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

#include "UI/Console.h"

void Console::addTab(std::string tabName) {
	tabContents.push_back(
			std::make_pair(tabName, std::vector<std::string>())
		);
}

void Console::deleteTab(std::string tabName) {
	for(
			std::vector<TabStorageEntry>::iterator itr = tabContents.begin();
			itr != tabContents.end();
			++itr
		) {
		if(itr->first == tabName) {
			tabContents.erase(itr);

			return;
		}
	}

	// not found
	programLog->report(ProgramLog::LOG_INTERNALERROR, "Console tab deletion on non-existant name requested.");
}

void Console::changeTabName(std::string newName, std::string oldName) {
	for(
			std::vector<TabStorageEntry>::iterator itr = tabContents.begin();
			itr != tabContents.end();
			++itr
		) {
		if(itr->first == oldName) {
			itr->first = newName;

			return;
		}
	}

	// not found
	programLog->report(ProgramLog::LOG_INTERNALERROR, "Console tab rename on non-existant name requested.");
}

void Console::changeTabPosition(unsigned int position, std::string tabName) {
	TabStorageEntry storage;

	for(size_t i = 0; i < tabContents.size(); ++i) {
		if(tabContents[i].first == tabName) {
			storage = tabContents[i];

			// erase old position
			std::vector<TabStorageEntry>::iterator itr = tabContents.begin();
			itr += i;
			tabContents.erase(itr);

			// insert new position
			itr = tabContents.begin();
			itr += position;
			tabContents.insert(itr, storage);

			return;
		}
	}

	// not found
	programLog->report(ProgramLog::LOG_INTERNALERROR, "Console tab re-position on non-existant name requested.");
}

void Console::addLine(std::string lineContents, std::string tabName) {
	for(
			std::vector<TabStorageEntry>::iterator itr = tabContents.begin();
			itr != tabContents.end();
			++itr
		) {
		if(itr->first == tabName)
			itr->second.push_back(lineContents);

		return;
	}

	// not found
	programLog->report(ProgramLog::LOG_INTERNALERROR, "Line insertion on non-existant tab name requested.");
}

void Console::deleteLine(unsigned int lineNumber, std::string tabName) {
	for(
			std::vector<TabStorageEntry>::iterator itr = tabContents.begin();
			itr != tabContents.end();
			++itr
		) {
		if(itr->first == tabName) {
			if(itr->second.size() <= lineNumber)
				programLog->report(
						ProgramLog::LOG_INTERNALERROR,
						"Line deletion on non-existant line index requested."
					);
			else
				itr->second.erase(itr->second.begin() + lineNumber);

			return;
		}
	}

	// not found
	programLog->report(ProgramLog::LOG_INTERNALERROR, "Line insertion on non-existant tab name requested.");
}

void Console::changeLineContents(std::string newContents, unsigned int lineNumber, std::string tabName) {
	for(
			std::vector<TabStorageEntry>::iterator itr = tabContents.begin();
			itr != tabContents.end();
			++itr
		) {
		if(itr->first == tabName) {
			if(itr->second.size() <= lineNumber)
				programLog->report(
						ProgramLog::LOG_INTERNALERROR,
						"Line change on non-existant line index requested."
					);
			else
				itr->second[lineNumber] = newContents;

			return;
		}
	}

	// not found
	programLog->report(ProgramLog::LOG_INTERNALERROR, "Line change on non-existant tab name requested.");
}
