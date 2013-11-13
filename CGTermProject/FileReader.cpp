#include "FileReader.h"

#include <fstream>

FileReader::FileReader()
{
}

FileReader::~FileReader()
{
}

void FileReader::readTargets( std::list< BaseTarget* > targets )
{
	std::ofstream file;
	file.open( "targetData.txt" );
}