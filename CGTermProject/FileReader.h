#ifndef FILE_READER_H
#define FILE_READER_H

#include <vector>

#include "BaseTarget.h"

class FileReader
{
public:
	FileReader();
	~FileReader();
	
	static void readTargets( std::vector< BaseTarget* >& targets );

	static void readHighScores( std::vector< int >& scores );

	static void writeHighScores( std::vector< int >& scores );

};

#endif