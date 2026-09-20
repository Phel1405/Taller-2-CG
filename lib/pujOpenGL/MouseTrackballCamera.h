// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __pujOpenGL__MouseTrackballCamera__h__
#define __pujOpenGL__MouseTrackballCamera__h__

#include "BaseCamera.h"

namespace pujOpenGL
{
  /**
   */
  class MouseTrackballCamera
    : public BaseCamera
  {
  public:
    using Self = MouseTrackballCamera;
    using Superclass = BaseCamera;

  public:
    MouseTrackballCamera( );
    virtual ~MouseTrackballCamera( );

    virtual void ConfigureFromBounds( const float* b ) override;
    virtual void MouseButtonEvent( int button, int state, int x, int y ) override;
    virtual void MouseMotionEvent( int x, int y ) override;
    virtual void Draw( ) const override;

  protected:
    virtual void _Reset( );
    virtual void _Update( );

  protected:
    float Phi         { 0 };
    float Theta       { 0 };
    float Radius      { 0 };
    float DeltaRadius { 0 };
    float OffRadius   { 0 };

    std::array< float, 16 > Transform
      {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
      };
  };
} // end namespace

#endif // __pujOpenGL__MouseTrackballCamera__h__

// eof - MouseTrackballCamera.h
