//
//  Point3D.h
//  
//  3D point class to be used in 3D spacial modeling.
//
//  Created by Wade Okawa-Scannell
//  Copyright (c) 2013 Wade Okawa-Scannell. All rights reserved.

#ifndef POINT_3D_H_
#define POINT_3D_H_

#include <cmath>

// TODO: give point class an ID property

// Point class
class Point3D
{
public:
    Point3D() : x( 0 ), y( 0 ), z( 0 ) {}
	Point3D( float x0, float y0, float z0 ) : x( x0 ), y( y0 ), z( z0 ) {}
    float x; // x value
    float y; // y value
	float z; // z value
	
	// < operator starts with x value, moves to y y value in case of tie
	bool operator < ( Point3D const& other ) const
	{
		if ( x != other.x )
			return ( x < other.x );
		else if ( y != other.y )
			return ( y < other.y );
		else
			return ( z < other.z );
	}

	float distanceTo( Point3D const& other ) const
	{
		return sqrt( ( other.x - x ) * ( other.x - x ) + ( other.y - y ) * ( other.y - y ) + ( other.z - z ) * ( other.z - z ) );
	}
};

#endif