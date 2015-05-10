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
    
    //! \var native - Whether binary compatible with ::POINT
    static constexpr bool native = sizeof(value_t) == sizeof(long32);
    
    // ----------------------------------- REPRESENTATION -----------------------------------

    value_t  x,       //!< X co-ordinate
             y;       //!< Y co-ordinate

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::Point constexpr
    //! Create empty point centred at origin
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr Point() : x(default<T>()), 
                        y(default<T>())
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
    
    CONSTEXPR_COPY_CTOR(Point);   //!< Performs a deep copy
    CONSTEXPR_MOVE_CTOR(Point);   //!< Performs a deep copy

    DEFAULT_COPY_ASSIGN(Point);   //!< Performs a deep copy
    DEFAULT_MOVE_ASSIGN(Point);   //!< Performs a deep copy
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::~Point 
    //! Non-virtual d-tor
    /////////////////////////////////////////////////////////////////////////////////////////
    ~Point()
    {}

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
      return *this == default<type>();
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
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::clear
    //! Reset all fields to zero
    /////////////////////////////////////////////////////////////////////////////////////////
    void  clear() 
    {
      *this = default<type>();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::operator ::POINT& 
    //! Implicit user conversion to win32 ::POINT reference
    //! 
    //! \return ::POINT& - Mutable reference to win32 ::POINT
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = std::enable_if_t<native>>
    operator ::POINT& ()
    {
      return *reinterpret_cast<::POINT*>(this);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::operator ::POINT*
    //! Explicit user conversion to win32 ::POINT pointer
    //! 
    //! \return ::POINT* - Mutable pointer to win32 ::POINT
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = std::enable_if_t<native>>
    explicit operator ::POINT* ()
    {
      return reinterpret_cast<::POINT*>(this);
    }
  };
  
  //! \var Point<T>::EMPTY - 'Empty' sentinel value 
  template <typename T>
  const Point<T>  Point<T>::EMPTY;

  //! \alias PointL - Point using long32 fields (binary compatible with Win32 ::RECT)
  using PointL = Point<long32>;
  
  //! \alias PointF - Point using floating point fields
  using PointF = Point<float>;



} // WTL namespace



#endif // WTL_POINT_HPP


