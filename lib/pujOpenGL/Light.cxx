// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include "Light.h"
#include <GL/gl.h>

// -------------------------------------------------------------------------
std::queue< long long > pujOpenGL::Light::AvailableLights
=
  [ ]( )
    {
      std::queue< long long > q;
      for( long long i = 0; i < GL_MAX_LIGHTS; ++i )
        q.push( GL_LIGHT0 + i );
      return( q );
    }( );

// -------------------------------------------------------------------------
pujOpenGL::Light::
Light( )
  : Superclass( )
{
  if( Self::AvailableLights.size( ) > 0 )
    {
      this->LightId = Self::AvailableLights.front( );
      Self::AvailableLights.pop( );
    }
  /* TODO
     else
     ERROR
  */
}

// -------------------------------------------------------------------------
pujOpenGL::Light::
~Light( )
{
  Self::AvailableLights.push( this->LightId );
}

// -------------------------------------------------------------------------
void pujOpenGL::Light::
Configure( )
{
  if( this->IsEnabled )
  {
    glEnable( this->LightId );

    /* TODO
       glLightfv( this->LightId, GL_AMBIENT,  this->Ambient.data( ) );
       glLightfv( this->LightId, GL_DIFFUSE,  this->Diffuse.data( ) );
       glLightfv( this->LightId, GL_SPECULAR, this->Specular.data( ) );
    */
    /* TODO
       glLightfv( this->LightId, GL_SPOT_DIRECTION, this->SpotDirection.data( ) );
       glLightf( this->LightId, GL_SPOT_CUTOFF, this->SpotCutoff );
       glLightf( this->LightId, GL_SPOT_EXPONENT, this->SpotExponent );

       glLightf( this->LightId, GL_CONSTANT_ATTENUATION,  this->ConstantAtt );
       glLightf( this->LightId, GL_LINEAR_ATTENUATION,    this->LinearAtt );
       glLightf( this->LightId, GL_QUADRATIC_ATTENUATION, this->QuadraticAtt );
    */
  }
  else
    glDisable( this->LightId );
}

// -------------------------------------------------------------------------
pujOpenGL::Light::
Self& pujOpenGL::Light::
SetPosition( float x, float y, float z, float w )
{
  this->Position = { x, y, z, w };
  return( *this );
}

// -------------------------------------------------------------------------
pujOpenGL::Light::
Self& pujOpenGL::Light::
SetDirectional( float x, float y, float z )
{
  this->Position = { x, y, z, 0 };
  return( *this );
}

// -------------------------------------------------------------------------
pujOpenGL::Light::
Self& pujOpenGL::Light::
SetAmbient( float r, float g, float b, float a )
{
  this->Ambient = { r, g, b, a };
  return( *this );
}

// -------------------------------------------------------------------------
pujOpenGL::Light::
Self& pujOpenGL::Light::
SetDiffuse( float r, float g, float b, float a )
{
  this->Diffuse = { r, g, b, a };
  return( *this );
}

// -------------------------------------------------------------------------
pujOpenGL::Light::
Self& pujOpenGL::Light::
SetSpecular( float r, float g, float b, float a )
{
  this->Specular = { r, g, b, a };
  return( *this );
}

// -------------------------------------------------------------------------
pujOpenGL::Light::
Self& pujOpenGL::Light::
SetColor( float r, float g, float b, float a )
{
  this->Ambient  = { r, g, b, a };
  this->Diffuse  = { r, g, b, a };
  this->Specular = { r, g, b, a };
  return( *this );
}

// -------------------------------------------------------------------------
pujOpenGL::Light::
Self& pujOpenGL::Light::
SetSpotlight( float dX, float dY, float dZ, float coD, float e )
{
  this->SpotDirection = { dX, dY, dZ };
  this->SpotCutoff = coD; // 0-90, or 180 to disable
  this->SpotExponent = e;
  return( *this );
}

// -------------------------------------------------------------------------
pujOpenGL::Light::
Self& pujOpenGL::Light::
SetAttenuation( float c, float l, float q )
{
  this->ConstantAtt = c;
  this->LinearAtt = l;
  this->QuadraticAtt = q;
  return( *this );
}

// -------------------------------------------------------------------------
pujOpenGL::Light::
Self& pujOpenGL::Light::
SetEnabled( bool e )
{
  this->IsEnabled = e;
  return( *this );
}

// -------------------------------------------------------------------------
pujOpenGL::Light::
Self& pujOpenGL::Light::
Enable( )
{
  return( this->SetEnabled( true ) );
}

// -------------------------------------------------------------------------
pujOpenGL::Light::
Self& pujOpenGL::Light::
Disable( )
{
  return( this->SetEnabled( false ) );
}

// -------------------------------------------------------------------------
long long pujOpenGL::Light::
GetId( ) const
{
  return( this->LightId );
}

// -------------------------------------------------------------------------
bool pujOpenGL::Light::
GetEnabled( ) const
{
  return( this->IsEnabled );
}

// -------------------------------------------------------------------------
void pujOpenGL::Light::
_SelfDraw( ) const
{
  glLightfv( this->LightId, GL_POSITION, this->Position.data( ) );
}

// eof - Light.cxx
