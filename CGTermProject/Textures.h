//
//  Textures.h
//  CGTermProject
//
//  Created by Kevin Sullivan on 11/13/13.
//  Copyright (c) 2013 com.sideapps. All rights reserved.
//

#ifndef __CGTermProject__Textures__
#define __CGTermProject__Textures__

#include <string>

#endif /* defined(__CGTermProject__Textures__) */

#define TEXTURE_COUNT 1

class Textures
{
public:
	Textures();
	~Textures();

	unsigned int* getTextureIndices() { return m_textureIndices; }

private:

	// Struct of bitmap file.
	class BitMapFile
	{
	public:
		BitMapFile() {};
		~BitMapFile() { delete[] m_data; } ;

		int sizeX;
		int sizeY;

		// Array of data
		unsigned char* m_data;
	};

	// Array of texture indices
	unsigned int* m_textureIndices;

	void loadTextures();

	BitMapFile* getBMPData( std::string filename );
};
