//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\Triangle.hpp
//! \brief Provides utility triangle type
//! \date 8 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_TRIANGLE_HPP
#define WTL_TRIANGLE_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Triangle - Encapsulates a triangle of any type
  //! 
  //! \tparam T - Dimension type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct Triangle 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias point_t - Defines point type
    using point_t = Point<T>;

    //! \alias size_t - Defines size type
    using size_t = Size<T>;

    //! \alias value_t - Defines dimension type
    using value_t = T;

    //! \alias array_t - Define win32 point array type
    using array_t = ::POINT (*)[3];

    //! \var EMPTY - Empty sentinel value 
    //static constexpr size_t EMPTY = size_t();
    
    // ----------------------------------- REPRESENTATION -----------------------------------

    point_t  Points[3];     //!< Points

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Triangle::Triangle constexpr
    //! Create empty size of zero width and height
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    Triangle() : Points({0,0,0})
    {}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Triangle::Triangle 
    //! Create from points
    //! 
    //! \param[in] const& a - Bottom left
    //! \param[in] const& b - Top point
    //! \param[in] const& c - Right point
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename A, typename B, typename C>
    Triangle(const Point<A>& a, const Point<B>& b, const Point<C>& c) //: Points({a,b,c})
    {
      Points[0] = a;
      Points[1] = b;
      Points[2] = c;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Triangle::Triangle 
    //! Create from point and dimensions
    //! 
    //! \param[in] const& btmLeft - Bottom left point
    //! \param[in] width - Triangle width
    //! \param[in] height - Triangle height
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U, typename W, typename Z>
    Triangle(const Point<U>& btmLeft, W width, Z height) /*: Points({btmLeft,
                                                                  btmLeft+Point<W>(width,0),
                                                                  btmLeft+Point<Z>(0,height)})*/
    {
      Points[0] = btmLeft;
      Points[1] = btmLeft + Point<Z>(width/2,-height);
      Points[2] = btmLeft + Point<W>(width,0);
    }

    ENABLE_COPY(Triangle);   //!< Performs a deep copy
    ENABLE_MOVE(Triangle);   //!< Performs a deep copy
     
    /////////////////////////////////////////////////////////////////////////////////////////
    // Triangle::~Triangle 
    //! Virtual d-tor
    /////////////////////////////////////////////////////////////////////////////////////////
    /*virtual ~Triangle()
    {};*/

    // ----------------------------------- STATIC METHODS -----------------------------------
  
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Triangle::empty const
    //! Query whether triangle is empty
    //! 
    //! \return bool - True iff all fields zero
    /////////////////////////////////////////////////////////////////////////////////////////
    bool empty() const
    {
      return Points[0].empty() && Points[1].empty() && Points[2].empty();
    };

    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Triangle::operator const ::POINT*
    //! Implicit user conversion to array of win32 points
    //! 
    //! \return array_t - Mutable pointer to array of win32 points
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = std::enable_if_t<sizeof(value_t) == sizeof(long32)>>
    operator const ::POINT* () const
    {
      return reinterpret_cast<const ::POINT*>(Points);
    }
  };
  
  //! \alias TriangleL - Triangle using long32 fields
  using TriangleL = Triangle<long32>;

  //! \alias TriangleF - Triangle using floating point fields
  using TriangleF = Triangle<float>;


} // WTL namespace



#endif // WTL_TRIANGLE_HPP


