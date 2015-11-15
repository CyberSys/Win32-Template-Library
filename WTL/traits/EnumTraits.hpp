//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\Enumeration.hpp
//! \brief Defines enumeration traits + helper global operators
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ENUMERATION_TRAITS_HPP
#define WTL_ENUMERATION_TRAITS_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/SFINAE.hpp>             //!< Type traits
#include <wtl/utils/Requires.hpp>           //!< requires
#include <iterator>                         //!< std::begin, std::end

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct is_attribute - Defines whether a type can be combined with bitwise OR
  //!
  //! \tparam E - Type used to define specialization
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename E>
  struct is_attribute : std::integral_constant<bool, false>
  {};


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct is_contiguous - Defines a type forms a contiguous range of values (true for arithmetic+enums, false for other types)
  //!
  //! \tparam E - Type used to define specialization
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename E>
  struct is_contiguous : std::integral_constant<bool, std::is_enum<E>::value || std::is_arithmetic<E>::value>
  {};

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct enable_if_attribute_t - Defines an SFINAE expression requiring an attribute enumeration
  //!
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename E, typename RET = void>
  using enable_if_attribute_t = std::enable_if_t<std::is_enum<E>::value && is_attribute<E>::value, RET>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct enable_if_contiguous_t - Defines an SFINAE expression requiring a contiguous enumeration
  //!
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename E, typename RET = void>
  using enable_if_contiguous_t = std::enable_if_t<std::is_enum<E>::value && is_contiguous<E>::value, RET>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct enable_if_not_attribute_t - Defines an SFINAE expression requiring a non-attribute enumeration
  //!
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename E, typename RET = void>
  using enable_if_not_attribute_t = std::enable_if_t<std::is_enum<E>::value && !is_attribute<E>::value, RET>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct enable_if_not_contiguous_t - Defines an SFINAE expression requiring a non-contiguous enumeration
  //!
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename E, typename RET = void>
  using enable_if_not_contiguous_t = std::enable_if_t<std::is_enum<E>::value && !is_contiguous<E>::value, RET>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \namespace concepts - Defines concepts used by 'Delegate'
  /////////////////////////////////////////////////////////////////////////////////////////
  namespace concepts
  {
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct AttributeEnumeration - Defines a concept requiring an enumeration of attribute type
    /////////////////////////////////////////////////////////////////////////////////////////
    struct AttributeEnumeration
    {
      template <typename U, typename = enable_if_attribute_t<U>> 
      static void* test( void* );
    };  

    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct ContiguousEnumeration - Defines a concept requiring an enumeration of contiguous type
    /////////////////////////////////////////////////////////////////////////////////////////
    struct ContiguousEnumeration
    {
      template <typename U, typename = enable_if_contiguous_t<U>> 
      static void* test( void* );
    };  
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct enum_names - Defines names for enumeration literals
  //!
  //! \tparam E - Enumeration type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename E>
  struct enum_names; /* Undefined */

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct enum_values - Provides a contiguous values array for enumeration literals
  //!
  //! \tparam E - Enumeration type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename E>
  struct enum_values; /* Undefined */


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enum_values_t - Enumeration literal sequence type accessor
  //!
  //! \tparam E - Enumeration type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename E>
  using enum_values_t = typename enum_values<E>::type;


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator | constexpr
  //! Compile-time bitwise-OR operator for combining enumeration with values without casting iff
  //! their type traits specify they support the operation
  //!
  //! \tparam ENUM - Enumeration type
  //! \tparam VALUE - Value type
  //!
  //! \param[in] a - Enumeration
  //! \param[in] b - Value
  //! \return ENUM - Bitwise-or combination of both values
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename VALUE> constexpr
  auto operator | (ENUM a, VALUE b) noexcept -> enable_if_enum_t<ENUM,ENUM>
  {
    concept_check(ENUM,AttributeEnumeration);
    
    // Perform operation upon underlying types
    return static_cast<ENUM>(static_cast<std::underlying_type_t<ENUM>>(a)
                           | static_cast<std::underlying_type_t<ENUM>>(b));
  }


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator|= constexpr
  //! Compile-time bitwise-OR assignment operator for combining enumerations without casting iff
  //! their type traits specify they support the operation
  //!
  //! \tparam ENUM - Enumeration type
  //! \tparam VALUE - Value type
  //!
  //! \param[in,out] &a - Value to modify
  //! \param[in] const &b - Value to be combined with 'a'
  //! \return ENUM& - Reference to 'a' (now combined with 'b')
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename VALUE> constexpr
  auto  operator|= (ENUM& a, VALUE b) noexcept -> enable_if_enum_t<ENUM, ENUM&>
  {
    concept_check(ENUM,AttributeEnumeration);

    return a = a | b;
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator & constexpr
  //! Compile-time bitwise-AND operator for combining enumerations without casting iff their type
  //! traits specify they support the operation
  //!
  //! \tparam ENUM - Enumeration type
  //! \tparam VALUE - Value type
  //!
  //! \param[in] const &a - Enumeration value
  //! \param[in] const &b - Another value
  //! \return ENUM - Bitwise-or combination of both values
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename VALUE> constexpr
  auto  operator& (ENUM a, VALUE b) -> enable_if_enum_t<ENUM,ENUM>
  {
    concept_check(ENUM,AttributeEnumeration);

    // Perform operation upon underlying types
    return static_cast<ENUM>(static_cast<std::underlying_type_t<ENUM>>(a)
                           & static_cast<std::underlying_type_t<ENUM>>(b));
  }


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator && constexpr
  //! Compile-time logical-AND operator for querying attributes without casting iff their type
  //! traits specify they support the operation
  //!
  //! \tparam ENUM - Enumeration type
  //! \tparam VALUE - Value type
  //!
  //! \param[in] const &a - Enumeration value
  //! \param[in] const &b - Another value
  //! \return bool - True iff one or more bits in common
  //! 
  //! \remarks Repurposing operators violates the principle of least surprise
  //! \remarks This should really be implemented as a function
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename VALUE> constexpr
  auto  operator && (ENUM a, VALUE b) -> enable_if_enum_t<ENUM,bool>
  {
    concept_check(ENUM,AttributeEnumeration);

    // Query whether all bits are present
    return (a & b) == static_cast<ENUM>(b);
  }


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator&= constexpr
  //! Compile-time bitwise-AND assignment operator for combining enumerations without casting iff
  //! their type traits specify they support the operation
  //!
  //! \tparam ENUM - Enumeration type
  //! \tparam VALUE - Value type
  //!
  //! \param[in,out] &a - Value to modify
  //! \param[in] const &b - Value to be combined with 'a'
  //! \return ENUM& - Reference to 'a' (now combined with 'b')
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename VALUE> constexpr
  auto  operator&= (ENUM& a, VALUE b) -> enable_if_enum_t<ENUM, ENUM&>
  {
    concept_check(ENUM,AttributeEnumeration);

    return a = a & b;
  }



  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator + constexpr
  //! Compile-time addition operator for enumerations
  //!
  //! \tparam ENUM - Enumeration type
  //! \tparam VALUE - Value type
  //!
  //! \param[in] const &a - Enumeration value
  //! \param[in] const &b - Another value
  //! \return ENUM - Sum of both values
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename VALUE> constexpr
  auto  operator + (ENUM a, VALUE b) -> enable_if_enum_t<ENUM,ENUM>
  {
    // Perform operation upon underlying types
    return static_cast<ENUM>(static_cast<std::underlying_type_t<ENUM>>(a)
                           + static_cast<std::underlying_type_t<ENUM>>(b));
  }


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator += constexpr
  //! Compile-time addition assignment operator for enumerations
  //!
  //! \tparam ENUM - Enumeration type
  //! \tparam VALUE - Value type
  //!
  //! \param[in,out] &a - Enumeration to modify
  //! \param[in] const &b - Value to be combined with 'a'
  //! \return ENUM& - Reference to 'a' (now summed with 'b')
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename VALUE> constexpr
  auto  operator += (ENUM& a, VALUE b) -> enable_if_enum_t<ENUM,ENUM&>
  {
    return a = a + b;
  }


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator - constexpr
  //! Compile-time addition operator for enumerations
  //!
  //! \tparam ENUM - Enumeration type
  //! \tparam VALUE - Value type
  //!
  //! \param[in] const &a - Enumeration value
  //! \param[in] const &b - Another value
  //! \return ENUM - Difference between a and b
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename VALUE> constexpr
  auto  operator - (ENUM a, VALUE b) -> enable_if_enum_t<ENUM,ENUM>
  {
    // Perform operation upon underlying types
    return static_cast<ENUM>(static_cast<std::underlying_type_t<ENUM>>(a)
                           - static_cast<std::underlying_type_t<ENUM>>(b));
  }


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator -= constexpr
  //! Compile-time addition assignment operator for enumerations
  //!
  //! \tparam ENUM - Enumeration type
  //! \tparam VALUE - Value type
  //!
  //! \param[in,out] &a - Enumeration to subtract from
  //! \param[in] const &b - Value to be subtracted from 'a'
  //! \return ENUM& - Reference to 'a'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename VALUE> constexpr
  auto  operator -= (ENUM& a, VALUE b) -> enable_if_enum_t<ENUM,ENUM&>
  {
    return a = a - b;
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator++ constexpr
  //! Compile-time prefix increment operator for contiguous enumerations
  //!
  //! \tparam ENUM - Enumeration type
  //!
  //! \param[in,out] &a - Value to increment
  //! \return ENUM& - Reference to incremented 'a'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM> constexpr
  auto  operator++ (ENUM& a) -> enable_if_enum_t<ENUM,ENUM&>
  {
    concept_check(ENUM,ContiguousEnumeration);

    return a = a + 1;
  }


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator++ constexpr
  //! Compile-time postfix increment operator for contiguous enumerations
  //!
  //! \tparam ENUM - Enumeration type
  //!
  //! \param[in,out] &a - Value to increment
  //! \param[in] - Ignored
  //! \return ENUM - Value upon input
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM> constexpr
  auto  operator++ (ENUM& a, int) -> enable_if_enum_t<ENUM,ENUM>
  {
    concept_check(ENUM,ContiguousEnumeration);

    return (++a, a - 1);      //!< NB: MSVC-14 does not support 'Extended constexpr' so only a single statement is alloed
  }


  //////////////////////////////////////////////////////////////////////////////////////////
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
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename VALUE> constexpr
  auto  operator < (const ENUM a, const VALUE b) -> enable_if_enum_t<ENUM,bool>
  {
    return static_cast<std::underlying_type_t<ENUM>>(a)
         < static_cast<std::underlying_type_t<ENUM>>(b);
  }

  //////////////////////////////////////////////////////////////////////////////////////////
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
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename VALUE> constexpr
  auto  operator <= (const ENUM a, const VALUE b) -> enable_if_enum_t<ENUM,bool>
  {
    return static_cast<std::underlying_type_t<ENUM>>(a)
        <= static_cast<std::underlying_type_t<ENUM>>(b);
  }


  //////////////////////////////////////////////////////////////////////////////////////////
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
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename VALUE> constexpr
  auto  operator > (const ENUM a, const VALUE b) -> enable_if_enum_t<ENUM,bool>
  {
    return static_cast<std::underlying_type_t<ENUM>>(a)
         > static_cast<std::underlying_type_t<ENUM>>(b);
  }

  //////////////////////////////////////////////////////////////////////////////////////////
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
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename VALUE> constexpr
  auto  operator >= (const ENUM a, const VALUE b) -> enable_if_enum_t<ENUM,bool>
  {
    return static_cast<std::underlying_type_t<ENUM>>(a)
        >= static_cast<std::underlying_type_t<ENUM>>(b);
  }



  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator == constexpr
  //! Compile-time equality operator for enumerations
  //!
  //! \tparam ENUM - Enumeration type
  //! \tparam VALUE - Type of value to compare against
  //!
  //! \param[in] const &a - Enumeration value
  //! \param[in] const &b - Any value
  //! \return bool - True iff a is greater-than-or-equal than b
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM, typename VALUE> constexpr
  auto  operator == (const ENUM a, const VALUE b) -> enable_if_enum_t<ENUM,bool>
  {
    return static_cast<std::underlying_type_t<ENUM>>(a)
        == static_cast<std::underlying_type_t<ENUM>>(b);
  }

} //namespace wtl
#endif // WTL_ENUMERATION_TRAITS_HPP

