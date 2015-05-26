//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\DebugInfo.hpp
//! \brief Provides a variadic helper type for writing any object to the debug console
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_DEBUG_INFO_HPP
#define WTL_DEBUG_INFO_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/ForEach.hpp"            //!< for_each
#include "wtl/utils/Point.hpp"              //!< Point
#include "wtl/utils/NameValuePair.hpp"      //!< NameValuePair
#include "wtl/io/Console.hpp"               //!< wtl::Console
#include "wtl/io/StreamIterator.hpp"        //!< wtl::StreamIterator
#include <tuple>                            //!< std::tuple
#include <type_traits>                      //!< std::enable_if

//! \namespace wtl - Windows template library
namespace wtl
{
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct DebugInfo - Helper object for writing object instances to the debug console
  //! 
  //! \tparam ARGS... - Attribute name/value pair types
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename... ARGS>
  struct DebugInfo
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias storage_t - Attributes collection
    using storage_t = std::tuple<ARGS...>;

    //! \var length - Number of attribute name/value pairs
    static constexpr uint32  length = sizeof...(ARGS);   
    
    // ----------------------------------- REPRESENTATION -----------------------------------

    const char*  Name;          //!< Type name
    storage_t    Attributes;    //!< Attribute name/value pair storage

    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    //////////////////////////////////////////////////////////////////////////////////////////
    // DebugInfo::DebugInfo
    //! Create from type name and attributes
    //! 
    //! \param[in] const* name - Type name
    //! \param[in] &&... args - [optional] Attribute name/value pairs
    //////////////////////////////////////////////////////////////////////////////////////////
    DebugInfo(const char* name, ARGS&&... args) : Name(name), 
                                                  Attributes(std::forward<ARGS>(args)...)
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
    ENABLE_COPY(DebugInfo);      //!< Can be deep copied
    ENABLE_MOVE(DebugInfo);      //!< Can be moved 
    ENABLE_POLY(DebugInfo);      //!< Can be polymorphic
  };


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::debug_info
  //! Create debug info for an object 
  //! 
  //! \param[in] const* name - Name of the type 
  //! \param[in] &&... args - [optional] Alternating sequence of attribute name/value pair c-tor arguments
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename... ARGS>
  DebugInfo<ARGS...>  debug_info(const char* name, ARGS&&... args)
  {
    static_assert(sizeof...(ARGS) % 2 == 0, "Cannot create debug-info from an odd number of arguments");

    // Interpret arguments as pairs of nvp c-tor arguments
    return DebugInfo<ARGS...>(name, name_value_pairs(std::forward<ARGS>(args)...));
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Write a tuple to the console
  //!
  //! \tparam ARGS... - Tuple element types
  //! 
  //! \param[in,out] &c - Console
  //! \param[in] const& t - Any tuple
  //! \return Console& : Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename... ARGS>
  Console&  operator << (Console& c, const std::tuple<ARGS...>& t)
  {
    // Write opening tag
    c << Cons::Grey << '{';

    // Write elements
    if (sizeof...(ARGS) > 0)
    {
      // Write first 
      c << std::get<0>(t);

      // Delimit remainder, if any
      for_each<1>(t, [&c] (auto& e) { c << ' ' << e;  } );
    }

    // Write closing tag
    return c << Cons::Grey << '}';
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Write debug-info and attributes to the console
  //!
  //! \tparam ARGS... - Attribute name/value pair types
  //! 
  //! \param[in,out] &c - Console
  //! \param[in] const& info - Debug info
  //! \return Console& : Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename... ARGS>
  Console&  operator << (Console& c, const DebugInfo<ARGS...>& info)
  {
    // Preface attributes with name
    return c << Cons::Cyan << info.Name << info.Attributes;
  }


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Prints a Point to the debug console
  //!
  //! \tparam T - Point field type
  //! 
  //! \param[in,out] &c - Debugging console
  //! \param[in] const &pt - Point
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  Console& operator << (Console& c, const Point<T>& pt)
  {
    return c << name_value_pairs("x", pt.x, 
                                 "y", pt.y);
  }
} // namespace wtl

#endif // WTL_DEBUG_INFO_HPP


