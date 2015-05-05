////////////////////////////////////////////////////////////////////////////////
//! \file wtl\Utils\Policies.hpp
//! \brief Provides allocation & initialization policies 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_POLICIES_HPP
#define WTL_POLICIES_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
 
  /////////////////////////////////////////////////////////////
  //! \struct static_alloc - Static allocator
  //! Provides creation, destruction, and reconstruction for any type on the stack
  //!
  //! \tparam T - Any type
  /////////////////////////////////////////////////////////////
  template <typename T, typename = void>
  struct static_alloc; 
 
  /////////////////////////////////////////////////////////////
  //! \struct static_alloc - Static allocator for fundamental types
  //!
  //! \tparam T - Any type
  /////////////////////////////////////////////////////////////
  template <typename T>
  struct static_alloc<T, typename std::enable_if_t<!std::is_class<T>::value>>
                                                /* std::is_integral<T>::value 
                                                || std::is_floating_point<T>::value>> */
  {
    /////////////////////////////////////////////////////////////
    // static_alloc::construct
    //! Default-construct value to zero
    //! 
    //! \param[in,out] *addr - Value address
    /////////////////////////////////////////////////////////////
    static void construct(T *addr)
		{	
		  *addr = zero<T>::value;
		}

    /////////////////////////////////////////////////////////////
    // static_alloc::construct
    //! Copy-construct value
    //! 
    //! \param[in,out] *addr - Value address
    //! \param[in] &obj - Source value
    /////////////////////////////////////////////////////////////
	  static void construct(T *addr, const T& obj)
		{	
      *addr = obj;
		}

    /////////////////////////////////////////////////////////////
    // static_alloc::construct
    //! Copy-construct value
    //! 
    //! \param[in,out] *addr - Value address
    //! \param[in] &&obj - Arguments
    /////////////////////////////////////////////////////////////
	  template <typename U, typename... ARGS>
		static void construct(U *addr, ARGS&&... args)
		{	
      *addr = U(args...);
		}

    /////////////////////////////////////////////////////////////
    // static_alloc::destroy
    //! Does nothing
    //! 
    //! \tparam U - Value type
    //! \param[in,out] *addr - Value address
    /////////////////////////////////////////////////////////////
	  template <typename U>
		static void destroy(U *addr)
		{	
//! \if DEVELOPMENT_MODE - Reset value to zero
#ifdef DEVELOPMENT_MODE
      *addr = zero<U>::value;
#endif
		}
    
    /////////////////////////////////////////////////////////////
    // static_alloc::reconstruct
    //! Overwrite value
    //! 
    //! \tparam U - Value type
    //! \tparam ARGS... - New value type
    //!
    //! \param[in,out] *addr - Value address
    //! \param[in,out] && args - New value
    /////////////////////////////////////////////////////////////
    template <typename U, typename... ARGS>
		static void reconstruct(U *addr, ARGS&&... args)
		{	
      *addr = U(args...);
		}
  }; 

  /////////////////////////////////////////////////////////////
  //! \struct static_alloc - Static allocator for aggregate types
  //! 
  //! \tparam T - Object type
  /////////////////////////////////////////////////////////////
  template <typename T>
  struct static_alloc<T, typename std::enable_if_t<std::is_class<T>::value>> 
  {
    /////////////////////////////////////////////////////////////
    // static_alloc::construct
    //! Default-construct object
    //! 
    //! \param[in,out] *addr - Object address
    /////////////////////////////////////////////////////////////
    static void construct(T *addr)
		{	
		  ::new (static_cast<void*>(addr)) T();
		}

    /////////////////////////////////////////////////////////////
    // static_alloc::construct
    //! Copy-construct object
    //! 
    //! \param[in,out] *addr - Object address
    //! \param[in] const &obj - Source object
    /////////////////////////////////////////////////////////////
	  static void construct(T *addr, const T& obj)
		{	
		  ::new (static_cast<void*>(addr)) T(obj);
		}

    /////////////////////////////////////////////////////////////
    // static_alloc::construct
    //! Variadic-construct object
    //! 
    //! \tparam U - Object type
    //! \tparam ARGS... - [optional] Object c-tor arguments
    //! 
    //! \param[in,out] *addr - Object address
    //! \param[in,out] &&...args - Constructor arguments
    /////////////////////////////////////////////////////////////
	  template <typename U, typename... ARGS>
		static void construct(U *addr, ARGS&&... args)
		{	
		  ::new (static_cast<void*>(addr)) U(std::forward<ARGS>(args)...);
		}

    /////////////////////////////////////////////////////////////
    // static_alloc::destroy
    //! Destruct object
    //! 
    //! \tparam U - Object type
    //!
    //! \param[in,out] *addr - Object address
    /////////////////////////////////////////////////////////////
	  template <typename U>
		static void destroy(U *addr)
		{	
		  addr->~U();
		}

    /////////////////////////////////////////////////////////////
    // static_alloc::reconstruct
    //! Reconstruct object in-place
    //! 
    //! \tparam U - Object type
    //! \tparam ARGS... - [optional] Object c-tor arguments
    //!
    //! \param[in,out] *addr - Object address
    /////////////////////////////////////////////////////////////
    template <typename U, typename... ARGS>
		static void reconstruct(U *addr, ARGS&&... args)
		{	
      destroy(addr);
      construct(addr, std::forward<ARGS>(args)...);
		}
  };


  ////////////////////////////////////////////////////////////////////////////////
  // wtl::reconstruct
  //! Reconstructs an object in place
  //! 
  //! \tparam T - Object type
  //! \tparam ARGS - Constructor argument types
  //!
  //! \param[in,out] &obj - Object 
  //! \param[in] &&... args - [optional] Object constructor arguments
  ////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename... ARGS>
  void reconstruct(T& obj, ARGS&&... args)
  {
    // Reconstruct object
    static_alloc<T>::reconstruct(&obj, std::forward<ARGS>(args)...);
  }
  

} //namespace wtl
#endif // WTL_POLICIES_HPP
