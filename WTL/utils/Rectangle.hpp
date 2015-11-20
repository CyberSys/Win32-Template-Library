//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Rectangle.hpp
//! \brief Provides utility rectangle type
//! \date 20 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RECT_HPP
#define WTL_RECT_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Point.hpp>          //!< Point
#include <wtl/utils/Size.hpp>           //!< Size
#include <wtl/utils/DebugInfo.hpp>      //!< DebugInfo

//! \namespace wtl - Windows template library
namespace wtl
{

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Rect - Encapsulates a rectangle of any type
  //!
  //! \tparam T - Dimension type
  //!
  //! \remarks In order to use the implicit conversion operators to Win32 types requires that
  //! \remarks type T model the appropriate Signed16BitFields or Signed32BitFields concepts
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct Rect
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Defines rectangle type
    using type = Rect<T>;

    //! \alias point_t - Defines point of matching type
    using point_t = Point<T>;

    //! \alias size_t - Defines size type
    using size_t = Size<T>;

    //! \alias value_t - Defines dimension type
    using value_t = T;

    //! \var EMPTY - Sentinel empty rectangle
    static const Rect<T> EMPTY;     // [MSVC-14 FIX] static constexpr Rect<T> EMPTY {0,0,0,0};

    //! \enum Relation - Layout calculation constants
    enum Relation { FromLeft, FromTop, FromRight, FromBottom, Centre };

    //! \struct LayoutVector - Layout calculation vector
    struct LayoutVector
    {
      /////////////////////////////////////////////////////////////////////////////////////////
      // LayoutVector::LayoutVector constexpr
      //! Create layout vector from edge identifier and distance
      /////////////////////////////////////////////////////////////////////////////////////////
      constexpr 
      LayoutVector(Relation r, int32_t n = 0) : Direction(r), Distance(n)
      {}

      Relation  Direction;    //!< Edge indicator
      int32_t   Distance;     //!< Distance from edge
    };

    // ----------------------------------- REPRESENTATION -----------------------------------

    value_t  Left,        //!< Left extent
             Top,         //!< Top extent
             Right,       //!< Right extent
             Bottom;      //!< Bottom extent

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::Rect constexpr
    //! Create empty rectangle with all fields initialized to zero
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    Rect() : Left(defvalue<T>()),
             Top(defvalue<T>()),
             Right(defvalue<T>()),
             Bottom(defvalue<T>())
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::Rect constexpr
    //! Create from Win32 rectangle
    //!
    //! \param[in] const& rc - Input rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    Rect(const ::RECT&  rc) : Left(static_cast<T>(rc.left)),
                              Top(static_cast<T>(rc.top)),
                              Right(static_cast<T>(rc.right)),
                              Bottom(static_cast<T>(rc.bottom))
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::Rect constexpr
    //! Create from rectangle of any type
    //!
    //! \tparam U - Input rectangle type
    //! \param[in] const& rc - Input rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U> constexpr
    Rect(const Rect<U>& rc) : Left(static_cast<T>(rc.Left)),
                              Top(static_cast<T>(rc.Top)),
                              Right(static_cast<T>(rc.Right)),
                              Bottom(static_cast<T>(rc.Bottom))
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::Rect constexpr
    //! Create from dimensions of any type
    //!
    //! \param[in] const l - Left
    //! \param[in] const t - Top
    //! \param[in] const r - Right
    //! \param[in] const b - Bottom
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename A, typename B, typename C, typename D> constexpr
    Rect(const A l, const B t, const C r, const D b) : Left(static_cast<T>(l)),
                                                       Top(static_cast<T>(t)),
                                                       Right(static_cast<T>(r)),
                                                       Bottom(static_cast<T>(b))
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::Rect constexpr
    //! Create from point and rectangle extent
    //!
    //! \param[in] const& topLeft - Top Left co-ordinate
    //! \param[in] const& size - Size of rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename A, typename B> constexpr
    Rect(const Point<A>& topLeft, const Size<B>& size) : Left(topLeft.X),
                                                         Right(topLeft.X+size.Width),
                                                         Top(topLeft.Y),
                                                         Bottom(topLeft.Y+size.Height)
    {}

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    CONSTEXPR_COPY_CTOR(Rect);      //!< Can be deep copied at compile-time
    CONSTEXPR_MOVE_CTOR(Rect);      //!< Can be moved at compile-time
    ENABLE_COPY_ASSIGN(Rect);       //!< Can be assigned
    ENABLE_MOVE_ASSIGN(Rect);       //!< Can be move-assigned
    DISABLE_POLY(Rect);             //!< Cannot be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::fromMidPoint constexpr
    //! Create from a mid point and rectangle dimensions
    //!
    //! \param[in] const& middle - Mid-point
    //! \param[in] const& size - Size of rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename A, typename B> constexpr
    static type fromMidPoint(const Point<A>&  middle, const Size<B>& size)
    {
      return type(static_cast<value_t>(middle.X) - (size.Width / 2),
                  static_cast<value_t>(middle.Y) - (size.Height / 2),
                  static_cast<value_t>(middle.X) + (size.Width / 2),
                  static_cast<value_t>(middle.Y) + (size.Height / 2));
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::arrange constexpr
    //! Arranges a sub-rectangle within this rectangle
    //!
    //! \param[in] const& sz - Size of sub-rectangle 
    //! \param[in] x - Horizontal layout
    //! \param[in] y - Vertical layout
    //!
    //! \return type - Co-ordinates of desired sub-rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename T> //!< [MSVC-14 FIX] constexpr  
    type arrange(const Size<T>& sz, LayoutVector x, LayoutVector y) const
    {
      // Calculate X or Y co-ordinate of resultant top-left corner
      auto calc = [&sz,this](LayoutVector& v, bool horz) -> value_t
      {
        switch (v.Direction)
        {
        case FromLeft:   return this->Left   + v.Distance;
        case FromRight:  return this->Right  - v.Distance;
        case FromTop:    return this->Top    + v.Distance;
        case FromBottom: return this->Bottom - v.Distance;
        default:
        case Centre:     return horz ? this->Left + (this->width() - sz.Width) / 2 : this->Top + (this->height() - sz.Height) / 2;
        }
      };

      // Generate rectangle
      return Rect<T>(Point<T>(calc(x,true), calc(y,false)), sz);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::area constexpr
    //! Query size of rectangle
    //!
    //! \return size_t - Size of rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    size_t  area() const
    {
      return {width(), height()};
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::bottomLeft constexpr
    //! Query Bottom-Left corner of rectangle
    //!
    //! \return point_t - Point defining rectangle corner
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    point_t  bottomLeft() const
    {
      return point_t(Left, Bottom);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::bottomRight constexpr
    //! Query Bottom-Right corner of rectangle
    //!
    //! \return point_t - Point defining rectangle corner
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    point_t  bottomRight() const
    {
      return point_t(Right, Bottom);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::centre constexpr
    //! Query rectangle mid point
    //!
    //! \return point_t - Point defining rectangle centre
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    point_t  centre() const
    {
      return point_t(width() / static_cast<value_t>(2),
                     height() / static_cast<value_t>(2));
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::contains constexpr
    //! Query whether a point is within the rectangle
    //!
    //! \tparam U - Point type
    //!
    //! \param[in] const& pt - Point
    //! \return bool - True if within boundaries
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U> constexpr
    bool  contains(const Point<U>&  pt) const
    {
      return static_cast<value_t>(pt.X) >= Left && static_cast<value_t>(pt.X) < Right
          && static_cast<value_t>(pt.Y) >= Top  && static_cast<value_t>(pt.Y) < Bottom;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::empty constexpr
    //! Query whether rectangle is empty
    //!
    //! \return bool - True iff all fields zero
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    bool empty() const
    {
      return *this == EMPTY;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::height constexpr
    //! Query rectangle height
    //!
    //! \return value_t - Rectangle height, in units
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    value_t  height() const
    {
      return Bottom - Top;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::size constexpr
    //! Query rectangle size
    //!
    //! \return size_t - Rectangle size
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    size_t size() const
    {
      return size_t(width(), height());
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::topLeft constexpr
    //! Query Top-Left corner of rectangle
    //!
    //! \return point_t - Point defining rectangle corner
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    point_t  topLeft() const
    {
      return point_t(Left, Top);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::topRight constexpr
    //! Query Top-Right corner of rectangle
    //!
    //! \return point_t - Point defining rectangle corner
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    point_t  topRight() const
    {
      return point_t(Right, Top);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::width constexpr
    //! Query rectangle width
    //!
    //! \return value_t - Rectangle width, in units
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    value_t  width() const
    {
      return Right - Left;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::operator == constexpr
    //! Equality operator
    //!
    //! \param[in] const& r - Another rect
    //! \return bool - True iff co-ordinates equal
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    bool operator == (const type& r) const
    {
      return Left == r.Left && Right == r.Right
          && Top == r.Top   && Bottom == r.Bottom;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::operator != constexpr
    //! Inequality operator
    //!
    //! \param[in] const& r - Another rect
    //! \return bool - True iff co-ordinates unequal
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    bool operator != (const type& r) const
    {
      return Left != r.Left || Right != r.Right
          || Top != r.Top   || Bottom != r.Bottom;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::operator + constexpr
    //! Create new rectangle from adding a horizontal and vertical offset
    //!
    //! \param[in] const& pt - Offset
    //! \return type - New instance added by 'x' horizontal units, and 'y' vertical units
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    type  operator + (const point_t& pt) const
    {
      return type(Left + static_cast<value_t>(pt.X),  Top + static_cast<value_t>(pt.Y),
                  Right + static_cast<value_t>(pt.X), Bottom + static_cast<value_t>(pt.Y));
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::operator - constexpr
    //! Create new rectangle from subtracting a horizontal and vertical offset
    //!
    //! \param[in] const& pt - Offset
    //! \return type - New instance subtracted by 'x' horizontal units, and 'y' vertical units
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    type  operator - (const point_t& pt) const
    {
      return type(Left - static_cast<value_t>(pt.X),  Top - static_cast<value_t>(pt.Y),
                  Right - static_cast<value_t>(pt.X), Bottom - static_cast<value_t>(pt.Y));
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::operator const ::RECT* const noexcept
    //! Implicit user-conversion to native ::RECT pointer
    //!
    //! \return const ::RECT* - Recter to self as immutable ::RECT
    //! 
    //! \remarks Requires value_t model the Signed32BitFields concept
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = void>
    operator const ::RECT* () const noexcept
    {
      concept_check(value_t,Signed32BitFields);

      return reinterpret_cast<const ::RECT*>(this);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::operator ::RECT const noexcept
    //! Implicit user-conversion to native ::RECT 
    //!
    //! \return ::RECT - Copy of current value as ::RECT
    //! 
    //! \remarks Requires value_t model the Signed32BitFields concept
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = void>
    operator  ::RECT () const noexcept
    {
      concept_check(value_t,Signed32BitFields);

      return {Left,Top,Right,Bottom};
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::clear noexcept
    //! Reset all fields to zero
    /////////////////////////////////////////////////////////////////////////////////////////
    void  clear() noexcept
    {
      *this = EMPTY;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::set noexcept
    //! Set from point and rectangle extent
    //!
    //! \param[in] const& topLeft - Top Left co-ordinate
    //! \param[in] const& size - Size of rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    void set(const Point<U>& topLeft, const Size<U>& size) noexcept
    {
      Left   = topLeft.X;
      Right  = topLeft.X + size.width;
      Top    = topLeft.Y;
      Bottom = topLeft.Y + size.height;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::operator += noexcept
    //! Add a horizontal and vertical offset
    //!
    //! \param[in] const& pt - Offset
    //! \return type& - Reference to self at updated position
    /////////////////////////////////////////////////////////////////////////////////////////
    type&  operator += (const point_t&  pt) noexcept
    {
      Left   += static_cast<value_t>(pt.X);
      Top    += static_cast<value_t>(pt.Y);
      Right  += static_cast<value_t>(pt.X);
      Bottom += static_cast<value_t>(pt.Y);
      return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::operator -=
    //! Subtract a horizontal and vertical offset
    //!
    //! \param[in] const& pt - Offset
    //! \return type& - Reference to self at updated position
    /////////////////////////////////////////////////////////////////////////////////////////
    type&  operator -= (const point_t&  pt) noexcept
    {
      Left   -= static_cast<value_t>(pt.X);
      Top    -= static_cast<value_t>(pt.Y);
      Right  -= static_cast<value_t>(pt.X);
      Bottom -= static_cast<value_t>(pt.Y);
      return *this;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Rect::operator ::RECT* noexcept
    //! Implicit user-conversion to native ::RECT pointer
    //!
    //! \return ::RECT* - Recter to self as mutable ::RECT
    //! 
    //! \remarks Requires value_t model the Signed32BitFields concept
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = void>
    operator ::RECT* () noexcept
    {
      concept_check(value_t,Signed32BitFields);

      return reinterpret_cast<::RECT*>(this);
    }
    
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \var Rect<T>::EMPTY - Definition of 'Empty' sentinel value
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  const Rect<T>  Rect<T>::EMPTY;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias RectL - Rectangle using long32_t fields (binary compatible with Win32 ::RECT)
  /////////////////////////////////////////////////////////////////////////////////////////
  using RectL = Rect<long32_t>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias RectF - Rectangle using floating point fields
  /////////////////////////////////////////////////////////////////////////////////////////
  using RectF = Rect<float>;


  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Prints a Rect to the debug console
  //!
  //! \tparam T - Rect field type
  //!
  //! \param[in,out] &c - Debugging console
  //! \param[in] const &rc - Rect
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  Console& operator << (Console& c, const Rect<T>& rc)
  {
    return c << make_nvpair_tuple("Left", rc.Left,
                                  "Top", rc.Top,
                                  "Right", rc.Right,
                                  "Bottom", rc.Bottom);
  }

} // WTL namespace



#endif // WTL_RECT_HPP


