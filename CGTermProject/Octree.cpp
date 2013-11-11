//
//  Octree.cpp
//  
//  Implementation of a Octree.
//  Uses AABB approximation for trimming the tree in queries.
//
//  Created by Wade Okawa-Scannell
//  Copyright (c) 2013 Wade Okawa-Scannell. All rights reserved.

#include "Octree.h"
#include <fstream>
#include <algorithm>
#include <cmath>

// constructor
Octree::Octree( float x0, float x1, float y0, float y1, float z0, float z1, int k0 ) :
  x0_( x0 )
, x1_( x1 )
, y0_( y0 )
, y1_( y1 )
, z0_( z0 )
, z1_( z1 )
, k0_( k0 )
{
	// initialize size to 0
	size_ = 0;
	// create root node
	root_ = new OctreeNode( x0_, x1_, y0_, y1_, z0_, z1_, 0 );
}

// copy constructor
Octree::Octree( Octree const& other )
{
	// copy variable data
	x0_ = other.x0_;
	x1_ = other.x1_;
	y0_ = other.y0_;
	y1_ = other.y1_;
	z0_ = other.z0_;
	z1_ = other.z1_;
	k0_ = other.k0_;
	size_ = other.size_;

	// calls copy_tree
	root_ = this->copy_tree( other.root_ );
}

// copy_tree
Octree::OctreeNode* Octree::copy_tree( OctreeNode* old_root )
{
	// if NULL root
	if ( !old_root )
		return 0;

	// make new node
	OctreeNode *p;
	p = new OctreeNode;

	// copy variable data
	p->points = old_root->points;
	p->x0_ = old_root->x0_;
	p->x1_ = old_root->x1_;
	p->y0_ = old_root->y0_;
	p->y1_ = old_root->y1_;
	p->z0_ = old_root->z0_;
	p->z1_ = old_root->z1_;
	p->xmid_ = old_root->xmid_;
	p->ymid_ = old_root->ymid_;
	p->zmid_ = old_root->zmid_;
	p->depth_ = old_root->depth_;

	// recursively call
	if ( old_root->NWN )
	{
		p->NWN = copy_tree( old_root->NWN );
	}
	if ( old_root->NEN )
	{
		p->NEN = copy_tree( old_root->NEN );
	}
	if ( old_root->SWN )
	{
		p->SWN = copy_tree( old_root->SWN );
	}
	if ( old_root->SEN )
	{
		p->SEN = copy_tree( old_root->SEN );
	}
	if ( old_root->NWF )
	{
		p->NWF = copy_tree( old_root->NWF );
	}
	if ( old_root->NEF )
	{
		p->NEF = copy_tree( old_root->NEF );
	}
	if ( old_root->SWF )
	{
		p->SWF = copy_tree( old_root->SWF );
	}
	if ( old_root->SEF )
	{
		p->SEF = copy_tree( old_root->SEF );
	}

	return p; // return node
}

// destructor
Octree::~Octree()
{
	// call destroy_tree
	this -> destroy_tree( root_ );

	// set pointer to 0
    root_ = 0;
}

void Octree::destroy_tree( OctreeNode* q )
{
	// if NULL node
	if ( !q )
	{
		return;
	}

	// recursively call
	destroy_tree( q->NWN );
	destroy_tree( q->NEN );
	destroy_tree( q->SWN );
	destroy_tree( q->SEN );
	destroy_tree( q->NWF );
	destroy_tree( q->NEF );
	destroy_tree( q->SWF );
	destroy_tree( q->SEF );

	// delete node
	delete q;
}

// assignment operator
Octree& Octree::operator=( Octree const& other )
{
	// if roots are not the same
	if ( other.root_ != this->root_ )
    {
		// call destroy_tree
        this -> destroy_tree( root_ );
		// call copy_tree
        root_ = this->copy_tree( other.root_ );
		// make sizes same
        size_ = other.size_;
    }

	// return node pointer
    return *this;
}

// clear
void Octree::clear()
{
	// destroy entire tree
	this->destroy_tree( root_ );

	// make new root_ with same original data
	root_ = new OctreeNode( x0_, x1_, y0_, y1_, z0_, z1_, 0 );
}

// return x0
float Octree::x0() const
{
	return x0_;
}

// return x1
float Octree::x1() const
{
	return x1_;
}

// return y0
float Octree::y0() const
{
	return y0_;
}

// return y1
float Octree::y1() const
{
	return y1_;
}

// return z0
float Octree::z0() const
{
	return z0_;
}

// return y1
float Octree::z1() const
{
	return z1_;
}

// return size
int Octree::size() const
{
	return size_;
}

// insert
bool Octree::insert( Point3D const& p )
{
	// check bounds
	if ( p.x >= x0_ && p.x < x1_ && p.y >= y0_ && p.y < y1_ && p.z >= z0_ && p.z < z1_ )
	{
		// call helper insert
		return insert( p, root_ );
	}
	else
	{
		// out of bounds
		return false;
	}
}

// insert helper
bool Octree::insert( const Point3D p, OctreeNode*& q )
{
	// if leaf node
	if ( !q->NWN )
	{
		// if has room for another point
		if ( q->points.size() < k0_ )
		{
			q->points.push_back( p );
			this->size_++;
			return true;
		}
		else
		{			
			// creating the children
			q->NWN = new OctreeNode( q->x0_, q->xmid_, q->y0_, q->ymid_, q->z0_, q->zmid_, q->depth_ + 1 );
			q->NEN = new OctreeNode( q->xmid_, q->x1_, q->y0_, q->ymid_, q->z0_, q->zmid_, q->depth_ + 1 );
			q->SWN = new OctreeNode( q->x0_, q->xmid_, q->ymid_, q->y1_, q->z0_, q->zmid_, q->depth_ + 1 );
			q->SEN = new OctreeNode( q->xmid_, q->x1_, q->ymid_, q->y1_, q->z0_, q->zmid_, q->depth_ + 1 );
			q->NWF = new OctreeNode( q->x0_, q->xmid_, q->y0_, q->ymid_, q->zmid_, z1_, q->depth_ + 1 );
			q->NEF = new OctreeNode( q->xmid_, q->x1_, q->y0_, q->ymid_, q->zmid_, z1_, q->depth_ + 1 );
			q->SWF = new OctreeNode( q->x0_, q->xmid_, q->ymid_, q->y1_, q->zmid_, z1_, q->depth_ + 1 );
			q->SEF = new OctreeNode( q->xmid_, q->x1_, q->ymid_, q->y1_, q->zmid_, z1_, q->depth_ + 1 );

			// put points in proper children
			for ( int i=0; i < q->points.size(); i++ )
			{
				// move to proper child
				const unsigned int child = whichChild( q->points[ i ], q );

				switch ( child )
				{
					case 0:
					{
						q->NWN->points.push_back( q->points[ i ] );
						break;
					}
					case 1:
					{
						q->NEN->points.push_back( q->points[ i ] );
						break;
					}
					case 2:
					{
						q->SWN->points.push_back( q->points[ i ] );
						break;
					}
					case 3:
					{
						q->SEN->points.push_back( q->points[ i ] );
						break;
					}
					case 4:
					{
						q->NWF->points.push_back( q->points[ i ] );
						break;
					}
					case 5:
					{
						q->NEF->points.push_back( q->points[ i ] );
						break;
					}
					case 6:
					{
						q->SWF->points.push_back( q->points[ i ] );
						break;
					}
					case 7:
					{
						q->SEF->points.push_back( q->points[ i ] );
						break;
					}
					default:
					{
						return false;
						break;
					}
				}
			}
			// clear parent vector
			q->points.clear();
		}
	}

	// find which child to put point in and recursively insert
	const unsigned int child = whichChild( p, q );

	switch ( child )
	{
		case 0:
		{
			return insert( p, q->NWN );
			break;
		}
		case 1:
		{
			return insert( p, q->NEN );
			break;
		}
		case 2:
		{
			return insert( p, q->SWN );
			break;
		}
		case 3:
		{
			return insert( p, q->SEN );
			break;
		}
		case 4:
		{
			return insert( p, q->NWF );
			break;
		}
		case 5:
		{
			return insert( p, q->NEF );
			break;
		}
		case 6:
		{
			return insert( p, q->SWF );
			break;
		}
		case 7:
		{
			return insert( p, q->SEF );
			break;
		}
		default:
		{
			return false;
			break;
		}
	}
}

// print_leaf
void Octree::print_leaf( Point3D const& p, ofstream& ostr )
{
	// out of range
	if ( !( p.x >= x0_ && p.x < x1_ && p.y >= y0_ && p.y < y1_ && p.z >= z0_ && p.z < z1_ ) )
	{
		ostr << "print_leaf: " << p.x << "," << p.y << "," << p.z << " out of range" << endl;
		return;
	}

	// find leaf node recursively
	OctreeNode* test = print_leaf( p, root_ );

	// output
	ostr << "print_leaf:" << endl;
	ostr << "  " << "x0 " << test->x0_ << ", " << "x1 " << test->x1_ << endl;
	ostr << "  " << "y0 " << test->y0_ << ", " << "y1 " << test->y1_ << endl;
	ostr << "  " << "z0 " << test->z0_ << ", " << "z1 " << test->z1_ << endl;
	ostr << "  " << "num points " << test->points.size() << endl;
	ostr << "  " << "points";

	// sort vector
	sort( test->points.begin(), test->points.end() );
	
	// output vector
	if ( test->points.size() == 0 )
	{
		ostr << " -none-" << endl;
	}
	else
	{
		for ( int i=0; i<test->points.size(); i++ )
		{
			ostr << " " << test->points[ i ].x << "," << test->points[ i ].y << "," << test->points[ i ].z;
		}
		ostr << endl;
	}
}

// print_leaf helper
Octree::OctreeNode* Octree::print_leaf( const Point3D p, OctreeNode*& q )
{
	if ( !q->NWN )
	{
		// return proper node
		return q;
	}

	// move to proper child
	const unsigned int child = whichChild( p, q );

	switch ( child )
	{
		case 0:
		{
			return print_leaf( p, q->NWN );
			break;
		}
		case 1:
		{
			return print_leaf( p, q->NEN );
			break;
		}
		case 2:
		{
			return print_leaf( p, q->SWN );
			break;
		}
		case 3:
		{
			return print_leaf( p, q->SEN );
			break;
		}
		case 4:
		{
			return print_leaf( p, q->NWF );
			break;
		}
		case 5:
		{
			return print_leaf( p, q->NEF );
			break;
		}
		case 6:
		{
			return print_leaf( p, q->SWF );
			break;
		}
		case 7:
		{
			return print_leaf( p, q->SEF );
			break;
		}
		default:
		{
			return 0;
			break;
		}
	}
}

// print_tree
void Octree::print_tree( ofstream& ostr )
{
	// call helper print
	print_tree( ostr, root_ );
}

// print_tree helper
void Octree::print_tree( ofstream& ostr, OctreeNode*& q )
{
	// if node exists
	if ( q )
	{
		// recursively call on all children
		print_tree( ostr, q->NWN );
		print_tree( ostr, q->NEN );
		print_tree( ostr, q->SWN );
		print_tree( ostr, q->SEN );
		print_tree( ostr, q->NWF );
		print_tree( ostr, q->NEF );
		print_tree( ostr, q->SWF );
		print_tree( ostr, q->SEF );

		// if leaf node
		if ( !q->NWN )
		{
			for ( int i=0; i < q->depth_; i++ )
				ostr << "+";
			ostr << "leaf: " << q->x0_ << ", " << q->x1_ << ", " << q->y0_ << ", " << q->y1_ << ", " << q->z0_ << ", " << q->z1_ << " pnts: " << q->points.size() << endl;
		}
	}
}

// find all points within a sphere
vector<Point3D> Octree::find( Point3D const& center, float radius ) const
{
	// create return vector
	vector<Point3D> ret;

	// call helper find
	find( center, radius, root_, ret );

	// sort return vector
	sort( ret.begin(), ret.end() );

	return ret;
}

// find helper
void Octree::find( Point3D const& center, float radius, OctreeNode* q, vector<Point3D>& ret )const
{
	// if leaf node
	if ( !q->NWN )
	{
		// check points
		for ( int i=0; i < q->points.size(); i++ )
		{
			if ( center.distanceTo( q->points[ i ] ) < radius )
			{
				// if in sphere push_back to return vector
				ret.push_back( q->points[ i ] );
			}
		}
	}
	else
	{
		// trim tree and recursively call on children
		if ( shouldEnterBoxTest( q->NWN, center, radius) )
		{
			find( center, radius, q->NWN, ret );
		}
		if ( shouldEnterBoxTest( q->NEN, center, radius) )
		{
			find( center, radius, q->NEN, ret );
		}
		if ( shouldEnterBoxTest( q->SWN, center, radius) )
		{
			find( center, radius, q->SWN, ret );
		}
		if ( shouldEnterBoxTest( q->SEN, center, radius) )
		{
			find( center, radius, q->SEN, ret );
		}
		if ( shouldEnterBoxTest( q->NWF, center, radius) )
		{
			find( center, radius, q->NWF, ret );
		}
		if ( shouldEnterBoxTest( q->NEF, center, radius) )
		{
			find( center, radius, q->NEF, ret );
		}
		if ( shouldEnterBoxTest( q->SWF, center, radius) )
		{
			find( center, radius, q->SWF, ret );
		}
		if ( shouldEnterBoxTest( q->SEF, center, radius) )
		{
			find( center, radius, q->SEF, ret );
		}
	}
}

// erase all points within a sphere
int Octree::erase( Point3D const& center, float radius )
{
	// number of erased
	int ret = 0;

	// call helper erase
	erase ( center, radius, root_, ret );

	// resize
	size_ -= ret;
	
	return ret;
}

// erase helper
void Octree::erase( Point3D const& center, float radius, OctreeNode*& q, int& ret )
{
	// if leaf node
	if ( !q->NWN )
	{
		// check points
		for ( int i=0; i < q->points.size(); i++ )
		{
			if ( center.distanceTo( q->points[ i ] ) < radius )
			{
				// if in sphere, erase
				q->points.erase( q->points.begin() + i );

				// step back iterator
				i--;

				// increase number of erased
				ret++;
			}

		}
	}
	else
	{
		// trim tree and recursively call on children
		if ( shouldEnterBoxTest( q->NWN, center, radius ) )
		{
			erase( center, radius, q->NWN, ret );
		}
		if ( shouldEnterBoxTest( q->NEN, center, radius ) )
		{
			erase( center, radius, q->NEN, ret );
		}
		if ( shouldEnterBoxTest( q->SWN, center, radius ) )
		{
			erase( center, radius, q->SWN, ret );
		}
		if ( shouldEnterBoxTest( q->SEN, center, radius ) )
		{
			erase( center, radius, q->SEN, ret );
		}
		if ( shouldEnterBoxTest( q->NWF, center, radius ) )
		{
			erase( center, radius, q->NWF, ret );
		}
		if ( shouldEnterBoxTest( q->NEF, center, radius ) )
		{
			erase( center, radius, q->NEF, ret );
		}
		if ( shouldEnterBoxTest( q->SWF, center, radius ) )
		{
			erase( center, radius, q->SWF, ret );
		}
		if ( shouldEnterBoxTest( q->SEF, center, radius ) )
		{
			erase( center, radius, q->SEF, ret );
		}

		// collapse node if necessary
		collapse( q );
	}
}

// collapse
void Octree::collapse( OctreeNode*& q )
{
	// if needs to be collapsed
	if ( q->size() <= k0_ )
	{
		for ( int i=0; i<q->NWN->points.size(); i++ )
		{
			// move points up
			q->points.push_back( q->NWN->points[ i ] );
		}
		// delete node
		delete q->NWN;
		// set pointer to 0
		q->NWN = 0;

		for ( int i=0; i<q->NEN->points.size(); i++ )
		{
			// move points up
			q->points.push_back( q->NEN->points[ i ] );
		}
		// delete node
		delete q->NEN;
		// set pointer to 0
		q->NEN = 0;

		for ( int i=0; i<q->SWN->points.size(); i++ )
		{
			// move points up
			q->points.push_back( q->SWN->points[ i ] );
		}
		// delete node
		delete q->SWN;
		// set pointer to 0
		q->SWN = 0;

		for ( int i=0; i<q->SEN->points.size(); i++ )
		{
			// move points up
			q->points.push_back( q->SEN->points[ i ] );
		}
		// delete node
		delete q->SEN;
		// set pointer to 0
		q->SEN = 0;

		for ( int i=0; i<q->NWF->points.size(); i++ )
		{
			// move points up
			q->points.push_back( q->NWF->points[ i ] );
		}
		// delete node
		delete q->NWF;
		// set pointer to 0
		q->NWF = 0;

		for ( int i=0; i<q->NEF->points.size(); i++ )
		{
			// move points up
			q->points.push_back( q->NEF->points[ i ] );
		}
		// delete node
		delete q->NEF;
		// set pointer to 0
		q->NEF = 0;

		for ( int i=0; i<q->SWF->points.size(); i++ )
		{
			// move points up
			q->points.push_back( q->SWF->points[ i ] );
		}
		// delete node
		delete q->SWF;
		// set pointer to 0
		q->SWF = 0;

		for ( int i=0; i<q->SEF->points.size(); i++ )
		{
			// move points up
			q->points.push_back( q->SEF->points[ i ] );
		}
		// delete node
		delete q->SEF;
		// set pointer to 0
		q->SEF = 0;
	}
}

bool Octree::shouldEnterBoxTest( OctreeNode* q, Point3D center, float radius ) const
{
	return    ( abs( q->xmid_ - center.x ) <= ( radius + ( q->xmid_ - q->x0_ ) ) )
	       && ( abs( q->ymid_ - center.y ) <= ( radius + ( q->ymid_ - q->y0_ ) ) )
		   && ( abs( q->zmid_ - center.z ) <= ( radius + ( q->zmid_ - q->z0_ ) ) );
}

// determines which child the point is inside
unsigned char Octree::whichChild( Point3D p, OctreeNode* q ) const
{
	if ( p.x < q->xmid_ && p.y < q->ymid_ && p.z < q->zmid_ )
	{
		return 0;
	}
	else if ( p.x >= q->xmid_ && p.y < q->ymid_ && p.z < q->zmid_ )
	{
		return 1;
	}
	else if ( p.x < q->xmid_ && p.y >= q->ymid_ && p.z < q->zmid_ )
	{
		return 2;
	}
	else if ( p.x >= q->xmid_ && p.y >= q->ymid_ && p.z < q->zmid_ )
	{
		return 3;
	}
	else if ( p.x < q->xmid_ && p.y < q->ymid_ && p.z >= q->zmid_ )
	{
		return 4;
	}
	else if ( p.x >= q->xmid_ && p.y < q->ymid_ && p.z >= q->zmid_ )
	{
		return 5;
	}
	else if ( p.x < q->xmid_ && p.y >= q->ymid_ && p.z >= q->zmid_ )
	{
		return 6;
	}
	else
	{
		return 7;
	}
}