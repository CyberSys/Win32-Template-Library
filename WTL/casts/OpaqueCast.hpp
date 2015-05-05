//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\OpaqueCast.hpp
//! \brief Converts between LPARAM and any type
//! \date 8 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_OPAQUE_CAST_HPP
#define WTL_OPAQUE_CAST_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{

  //////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::opaque_cast
  //! Encode the address of a value of type too large to fit within an LPARAM
  //! 
  //! \param[in] const& value - Reference to value too large to fit within an LPARAM
  //! \return LPARAM - 32-bit opaque data
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename = std::enable_if_t<(sizeof(T) > sizeof(LPARAM)) && !std::is_same<T,LPARAM>::value>>
  LPARAM  opaque_cast(const T& value)
  {
    // Encode address of object
    return reinterpret_cast<LPARAM>(&value);
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::opaque_cast
  //! Encodes any value of type small enough to fit within an LPARAM
  //! 
  //! \param[in] value - Value of type small enough to fit within an LPARAM
  //! \return LPARAM - 32-bit opaque data
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename = std::enable_if_t<(sizeof(T) <= sizeof(LPARAM)) && !std::is_same<T,LPARAM>::value>>
  LPARAM  opaque_cast(T value)
  {
    // Encode value 
    //return static_cast<LPARAM>(value);
    return LPARAM(value);
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::opaque_cast
  //! Encodes two WORDs into an LPARAM
  //! 
  //! \param[in] low - Low word
  //! \param[in] high - High word
  //! \return LPARAM - 32-bit opaque data
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename U>   //typename = std::enable_if_t<(sizeof(T) == sizeof(LPARAM)/2)>>
  LPARAM  opaque_cast(T low, U high)
  {
    // Encode value 
    return MAKELPARAM(low,high);
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::opaque_cast
  //! Decode any object's value or address from an LPARAM, expecting an address for types too large to fit
  //! 
  //! \tparam T - Any type
  //! 
  //! \param[in] data - Win32 opaque data
  //! \return auto - Value type if small enough to fit in an LPARAM, otherwise pointer type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  T opaque_cast(LPARAM data, std::enable_if_t<(sizeof(T) < sizeof(LPARAM))>* = nullptr)
  {
    return static_cast<T>(data);
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::opaque_cast
  //! Decode any object's value or address from an LPARAM, expecting an address for types too large to fit
  //! 
  //! \tparam T - Any type
  //! 
  //! \param[in] data - Win32 opaque data
  //! \return auto - Value type if small enough to fit in an LPARAM, otherwise pointer type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  T opaque_cast(LPARAM data, std::enable_if_t<sizeof(T) == sizeof(LPARAM)>* = nullptr)
  {
    return *reinterpret_cast<T*>(&data);
    //return reinterpret_cast<T>(data);
    //return (T)data; 
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::opaque_cast
  //! Decode any object's value or address from an LPARAM, expecting an address for types too large to fit
  //! 
  //! \tparam T - Any type
  //! 
  //! \param[in] data - Win32 opaque data
  //! \return auto - Value type if small enough to fit in an LPARAM, otherwise pointer type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename = std::enable_if_t<(sizeof(T) > sizeof(LPARAM))>>
  T*  opaque_cast(LPARAM data)
  {
    return reinterpret_cast<T*>(data);
  }
  

} // WTL namespace



#endif // WTL_OPAQUE_CAST_HPP


