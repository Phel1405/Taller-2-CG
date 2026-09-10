// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __pujOpenGL__VisualObject__h__
#define __pujOpenGL__VisualObject__h__

#include <functional>
#include <utility>
#include <vector>

#include "Node.h"




#include <GL/gl.h>



namespace pujOpenGL
{
  /**
   */
  class VisualObject
    : public pujOpenGL::Node
  {
  public:
    using Self       = VisualObject;
    using Superclass = pujOpenGL::Node;
    using FTransform = std::function< void() >;

  public:
    VisualObject( )
      : Superclass( )
      {
      }
    virtual ~VisualObject( ) override
      {
        for( std::pair< Superclass*, bool > n: this->Children )
          if( n.first != nullptr && n.second )
            delete n.first;
        this->Children.clear( );
      }

    Self& SetModelingTransform( FTransform f )
      {
        this->ModelingTransform = f;
        return( *this );
      }
    Self& SetLocalizationTransform( FTransform f )
      {
        this->LocalizationTransform = f;
        return( *this );
      }
    Self& SetAnimationTransform( FTransform f )
      {
        this->AnimationTransform = f;
        return( *this );
      }
    Self& AddChild( Node* c, bool take_ownership = true )
      {
        if( c != nullptr )
          this->Children.push_back( std::make_pair( c, take_ownership ) );
        return( *this );
      }

    virtual std::array< float, 6 > GetCompleteBoundingBox( ) const override
      {
        return( this->BoundingBox );
      }
    virtual void Draw( ) const
      {
        this->AnimationTransform( );

        glPushMatrix( );

        glPushMatrix( );
        this->ModelingTransform( );
        this->_SelfDraw( );
        glPopMatrix( );

        for( const std::pair< Superclass*, bool >& c: this->Children )
        {
          glPushMatrix( );
          this->LocalizationTransform( );
          c.first->Draw( );
          glPopMatrix( );
        } // end for

        glPopMatrix( );
      }

  protected:
    virtual void _SelfDraw( ) const = 0;

  protected:
    std::vector< std::pair< Superclass*, bool > > Children;

    FTransform ModelingTransform     { []() -> void {} };
    FTransform LocalizationTransform { []() -> void {} };
    FTransform AnimationTransform    { []() -> void {} };
  };
} // end namespace

#endif // __pujOpenGL__VisualObject__h__

// eof - VisualObject.h
