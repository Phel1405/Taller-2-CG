// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __pujOpenGL__Light__h__
#define __pujOpenGL__Light__h__

#include <array>
#include <queue>
#include "VisualObject.h"

namespace pujOpenGL
{
  /**
   */
  class Light
    : public pujOpenGL::VisualObject
  {
  public:
    using Self       = Light;
    using Superclass = pujOpenGL::VisualObject;

  public:
    Light( );
    virtual ~Light( ) override;

    virtual void Configure( );

    Self& SetPosition( float x, float y, float z, float w = 1 );
    Self& SetDirectional( float x, float y, float z );
    Self& SetAmbient( float r, float g, float b, float a = 1 );
    Self& SetDiffuse( float r, float g, float b, float a = 1 );
    Self& SetSpecular( float r, float g, float b, float a = 1 );
    Self& SetColor( float r, float g, float b, float a = 1 );

    Self& SetSpotlight( float dX, float dY, float dZ, float coD, float e = 0 );
    Self& SetAttenuation( float c, float l, float q );
    Self& SetEnabled( bool e );
    Self& Enable( );
    Self& Disable( );

    long long GetId( ) const;
    bool GetEnabled( ) const;

  protected:
    virtual void _SelfDraw( ) const override;

  protected:
    long long LightId { -1 };

    std::array< float, 4 > Position      { 0, 0, 0, 1 };
    std::array< float, 4 > Ambient       { 1, 1, 1, 1 };
    std::array< float, 4 > Diffuse       { 1, 1, 1, 1 };
    std::array< float, 4 > Specular      { 1, 1, 1, 1 };
    std::array< float, 3 > SpotDirection { 0, -1, 0 };

    bool IsEnabled     { true };
    float SpotCutoff   { 180 };
    float SpotExponent { 0 };
    float ConstantAtt  { 1 };
    float LinearAtt    { 0 };
    float QuadraticAtt { 0 };

    static std::queue< long long > AvailableLights;
  };
} // end namespace

#endif // __pujOpenGL__Light__h__

// eof - Light.h
