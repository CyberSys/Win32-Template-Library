//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\Point.hpp
//! \brief Provides point type
//! \date 8 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_POINT_HPP
#define WTL_POINT_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/Default.hpp"       //!< Default
#include "wtl/casts/NativeCast.hpp"    //!< NativeCast
#include <type_traits>                 //!< std::enable_if

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Point - Encapsulates a point of any type
  //!
  //! \tparam T - Dimension type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct Point
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Defines own type
    using type = Point<T>;

    //! \alias value_t - Defines co-ordinates type
    using value_t = T;

    //! \var EMPTY - Empty sentinel value
    static const type EMPTY;
    //static constexpr Point<T> EMPTY {0,0};

    // ----------------------------------- REPRESENTATION -----------------------------------

    value_t  x,       //!< X co-ordinate
             y;       //!< Y co-ordinate

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::Point constexpr
    //! Create empty point centred at origin
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr Point() : x(defvalue<T>()),
                        y(defvalue<T>())
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::Point
    //! Create from Win32 32-bit co-ordinates
    //!
    //! \param[in] const& pt - Input co-ordinates
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr Point(const ::POINT&  pt) : x(static_cast<T>(pt.x)),
                                          y(static_cast<T>(pt.y))
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::Point
    //! Create from Win32 16-bit co-ordinates
    //!
    //! \param[in] const& pt - Input co-ordinates
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr Point(const ::POINTS&  pt) : x(static_cast<T>(pt.x)),
                                           y(static_cast<T>(pt.y))
    {}


    // TODO: lParam

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::Point
    //! Create from co-ordinates of any type
    //!
    //! \param[in] const x - X co-ordinate
    //! \param[in] const y - Y co-ordinate
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename A, typename B> constexpr
    Point(const A X, const B Y) : x(static_cast<T>(X)),
                                  y(static_cast<T>(Y))
    {}

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    CONSTEXPR_COPY_CTOR(Point);      //!< Can be deep copied at compile-time
    CONSTEXPR_MOVE_CTOR(Point);      //!< Can be moved at compile-time
    ENABLE_COPY_ASSIGN(Point);       //!< Can be assigned
    ENABLE_MOVE_ASSIGN(Point);       //!< Can be move-assigned
    DISABLE_POLY(Point);             //!< Cannot be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::empty const
    //! Query whether point is empty
    //!
    //! \return bool - True iff all fields zero
    /////////////////////////////////////////////////////////////////////////////////////////
    bool empty() const
    {
      return *this == defvalue<type>();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::operator+ const
    //! Create new point translated by addition
    //!
    //! \param[in] const& pt - Another point of any type
    //! \return type - New point translated by adding 'pt'
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    type operator+ (const Point<U>&  pt) const
    {
      return type(x + static_cast<T>(pt.x),
                  y + static_cast<T>(pt.y));
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::operator- const
    //! Create new point translated by subtraction
    //!
    //! \param[in] const& pt - Another point of any type
    //! \return type - New point translated by subtracting 'pt'
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    type operator- (const Point<U>&  pt) const
    {
      return type(x - static_cast<T>(pt.x),
                  y - static_cast<T>(pt.y));
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::operator == const
    //! Equality operator
    //!
    //! \param[in] const& r - Another point
    //! \return bool - True iff co-ordinates equal
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator == (const type& r)
    {
      return x == r.x && y == r.y;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::operator != const
    //! Inequality operator
    //!
    //! \param[in] const& r - Another point
    //! \return bool - True iff co-ordinates unequal
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator != (const type& r)
    {
      return x != r.x || y != r.y;
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::clear
    //! Reset all fields to zero
    /////////////////////////////////////////////////////////////////////////////////////////
    void  clear()
    {
      *this = defvalue<type>();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::operator=
    //! Assignment operator
    //!
    //! \tparam U - Another value type
    //!
    //! \param[in] const& pt - Another point of any type
    //! \return type& - Reference to self
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    type& operator= (const Point<U>&  pt)
    {
      this->x = static_cast<value_t>(pt.x);
      this->y = static_cast<value_t>(pt.y);

      return *this;
    }

  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \var Point<T>::EMPTY - 'Empty' sentinel value
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  const Point<T>  Point<T>::EMPTY;

  //! \alias PointL - Point using long32_t fields (binary compatible with Win32 ::RECT)
  using PointL = Point<long32_t>;

  //! \alias PointF - Point using floating point fields
  using PointF = Point<float>;


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct native_conversion<Point<32-bit>> - Defines a conversion from Point<32-bit> to ::POINT
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct native_conversion<Point<T>, enable_if_sizeof_t<T,long32_t>>
  {
    //! \alias input_t - Define input type
    using input_t = Point<T>;

    //! \alias result_t - Define output type
    using result_t = ::POINT;
  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct native_conversion<Point<16-bit>> - Defines a conversion from Point<16-bit> to ::COORD
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct native_conversion<Point<T>, enable_if_sizeof_t<T,int16_t>>
  {
    //! \alias input_t - Define input type
    using input_t = Point<T>;

    //! \alias result_t - Define output type
    using result_t = ::COORD;
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct native_conversion<::COORD> - Defines a conversion from ::COORD to Point<int16_t>
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename U>
  struct native_conversion<::COORD,U>
  {
    //! \alias input_t - Define input type
    using input_t = ::COORD;

    //! \alias result_t - Define output type
    using result_t = Point<int16_t>;
  };
}


#endif // WTL_POINT_HPP


