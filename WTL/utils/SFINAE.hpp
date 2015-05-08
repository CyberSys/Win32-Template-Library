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
  //! \alias enable_if_class_constructible_t - Defines an SFINAE expression requiring an object of class type and a c-tor of specific signature
  //! 
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //! \tparam ARGS - [optional] Constructor argument types  (Default is none)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename RET = void, typename... ARGS>
  using enable_if_class_constructible_t = std::enable_if_t<std::is_class<T>::value && std::is_constructible<T,ARGS...>, RET>;


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
  //! \alias enable_if_standard_t - Defines an SFINAE expression requiring a standard layout type
  //! 
  //! \tparam T - Input type
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename RET = void>
  using enable_if_standard_t = std::enable_if_t<std::is_standard_layout<T>::value, RET>;

}



#endif // WTL_SFINAE_HPP


