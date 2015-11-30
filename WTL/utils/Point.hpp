//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Point.hpp
//! \brief Provides a point type
//! \date 20 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_POINT_HPP
#define WTL_POINT_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Default.hpp>       //!< default
#include <wtl/utils/Requires.hpp>      //!< requires
#include <wtl/utils/Concepts.hpp>      //!< Signed16BitFields, Signed32BitFields
#include <type_traits>                 //!< std::enable_if

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Point - Encapsulates a point of any type
  //!
  //! \tparam T - Dimension type
  //!
  //! \remarks In order to use the implicit conversion operators to Win32 types requires that
  //! \remarks type T model the appropriate Signed16BitFields or Signed32BitFields concepts
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
    static const Point<T> EMPTY;      // [MSVC14-FIX]: static constexpr Point<T> EMPTY {0,0};

    // ----------------------------------- REPRESENTATION -----------------------------------

    value_t  X,       //!< X co-ordinate
             Y;       //!< Y co-ordinate

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::Point constexpr
    //! Create empty point centred at origin
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr 
    Point() : X(defvalue<T>()),
              Y(defvalue<T>())
    {}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::Point constexpr
    //! Create from Win32 16-bit co-ordinates
    //!
    //! \param[in] const& pt - Input co-ordinates
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr 
    Point(const ::COORD&  pt) : X(static_cast<T>(pt.X)),
                                Y(static_cast<T>(pt.Y))
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::Point constexpr
    //! Create from Win32 32-bit co-ordinates
    //!
    //! \param[in] const& pt - Input co-ordinates
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr 
    Point(const ::POINT&  pt) : X(static_cast<T>(pt.x)),
                                Y(static_cast<T>(pt.y))
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::Point constexpr
    //! Create from Win32 16-bit co-ordinates
    //!
    //! \param[in] const& pt - Input co-ordinates
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr 
    Point(const ::POINTS&  pt) : X(static_cast<T>(pt.x)),
                                 Y(static_cast<T>(pt.y))
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::Point
    //! Create from co-ordinates of any type
    //!
    //! \param[in] const x - X co-ordinate
    //! \param[in] const y - Y co-ordinate
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename A, typename B> constexpr
    Point(const A X, const B Y) : X(static_cast<T>(X)),
                                  Y(static_cast<T>(Y))
    {}

    // TODO: Construct from an lParam?

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    CONSTEXPR_COPY_CTOR(Point);      //!< Can be deep copied at compile-time
    CONSTEXPR_MOVE_CTOR(Point);      //!< Can be moved at compile-time
    ENABLE_COPY_ASSIGN(Point);       //!< Can be assigned
    ENABLE_MOVE_ASSIGN(Point);       //!< Can be move-assigned
    DISABLE_POLY(Point);             //!< Cannot be polymorphic
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::operator= noexcept
    //! Assignment operator
    //!
    //! \tparam U - Another value type
    //!
    //! \param[in] const& pt - Another point of any type
    //! \return type& - Reference to self
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    type& operator= (const Point<U>&  pt) noexcept
    {
      this->X = static_cast<value_t>(pt.X);
      this->Y = static_cast<value_t>(pt.Y);

      return *this;
    }
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::empty constexpr
    //! Query whether point is empty
    //!
    //! \return bool - True iff all fields zero
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    bool empty() const
    {
      return *this == defvalue<type>();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::operator+ constexpr
    //! Calculate position if translated by addition
    //!
    //! \param[in] const& pt - Another point of any type
    //! \return type - New point translated by adding 'pt' to 'this'
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U> constexpr
    type operator + (const Point<U>&  pt) const
    {
      return type(X + static_cast<T>(pt.X),
                  Y + static_cast<T>(pt.Y));
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::operator- constexpr
    //! Calculate position if translated by subtraction
    //!
    //! \param[in] const& pt - Another point of any type
    //! \return type - New point translated by subtracting 'pt' from 'this'
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U> constexpr
    type operator - (const Point<U>&  pt) const
    {
      return type(X - static_cast<T>(pt.X),
                  Y - static_cast<T>(pt.Y));
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::operator == constexpr
    //! Equality operator
    //!
    //! \param[in] const& r - Another point
    //! \return bool - True iff co-ordinates equal
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    bool operator == (const type& r) const
    {
      return X == r.X && Y == r.Y;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::operator != constexpr
    //! Inequality operator
    //!
    //! \param[in] const& r - Another point
    //! \return bool - True iff co-ordinates unequal
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    bool operator != (const type& r) const
    {
      return X != r.X || Y != r.Y;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::operator const ::COORD* const noexcept
    //! Implicit user-conversion to native ::COORD pointer
    //!
    //! \return const ::COORD* - Pointer to self as immutable ::COORD
    //! 
    //! \remarks Requires value_t model the Signed16BitFields concept
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = void> 
    operator const ::COORD* () const noexcept
    {
      concept_check(value_t,Signed16BitFields);

      return reinterpret_cast<const ::COORD*>(this);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::operator ::COORD const noexcept
    //! Implicit user-conversion to native ::COORD 
    //!
    //! \return ::COORD - Copy of current value as ::COORD
    //! 
    //! \remarks Requires value_t model the Signed16BitFields concept
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = void>
    operator ::COORD () const noexcept
    {
      concept_check(value_t,Signed16BitFields);

      return {X,Y};
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::operator const ::POINT* const noexcept
    //! Implicit user-conversion to native ::POINT pointer
    //!
    //! \return const ::POINT* - Pointer to self as immutable ::POINT
    //! 
    //! \remarks Requires value_t model the Signed32BitFields concept
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = void>
    operator const ::POINT* () const noexcept
    {
      concept_check(value_t,Signed32BitFields);

      return reinterpret_cast<const ::POINT*>(this);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::operator ::POINT const noexcept
    //! Implicit user-conversion to native ::POINT 
    //!
    //! \return ::POINT - Copy of current value as ::POINT
    //! 
    //! \remarks Requires value_t model the Signed32BitFields concept
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = void>
    operator  ::POINT () const noexcept
    {
      concept_check(value_t,Signed32BitFields);

      return {X,Y};
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::clear noexcept
    //! Reset all fields to zero
    /////////////////////////////////////////////////////////////////////////////////////////
    void  clear() noexcept
    {
      *this = defvalue<type>();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::operator += noexcept
    //! Enlarge by a given point
    //!
    //! \param[in] const& pt - Another point of any type
    //! \return type - Reference to self, updated by subtracting 'pt' from 'this'
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    type& operator += (const Point<U>& pt) noexcept 
    {
      X += static_cast<T>(pt.X);
      Y += static_cast<T>(pt.Y);
      return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::operator -= noexcept
    //! Subtract by a given point
    //!
    //! \param[in] const& pt - Another point of any type
    //! \return type - Reference to self, updated by subtracting 'pt' from 'this'
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    type& operator -= (const Point<U>& pt) noexcept
    {
      X -= static_cast<T>(pt.X);
      Y -= static_cast<T>(pt.Y);
      return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::operator ::COORD* noexcept
    //! Implicit user-conversion to native ::COORD pointer
    //!
    //! \return ::COORD* - Pointer to self as mutable ::COORD
    //! 
    //! \remarks Requires value_t model the Signed16BitFields concept
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = void>
    operator ::COORD* () noexcept
    {
      concept_check(value_t,Signed16BitFields);

      return reinterpret_cast<::COORD*>(this);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Point::operator ::POINT* noexcept
    //! Implicit user-conversion to native ::POINT pointer
    //!
    //! \return ::POINT* - Pointer to self as mutable ::POINT
    //! 
    //! \remarks Requires value_t model the Signed32BitFields concept
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = void>
    operator ::POINT* () noexcept
    {
      concept_check(value_t,Signed32BitFields);

      return reinterpret_cast<::POINT*>(this);
    }
  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \var Point<T>::EMPTY - Definition of 'Empty' sentinel value
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  const Point<T>  Point<T>::EMPTY;


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias PointL - Point using long32_t fields (binary compatible with Win32 ::RECT)
  /////////////////////////////////////////////////////////////////////////////////////////
  using PointL = Point<long32_t>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias PointF - Point using floating point fields
  /////////////////////////////////////////////////////////////////////////////////////////
  using PointF = Point<float>;

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Prints a Point to a console output stream
  //!
  //! \tparam CHAR - Output stream character type
  //! \tparam TRAITS - Output stream character traits
  //! \tparam T - Point field type
  //! 
  //! \param[in,out] &c - Output stream
  //! \param[in] const &pt - Point
  //! \return std::basic_ostream<CHAR,TRAITS>& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename CHAR, typename TRAITS, typename T>
  std::basic_ostream<CHAR,TRAITS>& operator << (std::basic_ostream<CHAR,TRAITS>& c, const Point<T>& pt);

}


#endif // WTL_POINT_HPP


