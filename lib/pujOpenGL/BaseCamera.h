// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __pujOpenGL__BaseCamera__h__
#define __pujOpenGL__BaseCamera__h__

#include <array>
#include "Node.h"

namespace pujOpenGL
{
  /**
   */
  class BaseCamera
    : public pujOpenGL::Node
  {
  public:
    using Self       = BaseCamera;
    using Superclass = pujOpenGL::Node;

  public:
    BaseCamera( );
    virtual ~BaseCamera( ) override;
    virtual void InitializeFromBoundingBox( );
    virtual void ConfigureFromBounds( const float* b );
    virtual void Perspective( int w, int h );
    virtual void KeyEvent( int k, int x, int y, bool special, bool up );
    virtual void MouseButtonEvent( int button, int state, int x, int y );
    virtual void MouseMotionEvent( int x, int y );
    virtual void Draw( ) const override;

  protected:
    std::array< float, 3 > Eye    { 0, 0, 1 };
    std::array< float, 3 > LookAt { 0, 0, 0 };
    std::array< float, 3 > Up     { 0, 1, 0 };

    float Fovy { 45 };
    std::array< float, 2 > ZBounds { 1e-3, 1e3 };

    bool InitializeFromBoundingBoxFlag { false };
  };
} // end namespace

#endif // __pujOpenGL__BaseCamera__h__

// eof - BaseCamera.h
