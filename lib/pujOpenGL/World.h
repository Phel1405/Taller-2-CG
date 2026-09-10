// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __pujOpenGL__World__h__
#define __pujOpenGL__World__h__

#include <array>
#include <memory>
#include <vector>

namespace pujOpenGL
{
  class BaseCamera;
  class Light;
  class Node;

  /**
   */
  class World
  {
  public:
    using Self = World;

  public:
    static Self* Get( );

    Self& SetCamera( pujOpenGL::BaseCamera* c, bool take_ownership );

    template< class _TCamera >
    Self& SetCamera( _TCamera& c )
      {
        this->SetCamera( &c, false );
        return( *this );
      }

    Self& AddNode( Node* n, bool take_ownership );

    template< class _TNode >
    Self& AddNode( _TNode& n )
      {
        this->AddNode( &n, false );
        return( *this );
      }

    std::array< float, 6 > GetCompleteBoundingBox( ) const;

    void Init(
      int ac, char** av,
      const std::string& title,
      std::size_t w, std::size_t h
      );
    void Start( );

  protected:
    World( );
    virtual ~World( );

    void _cb_display( );
    void _cb_reshape( int w, int h );
    void _cb_overlay_display( );
    void _cb_close( );
    void _cb_idle( );
    void _cb_keyboard( int k, int x, int y, bool special, bool up );

    static void _CB_Display( );
    static void _CB_Reshape( int w, int h );
    static void _CB_OverlayDisplay( );
    static void _CB_Close( );
    static void _CB_Idle( );
    static void _CB_KeyboardDown( unsigned char k, int x, int y );
    static void _CB_KeyboardUp( unsigned char k, int x, int y );
    static void _CB_SpecialKeyboardDown( int k, int x, int y );
    static void _CB_SpecialKeyboardUp( int k, int x, int y );

  private:
    World( const Self& ) = delete;
    Self& operator=( const Self& ) = delete;

  protected:
    int WindowId { 0 };

    int Width  { 0 };
    int Height { 0 };
    std::array< float, 4 > ClearColor   { 0, 0, 0, 1 };
    std::array< float, 4 > AmbientLight { 5e-2, 5e-2, 5e-2, 1 };

    std::pair< pujOpenGL::BaseCamera*, bool > Camera { nullptr, true };
    std::vector< std::pair< pujOpenGL::Node*, bool > > Nodes;
    bool HasLights { false };

  protected:
    struct SingletonDeleter
    {
      void operator()( Self* p ) const { delete p; }
    };
    friend struct SingletonDeleter;
    static std::unique_ptr< Self, SingletonDeleter > Singleton;
  };
} // end namespace

#endif // __pujOpenGL__World__h__

// eof - World.h
