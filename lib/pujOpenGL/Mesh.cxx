// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include "Mesh.h"
#include <cmath>
#include <GL/gl.h>

// -------------------------------------------------------------------------
pujOpenGL::Mesh::
Mesh( )
  : Superclass( )
{
}

// -------------------------------------------------------------------------
pujOpenGL::Mesh::
~Mesh( )
{
}

// -------------------------------------------------------------------------
void pujOpenGL::Mesh::
Clear( )
{
  this->Vertices.clear( );
  this->Faces.clear( );
  this->Normals.Data.clear( );
  this->Normals.Indices.clear( );
  this->TexCoords.Data.clear( );
  this->TexCoords.Indices.clear( );
}

// -------------------------------------------------------------------------
void pujOpenGL::Mesh::
_UpdateBoundingBox( )
{
  if( this->Vertices.size( ) > 2 )
  {
    this->BoundingBox[ 0 ] = this->BoundingBox[ 1 ] = this->Vertices[ 0 ];
    this->BoundingBox[ 2 ] = this->BoundingBox[ 3 ] = this->Vertices[ 1 ];
    this->BoundingBox[ 4 ] = this->BoundingBox[ 5 ] = this->Vertices[ 2 ];
    for( std::size_t i = 3; i < this->Vertices.size( ); i += 3 )
    {
      float x = this->Vertices[ i ];
      float y = this->Vertices[ i + 1 ];
      float z = this->Vertices[ i + 2 ];

      this->BoundingBox[ 0 ] = std::min( x, this->BoundingBox[ 0 ] );
      this->BoundingBox[ 1 ] = std::max( x, this->BoundingBox[ 1 ] );
      this->BoundingBox[ 2 ] = std::min( y, this->BoundingBox[ 2 ] );
      this->BoundingBox[ 3 ] = std::max( y, this->BoundingBox[ 3 ] );
      this->BoundingBox[ 4 ] = std::min( z, this->BoundingBox[ 4 ] );
      this->BoundingBox[ 5 ] = std::max( z, this->BoundingBox[ 5 ] );
    } // end for

    float x = this->BoundingBox[ 1 ] -this->BoundingBox[ 0 ];
    float y = this->BoundingBox[ 3 ] -this->BoundingBox[ 2 ];
    float z = this->BoundingBox[ 5 ] -this->BoundingBox[ 4 ];
    this->LongestDiagonal = std::sqrt( ( x * x ) + ( y * y ) + ( z * z ) );
  }
  else
  {
    this->BoundingBox[ 0 ] = std::numeric_limits< float >::max( );
    this->BoundingBox[ 1 ] = std::numeric_limits< float >::lowest( );
    this->BoundingBox[ 2 ] = std::numeric_limits< float >::max( );
    this->BoundingBox[ 3 ] = std::numeric_limits< float >::lowest( );
    this->BoundingBox[ 4 ] = std::numeric_limits< float >::max( );
    this->BoundingBox[ 5 ] = std::numeric_limits< float >::lowest( );
    this->LongestDiagonal = 0;
  } // end if
}

// -------------------------------------------------------------------------
void pujOpenGL::Mesh::
_SelfDraw( ) const
{
  for( unsigned long fId = 0; fId < this->Faces.size( ); ++fId )
  {
    const std::vector< unsigned long >* f = &( this->Faces[ fId ] );
    const std::vector< unsigned long >* n = nullptr;
    if( this->Normals.Data.size( ) > 0 )
      n = &( this->Normals.Indices[ fId ] );
    if( f->size( ) > 2 ) glBegin( GL_POLYGON );
    else                 glBegin( GL_LINE_LOOP );
    for( unsigned long vId = 0; vId < f->size( ); ++vId )
    {
      if( n != nullptr )
        glNormal3fv( this->Normals.Data.data( ) + ( ( *n )[ vId ] * 3 ) );
      glVertex3fv( this->Vertices.data( ) + ( ( *f )[ vId ] * 3 ) );
    } // end for
    glEnd( );
  } // end for
}

// eof - Mesh.cxx
