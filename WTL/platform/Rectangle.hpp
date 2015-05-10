//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\Rectangle.hpp
//! \brief Provides utility rectangle type
//! \date 8 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RECT_HPP
#define WTL_RECT_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Rect - Encapsulates a rectangle of any type
  //! 
  //! \tparam T - Dimension type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct Rect
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias point_t - Defines point of matching type
    using point_t = Point<T>;

    //! \alias rect_t - Defines rectangle type
    using rect_t = Rect<T>;
    
    //! \alias size_t - Defines size type
    using size_t = Size<T>;

    //! \alias value_t - Defines dimension type
    using value_t = T;

    //! \var EMPTY - Sentinel empty rectangle
    static const rect_t EMPTY;

    //! \var native - Whether binary compatible with ::RECT
    static constexpr bool native = sizeof(value_t) == sizeof(long32);
    
    // ----------------------------------- REPRESENTATION -----------------------------------

    value_t  left,        //!< Left extent
             top,         //!< Top extent
             right,       //!< Right extent
             bottom;      //!< Bottom extent

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::Rect constexpr
    //! Create empty rectangle with all fields initialized to zero
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    Rect() : left(default<T>()), 
             top(default<T>()), 
             right(default<T>()), 
             bottom(default<T>())
    {}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::Rect 
    //! Create from Win32 rectangle
    //! 
    //! \param[in] const& rc - Input rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    Rect(const ::RECT&  rc) : left(static_cast<T>(rc.left)), 
                              top(static_cast<T>(rc.top)), 
                              right(static_cast<T>(rc.right)), 
                              bottom(static_cast<T>(rc.bottom))
    {}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::Rect 
    //! Create from rectangle of any type
    //! 
    //! \tparam U - Input rectangle type
    //! \param[in] const& rc - Input rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    Rect(const Rect<U>& rc) : left(static_cast<T>(rc.left)), 
                              top(static_cast<T>(rc.top)), 
                              right(static_cast<T>(rc.right)), 
                              bottom(static_cast<T>(rc.bottom))
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::Rect 
    //! Create from dimensions of any type
    //! 
    //! \param[in] const l - Left
    //! \param[in] const t - Top
    //! \param[in] const r - Right
    //! \param[in] const b - Bottom
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    Rect(const U l, const U t, const U r, const U b) : left(static_cast<T>(l)), 
                                                       top(static_cast<T>(t)), 
                                                       right(static_cast<T>(r)), 
                                                       bottom(static_cast<T>(b))
    {}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::Rect 
    //! Create from a mid point and rectangle dimensions
    //! 
    //! \param[in] const &middle - Mid-point
    //! \param[in] const width - Width
    //! \param[in] const height - Height
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    Rect(const Point<U>&  middle, const value_t  width, const value_t  height) : left(static_cast<value_t>(middle.x) - (width / 2)),
                                                                                 right(static_cast<value_t>(middle.x) + (width / 2)),
                                                                                 top(static_cast<value_t>(middle.y) - (height / 2)),
                                                                                 bottom(static_cast<value_t>(middle.y) + (height / 2))
    {}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::Rect 
    //! Create from point and rectangle extent
    //! 
    //! \param[in] const& topLeft - Top left co-ordinate
    //! \param[in] const& size - Size of rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    Rect(const Point<U>& topLeft, const Size<U>& size) : left(topLeft.x),
                                                         right(topLeft.x+size.width),
                                                         top(topLeft.y),
                                                         bottom(topLeft.y+size.height)
    {}

    
    DEFAULT_COPY(Rect);          //!< Performs a deep copy
    DEFAULT_MOVE(Rect);          //!< Performs a deep copy

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::~Rect 
    //! Non-Virtual d-tor
    /////////////////////////////////////////////////////////////////////////////////////////
    ~Rect() = default;

    // ----------------------------------- STATIC METHODS -----------------------------------
  
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::bottomLeft const
    //! Query bottom-left corner of rectangle 
    //! 
    //! \return point_t - Point defining rectangle corner
    /////////////////////////////////////////////////////////////////////////////////////////
    point_t  bottomLeft() const
    {
      return point_t(left, bottom);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::bottomRight const
    //! Query bottom-right corner of rectangle 
    //! 
    //! \return point_t - Point defining rectangle corner
    /////////////////////////////////////////////////////////////////////////////////////////
    point_t  bottomRight() const
    {
      return point_t(right, bottom);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::centre const
    //! Query rectangle mid point
    //! 
    //! \return point_t - Point defining rectangle centre
    /////////////////////////////////////////////////////////////////////////////////////////
    point_t  centre() const
    {
      return point_t(width() / static_cast<value_t>(2),
                     height() / static_cast<value_t>(2));
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::contains const
    //! Query whether a point is within the rectangle
    //! 
    //! \tparam U - Point type
    //! 
    //! \param[in] const& pt - Point
    //! \return bool - True if within boundaries
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    bool  contains(const Point<U>&  pt) const
    {
      return static_cast<value_t>(pt.x) >= left && static_cast<value_t>(pt.x) < right
          && static_cast<value_t>(pt.y) >= top && static_cast<value_t>(pt.y) < bottom;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::empty const
    //! Query whether rectangle is empty
    //! 
    //! \return bool - True iff all fields zero
    /////////////////////////////////////////////////////////////////////////////////////////
    bool empty() const
    {
      return *this == EMPTY;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::height const
    //! Query rectangle height
    //! 
    //! \return value_t - Rectangle height, in units
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  height() const
    {
      return bottom - top;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::size const
    //! Query rectangle size
    //! 
    //! \return size_t - Rectangle size
    /////////////////////////////////////////////////////////////////////////////////////////
    size_t size() const
    {
      return size_t(width(), height());
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::topLeft const
    //! Query top-left corner of rectangle 
    //! 
    //! \return point_t - Point defining rectangle corner
    /////////////////////////////////////////////////////////////////////////////////////////
    point_t  topLeft() const
    {
      return point_t(left, top);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::topRight const
    //! Query top-right corner of rectangle 
    //! 
    //! \return point_t - Point defining rectangle corner
    /////////////////////////////////////////////////////////////////////////////////////////
    point_t  topRight() const
    {
      return point_t(right, top);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::width const
    //! Query rectangle width
    //! 
    //! \return value_t - Rectangle width, in units
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  width() const
    {
      return right - left;
    }

    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::operator ::RECT& const 
    //! Implicit user conversion to win32 ::RECT reference
    //! 
    //! \return const ::RECT& - Immutable reference to win32 ::RECT
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = std::enable_if_t<native>>
    operator const ::RECT& () const
    {
      return *reinterpret_cast<const ::RECT*>(this);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::operator ::RECT* const 
    //! Explicit user conversion to win32 ::RECT pointer
    //! 
    //! \return const ::RECT* - Immutable pointer to win32 ::RECT
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = std::enable_if_t<native>>
    explicit operator const ::RECT* () const
    {
      return reinterpret_cast<const ::RECT*>(this);
    }


    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::operator+ const
    //! Create new rectangle from adding a horizontal and vertical offset 
    //! 
    //! \param[in] const& pt - Offset
    //! \return rect_t - New instance added by 'x' horizontal units, and 'y' vertical units
    /////////////////////////////////////////////////////////////////////////////////////////
    rect_t  operator+ (const point_t&  pt) const
    {
      return rect_t(left + static_cast<value_t>(pt.x),  top + static_cast<value_t>(pt.y),
                    right + static_cast<value_t>(pt.x), bottom + static_cast<value_t>(pt.y));
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::operator- const
    //! Create new rectangle from subtracting a horizontal and vertical offset 
    //! 
    //! \param[in] const& pt - Offset
    //! \return rect_t - New instance subtracted by 'x' horizontal units, and 'y' vertical units
    /////////////////////////////////////////////////////////////////////////////////////////
    rect_t  operator- (const point_t&  pt) const
    {
      return rect_t(left - static_cast<value_t>(pt.x),  top - static_cast<value_t>(pt.y),
                    right - static_cast<value_t>(pt.x), bottom - static_cast<value_t>(pt.y));
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::clear
    //! Reset all fields to zero
    /////////////////////////////////////////////////////////////////////////////////////////
    void  clear() 
    {
      *this = EMPTY;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::set
    //! Set from point and rectangle extent
    //! 
    //! \param[in] const& topLeft - Top left co-ordinate
    //! \param[in] const& size - Size of rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    void set(const Point<U>& topLeft, const Size<U>& size) 
    {
      left   = topLeft.x;
      right  = topLeft.x + size.width;
      top    = topLeft.y;
      bottom = topLeft.y + size.height;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::operator +=
    //! Add a horizontal and vertical offset 
    //! 
    //! \param[in] const& pt - Offset
    //! \return rect_t& - Reference to self at updated position
    /////////////////////////////////////////////////////////////////////////////////////////
    rect_t&  operator += (const point_t&  pt) 
    {
      return *this = *this + pt;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::operator -=
    //! Subtract a horizontal and vertical offset 
    //! 
    //! \param[in] const& pt - Offset
    //! \return rect_t& - Reference to self at updated position
    /////////////////////////////////////////////////////////////////////////////////////////
    rect_t&  operator -= (const point_t&  pt) 
    {
      return *this = *this - pt;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::operator ::RECT& 
    //! Implicit user conversion to win32 ::RECT reference
    //! 
    //! \return ::RECT& - Mutable reference to win32 ::RECT
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = std::enable_if_t<native>>
    operator ::RECT& ()
    {
      return *reinterpret_cast<::RECT*>(this);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::operator ::RECT*
    //! Explicit user conversion to win32 ::RECT pointer
    //! 
    //! \return ::RECT* - Mutable pointer to win32 ::RECT
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = std::enable_if_t<native>>
    explicit operator ::RECT* ()
    {
      return reinterpret_cast<::RECT*>(this);
    }
  };
  
  //! \var Rect<T>::EMPTY - 'Empty' sentinel value 
  template <typename T>
  const Rect<T>  Rect<T>::EMPTY;


  //! \alias RectL - Rectangle using long32 fields (binary compatible with Win32 ::RECT)
  using RectL = Rect<long32>;

  //! \alias RectF - Rectangle using floating point fields
  using RectF = Rect<float>;
  



} // WTL namespace



#endif // WTL_RECT_HPP


