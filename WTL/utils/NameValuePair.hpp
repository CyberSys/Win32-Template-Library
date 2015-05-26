//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\NameValuePair.hpp
//! \brief Defines a name value pair
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_NAME_VALUE_PAIR_HPP
#define WTL_NAME_VALUE_PAIR_HPP

#include "wtl/WTL.hpp"
#include "wtl/io/Console.hpp"       //!< Console
#include "wtl/utils/Point.hpp"      //!< Point

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct NameValuePair - Provides an immutable name/value pair
  //! 
  //! \tparam VALUE - Value type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE>
  struct NameValuePair
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    const char*   Name;     //!< Name
    const VALUE&  Value;    //!< Value reference

    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    /////////////////////////////////////////////////////////////////////////////////////////
    // NameValuePair::NameValuePair 
    //! Create a name-value pair
    //! 
    //! \param[in] const* name - Name
    //! \param[in] const& value - Value
    /////////////////////////////////////////////////////////////////////////////////////////
    NameValuePair(const char* name, const VALUE& value) : Name(name), Value(value)
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
    ENABLE_COPY(NameValuePair);      //!< Can be deep copied
    ENABLE_MOVE(NameValuePair);      //!< Can be moved 

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
  };


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::name_value_pair 
  //! Creates a name/value pair
  //! 
  //! \tparam VALUE - Value type
  //! 
  //! \param[in] const* name - Name
  //! \param[in] const& value - Value
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE>
  NameValuePair<VALUE>  name_value_pair(const char* name, const VALUE& value)
  {
    return NameValuePair<VALUE>(name, value);
  }
  

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::name_value_pairs
  //! Base-case for creating tuples of name/value pairs
  //! 
  //! \tparam VALUE - Value type
  //! 
  //! \param[in] const* name - Name
  //! \param[in] const& value - Value
  //! \return std::tuple<NameValuePair<VALUE>> - Tuple containing a single pair
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE>
  std::tuple<NameValuePair<VALUE>>  name_value_pairs(const char* name, const VALUE& value)
  {
    return std::make_tuple(NameValuePair<VALUE>(name, value));
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::name_value_pairs
  //! Creates a tuple of name/value pairs
  //! 
  //! \tparam VALUE - First pair value type
  //! 
  //! \param[in] const* name - First pair name
  //! \param[in] const& value - First pair value
  //! \param[in] &&...args - [optional] Subsequent name/value pair arguments
  //! \return std::tuple<NameValuePair<V1>, NameValuePair<V2>, ...> - Tuple of name-value pairs
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE, typename... ARGS>
  auto  name_value_pairs(const char* name, const VALUE& value, ARGS&&... args)
  {
    static_assert(sizeof...(ARGS) % 2 == 0, "Cannot create name-value pairs from an odd number of arguments");

    // Prepend & return first pair to anonymous sequence generated recursively
    return std::tuple_cat(name_value_pairs(name,value), name_value_pairs(args...));
  }
  
  
  

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes a non-string name-value pair to the debug console
  //!
  //! \tparam VALUE - Value type
  //! 
  //! \param[in,out] &c - Debug console
  //! \param[in] const& value - Name-Value pair representing a named value
  //! \return Console& : Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE>
  auto  operator << (Console& c, const NameValuePair<VALUE>& value) -> std::enable_if_t<!std::is_convertible<VALUE,const char*>::value 
                                                                                     && !std::is_convertible<VALUE,const wchar_t*>::value, Console&>
  {
    // Write: Name=Value
    return c << Cons::White  << value.Name 
             << Cons::Grey   << '='
             << Cons::Yellow << value.Value;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes a string-based name-value pair to the debug console
  //!
  //! \tparam VALUE - Value type
  //! 
  //! \param[in,out] &c - Debug console
  //! \param[in] const& value - Name-Value pair representing a named value
  //! \return Console& : Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE>
  auto  operator << (Console& c, const NameValuePair<VALUE>& value) -> std::enable_if_t<std::is_convertible<VALUE,const char*>::value 
                                                                                     || std::is_convertible<VALUE,const wchar_t*>::value, Console&>
  {
    // Write: Name='Value'
    return c << Cons::White  << value.Name 
             << Cons::Grey   << "='"
             << Cons::Yellow << value.Value 
             << Cons::Grey   << "'";
  }

}



#endif // WTL_NAME_VALUE_PAIR_HPP


