// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __pujOpenGL__Node__h__
#define __pujOpenGL__Node__h__

#include <array>
#include <limits>

namespace pujOpenGL
{
  /**
   */
  class Node
  {
  public:
    using Self = Node;

  public:
    Node( )
      {
      }
    virtual ~Node( )
      {
      }

    const std::array< float, 6 >& GetBoundingBox( ) const
      {
        return( this->BoundingBox );
      }
    virtual std::array< float, 6 > GetCompleteBoundingBox( ) const
      {
        return( this->BoundingBox );
      }
    const float& GetLongestDiagonal( ) const
      {
        return( this->LongestDiagonal );
      }

    virtual void Draw( ) const = 0;

  protected:
    std::array< float, 6 > BoundingBox
      {
        std::numeric_limits< float >::max( ),
        std::numeric_limits< float >::lowest( ),
        std::numeric_limits< float >::max( ),
        std::numeric_limits< float >::lowest( ),
        std::numeric_limits< float >::max( ),
        std::numeric_limits< float >::lowest( )
      };
    float LongestDiagonal { 0 };
  };
} // end namespace

#endif // __pujOpenGL__Node__h__

// eof - Node.h
