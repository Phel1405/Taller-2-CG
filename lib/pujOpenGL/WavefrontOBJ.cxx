// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include "WavefrontOBJ.h"

#include <fstream>
#include <sstream>

// -------------------------------------------------------------------------
pujOpenGL::WavefrontOBJ::
WavefrontOBJ( const std::string& fname )
  : Superclass( )
{
  std::ifstream ffile( fname, std::ios::in | std::ios::binary );
  if( ffile.is_open( ) )
  {
    std::stringstream file;
    file << ffile.rdbuf( );

    this->Clear( );
    std::string line;
    while( std::getline( file, line ) )
    {
      std::istringstream iss( line );
      std::string tag;
      iss >> tag;

      if( tag == "v" )
      {
        float x, y, z;
        iss >> x >> y >> z;
        this->Vertices.push_back( x );
        this->Vertices.push_back( y );
        this->Vertices.push_back( z );
      }
      else if( tag == "vn" )
      {
        float x, y, z;
        iss >> x >> y >> z;
        this->Normals.Data.push_back( x );
        this->Normals.Data.push_back( y );
        this->Normals.Data.push_back( z );
      }
      else if( tag == "vt" )
      {
        float u, v;
        iss >> u >> v;
        this->TexCoords.Data.push_back( u );
        this->TexCoords.Data.push_back( v );
      }
      else if( tag == "f" )
      {
        std::vector< unsigned long > faceVertexIdx;
        std::vector< unsigned long > faceNormalIdx;
        std::vector< unsigned long > faceTexCoordIdx;

        long vertexCount   = long( this->Vertices.size( ) / 3 );
        long normalCount   = long( this->Normals.Data.size( ) / 3 );
        long texCoordCount = long( this->TexCoords.Data.size( ) / 2 );

        std::string token;
        while( iss >> token )
        {
          unsigned long vIdx = 0, vtIdx = 0, vnIdx = 0;
          bool hasVt = false, hasVn = false;
          WavefrontOBJ::_ParseFace(
            token, vertexCount, normalCount, texCoordCount,
            vIdx, hasVt, vtIdx, hasVn, vnIdx
            );
          faceVertexIdx.push_back( vIdx );
          if( hasVt ) faceTexCoordIdx.push_back( vtIdx );
          if( hasVn ) faceNormalIdx.push_back( vnIdx );
        } // end while

        this->Faces.push_back( std::move( faceVertexIdx ) );
        this->Normals.Indices.push_back( std::move( faceNormalIdx ) );
        this->TexCoords.Indices.push_back( std::move( faceTexCoordIdx ) );
      } // end if
      // TODO: Other tags (o, g, s, mtllib, usemtl, comments, ...) are ignored.
    } // end while

    this->_UpdateBoundingBox( );
  } // end if
  ffile.close( );
}

// -------------------------------------------------------------------------
pujOpenGL::WavefrontOBJ::
~WavefrontOBJ( )
{
}

// -------------------------------------------------------------------------
void pujOpenGL::WavefrontOBJ::
_ParseFace(
  const std::string& token,
  long vertexCount, long normalCount, long texCoordCount,
  unsigned long& vIdx,
  bool& hasVt, unsigned long& vtIdx,
  bool& hasVn, unsigned long& vnIdx
  )
{
  hasVt = false;
  hasVn = false;

  long v = 0, vt = 0, vn = 0;
  std::size_t fS = token.find( '/' );
  if( fS == std::string::npos )
  {
    // Just a "v"
    v = std::stol( token );
  }
  else
  {
    v = std::stol( token.substr( 0, fS ) );
    std::size_t sS = token.find( '/', fS + 1 );
    if( sS == std::string::npos )
    {
      // "v/vt": vertex and texture
      vt = std::stol( token.substr( fS + 1 ) );
      hasVt = true;
    }
    else if( sS == fS + 1 )
    {
      // "v//vn": vertex and normal
      vn = std::stol( token.substr( sS + 1 ) );
      hasVn = true;
    }
    else
    {
      // "v/vt/vn": vertex, texture and normal
      vt = std::stol( token.substr( fS + 1, sS - fS - 1 ) );
      vn = std::stol( token.substr( sS + 1 ) );
      hasVt = true;
      hasVn = true;
    } // end if
  } // end if

  vIdx = ( unsigned long )( ( v > 0 )? v - 1: vertexCount + v );
  if( hasVt )
    vtIdx = ( unsigned long )( ( vt > 0 )? vt - 1: texCoordCount + vt );
  if( hasVn )
    vnIdx = ( unsigned long )( ( vn > 0 )? vn - 1: normalCount + vn );
}

// eof - WavefrontOBJ.cxx
