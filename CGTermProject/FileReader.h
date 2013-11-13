#ifndef FILE_READER_H
#define FILE_READER_H

#include <list>

#include "BaseTarget.h"

class FileReader
{
public:
	FileReader();
	~FileReader();
	
	static void readTargets( std::list< BaseTarget* > targets );

};

#endif