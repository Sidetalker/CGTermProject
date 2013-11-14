//
//  Textures.h
//  CGTermProject
//
//  Created by Kevin Sullivan on 11/13/13.
//  Copyright (c) 2013 com.sideapps. All rights reserved.
//

#ifndef __CGTermProject__Textures__
#define __CGTermProject__Textures__

#include <iostream>
#include <fstream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#endif /* defined(__CGTermProject__Textures__) */

#define TEXTURE_COUNT 1

// Array of texture indices
static unsigned int texture[TEXTURE_COUNT];

class Textures
{
public:
    // Static function to load textures
    static void loadTextures();
};
