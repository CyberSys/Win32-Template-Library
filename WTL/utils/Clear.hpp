////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Clear.hpp
//! \brief Provides a generalised method of clearing objects
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CLEAR_HPP
#define WTL_CLEAR_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
 
  ////////////////////////////////////////////////////////////////////////////////
  // wtl::clear
  //! Clears any object of class type 
  //! 
  //! \tparam T - Object type
  //!
  //! \param[in,out] &obj - Object containing a .clear() method
  ////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
	enable_if_not_pod_t<T>  clear(T& obj) 
	{	
	  obj.clear();
	}

  ////////////////////////////////////////////////////////////////////////////////
  // wtl::clear
  //! Clears any object of POD type using memset 
  //! 
  //! \tparam T - Object type
  //!
  //! \param[in,out] &obj - Object of POD type
  ////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
	enable_if_pod_t<T>  clear(T& obj) 
	{	
	  memset(&obj, 0x00, sizeof(T));
	}
  
  ////////////////////////////////////////////////////////////////////////////////
  // wtl::clear
  //! Clears any array of POD type using memset 
  //! 
  //! \tparam T - Object type
  //! \tparam LEN - Array capacity
  //!
  //! \param[in,out] &arr - Array of objects of POD type
  ////////////////////////////////////////////////////////////////////////////////
  template <typename T, unsigned LEN> 
	enable_if_pod_t<T>  clear(T (&arr)[LEN]) 
	{	
	  memset(&arr, 0x00, sizeof(T)*LEN);
	}

}



#endif // WTL_CLEAR_HPP


