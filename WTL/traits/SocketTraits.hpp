//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\SocketTraits.hpp
//! \brief Defines socket handle traits
//! \date 4 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SOCKET_TRAITS_HPP
#define WTL_SOCKET_TRAITS_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/EnumCast.hpp>               //!< enum_cast
#include <wtl/utils/Handle.hpp>                 //!< Handle
#include <wtl/utils/Default.hpp>                //!< defvalue
#include <wtl/platform/SocketFlags.hpp>         //!< AddressFamily, SocketType, SocketProtocol
#include <winsock2.h>                           //!< socket(), closesocket()

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias HSocket - Shared socket handle
  /////////////////////////////////////////////////////////////////////////////////////////
  using HSocket = Handle<::SOCKET>;  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc - Encapsulates allocating socket handles  
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<::SOCKET>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \var npos - Invalid handle sentinel value
    static constexpr ::SOCKET npos = INVALID_SOCKET; 
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(handle_alloc);     //!< Cannot instantiate

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(handle_alloc);     //!< Cannot instantiate
    DISABLE_MOVE(handle_alloc);     //!< Cannot instantiate
    DISABLE_DTOR(handle_alloc);     //!< Cannot instantiate

    // ----------------------------------- STATIC METHODS -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc::create
    //! Create socket handle 
    //! 
    //! \param[in] a - Socket address family
    //! \param[in] t - Socket type
    //! \param[in] p - Socket protocol
    //! \return NativeHandle<::SOCKET> - Created handle
    //! 
    //! \throw wtl::socket_error - Failed to create socket handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static NativeHandle<::SOCKET> create(AddressFamily a, SocketType t, SocketProtocol p) 
    { 
      ::SOCKET s;
      
      // Create socket
      if ((s = ::socket(enum_cast(a), enum_cast(t), enum_cast(p))) != npos)
        return { s, AllocType::Create };
      
      // Error: Failed  
      throw socket_error(HERE, "Unable to create socket");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc::clone
    //! Clone handle
    //! 
    //! \param[in] s - Socket handle
    //! \return NativeHandle<::SOCKET> - Duplicate of handle
    //! 
    //! \throw wtl::socket_error - Failed to clone handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static NativeHandle<::SOCKET> clone(NativeHandle<::SOCKET> s);

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc::destroy noexcept
    //! Release handle without throwing
    //! 
    //! \param[in] s - Socket handle
    //! \return bool - True iff closed successfully
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool destroy(NativeHandle<::SOCKET> s) noexcept
    {
      // Delete without checking if handle is valid
      switch (s.Method)
      {
      case AllocType::Accquire: return true;
      case AllocType::Create:   return closesocket(s.Handle) != SOCKET_ERROR;
      case AllocType::WeakRef:  return true;
      }
      return false;
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

  

} //namespace wtl
#endif // WTL_SOCKET_TRAITS_HPP

