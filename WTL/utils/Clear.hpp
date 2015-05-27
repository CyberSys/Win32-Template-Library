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
  struct clear_t;   /* Undefined */
  

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct clear_t<method> - Invoke a 'clear()' mutator method if available
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  struct clear_t<T,std::enable_if_t<has_clear_method<T>::value>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(clear_t);     //!< Cannot instantiate

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(clear_t);     //!< Cannot instantiate
    DISABLE_MOVE(clear_t);     //!< Cannot instantiate
    DISABLE_DTOR(clear_t);     //!< Cannot instantiate

    // ----------------------------------- STATIC METHODS -----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // clear_t::exec
    //! Clear object using provided .clear() method
    //! 
    //! \param[in] obj - Object 
    /////////////////////////////////////////////////////////////////////////////////////////
    static void exec(T& obj)
    {
      obj.clear();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // clear_t::exec
    //! Clear array of objects using provided .clear() method
    //! 
    //! \param[in] &arr - Array of objects
    /////////////////////////////////////////////////////////////////////////////////////////
    template <unsigned LEN>
    static void exec(T (&arr)[LEN])
    {
      for (uint32_t i = 0; i < LEN; ++i)
        arr[i].clear();
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct clear_t<standard> - Clear objects of standard layout using 'memset()'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  struct clear_t<T, enable_if_standard_t<T>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(clear_t);     //!< Cannot instantiate

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(clear_t);     //!< Cannot instantiate
    DISABLE_MOVE(clear_t);     //!< Cannot instantiate
    DISABLE_DTOR(clear_t);     //!< Cannot instantiate

    // ----------------------------------- STATIC METHODS -----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // clear_t::exec
    //! Clear object using memset()
    //! 
    //! \param[in] obj - Object 
    /////////////////////////////////////////////////////////////////////////////////////////
    static void exec(T& obj)
    {
      memset(&obj, 0x00, sizeof(T));
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // clear_t::exec
    //! Clear array of objects using memset()
    //! 
    //! \param[in] &arr - Array of objects
    /////////////////////////////////////////////////////////////////////////////////////////
    template <unsigned LEN>
    static void exec(T (&arr)[LEN])
    {
      memset(&arr, 0x00, sizeof(T)*LEN);
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------

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
  
}



#endif // WTL_CLEAR_HPP


