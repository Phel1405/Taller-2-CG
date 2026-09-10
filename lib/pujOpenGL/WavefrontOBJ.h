// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __pujOpenGL__WavefrontOBJ__h__
#define __pujOpenGL__WavefrontOBJ__h__

#include <string>
#include "Mesh.h"

namespace pujOpenGL
{
  /**
   */
  class WavefrontOBJ
    : public pujOpenGL::Mesh
  {
  public:
    using Self       = WavefrontOBJ;
    using Superclass = pujOpenGL::Mesh;

  public:
    WavefrontOBJ( const std::string& fname );
    virtual ~WavefrontOBJ( );

  private:
    static void _ParseFace(
      const std::string& token,
      long vertexCount, long normalCount, long texCoordCount,
      unsigned long& vIdx,
      bool& hasVt, unsigned long& vtIdx,
      bool& hasVn, unsigned long& vnIdx
      );
  };
} // end namespace

#endif // __pujOpenGL__WavefrontOBJ__h__

// eof - WavefrontOBJ.h
