#ifndef VHT_VOL_INT_H
#define VHT_VOL_INT_H

#ifdef WIN32
#   include <stdlib.h>
#endif

#if defined(_WIN32)
#if defined(__cplusplus)
extern "C" {
#endif
#else
#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif
#endif
/*
   ============================================================================
   constants
   ============================================================================
*/

#define MAX_VERTS 500     /* maximum number of polyhedral vertices */
#define MAX_FACES 500     /* maximum number of polyhedral faces */
#define MAX_POLYGON_SZ 3 /* maximum number of verts per polygonal face */

#define X 0
#define Y 1
#define Z 2

/*
   ============================================================================
   data structures
   ============================================================================
*/

typedef struct {
  int numVerts;
  double norm[3];
  double w;
  int verts[MAX_POLYGON_SZ];
  struct polyhedron *poly;
} FACE;

typedef struct polyhedron {
  int numVerts, numFaces;
  double verts[MAX_VERTS][3];
  FACE faces[MAX_FACES];
} POLYHEDRON;


void compVolumeIntegrals(POLYHEDRON *p);
void vhtVol_getCenterOfMass( double cm[3] );
void vhtVol_getMOIMatrix( double mass, double moi[3][3] );


#if defined(_WIN32)
#if defined(__cplusplus)
}
#endif
#else
#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif
#endif

#endif


