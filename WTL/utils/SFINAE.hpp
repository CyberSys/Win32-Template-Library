//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\SFINAE.hpp
//! \brief Collection of SFINAE expressions
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SFINAE_HPP
#define WTL_SFINAE_HPP

#include <wtl/WTL.hpp>
#include <type_traits>                //!< std::enable_if, std::integral_constant

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_always_t - Sentinel SFINAE expression that always succeeds
  //! 
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename RET = void>
  using enable_always_t = std::enable_if_t<true, RET>;

  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_t - Defines an SFINAE expression requiring an expression be true
  //! 
  //! \tparam VALUE - Conditional value
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <bool VALUE, typename RET = void>
  using enable_if_t = std::enable_if_t<VALUE, RET>;
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_not_t - Defines an SFINAE expression requiring an expression be false
  //! 
  //! \tparam VALUE - Conditional value
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <bool VALUE, typename RET = void>
  using enable_if_not_t = std::enable_if_t<!VALUE, RET>;


  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_array_t - Defines an SFINAE expression requiring an object of array type 
  //! 
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename RET = void>
  using enable_if_array_t = std::enable_if_t<std::is_array<T>::value, RET>;


  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_boolean_t - Defines an SFINAE expression requiring an boolean type
  //! 
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename RET = void>
  using enable_if_boolean_t = std::enable_if_t<std::is_same<T,bool>::value, RET>;
  
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_class_t - Defines an SFINAE expression requiring an object of class/struct type 
  //! 
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename RET = void>
  using enable_if_class_t = std::enable_if_t<std::is_class<T>::value, RET>;
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_not_class_t - Defines an SFINAE expression requiring an object of non-class/struct type 
  //! 
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename RET = void>
  using enable_if_not_class_t = std::enable_if_t<!std::is_class<T>::value, RET>;



  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_class_constructible_t - Defines an SFINAE expression requiring an object of class type and a c-tor of specific signature
  //! 
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //! \tparam ARGS - [optional] Constructor argument types  (Default is none)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename RET = void, typename... ARGS>
  using enable_if_class_constructible_t = std::enable_if_t<std::is_class<T>::value && std::is_constructible<T,ARGS...>::value, RET>;
  

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_convertible_t - Defines an SFINAE expression requiring an accessible conversion from A to B
  //! 
  //! \tparam FROM - Input type
  //! \tparam TO - Desired type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename FROM, typename TO, typename RET = void>
  using enable_if_convertible_t = std::enable_if_t<std::is_convertible<FROM,TO>::value, RET>;
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_not_convertible_t - Defines an SFINAE expression requiring no accessible conversion from A to B
  //! 
  //! \tparam FROM - Input type
  //! \tparam TO - Desired type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename FROM, typename TO, typename RET = void>
  using enable_if_not_convertible_t = std::enable_if_t<!std::is_convertible<FROM,TO>::value, RET>;


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct enable_if_enum_t - Defines an SFINAE expression requiring an enumeration
  //! 
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename E, typename RET = void>
  using enable_if_enum_t = std::enable_if_t<std::is_enum<E>::value, RET>;
  
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_equal_t - Defines an SFINAE expression requiring two integers, A and B, be equal
  //! 
  //! \tparam A - Value
  //! \tparam B - Value
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <size_t A, size_t B, typename RET = void>
  using enable_if_equal_t = std::enable_if_t<A == B, RET>;
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_not_equal_t - Defines an SFINAE expression requiring two integers, A and B, be unequal
  //! 
  //! \tparam A - Value
  //! \tparam B - Value
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <size_t A, size_t B, typename RET = void>
  using enable_if_not_equal_t = std::enable_if_t<A != B, RET>;
  

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_floating_t - Defines an SFINAE expression requiring a floating point type
  //! 
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename RET = void>
  using enable_if_floating_t = std::enable_if_t<std::is_floating_point<T>::value, RET>;
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_greater_t - Defines an SFINAE expression requiring an integer A be greater than an integer B
  //! 
  //! \tparam A - Value
  //! \tparam B - Value
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <size_t A, size_t B, typename RET = void>
  using enable_if_greater_t = std::enable_if_t<(A > B), RET>;
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_less_t - Defines an SFINAE expression requiring an integer A be less than an integer B
  //! 
  //! \tparam A - Value
  //! \tparam B - Value
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <size_t A, size_t B, typename RET = void>
  using enable_if_less_t = std::enable_if_t<(A < B), RET>;
  

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_integer_t - Defines an SFINAE expression requiring an integer type
  //! 
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename RET = void>
  using enable_if_integer_t = std::enable_if_t<std::is_integral<T>::value 
                                            && !std::is_same<T,bool>::value, RET>;

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_larger_t - Defines an SFINAE expression requiring type A be larger than a type B
  //! 
  //! \tparam A - Input type
  //! \tparam B - Another type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename A, typename B, typename RET = void>
  using enable_if_larger_t = std::enable_if_t<(sizeof(A) > sizeof(B)), RET>;
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_integral_t - Defines an SFINAE expression requiring an integral type
  //! 
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename RET = void>
  using enable_if_integral_t = std::enable_if_t<std::is_integral<T>::value, RET>;

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct enable_if_numeric_t - Defines an SFINAE expression requiring an integer or enumeration type [Discards boolean]
  //!
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename E, typename RET = void>
  using enable_if_numeric_t = std::enable_if_t<(std::is_enum<E>::value 
                                            || std::is_integral<E>::value)
                                            && !std::is_same<E,bool>::value, RET>;
  
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_same_t - Defines an SFINAE expression requiring a specific type
  //! 
  //! \tparam T - Input type
  //! \tparam U - Required type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename U, typename RET = void>
  using enable_if_same_t = std::enable_if_t<std::is_same<T,U>::value, RET>;
  

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_not_same_t - Defines an SFINAE expression blocking a specific type
  //! 
  //! \tparam T - Input type
  //! \tparam U - Required type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename U, typename RET = void>
  using enable_if_not_same_t = std::enable_if_t<!std::is_same<T,U>::value, RET>;
  

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_not_pod_t - Defines an SFINAE expression requiring an object of class type
  //! 
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename RET = void>
  using enable_if_not_pod_t = std::enable_if_t<!std::is_pod<T>::value, RET>;


  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_pod_t - Defines an SFINAE expression requiring an object of POD type
  //! 
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename RET = void>
  using enable_if_pod_t = std::enable_if_t<std::is_pod<T>::value, RET>;
  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct enable_if_scalar_t - Defines an SFINAE expression requiring an scalar type (integral,float,enum,pointer,mem-pointer,nullptr)
  //! 
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename E, typename RET = void>
  using enable_if_scalar_t = std::enable_if_t<std::is_scalar<E>::value, RET>;
  
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_sizeof_t - Defines an SFINAE expression requiring type A equal the size of type B
  //! 
  //! \tparam A - Input type
  //! \tparam B - Another type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename A, typename B, typename RET = void>
  using enable_if_sizeof_t = std::enable_if_t<sizeof(A) == sizeof(B), RET>;
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_smaller_t - Defines an SFINAE expression requiring type A be smaller than a type B
  //! 
  //! \tparam A - Input type
  //! \tparam B - Another type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename A, typename B, typename RET = void>
  using enable_if_smaller_t = std::enable_if_t<(sizeof(A) < sizeof(B)), RET>;
  

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_standard_t - Defines an SFINAE expression requiring a standard layout type
  //! 
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename RET = void>
  using enable_if_standard_t = std::enable_if_t<std::is_standard_layout<T>::value, RET>;
  

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_pointer_t - Defines an SFINAE expression requiring an pointer type
  //! 
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename RET = void>
  using enable_if_pointer_t = std::enable_if_t<std::is_pointer<T>::value, RET>;

}



#endif // WTL_SFINAE_HPP


