////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\Enumeration.hpp
//! \brief Defines enumeration traits + helper global operators
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ENUMERATION_TRAITS_HPP
#define WTL_ENUMERATION_TRAITS_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \typedef string_literal_t - String literal decay type
  typedef const char* string_literal_t;

    
  /////////////////////////////////////////////////////////////
  //! \struct enum_traits - Defines enumeration traits
  //! 
  //! \tparam E - Type used to define specialization
  /////////////////////////////////////////////////////////////
  //template <typename E>
  //struct enum_traits
  //{
  //  static_assert(std::is_enum<E>::value, "Only enumeration types supported");

  //  //! \var is_flag - Whether enum values can be combined
  //  static constexpr bool is_flag = false;
  //  
  //  //! \var is_contiguous - Whether enum values can be incremented (ie. form a contiguous range)
  //  static constexpr bool is_contiguous = true;
  //};
  
  /////////////////////////////////////////////////////////////
  //! \struct is_attribute - Defines whether a type can be combined with bitwise OR
  //! 
  //! \tparam E - Type used to define specialization
  /////////////////////////////////////////////////////////////
  template <typename E>
  struct is_attribute : std::false_type
  {};
  
  /////////////////////////////////////////////////////////////
  //! \struct is_contiguous - Defines a type forms a contiguous range of values (true for arithmetic+enums, false for other types)
  //! 
  //! \tparam E - Type used to define specialization
  /////////////////////////////////////////////////////////////
  template <typename E>
  struct is_contiguous : std::conditional<std::is_enum<E>::value||std::is_arithmetic<E>::value, std::true_type, std::false_type>
  {};

  /////////////////////////////////////////////////////////////
  //! \struct enum_names - Defines names for enumeration literals
  //! 
  //! \tparam E - Enumeration type
  /////////////////////////////////////////////////////////////
  template <typename E>
  struct enum_names
  {
    //! \var values - String representations of values
    static const char* values[];
  };
  
  /////////////////////////////////////////////////////////////
  //! \struct enum_values - Provides a contiguous values array for enumeration literals
  //! 
  //! \tparam E - Enumeration type
  /////////////////////////////////////////////////////////////
  template <typename E>
  struct enum_values
  {
    //! \var values - Values array
    static const E values[];
  };

  /////////////////////////////////////////////////////////////
  //! \struct min_value - Defines minimum value for a type
  //! 
  //! \tparam E - Type used to define specialization
  /////////////////////////////////////////////////////////////
  template <typename E>
  struct min_value : std::integral_constant<E,0>
  {};

  /////////////////////////////////////////////////////////////
  //! \struct max_value - Defines maximum value for a type
  //! 
  //! \tparam E - Type used to define specialization
  /////////////////////////////////////////////////////////////
  template <typename E>
  struct max_value
  {
    //! \var value - Maximum allowed value (inclusive)
    static constexpr E  value = 0;
  };

  
  /////////////////////////////////////////////////////////////
  // wtl::toString 
  //! Get the string representation of an enumeration value
  //! 
  //! \tparam ENUM - Enumeration type (for contiguous sets of values)
  //! 
  //! \param[in] e - Enumeration value
  //! \return const char* - String representation
  /////////////////////////////////////////////////////////////
  template <typename ENUM, typename = std::enable_if_t<is_attribute<ENUM>::value>> 
  const char* toString(ENUM e)
  {
    CHECKED_INDEX(e, min_value<ENUM>::value, max_value<ENUM>::value+1);

    // Lookup name
    return enum_names<ENUM>::values[static_cast<std::underlying_type_t<ENUM>>(e)
                                  - static_cast<std::underlying_type_t<ENUM>>(min_value<ENUM>::value)];
  }

  
  /////////////////////////////////////////////////////////////
  // wtl::toString 
  //! Get the string representation of an enumeration value
  //! 
  //! \tparam ENUM - Enumeration type (for non-contiguous sets of values)
  //! 
  //! \param[in] e - Enumeration value
  //! \return const char* - String representation
  /////////////////////////////////////////////////////////////
  template <typename ENUM> 
  const char* toString(ENUM e, std::enable_if_t<!is_attribute<ENUM>::value>* = nullptr)
  {
    CHECKED_INDEX(e, min_value<ENUM>::value, max_value<ENUM>::value+1);

    // Lookup name
    auto last = std::end(enum_values<ENUM>::values);
    auto name = std::find(std::begin(enum_values<ENUM>::values), last, e);
     
    // Return if found
    return name != last ? *name : "Error";
  }
  
  ////////////////////////////////////////////////////////////////////////////////
  // wtl::parseEnum 
  //! Parses an enumeration value from its string representation 
  //! 
  //! \tparam ENUM - Enumeration type
  //! 
  //! \param[in] const* str - String representation
  //! \param[in,out] &value - Output enumeration value, if parsed successfully
  //! \return bool - True iff parsed successfully
  ////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename = std::enable_if_t<std::is_enum<ENUM>::value 
                                                    && is_attribute<ENUM>::value>> 
  inline bool parseEnum(const char* str, ENUM& value)
  {
    static_assert(std::is_enum<ENUM>::value, "Cannot parse from non-enumeration types");
    static_assert(is_attribute<ENUM>::value, "Cannot parse from non-iteratable enumerations");

    REQUIRED_PARAM(str);
    
    // Linear search, from min to max values
    for (ENUM it = min_value<ENUM>::value; it < max_value<ENUM>::value+1; it = it + 1)
      if (stricmp(str, toString(it)) == 0)
        // Set value and return true
        return (value = it, true);

    // Return false and set value to default
    return (value = zero<ENUM>::value, false);
  }
  
  ////////////////////////////////////////////////////////////////////////////////
  // wtl::operator | constexpr
  //! Compile-time bitwise-OR operator for combining enumeration with values without casting iff 
  //! their type traits specify they support the operation
  //!
  //! \tparam ENUM - Enumeration type
  //!
  //! \param[in] a - Enumeration 
  //! \param[in] b - Value
  //! \return ENUM - Bitwise-or combination of both values
  ////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename VALUE, typename = std::enable_if_t<std::is_enum<ENUM>::value>> constexpr
  ENUM operator| (ENUM a, VALUE b)
  {
    static_assert(is_attribute<ENUM>::value, "Enumeration does not support bitwise OR");

    return static_cast<ENUM>(static_cast<std::underlying_type_t<ENUM>>(a) 
                           | static_cast<std::underlying_type_t<ENUM>>(b));
  }
  
  ////////////////////////////////////////////////////////////////////////////////
  // wtl::operator | constexpr
  //! Compile-time bitwise-OR operator for combining two enumerations without casting iff 
  //! their type traits specify they support the operation
  //!
  //! \tparam A - Enumeration type
  //! \tparam B - Enumeration type
  //!
  //! \param[in] a - Enumeration value
  //! \param[in] b - Another value
  //! \return A - Bitwise-or combination of both values
  ////////////////////////////////////////////////////////////////////////////////
  /*template <typename A, typename B, typename = std::enable_if_t<std::is_enum<A>::value && std::is_enum<B>::value>> constexpr
  A operator| (A a, B b)
  {
    static_assert(is_attribute<A>::value, "LHS enumeration does not support bitwise OR");
    static_assert(is_attribute<B>::value, "RHS enumeration does not support bitwise OR");
    static_assert(sizeof(std::underlying_type_t<A>) != sizeof(std::underlying_type_t<B>), "Cannot bitwise combine attributes of different sizes");

    return static_cast<A>(static_cast<std::underlying_type_t<A>>(a) 
                        | static_cast<std::underlying_type_t<B>>(b));
  }*/


  ////////////////////////////////////////////////////////////////////////////////
  // wtl::operator|= constexpr
  //! Compile-time bitwise-OR assignment operator for combining enumerations without casting iff 
  //! their type traits specify they support the operation
  //!
  //! \tparam ENUM - Enumeration type
  //!
  //! \param[in,out] &a - Value to modify
  //! \param[in] const &b - Value to be combined with 'a'
  //! \return ENUM& - Reference to 'a' (now combined with 'b')
  ////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM> constexpr
  std::enable_if_t<std::is_enum<ENUM>::value && is_attribute<ENUM>::value, ENUM&>
  /*ENUM&*/ operator|= (ENUM& a, const ENUM& b)
  {
    return a = a | b;
  }
  
  ////////////////////////////////////////////////////////////////////////////////
  // wtl::operator & constexpr
  //! Compile-time bitwise-AND operator for combining enumerations without casting iff their type
  //! traits specify they support the operation
  //!
  //! \tparam ENUM - Enumeration type
  //!
  //! \param[in] const &a - Enumeration value
  //! \param[in] const &b - Another value
  //! \return ENUM - Bitwise-or combination of both values
  ////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename VALUE, typename = std::enable_if_t<std::is_enum<ENUM>::value>> constexpr
  ENUM operator& (ENUM a, VALUE b)
  {
    static_assert(is_attribute<ENUM>::value, "Enumeration does not support bitwise AND");

    return static_cast<ENUM>(static_cast<std::underlying_type_t<ENUM>>(a) 
                           & static_cast<std::underlying_type_t<ENUM>>(b));
  }


  ////////////////////////////////////////////////////////////////////////////////
  // wtl::operator&= constexpr
  //! Compile-time bitwise-AND assignment operator for combining enumerations without casting iff
  //! their type traits specify they support the operation
  //!
  //! \tparam ENUM - Enumeration type
  //!
  //! \param[in,out] &a - Value to modify
  //! \param[in] const &b - Value to be combined with 'a'
  //! \return ENUM& - Reference to 'a' (now combined with 'b')
  ////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM> constexpr
  std::enable_if_t<std::is_enum<ENUM>::value && is_attribute<ENUM>::value, ENUM&>
  /*ENUM&*/ operator&= (ENUM& a, const ENUM& b)
  {
    return a = a & b;
  }
  
  
  ////////////////////////////////////////////////////////////////////////////////
  // wtl::operator++ constexpr
  //! Compile-time prefix increment operator for enumerations (without casting) iff their type traits 
  //! specify they support the operation
  //!
  //! \tparam ENUM - Enumeration type
  //!
  //! \param[in,out] &a - Value to modify
  //! \return ENUM& - Reference to incremented 'a'
  ////////////////////////////////////////////////////////////////////////////////
  //template <typename ENUM> constexpr 
  //std::enable_if_t<std::is_enum<ENUM>::value && enum_traits<ENUM>::can_iterate, ENUM&>
  //operator++ (ENUM& a)
  //{
  //  return a; // = a + 1;
  //}
  //template <typename ENUM> constexpr 
  //std::enable_if_t<std::is_enum<ENUM>::value && enum_traits<ENUM>::can_iterate, ENUM>
  //operator++ (ENUM& a, int)
  //{
  //  return a; // = a + 1;
  //}

  ////////////////////////////////////////////////////////////////////////////////
  // wtl::operator- constexpr 
  //! Compile-time substraction operator for enumerations
  //!
  //! \tparam ENUM - Enumeration type
  //!
  //! \param[in] const &a - Enumeration value
  //! \param[in] const &b - Value
  //! \return ENUM - Difference of both values 
  ////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename = std::enable_if_t<std::is_enum<ENUM>::value>> constexpr
  ENUM operator- (const ENUM& a, const std::underlying_type_t<ENUM>& b)
  {
    return static_cast<ENUM>( static_cast<std::underlying_type_t<ENUM>>(a) - b );
  }
  
  ////////////////////////////////////////////////////////////////////////////////
  // wtl::operator + constexpr 
  //! Compile-time addition operator for enumerations
  //!
  //! \tparam ENUM - Enumeration type
  //!
  //! \param[in] const &a - Enumeration value
  //! \param[in] const &b - Value
  //! \return ENUM - Sum of both values 
  ////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename = std::enable_if_t<std::is_enum<ENUM>::value>> constexpr
  ENUM operator+ (const ENUM& a, const std::underlying_type_t<ENUM>& b)
  {
    return static_cast<ENUM>( static_cast<std::underlying_type_t<ENUM>>(a) + b );
  }
  
  
  ////////////////////////////////////////////////////////////////////////////////
  // wtl::operator < constexpr
  //! Compile-time less-than relational operator for comparing an enumeration against any value to
  //! which the enumeration can be implicitly cast
  //!
  //! \tparam ENUM - Enumeration type
  //! \tparam VALUE - Value type
  //!
  //! \param[in] const &a - Enumeration value
  //! \param[in] const &b - Any value
  //! \return bool - True iff a is less than b
  ////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename VALUE> constexpr
  std::enable_if_t<std::is_enum<ENUM>::value, bool> 
  /*bool*/ operator < (const ENUM& a, const VALUE& b)
  {
    return static_cast<std::underlying_type_t<ENUM>>(a) < b;
  }
  

  ////////////////////////////////////////////////////////////////////////////////
  // wtl::operator > constexpr
  //! Compile-time greater-than relational operator for comparing an enumeration against any value 
  //! to which the enumeration can be implicitly cast
  //!
  //! \tparam ENUM - Enumeration type
  //! \tparam VALUE - Type of value to compare against
  //!
  //! \param[in] const &a - Enumeration value
  //! \param[in] const &b - Any value
  //! \return bool - True iff a is greater-than than b
  ////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename VALUE> constexpr
  std::enable_if_t<std::is_enum<ENUM>::value, bool> 
  /*bool*/ operator > (const ENUM& a, const VALUE& b)
  {
    return static_cast<std::underlying_type_t<ENUM>>(a) 
         > static_cast<std::underlying_type_t<ENUM>>(b);
  }



  ////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <= constexpr
  //! Compile-time less-than-or-equal relational operator for comparing an enumeration against any 
  //! value to which the enumeration can be implicitly cast
  //!
  //! \tparam ENUM - Enumeration type
  //! \tparam VALUE - Type of value to compare against
  //!
  //! \param[in] const &a - Enumeration value
  //! \param[in] const &b - Any value
  //! \return bool - True iff a is less-than-or-equal than b
  ////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename VALUE> constexpr
  std::enable_if_t<std::is_enum<ENUM>::value, bool> 
  /*bool*/ operator <= (const ENUM& a, const VALUE& b)
  {
    return static_cast<std::underlying_type_t<ENUM>>(a) 
        <= static_cast<std::underlying_type_t<ENUM>>(b);
  }
  

  ////////////////////////////////////////////////////////////////////////////////
  // wtl::operator >= constexpr
  //! Compile-time greater-than-or-equal relational operator for comparing an enumeration against 
  //! any value to which the enumeration can be implicitly cast
  //!
  //! \tparam ENUM - Enumeration type
  //! \tparam VALUE - Type of value to compare against
  //!
  //! \param[in] const &a - Enumeration value
  //! \param[in] const &b - Any value
  //! \return bool - True iff a is greater-than-or-equal than b
  ////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename VALUE> constexpr
  std::enable_if_t<std::is_enum<ENUM>::value, bool> 
  /*bool*/ operator >= (const ENUM& a, const VALUE& b)
  {
    return static_cast<std::underlying_type_t<ENUM>>(a) 
        >= static_cast<std::underlying_type_t<ENUM>>(b);
  }
  
  ////////////////////////////////////////////////////////////////////////////////
  // wtl::operator == constexpr
  //! Compile-time equality operator for enumerations
  //!
  //! \tparam ENUM - Enumeration type
  //! \tparam VALUE - Type of value to compare against
  //!
  //! \param[in] const &a - Enumeration value
  //! \param[in] const &b - Any value
  //! \return bool - True iff a is greater-than-or-equal than b
  ////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename VALUE> constexpr
  std::enable_if_t<std::is_enum<ENUM>::value, bool> 
  /*bool*/ operator == (const ENUM& a, const VALUE& b)
  {
    return static_cast<std::underlying_type_t<ENUM>>(a) 
        == static_cast<std::underlying_type_t<ENUM>>(b);
  }

} //namespace wtl
#endif // WTL_ENUMERATION_TRAITS_HPP

