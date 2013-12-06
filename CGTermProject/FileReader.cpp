#include "FileReader.h"

#include <fstream>
#include <iostream>

#include "Target.h"
#include "TargetDefs.h"

FileReader::FileReader()
{
}

FileReader::~FileReader()
{
}

// reads Data/TargetData.txt and creates Target objects in the heap
void FileReader::readTargets( std::vector< BaseTarget* >& targets )
{
	int typeInt;
	float speed;
	float xPos, yPos, zPos;
	uint numKeyFrames;
	bool repeating;
	float rotSpeed;
	float rotAxisX, rotAxisY, rotAxisZ;
	
	std::ifstream file( "Data/TargetData.txt" );
	{
		if ( !file )
		{
			std::cout << "couldn't find file \"TargetData.txt\"" << std::endl;
		}
	}

	file >> typeInt;

	// keep reading target data until delimeter is met
	while ( typeInt != -1 )
	{
		TargetTypes::id type = ( TargetTypes::id ) typeInt;

		file >> speed;
		file >> xPos >> yPos >> zPos;

		file >> rotSpeed;
		file >> rotAxisX >> rotAxisY >> rotAxisZ;

		BaseTarget* t;

		switch ( type )
		{
			case TargetTypes::BULLSEYE:
			{
				t = new Target( Vector( xPos, yPos, zPos ), speed, 2.0, Vector( rotAxisX, rotAxisY, rotAxisZ ), rotSpeed ); 
				break;
			}
			default:
			{
				std::cout << "ERROR: Unhandled type read from file." << std::endl;
			}
		}

		t->addKeyFrame( Vector( xPos, yPos, zPos ) );

		file >> numKeyFrames;

		// get all keyframes
		for ( uint i = 1; i < numKeyFrames; ++i )
		{
			float keyX, keyY, keyZ;
			file >> keyX >> keyY >> keyZ;
			t->addKeyFrame( Vector( keyX, keyY, keyZ ) );
		}

		file >> repeating;

		t->setRepeatFrames( repeating );

		targets.push_back( t );

		// looking for -1 to denote EOF
		file >> typeInt;
	}

}

// adds high scores from Data/HighScoreData.txt to vector
void FileReader::readHighScores( std::vector< Score >& scores )
{
	std::ifstream file( "Data/HighScoreData.txt" );
	{
		if ( !file )
		{
			std::cout << "couldn't find file \"HighScoreData.txt\"" << std::endl;
		}
	}
	
	float score;
	std::string initials;

	while ( file >> score )
	{
		file >> initials;
		
		scores.push_back( Score( (int)score, initials ) );
	}

	// populate with at least 10 values (if initially empy or incomplete file)
	while ( scores.size() != 10 )
	{
		scores.push_back( Score( 1, "AAA" ) );
	}
}

// writes current highscores to disk
void FileReader::writeHighScores( std::vector< Score >& scores )
{
	std::ofstream file( "Data/HighScoreData.txt" );
	{
		if ( !file )
		{
			std::cout << "couldn't open file \"HighScoreData.txt\"" << std::endl;
		}
	}

	// write top 10 scores
	for ( uint i = 0; i < 10; ++i )
	{
		file << scores[ i ].m_score << " " << scores[ i ].m_initials << '\n';
	}
}