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
  //! \struct enable_if_not_pod_t - Defines an SFINAE expression requiring an object of class type
  //! 
  //! \tparam OBJ - Object type
  //! \tparam RET - [optional] Return type  (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename OBJ, typename RET = void>
  using enable_if_not_pod_t = std::enable_if_t<!std::is_pod<OBJ>::value, RET>;

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct enable_if_pod_t - Defines an SFINAE expression requiring an object of POD type
  //! 
  //! \tparam OBJ - Object type
  //! \tparam RET - [optional] Return type  (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename OBJ, typename RET = void>
  using enable_if_pod_t = std::enable_if_t<std::is_pod<OBJ>::value, RET>;

  
}



#endif // WTL_SFINAE_HPP


