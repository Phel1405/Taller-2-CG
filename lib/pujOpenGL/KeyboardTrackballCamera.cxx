// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================


/* TODO
   #include <iostream>
*/

#include "KeyboardTrackballCamera.h"
#include <cmath>
#include <cstdlib>
#include <GL/gl.h>

// -------------------------------------------------------------------------
pujOpenGL::KeyboardTrackballCamera::
KeyboardTrackballCamera( )
  : Superclass( )
{
}

// -------------------------------------------------------------------------
pujOpenGL::KeyboardTrackballCamera::
~KeyboardTrackballCamera( )
{
}

// -------------------------------------------------------------------------
void pujOpenGL::KeyboardTrackballCamera::
ConfigureFromBounds( const float* b )
{
  this->Superclass::ConfigureFromBounds( b );
  if( this->InitializeFromBoundingBoxFlag )
    this->_Reset( );
}

// -------------------------------------------------------------------------
void pujOpenGL::KeyboardTrackballCamera::
KeyEvent( int k, int x, int y, bool special, bool up )
{
  static const float _dA = std::atan( float( 1 ) ) / float( 45 );
  static float _oA = 1;

  // TODO: std::cout << special << " " << up << " " << k << std::endl;
  if( special && k == 100 /*LEFT*/ )
    {
      this->Theta -= _dA * _oA;
      this->_Update( );
    }
  else if( special && k == 102 /*RIGHT*/ )
    {
      this->Theta += _dA * _oA;
      this->_Update( );
    }
  else if( special && k == 101 /*UP*/ )
    {
      this->Phi -= _dA * _oA;
      this->_Update( );
    }
  else if( special && k == 103 /*DOWN*/ )
    {
      this->Phi += _dA * _oA;
      this->_Update( );
    }
  else if( !special && k == '+' )
    {
      this->DeltaRadius = -1;
      this->_Update( );
    }
  else if( !special && k == '-' )
    {
      this->DeltaRadius = 1;
      this->_Update( );
    }
  else if( !special && ( k == 'r' || k == 'R' ) )
    {
      this->_Reset( );
      this->_Update( );
    }
  else if( special && ( k == 112 || k == 113 ) /*SHIFT*/ )
    _oA = ( up )? 1: 10;
  else if( !special && k == 27 /*ESC*/ )
    std::exit( EXIT_SUCCESS );
}

// -------------------------------------------------------------------------
void pujOpenGL::KeyboardTrackballCamera::
Draw( ) const
{
  glMultMatrixf( this->Transform.data( ) );
}

// -------------------------------------------------------------------------
void pujOpenGL::KeyboardTrackballCamera::
_Reset( )
{
  float* T = this->Transform.data( );
  float* L = this->LookAt.data( );
  float* E = this->Eye.data( );

  float x = E[ 0 ] - L[ 0 ];
  float y = E[ 1 ] - L[ 1 ];
  float z = E[ 2 ] - L[ 2 ];

  this->Radius = std::sqrt( ( x * x ) + ( y * y ) + ( z * z ) );
  this->OffRadius = this->Radius / 100;
  this->DeltaRadius = this->Phi = this->Theta = 0;

  std::fill( T, T + 16, 0 );
  T[ 0 ] = T[ 5 ] = T[ 10 ] = T[ 15 ] = 1;
  T[ 12 ] = -L[ 0 ];
  T[ 13 ] = -L[ 1 ];
  T[ 14 ] = -L[ 2 ] - this->Radius;
}

// -------------------------------------------------------------------------
void pujOpenGL::KeyboardTrackballCamera::
_Update( )
{
  float* T = this->Transform.data( );
  float A = T[ 0 ]; float D = T[ 4 ]; float G = T[  8 ]; float J = T[ 12 ];
  float B = T[ 1 ]; float E = T[ 5 ]; float H = T[  9 ]; float K = T[ 13 ];
  float C = T[ 2 ]; float F = T[ 6 ]; float I = T[ 10 ]; float L = T[ 14 ];
  float R = this->Radius;

  if( this->Phi != 0 )
    {
      float s = std::sin( this->Phi );
      float c = std::cos( this->Phi );

      T[  1 ] = ( B * c ) - ( C * s );
      T[  2 ] = ( B * s ) + ( C * c );
      T[  5 ] = ( E * c ) - ( F * s );
      T[  6 ] = ( E * s ) + ( F * c );
      T[  9 ] = ( H * c ) - ( I * s );
      T[ 10 ] = ( H * s ) + ( I * c );
      T[ 13 ] = ( K * c ) - ( L * s ) - ( R * s );
      T[ 14 ] = ( K * s ) + ( L * c ) + ( R * c ) - R;
    }
  else if( this->Theta != 0 )
    {
      float s = std::sin( this->Theta );
      float c = std::cos( this->Theta );

      T[  0 ] = ( C * s ) + ( A * c );
      T[  2 ] = ( C * c ) - ( A * s );
      T[  4 ] = ( F * s ) + ( D * c );
      T[  6 ] = ( F * c ) - ( D * s );
      T[  8 ] = ( I * s ) + ( G * c );
      T[ 10 ] = ( I * c ) - ( G * s );
      T[ 12 ] = ( L * s ) + ( J * c ) + ( R * s );
      T[ 14 ] = ( L * c ) - ( J * s ) + ( R * c ) - R;
    }
  else if( this->DeltaRadius != 0 )
    {
      float d = this->DeltaRadius * this->OffRadius;
      T[ 14 ] -= d;
      this->Radius += d;
    } // end if

  // Ensure homogeneousness
  T[ 3 ] = T[ 7 ] = T[ 11 ] = 0;
  T[ 15 ] = 1;

  // Reset angles and delta radius
  this->DeltaRadius = this->Phi = this->Theta = 0;
}

// eof - KeyboardTrackballCamera.cxx
