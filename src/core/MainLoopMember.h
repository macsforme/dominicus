// MainLoopMember.h
// Crucible Island

#ifndef MAINLOOPMEMBER_H
#define MAINLOOPMEMBER_H

class MainLoopMember {
protected:
	unsigned int last;
	int targetSleepMills;
	int workingSleepMills;

	unsigned int lastRunCountCheck;
	unsigned int runsCounter;

	MainLoopMember(unsigned int requestedFrequency);

	// called by inherited classes to calculate milliseconds needed to sleep
	unsigned int getSleepTime();

public:
	unsigned int runRate;

	// returns milliseconds to sleep
	virtual unsigned int execute(bool unScheduled = false) = 0;

	// runcount checking
	void trackRunCount();
};

#endif // MAINLOOPMEMBER_H
