//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Allocator.hpp
//! \brief Provides allocation & initialization policies 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_POLICIES_HPP
#define WTL_POLICIES_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/SFINAE.hpp"             //!< wtl::enable_if_class_t
#include <utility>                          //!< std::forward

//! \namespace wtl - Windows template library
namespace wtl
{
 
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct static_alloc - Static allocator
  //! \remarks Provides creation, destruction, and reconstruction for any type on the stack
  //!
  //! \tparam T - Any type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename = void>
  struct static_alloc; 
 
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct static_alloc - Static allocator for fundamental types
  //!
  //! \tparam T - Any type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct static_alloc<T, enable_if_not_class_t<T>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = static_alloc<T>;
  
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(static_alloc);     //!< Cannot instantiate

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(static_alloc);     //!< Cannot instantiate
    DISABLE_MOVE(static_alloc);     //!< Cannot instantiate
    DISABLE_DTOR(static_alloc);     //!< Cannot instantiate

    // ----------------------------------- STATIC METHODS -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // static_alloc::construct
    //! Default-construct value to zero
    //! 
    //! \param[in,out] *addr - Value address
    /////////////////////////////////////////////////////////////////////////////////////////
    static void construct(T *addr)
		{	
		  *addr = defvalue<T>();
		}

    /////////////////////////////////////////////////////////////////////////////////////////
    // static_alloc::construct
    //! Copy-construct value
    //! 
    //! \param[in,out] *addr - Value address
    //! \param[in] &obj - Source value
    /////////////////////////////////////////////////////////////////////////////////////////
	  static void construct(T *addr, const T& obj)
		{	
      *addr = obj;
		}

    /////////////////////////////////////////////////////////////////////////////////////////
    // static_alloc::construct
    //! Copy-construct value
    //! 
    //! \param[in,out] *addr - Value address
    //! \param[in] &&obj - Arguments
    /////////////////////////////////////////////////////////////////////////////////////////
	  template <typename U, typename... ARGS>
		static void construct(U *addr, ARGS&&... args)
		{	
      *addr = U(args...);
		}

    /////////////////////////////////////////////////////////////////////////////////////////
    // static_alloc::destroy
    //! Does nothing
    //! 
    //! \tparam U - Value type
    //! \param[in,out] *addr - Value address
    /////////////////////////////////////////////////////////////////////////////////////////
	  template <typename U>
		static void destroy(U *addr)
		{	
//! \if DEVELOPMENT_MODE - Reset value to zero
#ifdef DEVELOPMENT_MODE
      *addr = defvalue<U>();
#endif
		}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // static_alloc::reconstruct
    //! Overwrite value
    //! 
    //! \tparam U - Value type
    //! \tparam ARGS... - New value type
    //!
    //! \param[in,out] *addr - Value address
    //! \param[in,out] && args - New value
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U, typename... ARGS>
		static void reconstruct(U *addr, ARGS&&... args)
		{	
      *addr = U(args...);
		}
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  }; 

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct static_alloc - Static allocator for aggregate types
  //! 
  //! \tparam T - Object type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct static_alloc<T, enable_if_class_t<T>> 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = static_alloc<T>;
  
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(static_alloc);     //!< Cannot instantiate

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(static_alloc);     //!< Cannot instantiate
    DISABLE_MOVE(static_alloc);     //!< Cannot instantiate
    DISABLE_DTOR(static_alloc);     //!< Cannot instantiate

    // ----------------------------------- STATIC METHODS -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // static_alloc::construct
    //! Default-construct object
    //! 
    //! \param[in,out] *addr - Object address
    /////////////////////////////////////////////////////////////////////////////////////////
    static void construct(T *addr)
		{	
		  ::new (static_cast<void*>(addr)) T();
		}

    /////////////////////////////////////////////////////////////////////////////////////////
    // static_alloc::construct
    //! Copy-construct object
    //! 
    //! \param[in,out] *addr - Object address
    //! \param[in] const &obj - Source object
    /////////////////////////////////////////////////////////////////////////////////////////
	  static void construct(T *addr, const T& obj)
		{	
		  ::new (static_cast<void*>(addr)) T(obj);
		}

    /////////////////////////////////////////////////////////////////////////////////////////
    // static_alloc::construct
    //! Variadic-construct object
    //! 
    //! \tparam U - Object type
    //! \tparam ARGS... - [optional] Object c-tor arguments
    //! 
    //! \param[in,out] *addr - Object address
    //! \param[in,out] &&...args - Constructor arguments
    /////////////////////////////////////////////////////////////////////////////////////////
	  template <typename U, typename... ARGS>
		static void construct(U *addr, ARGS&&... args)
		{	
		  ::new (static_cast<void*>(addr)) U(std::forward<ARGS>(args)...);
		}

    /////////////////////////////////////////////////////////////////////////////////////////
    // static_alloc::destroy
    //! Destruct object
    //! 
    //! \tparam U - Object type
    //!
    //! \param[in,out] *addr - Object address
    /////////////////////////////////////////////////////////////////////////////////////////
	  template <typename U>
		static void destroy(U *addr)
		{	
		  addr->~U();
		}

    /////////////////////////////////////////////////////////////////////////////////////////
    // static_alloc::reconstruct
    //! Reconstruct object in-place
    //! 
    //! \tparam U - Object type
    //! \tparam ARGS... - [optional] Object c-tor arguments
    //!
    //! \param[in,out] *addr - Object address
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename U, typename... ARGS>
		static void reconstruct(U *addr, ARGS&&... args)
		{	
      destroy(addr);
      construct(addr, std::forward<ARGS>(args)...);
		}
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::reconstruct
  //! Reconstructs an object in place
  //! 
  //! \tparam T - Object type
  //! \tparam ARGS - Constructor argument types
  //!
  //! \param[in,out] &obj - Object 
  //! \param[in] &&... args - [optional] Object constructor arguments
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename... ARGS>
  void reconstruct(T& obj, ARGS&&... args)
  {
    // Reconstruct object
    static_alloc<T>::reconstruct(&obj, std::forward<ARGS>(args)...);
  }
  

} //namespace wtl
#endif // WTL_POLICIES_HPP
