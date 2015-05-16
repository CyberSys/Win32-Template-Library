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
#include "wtl/utils/Point.hpp"              //!< Point
#include "wtl/utils/NameValuePair.hpp"      //!< NameValuePair
#include "wtl/io/Console.hpp"               //!< wtl::Console
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
    
    //! \var attributes - Whether element has any attributes
    static constexpr bool  attributes = (length != 0);
    
    // ----------------------------------- REPRESENTATION -----------------------------------

    const char*  Name;          //!< Type name
    storage_t    Attributes;    //!< Attribute name/value pair storage

    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    //////////////////////////////////////////////////////////////////////////////////////////
    // DebugInfo::DebugInfo
    //! Define type name and attributes
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
  //! Create debug info for an instance and specify its attributes
  //! 
  //! \param[in] const& name - Type name 
  //! \param[in] &&... args - [optional] Attribute name/value pairs
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename... ARGS>
  DebugInfo<ARGS...>  debug_info(const char* name, ARGS&&... args)
  {
    return DebugInfo<ARGS...>(name, std::forward<ARGS>(args)...);
  }


  //////////////////////////////////////////////////////////////////////////////////////////
	//! \struct info_unpacker - Writes a name/value pair tuple to an output stream
  //! 
  //! \tparam IDX - Zero-based iteration index
  //! \tparam ARGS - Name/value pairs
  //////////////////////////////////////////////////////////////////////////////////////////
  template <unsigned IDX, unsigned COUNT, typename... ARGS>
  struct info_unpacker
  {
    //////////////////////////////////////////////////////////////////////////////////////////
	  //! info_unpacker::unpack
    //! Unpacks attributes and writes them as a sequence of space-delimited name/value pairs
    //! 
    //! \param[in,out] &c - Console
    //! \param[in] const& info - Debug info
    //////////////////////////////////////////////////////////////////////////////////////////
    static void unpack(Console& c, const DebugInfo<ARGS...>& info)
    {
      c << ' ' << std::get<IDX>(info.Attributes);
      info_unpacker<IDX+1,COUNT,ARGS...>::unpack(c, info);
    }
  };


  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct info_unpacker - Base case 
  //////////////////////////////////////////////////////////////////////////////////////////
  template <unsigned COUNT, typename... ARGS>
  struct info_unpacker<COUNT, COUNT, ARGS...>
  {
    static void unpack(Console& c, const DebugInfo<ARGS...>& info)
    { /*no-op*/ }
  };


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes object debug info to the debug console
  //!
  //! \tparam ARGS... - Attribute name/value pair types
  //! 
  //! \param[in,out] &c - Console
  //! \param[in] const& info - Debug info
  //! \return Console& : Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename... ARGS>
  std::enable_if_t<DebugInfo<ARGS...>::length != 0, Console&>
  /*Console&*/ operator << (Console& c, const DebugInfo<ARGS...>& info)
  {
    static_assert(sizeof...(ARGS) != 0, "Invalid parameter pack size");

    // Open tag
    c << Cons::Yellow << '{';
    
    // Write attributes
    c << Cons::White  << info.Name << ':';
    info_unpacker<0,sizeof...(ARGS),ARGS...>::unpack(c, info);

    // Close
    return c << Cons::Yellow << '}';
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes the opening tag of object debug info to the debug console
  //!
  //! \tparam ARGS... - Empty pack
  //! 
  //! \param[in,out] &c - Console
  //! \param[in] const& info - Debug info
  //! \return Console& : Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename... ARGS>
  std::enable_if_t<DebugInfo<ARGS...>::length == 0, Console&>
  /*Console&*/ operator << (Console& c, const DebugInfo<ARGS...>& info)
  {
    static_assert(sizeof...(ARGS) == 0, "Invalid parameter pack size");

    // Write opening tag
    return c << Cons::Yellow << '{'
             << Cons::White  << info.Name << ':';
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
    return c << debug_info("Point", name_value_pair("x", pt.x), 
                                    name_value_pair("y", pt.y));
  }
} // namespace wtl

#endif // WTL_DEBUG_INFO_HPP


