////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\Shapes.hpp
//! \brief Provides utility types encapsulating points, sizes, rectangles, etc.
//! \date 8 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RECT_HPP
#define WTL_RECT_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct Point - Encapsulates a point of any type
  //! 
  //! \tparam T - Dimension type
  ///////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct Point 
  {
    // ---------------------- TYPES & CONSTANTS -------------------

    //! \alias point_t - Defines point type
    using point_t = Point<T>;

    //! \alias value_t - Defines co-ordinates type
    using value_t = T;
    
    //! \var EMPTY - Empty sentinel value 
    static const point_t EMPTY;

    // ----------------------- CONSTRUCTION -----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // Point::Point constexpr
    //! Create empty point centred at origin
    ///////////////////////////////////////////////////////////////////////////////
    constexpr
    Point() : x(zero<T>::value), 
              y(zero<T>::value)
    {}
    
    ///////////////////////////////////////////////////////////////////////////////
    // Point::Point 
    //! Create from Win32 32-bit co-ordinates
    //! 
    //! \param[in] const& pt - Input co-ordinates
    ///////////////////////////////////////////////////////////////////////////////
    Point(const ::POINT&  pt) : x(static_cast<T>(pt.x)), 
                                y(static_cast<T>(pt.y))
    {}
    
    ///////////////////////////////////////////////////////////////////////////////
    // Point::Point 
    //! Create from Win32 16-bit co-ordinates
    //! 
    //! \param[in] const& pt - Input co-ordinates
    ///////////////////////////////////////////////////////////////////////////////
    Point(const ::POINTS&  pt) : x(static_cast<T>(pt.x)), 
                                 y(static_cast<T>(pt.y))
    {}


    // TODO: lParam 

    ///////////////////////////////////////////////////////////////////////////////
    // Point::Point 
    //! Create from co-ordinates of any type
    //! 
    //! \param[in] const x - X co-ordinate
    //! \param[in] const y - Y co-ordinate
    ///////////////////////////////////////////////////////////////////////////////
    template <typename A, typename B>
    Point(const A X, const B Y) : x(static_cast<T>(X)), 
                                  y(static_cast<T>(Y))
    {}
    

    DEFAULT_COPY(Point);   //!< Performs a deep copy
    DEFAULT_COPY_ASSIGN(Point);   //!< Performs a deep copy
    DEFAULT_MOVE(Point);   //!< Performs a deep copy

    ///////////////////////////////////////////////////////////////////////////////
    // Point::~Point 
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    /*virtual ~Point()
    {};*/

    // ------------------------- STATIC ---------------------------
  
    // ------------------------ ACCESSORS -------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // Point::empty const
    //! Query whether point is empty
    //! 
    //! \return bool - True iff all fields zero
    ///////////////////////////////////////////////////////////////////////////////
    bool empty() const
    {
      return x == zero<value_t>::value && y == zero<value_t>::value;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // Point::operator+ const
    //! Create new point translated by addition
    //! 
    //! \param[in] const& pt - Another point of any type
    //! \return point_t - New point translated by adding 'pt'
    ///////////////////////////////////////////////////////////////////////////////
    template <typename U>
    point_t operator+ (const Point<U>&  pt) const 
    { 
      return point_t(x + static_cast<T>(pt.x), 
                     y + static_cast<T>(pt.y)); 
    };

    ///////////////////////////////////////////////////////////////////////////////
    // Point::operator- const
    //! Create new point translated by subtraction
    //! 
    //! \param[in] const& pt - Another point of any type
    //! \return point_t - New point translated by subtracting 'pt'
    ///////////////////////////////////////////////////////////////////////////////
    template <typename U>
    point_t operator- (const Point<U>&  pt) const 
    { 
      return point_t(x - static_cast<T>(pt.x), 
                     y - static_cast<T>(pt.y)); 
    };

    // ------------------------- MUTATORS -------------------------

    ///////////////////////////////////////////////////////////////////////////////
    // Point::operator ::POINT& 
    //! Implicit user conversion to win32 point 
    //! 
    //! \return ::POINT& - Mutable reference to win32 point
    ///////////////////////////////////////////////////////////////////////////////
    template <typename = std::enable_if_t<sizeof(value_t) == sizeof(long32)>>
    operator ::POINT& ()
    {
      return *reinterpret_cast<::POINT*>(this);
    }

    // -------------------- REPRESENTATION ---------------------

    value_t  x,       //!< X co-ordinate
             y;       //!< Y co-ordinate
  };

  //! \var Point<T>::EMPTY - 'Empty' sentinel value 
  template <typename T>
  const Point<T>  Point<T>::EMPTY;

  
  //! \alias PointL - Point using long32 fields
  using PointL = Point<long32>;

  //! \alias PointF - Point using floating point fields
  using PointF = Point<float>;


  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct Size - Encapsulates a size of any type
  //! 
  //! \tparam T - Dimension type
  ///////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct Size 
  {
    // ---------------------- TYPES & CONSTANTS -------------------

    //! \alias point_t - Defines point of matching type
    using point_t = Point<T>;

    //! \alias size_t - Defines size type
    using size_t = Size<T>;

    //! \alias value_t - Defines dimension type
    using value_t = T;

    //! \var EMPTY - Empty sentinel value 
    static const size_t EMPTY;

    // ----------------------- CONSTRUCTION -----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // Size::Size constexpr
    //! Create empty size of zero width and height
    ///////////////////////////////////////////////////////////////////////////////
    constexpr
    Size() : width(zero<T>::value), 
             height(zero<T>::value)
    {}
    
    ///////////////////////////////////////////////////////////////////////////////
    // Size::Size 
    //! Create from Win32 size
    //! 
    //! \param[in] const& sz - Input size
    ///////////////////////////////////////////////////////////////////////////////
    Size(const ::SIZE&  sz) : width(static_cast<T>(sz.cx)), 
                              height(static_cast<T>(sz.cy))
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // Size::Size 
    //! Create from dimensions of any type
    //! 
    //! \param[in] const w - Width
    //! \param[in] const h - Height
    ///////////////////////////////////////////////////////////////////////////////
    template <typename U>
    Size(const U w, const U h) : width(static_cast<T>(w)), 
                                 height(static_cast<T>(h))
    {}
    
    DEFAULT_COPY(Size);   //!< Performs a deep copy
    DEFAULT_MOVE(Size);   //!< Performs a deep copy

    ///////////////////////////////////////////////////////////////////////////////
    // Size::~Size 
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    /*virtual ~Size()
    {};*/

    // ------------------------- STATIC ---------------------------
  
    // ------------------------ ACCESSORS -------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // Size::empty const
    //! Query whether size is empty
    //! 
    //! \return bool - True iff all fields zero
    ///////////////////////////////////////////////////////////////////////////////
    bool empty() const
    {
      return width == zero<value_t>::value && height == zero<value_t>::value;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // Size::operator ::SIZE& 
    //! Implicit user conversion to win32 size 
    //! 
    //! \return ::SIZE& - Mutable reference to win32 size
    ///////////////////////////////////////////////////////////////////////////////
    template <typename = std::enable_if_t<sizeof(value_t) == sizeof(long32)>>
    operator ::SIZE& ()
    {
      return *reinterpret_cast<::SIZE*>(this);
    }

    // ------------------------- MUTATORS -------------------------
    
    // -------------------- REPRESENTATION ---------------------

    value_t  width,        //!< Width extent
             height;       //!< Height extent
  };
  
  
  //! \var Size<T>::EMPTY - 'Empty' sentinel value 
  template <typename T>
  const Size<T>  Size<T>::EMPTY;

  
  //! \alias SizeL - Size using long32 fields
  using SizeL = Size<long32>;

  //! \alias SizeF - Size using floating point fields
  using SizeF = Size<float>;



  ///////////////////////////////////////////////////////////////////////////////
  //! \struct Rect - Encapsulates a rectangle of any type
  //! 
  //! \tparam T - Dimension type
  ///////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct Rect
  {
    // ---------------------- TYPES & CONSTANTS -------------------

    //! \alias point_t - Defines point of matching type
    using point_t = Point<T>;

    //! \alias rect_t - Defines rectangle type
    using rect_t = Rect<T>;

    //! \alias value_t - Defines dimension type
    using value_t = T;

    //! \var EMPTY - Sentinel empty rectangle
    static const rect_t EMPTY;

    // ----------------------- CONSTRUCTION -----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // Rect::Rect constexpr
    //! Create empty rectangle with all fields initialized to zero
    ///////////////////////////////////////////////////////////////////////////////
    constexpr
    Rect() : left(zero<T>::value), 
             top(zero<T>::value), 
             right(zero<T>::value), 
             bottom(zero<T>::value)
    {}
    
    ///////////////////////////////////////////////////////////////////////////////
    // Rect::Rect 
    //! Create from Win32 rectangle
    //! 
    //! \param[in] const& rc - Input rectangle
    ///////////////////////////////////////////////////////////////////////////////
    Rect(const ::RECT&  rc) : left(static_cast<T>(rc.left)), 
                              top(static_cast<T>(rc.top)), 
                              right(static_cast<T>(rc.right)), 
                              bottom(static_cast<T>(rc.bottom))
    {}
    
    ///////////////////////////////////////////////////////////////////////////////
    // Rect::Rect 
    //! Create from rectangle of any type
    //! 
    //! \tparam U - Input rectangle type
    //! \param[in] const& rc - Input rectangle
    ///////////////////////////////////////////////////////////////////////////////
    template <typename U>
    Rect(const Rect<U>& rc) : left(static_cast<T>(rc.left)), 
                              top(static_cast<T>(rc.top)), 
                              right(static_cast<T>(rc.right)), 
                              bottom(static_cast<T>(rc.bottom))
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // Rect::Rect 
    //! Create from dimensions of any type
    //! 
    //! \param[in] const l - Left
    //! \param[in] const t - Top
    //! \param[in] const r - Right
    //! \param[in] const b - Bottom
    ///////////////////////////////////////////////////////////////////////////////
    template <typename U>
    Rect(const U l, const U t, const U r, const U b) : left(static_cast<T>(l)), 
                                                       top(static_cast<T>(t)), 
                                                       right(static_cast<T>(r)), 
                                                       bottom(static_cast<T>(b))
    {}
    
    ///////////////////////////////////////////////////////////////////////////////
    // Rect::Rect 
    //! Create from a mid point and rectangle dimensions
    //! 
    //! \param[in] const &middle - Mid-point
    //! \param[in] const width - Width
    //! \param[in] const height - Height
    ///////////////////////////////////////////////////////////////////////////////
    template <typename U>
    Rect(const Point<U>&  middle, const value_t  width, const value_t  height) : left(static_cast<value_t>(middle.x) - (width / 2)),
                                                                                 right(static_cast<value_t>(middle.x) + (width / 2)),
                                                                                 top(static_cast<value_t>(middle.y) - (height / 2)),
                                                                                 bottom(static_cast<value_t>(middle.y) + (height / 2))
    {}
    
    ///////////////////////////////////////////////////////////////////////////////
    // Rect::Rect 
    //! Create from point and rectangle extent
    //! 
    //! \param[in] const& topLeft - Top left co-ordinate
    //! \param[in] const& size - Size of rectangle
    ///////////////////////////////////////////////////////////////////////////////
    template <typename U>
    Rect(const Point<U>& topLeft, const Size<U>& size) : left(topLeft.x),
                                                         right(topLeft.x+size.width),
                                                         top(topLeft.y),
                                                         bottom(topLeft.y+size.height)
    {}

    
    DEFAULT_COPY(Rect);   //!< Performs a deep copy
    DEFAULT_MOVE(Rect);   //!< Performs a deep copy

    
    ///////////////////////////////////////////////////////////////////////////////
    // Rect::~Rect 
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    /*virtual ~Rect()
    {};*/

    // ------------------------- STATIC ---------------------------
  
    // ------------------------ ACCESSORS -------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // Rect::centre const
    //! Query rectangle mid point
    //! 
    //! \return point_t - Point defining rectangle centre
    ///////////////////////////////////////////////////////////////////////////////
    point_t  centre() const
    {
      return point_t(width() / static_cast<value_t>(2),
                     height() / static_cast<value_t>(2));
    };

    ///////////////////////////////////////////////////////////////////////////////
    // Rect::contains const
    //! Query whether a point is within the rectangle
    //! 
    //! \tparam U - Point type
    //! 
    //! \param[in] const& pt - Point
    //! \return bool - True if within boundaries
    ///////////////////////////////////////////////////////////////////////////////
    template <typename U>
    bool  contains(const Point<U>&  pt) const
    {
      return static_cast<value_t>(pt.x) >= left && static_cast<value_t>(pt.x) < right
          && static_cast<value_t>(pt.y) >= top && static_cast<value_t>(pt.y) < bottom;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Rect::empty const
    //! Query whether rectangle is empty
    //! 
    //! \return bool - True iff all fields zero
    ///////////////////////////////////////////////////////////////////////////////
    bool empty() const
    {
      return left == zero<value_t>::value && right  == zero<value_t>::value
          && top  == zero<value_t>::value && bottom == zero<value_t>::value;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // Rect::height const
    //! Query rectangle height
    //! 
    //! \return value_t - Rectangle height, in units
    ///////////////////////////////////////////////////////////////////////////////
    value_t  height() const
    {
      return bottom - top;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Rect::width const
    //! Query rectangle width
    //! 
    //! \return value_t - Rectangle width, in units
    ///////////////////////////////////////////////////////////////////////////////
    value_t  width() const
    {
      return right - left;
    };

    
    ///////////////////////////////////////////////////////////////////////////////
    // Rect::operator ::RECT* const
    //! Implicit user conversion to win32 rectangle 
    //! 
    //! \return const ::RECT* - Immutable pointer to win32 rectangle
    ///////////////////////////////////////////////////////////////////////////////
    template <typename = std::enable_if_t<sizeof(value_t) == sizeof(long32)>>
    operator const ::RECT* () const
    {
      return reinterpret_cast<const ::RECT*>(this);
    }


    ///////////////////////////////////////////////////////////////////////////////
    // Rect::operator+ const
    //! Create new rectangle offset by horizontal and vertical units
    //! 
    //! \return rect_t - New instance shifted by 'x' horizontal units, and 'y' vertical units
    ///////////////////////////////////////////////////////////////////////////////
    rect_t  operator+ (const point_t&  pt) const
    {
      return rect_t(left + static_cast<value_t>(pt.x),  top + static_cast<value_t>(pt.y),
                    right + static_cast<value_t>(pt.x), bottom + static_cast<value_t>(pt.y));
    }
    
    // ------------------------- MUTATORS -------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // Rect::set
    //! Set from point and rectangle extent
    //! 
    //! \param[in] const& topLeft - Top left co-ordinate
    //! \param[in] const& size - Size of rectangle
    ///////////////////////////////////////////////////////////////////////////////
    template <typename U>
    void set(const Point<U>& topLeft, const Size<U>& size) 
    {
      left   = topLeft.x;
      right  = topLeft.x + size.width;
      top    = topLeft.y;
      bottom = topLeft.y + size.height;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Rect::operator ::RECT& 
    //! Implicit user conversion to win32 rectangle 
    //! 
    //! \return ::RECT& - Mutable reference to win32 rectangle
    ///////////////////////////////////////////////////////////////////////////////
    template <typename = std::enable_if_t<sizeof(value_t) == sizeof(long32)>>
    operator ::RECT& ()
    {
      return *reinterpret_cast<::RECT*>(this);
    }
    
    
    ///////////////////////////////////////////////////////////////////////////////
    // Rect::operator ::RECT*
    //! Implicit user conversion to win32 rectangle 
    //! 
    //! \return ::RECT* - Mutable pointer to win32 rectangle
    ///////////////////////////////////////////////////////////////////////////////
    template <typename = std::enable_if_t<sizeof(value_t) == sizeof(long32)>>
    operator ::RECT* ()
    {
      return reinterpret_cast<::RECT*>(this);
    }
    
    
    // -------------------- REPRESENTATION ---------------------

    value_t  left,        //!< Left extent
             top,         //!< Top extent
             right,       //!< Right extent
             bottom;      //!< Bottom extent
  };
  
  //! \var Rect<T>::EMPTY - 'Empty' sentinel value 
  template <typename T>
  const Rect<T>  Rect<T>::EMPTY;


  //! \alias RectL - Rectangle using long32 fields
  using RectL = Rect<long32>;

  //! \alias RectF - Rectangle using floating point fields
  using RectF = Rect<float>;



  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct Triangle - Encapsulates a triangle of any type
  //! 
  //! \tparam T - Dimension type
  ///////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct Triangle 
  {
    // ---------------------- TYPES & CONSTANTS -------------------

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

    // ----------------------- CONSTRUCTION -----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // Triangle::Triangle constexpr
    //! Create empty size of zero width and height
    ///////////////////////////////////////////////////////////////////////////////
    constexpr
    Triangle() : Points({0,0,0})
    {}
    
    ///////////////////////////////////////////////////////////////////////////////
    // Triangle::Triangle 
    //! Create from points
    //! 
    //! \param[in] const& a - Bottom left
    //! \param[in] const& b - Top point
    //! \param[in] const& c - Right point
    ///////////////////////////////////////////////////////////////////////////////
    template <typename A, typename B, typename C>
    Triangle(const Point<A>& a, const Point<B>& b, const Point<C>& c) //: Points({a,b,c})
    {
      Points[0] = a;
      Points[1] = b;
      Points[2] = c;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Triangle::Triangle 
    //! Create from point and dimensions
    //! 
    //! \param[in] const& btmLeft - Bottom left point
    //! \param[in] width - Triangle width
    //! \param[in] height - Triangle height
    ///////////////////////////////////////////////////////////////////////////////
    template <typename U, typename W, typename Z>
    Triangle(const Point<U>& btmLeft, W width, Z height) /*: Points({btmLeft,
                                                                  btmLeft+Point<W>(width,0),
                                                                  btmLeft+Point<Z>(0,height)})*/
    {
      Points[0] = btmLeft;
      Points[1] = btmLeft + Point<Z>(width/2,-height);
      Points[2] = btmLeft + Point<W>(width,0);
    }

    DEFAULT_COPY(Triangle);   //!< Performs a deep copy
    DEFAULT_MOVE(Triangle);   //!< Performs a deep copy
     
    ///////////////////////////////////////////////////////////////////////////////
    // Triangle::~Triangle 
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    /*virtual ~Triangle()
    {};*/

    // ------------------------- STATIC ---------------------------
  
    // ------------------------ ACCESSORS -------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // Triangle::empty const
    //! Query whether triangle is empty
    //! 
    //! \return bool - True iff all fields zero
    ///////////////////////////////////////////////////////////////////////////////
    bool empty() const
    {
      return Points[0].empty() && Points[1].empty() && Points[2].empty();
    };

    // ------------------------- MUTATORS -------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // Triangle::operator const ::POINT*
    //! Implicit user conversion to array of win32 points
    //! 
    //! \return array_t - Mutable pointer to array of win32 points
    ///////////////////////////////////////////////////////////////////////////////
    template <typename = std::enable_if_t<sizeof(value_t) == sizeof(long32)>>
    operator const ::POINT* () const
    {
      return reinterpret_cast<const ::POINT*>(Points);
    }
    
    // -------------------- REPRESENTATION ---------------------

    point_t  Points[3];     //!< Points
  };
  
  //! \alias TriangleL - Triangle using long32 fields
  using TriangleL = Triangle<long32>;

  //! \alias TriangleF - Triangle using floating point fields
  using TriangleF = Triangle<float>;


} // WTL namespace



#endif // WTL_RECT_HPP

#pragma once
