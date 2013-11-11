//
//  Quadtree.h
//  
//  Implementation of a Quadtree.
//  Uses AABB approximation for trimming the tree in queries.
//
//  Created by Wade Okawa-Scannell
//  Copyright (c) 2013 Wade Okawa-Scannell. All rights reserved.

#ifndef OCTREE_H_
#define OCTREE_H_

#include <vector>
#include <fstream>

#include "Point3D.h"

using namespace std;

class Octree
{
private:
	// OctreeNode Class
	class OctreeNode
	{
	public:
		// default constructor
		OctreeNode() :
		  NWN( 0 )
		, NEN( 0 )
		, SWN( 0 )
		, SEN( 0 )
		, NWF( 0 )
		, NEF( 0 )
		, SWF( 0 )
		, SEF( 0 )
		, x0_( 0 )
		, x1_( 0 )
		, y0_( 0 )
		, y1_( 0 )
		, z0_( 0 )
		, z1_( 0 )
		{
			xmid_ = 0;
			ymid_ = 0;
			zmid_ = 0;
		}

		// constructor
		OctreeNode( float x0, float x1, float y0, float y1, float z0, float z1, int depth ) :
		  NWN( 0 )
		, NEN( 0 )
		, SWN( 0 )
		, SEN( 0 )
		, NWF( 0 )
		, NEF( 0 )
		, SWF( 0 )
		, SEF( 0 )
		, x0_( x0 )
		, x1_( x1 )
		, y0_( y0 )
		, y1_( y1 )
		, z0_( z0 )
		, z1_( z1 )
		, depth_( depth )
		{
			xmid_ = x0_ + ( x1_ - x0_ ) / 2;
			ymid_ = y0_ + ( y1_ - y0_ ) / 2;
			zmid_ = z0_ + ( z1_ - z0_ ) / 2;
		}

		OctreeNode* NWN;           // NWN child pointer
		OctreeNode* NEN;           // NEN child pointer
		OctreeNode* SWN;           // SWN child pointer
		OctreeNode* SEN;           // SEN child pointer
		OctreeNode* NWF;           // NWF child pointer
		OctreeNode* NEF;           // NEF child pointer
		OctreeNode* SWF;           // SWF child pointer
		OctreeNode* SEF;           // SEF child pointer

		vector<Point3D> points;     // vector of points contained in node

		float x0_;                  // x0 boundary
		float x1_;                  // x1 boundary
		float y0_;                  // y0 boundary
		float y1_;                  // y1 boundary
		float z0_;                  // z0 boundary
		float z1_;                  // z1 boundary

		float xmid_;                // calculated x center
		float ymid_;                // calculated y center
		float zmid_;                // calculated z center

		int depth_;                 // depth of node

		int size()                  // returns number of points in node or total held by children
		{
			int ret = 0;
			size( this, ret );
			return ret;
		}

		void size( OctreeNode* q, int& ret )  // recursively adds all points held by children and self
		{
			if ( !q->NWN )
				ret += q->points.size();
			else
			{
				size( q->NWN, ret );
				size( q->NEN, ret );
				size( q->SWN, ret );
				size( q->SEN, ret );
				size( q->NWF, ret );
				size( q->NEF, ret );
				size( q->SWF, ret );
				size( q->SEF, ret );
			}
		}
	};

private:
	OctreeNode* root_;	// root node
	float x0_;				// x0 boundary
	float x1_;				// x1 boundary
	float y0_;				// y0 boundary
	float y1_;				// y1 boundary
	float z0_;				// z0 boundary
	float z1_;				// z1 boundary
	int k0_;				// k0 value
	int size_;				// points contained in Octree
public:

    //: Main constructor.  When a leaf node contains more than split_k
    //  points it must be split, creating four children.
    Octree( float          x0,
            float          x1,
            float          y0,
            float          y1,
			float          z0,
            float          z1,
            int            k0 );

    //: Copy constructor.  All values must be copied, including the
    //  bounds and the k0 value.
	Octree( Octree const& other );

    //: Destructor
    ~Octree();

    //: Assignment operator.  Behaves just like the copy constructor
    //  except that it avoids self-assignment.
    Octree& operator=( Octree const& other );

    //: Access the bounds on the area covered by the tree.
    float x0() const;
    float x1() const;
    float y0() const;
    float y1() const;
	float z0() const;
    float z1() const;

    //: How many points are stored in the octree?
    int size() const;

    //: Output information about the leaf node whose region contains the
    //  p.  This includes the x and y bounds, the number of points and
    //  the points themselves. These points must be ordered by
    //  increasing x value, with ties broken by y values.
    void print_leaf( Point3D      const& p,
                     ofstream   & ostr );

    //: Output information about the entire tree, including the x,y
    //  bounds and the number of points at each leaf node. Use one line
    //  for each node (both leaf and internal).  Use leading '+' to
    //  represent different depth.  This is similar to the sideways
    //  printing we did for binary search trees.  The order of recursion
    //  (and therefore output) should be NW, NE, SW, SE.  See example
    //  output.
    void print_tree( ofstream   & ostr );

    //: Remove all points from the tree.
    void clear();

	//: Insert a point into the tree.  This should return false if and
	//  only if the point is outside the region covered by the quad tree.
	bool insert( Point3D const& p );

	//: Return the points in the quad tree that fall inside the region
	//  covered the sphere.  A point must inside the sphere rather than
	//  on the boundary.
	vector<Point3D> find( Point3D const& center,
	                      float          radius ) const;

    //: Erase the points in the quad tree that fall inside the region
    //  covered the sphere.  A point must inside the sphere rather than
    //  on the boundary.  Return the number of points erased.
	int erase( Point3D const& center,
	           float          radius );

private:
	// recursively deletes nodes including root
	void destroy_tree( OctreeNode* q );
	
	// returns pointer to OctreeNode
	// recursively copies all nodes and sets appropriate children pointers
	OctreeNode* copy_tree( OctreeNode* old_root );
	
	// helper function to insert
	// checks node to see if it has children. if it does, it moves to the
	// proper child. When it reaches a leaf, it checks if it can hold the point.
	// if not, it creates four children nodes and reassigns its points to the proper
	// children and inserts the point correctly. it works recursively and will split
	// again if necessary.
	bool insert( const Point3D p, OctreeNode*& q );

	// helper function to print_leaf
	// recursively find and returns node that should contain the point.
	// it will error if the point is not in bounds
	OctreeNode* print_leaf( const Point3D p, OctreeNode*& q );

	// helper function to print_tree
	// recursively finds leaf nodes and prints their information
	void print_tree( ofstream& ostr, OctreeNode*& q );
	
	// helper function to find
	// recursively finds all leaf nodes and pushes back all points within the cirscle
	void find( Point3D const& center, float radius, OctreeNode* q, vector <Point3D>& ret ) const;
	
	// helper function to erase
	// recursively finds all leaf nodes and removes all points within the sphere
	// calls collapse function
	void erase( Point3D const& center, float radius, OctreeNode*& q, int& ret );
	
	// checks to see if the node is collapsible (sum of children is < k0)
	// if so, copies children points into parent points, and deletes the children.
	void collapse( OctreeNode*& q );

	// uses an AABB test
	// checks to see if the child node should be entered given a point and a radius
	bool shouldEnterBoxTest( OctreeNode* q, Point3D center, float radius ) const;

	// determines which child the point lies inside
	unsigned char whichChild( Point3D point, OctreeNode* q ) const;
};

#endif