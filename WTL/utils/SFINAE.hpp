//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\SFINAE.hpp
//! \brief Collection of SFINAE expressions
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SFINAE_HPP
#define WTL_SFINAE_HPP

#include "wtl/WTL.hpp"
#include <type_traits>                //!< std::enable_if, std::integral_constant

//! \namespace wtl - Windows template library
namespace wtl
{
  
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
  using enable_if_class_constructible_t = std::enable_if_t<std::is_class<T>::value && std::is_constructible<T,ARGS...>, RET>;

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct enable_if_enum_t - Defines an SFINAE expression requiring an enumeration
  //! 
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename E, typename RET = void>
  using enable_if_enum_t = std::enable_if_t<std::is_enum<E>::value, RET>;
  

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_floating_t - Defines an SFINAE expression requiring a floating point type
  //! 
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename RET = void>
  using enable_if_floating_t = std::enable_if_t<std::is_floating_point<T>::value, RET>;
  

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
  //! \alias enable_if_integral_t - Defines an SFINAE expression requiring an integral type
  //! 
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename RET = void>
  using enable_if_integral_t = std::enable_if_t<std::is_integral<T>::value, RET>;

  
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_is_t - Defines an SFINAE expression requiring a specific type
  //! 
  //! \tparam T - Input type
  //! \tparam U - Required type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename U, typename RET = void>
  using enable_if_is_t = std::enable_if_t<std::is_same<T,U>::value, RET>;
  

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_is_not_t - Defines an SFINAE expression blocking a specific type
  //! 
  //! \tparam T - Input type
  //! \tparam U - Required type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename U, typename RET = void>
  using enable_if_is_not_t = std::enable_if_t<!std::is_same<T,U>::value, RET>;
  

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
  

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_sizeof_t - Defines an SFINAE expression requiring a type of exact size
  //! 
  //! \tparam T - Input type
  //! \tparam SZ - Type defining the required size
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename SZ, typename RET = void>
  using enable_if_sizeof_t = std::enable_if_t<sizeof(T) == sizeof(SZ), RET>;
  

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_standard_t - Defines an SFINAE expression requiring a standard layout type
  //! 
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename RET = void>
  using enable_if_standard_t = std::enable_if_t<std::is_standard_layout<T>::value, RET>;

}



#endif // WTL_SFINAE_HPP


