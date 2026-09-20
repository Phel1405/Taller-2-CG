// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include "World.h"

#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include "BaseCamera.h"
#include "Light.h"
#include "Node.h"
// -------------------------------------------------------------------------
std::unique_ptr< pujOpenGL::World, pujOpenGL::World::SingletonDeleter >
pujOpenGL::World::Singleton;

// -------------------------------------------------------------------------
pujOpenGL::World::
Self* pujOpenGL::World::
Get( )
{
  if( Self::Singleton.get( ) == nullptr )
    Self::Singleton.reset( new pujOpenGL::World( ) );
  return( Self::Singleton.get( ) );
}

// -------------------------------------------------------------------------
pujOpenGL::World::
Self& pujOpenGL::World::
SetCamera( pujOpenGL::BaseCamera* c, bool take_ownership )
{
  this->Camera = std::make_pair( c, take_ownership );
  return( *this );
}

// -------------------------------------------------------------------------
pujOpenGL::World::
Self& pujOpenGL::World::
AddNode( Node* n, bool take_ownership )
{
  if( n !=  nullptr )
  {
    this->HasLights |= ( dynamic_cast< pujOpenGL::Light* >( n ) != nullptr );
    this->Nodes.push_back( std::make_pair( n, take_ownership ) );
  } // end if
  return( *this );
}

// -------------------------------------------------------------------------
std::array< float, 6 > pujOpenGL::World::
GetCompleteBoundingBox( ) const
{
  std::array< float, 6 > bb
    {
      std::numeric_limits< float >::max( ),
      std::numeric_limits< float >::lowest( ),
      std::numeric_limits< float >::max( ),
      std::numeric_limits< float >::lowest( ),
      std::numeric_limits< float >::max( ),
      std::numeric_limits< float >::lowest( )
    };

  for( const std::pair< pujOpenGL::Node*, bool >& n: this->Nodes )
  {
    const std::array< float, 6 >& nbb = n.first->GetCompleteBoundingBox( );
    bb[ 0 ] = std::min( bb[ 0 ], nbb[ 0 ] );
    bb[ 1 ] = std::max( bb[ 1 ], nbb[ 1 ] );
    bb[ 2 ] = std::min( bb[ 2 ], nbb[ 2 ] );
    bb[ 3 ] = std::max( bb[ 3 ], nbb[ 3 ] );
    bb[ 4 ] = std::min( bb[ 4 ], nbb[ 4 ] );
    bb[ 5 ] = std::max( bb[ 5 ], nbb[ 5 ] );
  } // end for

  return( bb );
}

// -------------------------------------------------------------------------
void pujOpenGL::World::
Init(
  int ac, char** av, const std::string& title, std::size_t w, std::size_t h
  )
{
  unsigned int dm = GLUT_RGBA | GLUT_DOUBLE;
  if( this->HasLights )
    dm |= GLUT_DEPTH;

  // Create window
  glutInit( &ac, av );
  glutInitDisplayMode( dm );
  glutInitWindowSize( w, h );

  // Initialize world
  this->WindowId = glutCreateWindow( title.c_str( ) );
  glClearColor(
    this->ClearColor[ 0 ], this->ClearColor[ 1 ],
    this->ClearColor[ 2 ], this->ClearColor[ 3 ]
    );

  // Configure lights
  if( this->HasLights )
  {
    glEnable( GL_DEPTH_TEST );
    glShadeModel( GL_FLAT ); // TODO
    glEnable( GL_LIGHTING );
    for( std::pair< pujOpenGL::Node*, bool >& n: this->Nodes )
    {
      pujOpenGL::Light* l = dynamic_cast< pujOpenGL::Light* >( n.first );
      if( l != nullptr )
        l->Configure( );
    } // end for
    glEnable( GL_NORMALIZE );
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, this->AmbientLight.data( ) );
  } // end if

  // Configure camera
  pujOpenGL::BaseCamera* cam = this->Camera.first;
  if( cam != nullptr )
  {
    std::array< float, 6 > bb = this->GetCompleteBoundingBox( );
    cam->ConfigureFromBounds( bb.data( ) );
  } // end if
}

// -------------------------------------------------------------------------
void pujOpenGL::World::
Start( )
{
  // Window related
  glutDisplayFunc( Self::_CB_Display );
  glutReshapeFunc( Self::_CB_Reshape );
  glutOverlayDisplayFunc( Self::_CB_OverlayDisplay );
  std::atexit( Self::_CB_Close );

  // Timer related
  glutIdleFunc( Self::_CB_Idle );
  // TODO: glutTimerFunc(msecs, func, value)

  // Keyboard related
  glutKeyboardFunc( Self::_CB_KeyboardDown );
  glutKeyboardUpFunc( Self::_CB_KeyboardUp );
  glutSpecialFunc( Self::_CB_SpecialKeyboardDown );
  glutSpecialUpFunc( Self::_CB_SpecialKeyboardUp );

  // Mouse related
  glutMouseFunc( Self::_CB_MouseButtonPress );
  glutMotionFunc( Self::_CB_MouseMotion );
  glutPassiveMotionFunc( Self::_CB_MouseMotion );
  /* TODO
     glutMouseWheelFunc(func)
     glutEntryFunc(func)

     glutJoystickFunc(func, pollInterval)

     glutMenuStatusFunc(func)
     glutMenuStateFunc(func)

     glutVisibilityFunc(func)
     glutWindowStatusFunc(func)

     glutSpaceballMotionFunc(func)
     glutSpaceballRotateFunc(func)
     glutSpaceballButtonFunc(func)

     glutButtonBoxFunc(func)
     glutDialsFunc(func)

     glutTabletMotionFunc(func)
     glutTabletButtonFunc(func)

     glutMultiEntryFunc, glutMultiButtonFunc, glutMultiMotionFunc, glutMultiPassiveMotionFunc
     glutInitContextFlags
  */
  glutMainLoop( );
}

// -------------------------------------------------------------------------
pujOpenGL::World::
World( )
{
}

// -------------------------------------------------------------------------
pujOpenGL::World::
~World( )
{
  if( this->Camera.first != nullptr && this->Camera.second )
    delete this->Camera.first;

  for( std::pair< pujOpenGL::Node*, bool >& n: this->Nodes )
    if( n.first != nullptr && n.second )
      delete n.first;
  this->Nodes.clear( );
}

// -------------------------------------------------------------------------
void pujOpenGL::World::
_cb_display( )
{
  unsigned int cl = GL_COLOR_BUFFER_BIT;
  if( this->HasLights )
    cl |= GL_DEPTH_BUFFER_BIT;
  glClear( cl );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );

  if( this->Camera.first != nullptr )
    this->Camera.first->Draw( );
  for( std::pair< pujOpenGL::Node*, bool >& n: this->Nodes )
    n.first->Draw( );
}

// -------------------------------------------------------------------------
void pujOpenGL::World::
_cb_reshape( int w, int h )
{
  // Update window size
  this->Width = w;
  this->Height = ( h == 0 )? 1: h;

  // Define viewport and perspective
  glViewport( 0, 0, w, h );
  if( this->Camera.first != nullptr )
    this->Camera.first->Perspective( this->Width, this->Height );
}

// -------------------------------------------------------------------------
void pujOpenGL::World::
_cb_overlay_display( )
{
}

// -------------------------------------------------------------------------
void pujOpenGL::World::
_cb_close( )
{
}

// -------------------------------------------------------------------------
void pujOpenGL::World::
_cb_idle( )
{
}

// -------------------------------------------------------------------------
void pujOpenGL::World::
_cb_keyboard( int k, int x, int y, bool special, bool up )
{
  if( this->Camera.first != nullptr )
  {
    this->Camera.first->KeyEvent( k, x, y, special, up );
    glutPostRedisplay( );
  } // end if
}

// -------------------------------------------------------------------------
void pujOpenGL::World::
_cb_mouse_button_press( int button, int state, int x, int y )
{
  if( this->Camera.first != nullptr )
  {
    this->Camera.first->MouseButtonEvent( button, state, x, y );
    glutPostRedisplay( );
  } // end if
}

// -------------------------------------------------------------------------
void pujOpenGL::World::
_cb_mouse_motion( int x, int y )
{
  if( this->Camera.first != nullptr )
  {
    this->Camera.first->MouseMotionEvent( x, y );
    glutPostRedisplay( );
  } // end if
}


// -------------------------------------------------------------------------
void pujOpenGL::World::
_CB_Display( )
{
  if( Self::Get( ) != nullptr )
  {
    Self::Get( )->_cb_display( );
    glutSwapBuffers( );
  } // end if
}

// -------------------------------------------------------------------------
void pujOpenGL::World::
_CB_Reshape( int w, int h )
{
  if( Self::Get( ) != nullptr )
  {
    Self::Get( )->_cb_reshape( w, h );
    glutPostRedisplay( );
  } // end if
}

// -------------------------------------------------------------------------
void pujOpenGL::World::
_CB_OverlayDisplay( )
{
  if( Self::Get( ) != nullptr )
    Self::Get( )->_cb_overlay_display( );
}

// -------------------------------------------------------------------------
void pujOpenGL::World::
_CB_Close( )
{
  if( Self::Get( ) != nullptr )
    Self::Get( )->_cb_close( );
}

// -------------------------------------------------------------------------
void pujOpenGL::World::
_CB_Idle( )
{
  if( Self::Get( ) != nullptr )
  {
    Self::Get( )->_cb_idle( );
    glutPostRedisplay( );
  } // end if
}

// -------------------------------------------------------------------------
void pujOpenGL::World::
_CB_KeyboardDown( unsigned char k, int x, int y )
{
  if( Self::Get( ) != nullptr )
    Self::Get( )->_cb_keyboard( int( k ), x, y, false, false );
}

// -------------------------------------------------------------------------
void pujOpenGL::World::
_CB_KeyboardUp( unsigned char k, int x, int y )
{
  if( Self::Get( ) != nullptr )
    Self::Get( )->_cb_keyboard( int( k ), x, y, false, true );
}

// -------------------------------------------------------------------------
void pujOpenGL::World::
_CB_SpecialKeyboardDown( int k, int x, int y )
{
  if( Self::Get( ) != nullptr )
    Self::Get( )->_cb_keyboard( k, x, y, true, false );
}

// -------------------------------------------------------------------------
void pujOpenGL::World::
_CB_SpecialKeyboardUp( int k, int x, int y )
{
  if( Self::Get( ) != nullptr )
    Self::Get( )->_cb_keyboard( k, x, y, true, true );
}

// -------------------------------------------------------------------------
void pujOpenGL::World::
_CB_MouseButtonPress( int button, int state, int x, int y )
{
  if( Self::Get( ) != nullptr )
    Self::Get( )->_cb_mouse_button_press( button, state, x, y );
}

// -------------------------------------------------------------------------
void pujOpenGL::World::
_CB_MouseMotion( int x, int y )
{
  if( Self::Get( ) != nullptr )
    Self::Get( )->_cb_mouse_motion( x, y );
}

// eof - World.cxx
