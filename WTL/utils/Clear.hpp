//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Clear.hpp
//! \brief Provides a generalised method of clearing objects
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CLEAR_HPP
#define WTL_CLEAR_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/SFINAE.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct has_clear_method - Defines an SFINAE constant reflecting whether type has clear() method
  //! 
  //! \tparam T - Type to query
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename MFN = decltype(&T::clear)>
  struct has_clear_method : std::is_member_function_pointer<MFN>
  {};

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct clear_t - Defines the default value for any type
  //! 
  //! \tparam T - Any type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename = void> 
  struct clear_t; 
  /* Undefined */

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct clear_t<method> - Invoke a 'clear()' mutator method if available
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  struct clear_t<T,std::enable_if_t<has_clear_method<T>::value>>
  {
    static void exec(T& obj)
    {
      obj.clear();
    }
    
    template <unsigned LEN>
    static void exec(T (&arr)[LEN])
    {
      for (uint32 i = 0; i < LEN; ++i)
        arr[i].clear();
    }
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct clear_t<standard> - Clear objects of standard layout using 'memset()'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  struct clear_t<T, enable_if_standard_t<T>>
  {
    static void exec(T& obj)
    {
      memset(&obj, 0x00, sizeof(T));
    }

    template <unsigned LEN>
    static void exec(T (&arr)[LEN])
    {
      memset(&arr, 0x00, sizeof(T)*LEN);
    }
  };

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::clear
  //! Clears any object 
  //! 
  //! \tparam T - Object type
  //!
  //! \param[in,out] &obj - Object reference
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
	void  clear(T& obj) 
	{	
    // Clear object
	  clear_t<T>::exec(obj);
	}
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::clear
  //! Clears any object 
  //! 
  //! \tparam T - Object type
  //!
  //! \param[in,out] *obj - Object pointer
  //! 
  //! \throw wtl::invalid_argument - Missing object
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
	void  clear(T* obj) 
	{	
    REQUIRED_PARAM(obj);
    
    // Clear object
    clear_t<T>::exec(*obj);
	}
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::clear
  //! Clears any array of objects
  //! 
  //! \tparam T - Object type
  //! \tparam LEN - Number of objects
  //!
  //! \param[in,out] &arr - Array of objects
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, unsigned LEN> 
	void  clear(T (&arr)[LEN]) 
	{	
    // Clear array
	  clear_t<T>::exec(arr);
	}
  

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::clear
  //! Clears any object 
  //! 
  //! \tparam T - Object type
  //!
  //! \param[in,out] &obj - Object 
  //////////////////////////////////////////////////////////////////////////////////////////
  /*template <typename T, unsigned LEN> 
	void  clear(T (&arr)[LEN]) 
	{	
	  clear_t<T>::exec(arr);
	}*/




  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::clear
  //! Clears any object of class type 
  //! 
  //! \tparam T - Object type
  //!
  //! \param[in,out] &obj - Object containing a .clear() method
  //////////////////////////////////////////////////////////////////////////////////////////
  /*template <typename T> 
	enable_if_not_pod_t<T>  clear(T& obj) 
	{	
	  obj.clear();
	}*/

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::clear
  //! Clears any object of POD type using memset 
  //! 
  //! \tparam T - Object type
  //!
  //! \param[in,out] &obj - Object of POD type
  //////////////////////////////////////////////////////////////////////////////////////////
  /*template <typename T> 
	enable_if_pod_t<T>  clear(T& obj) 
	{	
	  memset(&obj, 0x00, sizeof(T));
	}*/
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::clear
  //! Clears any array of POD type using memset 
  //! 
  //! \tparam T - Object type
  //! \tparam LEN - Array capacity
  //!
  //! \param[in,out] &arr - Array of objects of POD type
  //////////////////////////////////////////////////////////////////////////////////////////
  /*template <typename T, unsigned LEN> 
	enable_if_pod_t<T>  clear(T (&arr)[LEN]) 
	{	
	  memset(&arr, 0x00, sizeof(T)*LEN);
	}*/

}



#endif // WTL_CLEAR_HPP


