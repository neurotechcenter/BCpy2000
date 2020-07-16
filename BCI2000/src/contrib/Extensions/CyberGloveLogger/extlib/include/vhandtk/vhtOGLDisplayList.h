/********************************************************************
 FILE: $Id: vhtOGLDisplayList.h,v 1.13 2002/08/28 23:16:20 jsilver Exp $
 AUTHOR: C Ullrich
 DATE: 1998

 DESCRIPTION:
  - OpenGL display list representation

 HISTORY:
  -

           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/
#ifndef VHT_OGL_DISPLAY_LIST
#define VHT_OGL_DISPLAY_LIST

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#ifdef _WIN32
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0400
	#endif
	#include <windows.h>
	#include <string>
#else
	#include <string.h>
#endif

#if !defined(_NO_GL)
	#include <GL/gl.h>
#endif


class vhtGeometry;
class vhtConvexHull;
class vhtTransform3D;


//: OpenGL display list - Depreciated.
// Methods for generating and indexing geometry specific OpenGL display lists.
// Use this class to display nodes from the haptic scene graph.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtOGLDisplayList {

public:	
	static void displayConvexHull(vhtConvexHull *hull);
	//: Display the given convex hull.
	// No display list is generated in this case. All rendering is done in
	// immediate mode.
public:
    vhtOGLDisplayList();
    //: Construct null display list.
    vhtOGLDisplayList( vhtOGLDisplayList &dList );
    //: Copy constructor.
    vhtOGLDisplayList( vhtGeometry *geom );
    //: Construct a display list for the indicated geometry.
    // Subsequent calls need only execute.
    ~vhtOGLDisplayList();
    //: Destructor.

    inline void setGeometry( vhtGeometry *geom ) 
	{ 
	    m_geom = geom; 
	    generateList();
	}
    //: Set the geometry.
    // Also constructs the display list.
    inline void setColor( double color[4] ) 
	{
	    for( int i=0; i < 4; i++ )
		m_color[i] = (float) color[i];

	    generateList();
	}
    //: Set color parameters.
    // Color is specified in RGBA format where each index is in the range 0 to 1.
    
#if defined(_NO_GL)
    inline int  getListId() { return m_listId; }
    //: Get the OpenGL list id.
    inline void execute() const { ; }
    //: Execute this list
    // Depreciated.
#else
    inline GLint  getListId() { return m_listId; }
    //: Get the OpenGL list id.
    inline void execute() const { glCallList( m_listId ); }
    //: Execute this list
    // Instructs OpenGL to render this display list.
#endif    


private:
#if defined(_NO_GL)
    int m_listId;
#else
    GLint        m_listId;
#endif
    vhtGeometry *m_geom;
    float        m_color[4];

    void         generateList();
    //: Generate the display list.
    // Generates the list corresponding to the current geometry and color
    // settings.
};

#endif


