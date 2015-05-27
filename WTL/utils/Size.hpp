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
#include "wtl/casts/NativeCast.hpp"    //!< NativeCast
#include "wtl/utils/DebugInfo.hpp"     //!< DebugInfo
#include <type_traits>                 //!< std::enable_if

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

    //! \alias type - Defines own type
    using type = Size<T>;

    //! \alias value_t - Defines dimension type
    using value_t = T;

    //! \var EMPTY - Empty sentinel value 
    static const type EMPTY;

    // ----------------------------------- REPRESENTATION -----------------------------------

    value_t  width,        //!< Width extent
             height;       //!< Height extent

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::Size constexpr
    //! Create empty size of zero width and height
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    Size() : width(default<T>()), 
             height(default<T>())
    {}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::Size constexpr
    //! Create from Win32 size
    //! 
    //! \param[in] const& sz - Input size
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr
    Size(const ::SIZE&  sz) : width(static_cast<T>(sz.cx)), 
                              height(static_cast<T>(sz.cy))
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::Size constexpr
    //! Create from dimensions of any type
    //! 
    //! \param[in] const w - Width
    //! \param[in] const h - Height
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U> constexpr
    Size(const U w, const U h) : width(static_cast<T>(w)), 
                                 height(static_cast<T>(h))
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
    CONSTEXPR_COPY_CTOR(Size);      //!< Can be deep copied at compile-time
    CONSTEXPR_MOVE_CTOR(Size);      //!< Can be moved at compile-time
    ENABLE_COPY_ASSIGN(Size);       //!< Can be assigned
    ENABLE_MOVE_ASSIGN(Size);       //!< Can be move-assigned
    DISABLE_POLY(Size);             //!< Cannot be polymorphic
    
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
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::operator == const
    //! Equality operator
    //! 
    //! \param[in] const& r - Another size
    //! \return bool - True iff co-ordinates equal
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator == (const type& r)
    {
      return width  == r.width 
          && height == r.height;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Size::operator != const
    //! Inequality operator
    //! 
    //! \param[in] const& r - Another size
    //! \return bool - True iff co-ordinates unequal
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator != (const type& r)
    {
      return width != r.width 
         || height != r.height;
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
    
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \var Size<T>::EMPTY - 'Empty' sentinel value 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  const Size<T>  Size<T>::EMPTY;

  
  //! \alias SizeL - Size using long32_t fields (binary compatible with Win32 ::RECT)
  using SizeL = Size<long32_t>;

  //! \alias SizeF - Size using floating point fields
  using SizeF = Size<float>;


  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct native_conversion<32-bit>> - Defines a conversion from Size<32-bit> to ::SIZE
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  struct native_conversion<Size<T>, enable_if_sizeof_t<T,int32_t>> 
  {
    //! \alias input_t - Define input type
    using input_t = Size<T>;

    //! \alias result_t - Define output type
    using result_t = ::SIZE;
  };


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Prints a Size to the debug console
  //!
  //! \tparam T - Size field type
  //! 
  //! \param[in,out] &c - Debugging console
  //! \param[in] const& sz - Size
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  Console& operator << (Console& c, const Size<T>& sz)
  {
    return c << make_nvpair_tuple("width",  sz.width, 
                                  "height", sz.height);
  };


} // WTL namespace



#endif // WTL_SIZE_HPP


