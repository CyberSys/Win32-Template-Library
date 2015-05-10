//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\Size.hpp
//! \brief Provides utility size type
//! \date 8 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SIZE_HPP
#define WTL_SIZE_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Size - Encapsulates a size of any type
  //! 
  //! \tparam T - Dimension type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct Size 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias point_t - Defines point of matching type
    using point_t = Point<T>;

    //! \alias size_t - Defines size type
    using size_t = Size<T>;

    //! \alias value_t - Defines dimension type
    using value_t = T;

    //! \var EMPTY - Empty sentinel value 
    static const size_t EMPTY;

    //! \var native - Whether binary compatible with ::SIZE
    static constexpr bool native = sizeof(value_t) == sizeof(long32);
    
    // ----------------------------------- REPRESENTATION -----------------------------------

    value_t  width,        //!< Width extent
             height;       //!< Height extent

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::Size constexpr
    //! Create empty size of zero width and height
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    Size() : width(default<T>()), 
             height(default<T>())
    {}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::Size 
    //! Create from Win32 size
    //! 
    //! \param[in] const& sz - Input size
    /////////////////////////////////////////////////////////////////////////////////////////
    Size(const ::SIZE&  sz) : width(static_cast<T>(sz.cx)), 
                              height(static_cast<T>(sz.cy))
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::Size 
    //! Create from dimensions of any type
    //! 
    //! \param[in] const w - Width
    //! \param[in] const h - Height
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    Size(const U w, const U h) : width(static_cast<T>(w)), 
                                 height(static_cast<T>(h))
    {}
    
    DEFAULT_COPY(Size);   //!< Performs a deep copy
    DEFAULT_MOVE(Size);   //!< Performs a deep copy

    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::~Size 
    //! Non-virtual d-tor
    /////////////////////////////////////////////////////////////////////////////////////////
    ~Size()
    {}

    // ----------------------------------- STATIC METHODS -----------------------------------
  
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::empty const
    //! Query whether size is empty
    //! 
    //! \return bool - True iff all fields zero
    /////////////////////////////////////////////////////////////////////////////////////////
    bool empty() const
    {
      return *this == EMPTY;
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::clear
    //! Reset all fields to zero
    /////////////////////////////////////////////////////////////////////////////////////////
    void  clear() 
    {
      *this = EMPTY;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::operator ::SIZE& 
    //! Implicit user conversion to win32 ::SIZE reference
    //! 
    //! \return ::SIZE& - Mutable reference to win32 ::SIZE
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = std::enable_if_t<native>>
    operator ::SIZE& ()
    {
      return *reinterpret_cast<::SIZE*>(this);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::operator ::SIZE*
    //! Explicit user conversion to win32 ::SIZE pointer
    //! 
    //! \return ::SIZE* - Mutable pointer to win32 ::SIZE
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename = std::enable_if_t<native>>
    explicit operator ::SIZE* ()
    {
      return reinterpret_cast<::SIZE*>(this);
    }
  };
  
  
  //! \var Size<T>::EMPTY - 'Empty' sentinel value 
  template <typename T>
  const Size<T>  Size<T>::EMPTY;

  
  //! \alias SizeL - Size using long32 fields (binary compatible with Win32 ::RECT)
  using SizeL = Size<long32>;

  //! \alias SizeF - Size using floating point fields
  using SizeF = Size<float>;



} // WTL namespace



#endif // WTL_SIZE_HPP


