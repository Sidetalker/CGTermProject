#ifndef FILE_READER_H
#define FILE_READER_H

#include <vector>
#include <string>

#include "BaseTarget.h"

class Score
{
public:
	Score( int score, std::string initials ) :
	  m_score( score )
	, m_initials( initials ) {}

	int m_score;
	std::string m_initials;

	bool operator<( Score other )
	{
		return m_score > other.m_score;
	}
};

class FileReader
{
public:
	FileReader();
	~FileReader();
	
	static void readTargets( std::vector< BaseTarget* >& targets );

	static void readHighScores( std::vector< Score >& scores );

	static void writeHighScores( std::vector< Score >& scores );

};

#endif