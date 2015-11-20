//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\casts\OpaqueCast.hpp
//! \brief Converts between LPARAM and any type
//! \date 20 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_OPAQUE_CAST_HPP
#define WTL_OPAQUE_CAST_HPP

#include <wtl/WTL.hpp>
#include <type_traits>                  //!< std::enable_if, std::integral_constant
#include <wtl/utils/SFINAE.hpp>         //!< Various expressions

//! \namespace wtl - Windows template library
namespace wtl
{
  //////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::opaque_cast
  //! Encodes the address of an object (of type too large to fit within an LPARAM) into an LPARAM
  //! 
  //! \param[in] const& value - Object
  //! \return LPARAM - Address of object encoded into 32/64-bit opaque data
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename = enable_if_larger_t<T,LPARAM>>
  LPARAM  opaque_cast(const T& value) 
  {
    return reinterpret_cast<LPARAM>(&value);
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::opaque_cast
  //! Encodes a pointer into an LPARAM
  //! 
  //! \param[in] value - Value to encode
  //! \return LPARAM - Address encoded into 32/64-bit opaque data
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  LPARAM opaque_cast(T* value) 
  {
    return reinterpret_cast<LPARAM>(value);
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::opaque_cast
  //! Encodes the value of an object (of type smaller/equal in size to an LPARAM) into an LPARAM
  //! 
  //! \param[in] value - Value to encode
  //! \return LPARAM - Input value encoded into 32/64-bit opaque data
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename = enable_if_smaller_or_equal_t<T,LPARAM>>
  LPARAM opaque_cast(T value) 
  {
    return static_cast<LPARAM>(value);
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::opaque_cast
  //! Encodes two WORDs into an LPARAM
  //! 
  //! \param[in] low - Low word
  //! \param[in] high - High word
  //! \return LPARAM - Input values packed into 32/64-bit opaque data
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename U> 
  LPARAM  opaque_cast(T low, U high)
  {
    // Encode value 
    return MAKELPARAM(low,high);
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::opaque_cast
  //! Decode the address of an object (of type larger than an LPARAM) previously encoded into an LPARAM
  //! 
  //! \tparam T - Any type
  //! 
  //! \param[in] data - 32/64-bit opaque data containing an address
  //! \return T* - Decoded address
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  auto  opaque_cast(LPARAM data) -> enable_if_larger_t<T,LPARAM,T*>
  {
    return reinterpret_cast<T*>(data);
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::opaque_cast
  //! Decode the address of an object (of pointer type) previously stored in an LPARAM
  //! 
  //! \tparam T - Pointer type
  //! 
  //! \param[in] data - 32/64-bit opaque data containing an address
  //! \return T* - Decoded address
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  auto opaque_cast(LPARAM data) -> enable_if_pointer_t<T,T*>
  {
    return reinterpret_cast<T*>(data);
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::opaque_cast
  //! Decode the value of an object (of type smaller or equal than an LPARAM) previously stored in an LPARAM
  //! 
  //! \tparam T - Any type
  //! 
  //! \param[in] data - 32/64-bit opaque data containing a value
  //! \return T - Decoded value 
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  auto opaque_cast(LPARAM data) -> enable_if_smaller_or_equal_t<T,LPARAM,T>
  {
    return static_cast<T>(data);
  }

} // WTL namespace



#endif // WTL_OPAQUE_CAST_HPP


