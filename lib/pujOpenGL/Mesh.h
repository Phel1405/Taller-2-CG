// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __pujOpenGL__Mesh__h__
#define __pujOpenGL__Mesh__h__

#include <vector>
#include "VisualObject.h"

namespace pujOpenGL
{
  /**
   */
  class Mesh
    : public VisualObject
  {
  public:
    using Self       = Mesh;
    using Superclass = VisualObject;

    /**
     */
    struct StrAttribute
    {
      std::vector< float > Data;
      std::vector< std::vector< unsigned long > > Indices;
    };

  public:
    Mesh( );
    virtual ~Mesh( );

    virtual void Clear( );

  protected:
    virtual void _UpdateBoundingBox( );
    virtual void _SelfDraw( ) const override;

  protected:
    std::vector< float > Vertices;
    std::vector< std::vector< unsigned long > > Faces;
    StrAttribute Normals;
    StrAttribute TexCoords;
  };
} // end namespace

#endif // __pujOpenGL__Mesh__h__

// eof - Mesh.h
