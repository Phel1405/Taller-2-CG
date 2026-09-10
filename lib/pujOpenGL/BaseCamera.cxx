// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include "BaseCamera.h"
#include <cmath>
#include <GL/glu.h>

// -------------------------------------------------------------------------
pujOpenGL::BaseCamera::
BaseCamera( )
  : Superclass( )
{
}

// -------------------------------------------------------------------------
pujOpenGL::BaseCamera::
~BaseCamera( )
{
}

// -------------------------------------------------------------------------
void pujOpenGL::BaseCamera::
InitializeFromBoundingBox( )
{
  this->InitializeFromBoundingBoxFlag = true;
}

// -------------------------------------------------------------------------
void pujOpenGL::BaseCamera::
ConfigureFromBounds( const float* b )
{
  if( this->InitializeFromBoundingBoxFlag )
  {
    float* l = this->LookAt.data( );
    float* e = this->Eye.data( );
    float* u = this->Up.data( );

    l[ 0 ] = ( b[ 1 ] + b[ 0 ] ) * 0.5;
    l[ 1 ] = ( b[ 3 ] + b[ 2 ] ) * 0.5;
    l[ 2 ] = ( b[ 5 ] + b[ 4 ] ) * 0.5;

    e[ 0 ] = ( ( b[ 1 ] - l[ 0 ] ) * 3 ) + l[ 0 ];
    e[ 1 ] = ( ( b[ 3 ] - l[ 1 ] ) * 3 ) + l[ 1 ];
    e[ 2 ] = ( ( b[ 5 ] - l[ 2 ] ) * 3 ) + l[ 2 ];

    float x = l[ 0 ] - e[ 0 ];
    float y = l[ 1 ] - e[ 1 ];
    float z = l[ 2 ] - e[ 2 ];
    float n = std::sqrt( ( x * x ) + ( y * y ) + ( z * z ) );
    if( n == 0 )
      n = 1;
    float f[ 3 ];
    f[ 0 ] = x / n;
    f[ 1 ] = y / n;
    f[ 2 ] = z / n;
    n = std::sqrt( ( f[ 0 ] * f[ 0 ] ) + ( f[ 2 ] * f[ 2 ] ) );
    if( n == 0 )
      n = 1;
    x = -( f[ 0 ] * f[ 1 ] / n );
    y = ( ( f[ 0 ] * f[ 0 ] ) + ( f[ 2 ] * f[ 2 ] ) ) / n;
    z = -( f[ 1 ] * f[ 2 ] / n );
    n = std::sqrt( ( x * x ) + ( y * y ) + ( z * z ) );
    if( n == 0 )
      n = 1;
    u[ 0 ] = x / n;
    u[ 1 ] = y / n;
    u[ 2 ] = z / n;

    x = b[ 1 ] - b[ 0 ];
    y = b[ 3 ] - b[ 2 ];
    z = b[ 5 ] - b[ 4 ];
    n = std::sqrt( ( x * x ) + ( y * y ) + ( z * z ) );
    this->ZBounds[ 0 ] = 1e-3;
    this->ZBounds[ 1 ] = n * 1e2;
  } // end if
}

// -------------------------------------------------------------------------
void pujOpenGL::BaseCamera::
Perspective( int w, int h )
{
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );
  gluPerspective(
    this->Fovy,
    float( w ) / float( ( h != 0 )? h: 1 ),
    this->ZBounds[ 0 ], this->ZBounds[ 1 ]
    );
}

// -------------------------------------------------------------------------
void pujOpenGL::BaseCamera::
KeyEvent( int k, int x, int y, bool special, bool up )
{
  // Do nothing
}

// -------------------------------------------------------------------------
void pujOpenGL::BaseCamera::
Draw( ) const
{
  gluLookAt(
    this->Eye[ 0 ], this->Eye[ 1 ], this->Eye[ 2 ],
    this->LookAt[ 0 ], this->LookAt[ 1 ], this->LookAt[ 2 ],
    this->Up[ 0 ], this->Up[ 1 ], this->Up[ 2 ]
    );
}

// eof - BaseCamera.cxx
