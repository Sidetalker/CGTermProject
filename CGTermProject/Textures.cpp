//
//  Textures.cpp
//  CGTermProject
//
//  Created by Kevin Sullivan on 11/13/13.
//  Copyright (c) 2013 com.sideapps. All rights reserved.
//

#include "Textures.h"
#include "TextureDefs.h"

#include <iostream>
#include <fstream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

Textures::Textures()
{
	m_textureIndices = new unsigned int[ TextureTypes::NUM_TEXTURES ];
	// generates names array
	glGenTextures( TextureTypes::NUM_TEXTURES, m_textureIndices );
	loadTextures();
}

Textures::~Textures()
{
	// clear names array
	glDeleteTextures( TextureTypes::NUM_TEXTURES, m_textureIndices );

	// free array
	delete[] m_textureIndices;
}

Textures::BitMapFile* Textures::getBMPData( std::string filename )
{
	// ***MUST CLEAN UP AFTER THIS ALLOC (Handled at end of loadTextures loop.)***
	BitMapFile *bmp = new BitMapFile;
	unsigned int size, offset, headerSize;
    
	// Read input file name.
	ifstream infile( filename.c_str(), ios::binary );

	if( !infile )
	{
		cout << "couldn't find texture file " << filename << endl;
	}
    
	// Get the starting point of the image data.
	infile.seekg(10);
	infile.read((char *) &offset, 4);
    
	// Get the header size of the bitmap.
	infile.read((char *) &headerSize,4);
    
	// Get width and height values in the bitmap header.
	infile.seekg(18);
	infile.read( (char *) &bmp->sizeX, 4);
	infile.read( (char *) &bmp->sizeY, 4);
    
	// Allocate buffer for the image.
	size = bmp->sizeX * bmp->sizeY * 24;

	// ***CLEANED UP IN DESTRUCTOR OF BitMapFile***
	bmp->m_data = new unsigned char[ size ];
    
	// Read bitmap data.
	infile.seekg(offset);
	infile.read((char *) bmp->m_data , size);
    
	// Reverse color from bgr to rgb.
	int temp;
	for ( unsigned int i = 0; i < size; i += 3 )
	{
		temp = bmp->m_data[i];
		bmp->m_data[i] = bmp->m_data[i+2];
		bmp->m_data[i+2] = temp;
	}
	
	return bmp;
}

void Textures::loadTextures()
{
	// index number for texture
	unsigned int textureIndex = 0;
	string textureFilename;

	ifstream textureFilenames( "Data/TextureData.txt" );
	{
		if ( !textureFilenames )
		{
			cout << "couldn't find file \"TextureData.txt\"" << endl;
		}
	}

	while ( textureIndex < TextureTypes::NUM_TEXTURES )
	{
		// filter mode
		unsigned int filter;

		textureFilenames >> textureFilename;
		textureFilenames >> filter;

		// Load the textures.
		BitMapFile* image = getBMPData( "Textures/" + textureFilename );

		glBindTexture( GL_TEXTURE_2D, m_textureIndices[ textureIndex ] );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

		// switch for filtering
		switch ( filter )
		{
			case 0:
			{
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, image->sizeX, image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image->m_data );
				break;
			}
			case 1:
			{
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, image->sizeX, image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image->m_data );
				break;
			}
			case 2:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image->sizeX, image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image->m_data);
				break;
			}
			case 3:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image->sizeX, image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image->m_data);
				break;
			}
			case 4:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image->sizeX, image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image->m_data);
				break;
			}
			case 5:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image->sizeX, image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image->m_data);
				break;
			}
			default:
			{
				// error, invalid filter
			}
		}

		// ***HANDLE UNHANDLED CLEANUP***
		delete image;

		++textureIndex;
	}
}
