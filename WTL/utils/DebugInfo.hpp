//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\RTVI.hpp
//! \brief Provides run-time value introspection for any type
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
  //! \struct ObjectInfo - Encapsulates the name of an object, its properties, and their values to enable runtime value introspection
  //! 
  //! \tparam ARGS... - Attribute name/value pair types
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename... ARGS>
  struct ObjectInfo
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias storage_t - Attributes collection
    using storage_t = std::tuple<ARGS...>;

    //! \var length - Number of attribute name/value pairs
    static constexpr uint32_t  length = sizeof...(ARGS);   
    
    // ----------------------------------- REPRESENTATION -----------------------------------

    const char*  Name;          //!< Type name
    storage_t    Attributes;    //!< Attribute name/value pair storage

    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    //////////////////////////////////////////////////////////////////////////////////////////
    // ObjectInfo::ObjectInfo
    //! Create from type name and attributes
    //! 
    //! \param[in] const* name - Type name
    //! \param[in] const& attr - Tuple of attribute name/value pairs
    //////////////////////////////////////////////////////////////////////////////////////////
    ObjectInfo(const char* name, const std::tuple<ARGS...>& attr) : Name(name), Attributes(attr)
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
    ENABLE_COPY(ObjectInfo);      //!< Can be deep copied
    ENABLE_MOVE(ObjectInfo);      //!< Can be moved 
    ENABLE_POLY(ObjectInfo);      //!< Can be polymorphic
  };
  

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::make_object_info
  //! Create runtime value
  //! 
  //! \tparam PAIRS - Name/value pair types
  //! 
  //! \param[in] const* name - Name of the type 
  //! \param[in] const& attr - Tuple of attribute name-value pairs
  //! \return ObjectInfo<PAIRS...> - Debug info
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename... PAIRS>
  ObjectInfo<PAIRS...>  make_object_info(const char* name, std::tuple<PAIRS...>& attr)
  {
    // Create debug info
    return ObjectInfo<PAIRS...>(name, attr);
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::object_info
  //! Create debug-info for an object from a list of names and values
  //! 
  //! \param[in] const* name - Name of the type 
  //! \param[in] &&... args - [optional] Alternating sequence of attribute name/value pair c-tor arguments
  //! \return ObjectInfo<auto> - Debug info
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename... ARGS>
  auto  object_info(const char* name, ARGS&&... args)
  {
    static_assert(sizeof...(ARGS) % 2 == 0, "Cannot create debug-info from an odd number of arguments");

    // Interpret arguments as pairs of nvp c-tor arguments
    return make_object_info(name, make_nvpair_tuple(std::forward<ARGS>(args)...));
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
  Console&  operator << (Console& c, const ObjectInfo<ARGS...>& info)
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
    return c << make_nvpair_tuple("x", pt.x, 
                                  "y", pt.y);
  }
} // namespace wtl

#endif // WTL_DEBUG_INFO_HPP


