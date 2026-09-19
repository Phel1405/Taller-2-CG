// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <array>
#include <iostream>
#include <string>

// #include <pujOpenGL/KeyboardTrackballCamera.h>
#include "../lib/pujOpenGL/KeyboardTrackballCamera.h"
// #include <pujOpenGL/WavefrontOBJ.h>
#include "../lib/pujOpenGL/WavefrontOBJ.h"
// #include <pujOpenGL/World.h>
#include "../lib/pujOpenGL/World.h"

int main( int argc, char** argv )
{
  if( argc < 2 )
  {
    std::cerr << "Usage: " << argv[ 0 ] << " input.obj" << std::endl;
    return( EXIT_FAILURE );
  } // end if
  std::string input_fname = argv[ 1 ];

  // 3D world
  pujOpenGL::World* world = pujOpenGL::World::Get( );

  // Read model to be shown
  pujOpenGL::WavefrontOBJ mesh( argv[ 1 ] );
  world->AddNode( mesh );

  // Camera
  pujOpenGL::KeyboardTrackballCamera camera;
  camera.InitializeFromBoundingBox( );
  world->SetCamera( camera );

  // Initialize world and start
  world->Init( argc, argv, "WavefrontOBJ viewer", 500, 500 );
  world->Start( );

  return( EXIT_SUCCESS );
}

// eof - OBJViewer.cxx
