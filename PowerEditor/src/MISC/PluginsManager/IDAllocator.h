// IDAllocator.h code is copyrighted (C) 2010 by Dave Brotherstone
// Part of Notepad++ - Copyright Don Ho
#ifndef MISC_PLUGINSMANAGER_IDALLOCATOR_H
#define MISC_PLUGINSMANAGER_IDALLOCATOR_H

class IDAllocator
{
public:
	IDAllocator(int start, int maximumID);

	/// Returns -1 if not enough available
	int allocate(int quantity);

	bool isInRange(int id) { return (id >= _start && id < _nextID); }

private:
	int _start;
	int _nextID;
	int _maximumID;
};

#endif // MISC_PLUGINSMANAGER_IDALLOCATOR_H
