//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\DeviceContextTraits.hpp
//! \brief Defines device context traits 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_DEVICE_CONTEXT_TRAITS_HPP
#define WTL_DEVICE_CONTEXT_TRAITS_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/Handle.hpp"
#include "wtl/utils/Default.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct NativeHandle<::HDC> - Associates window handle, DC handle, and allocation method
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct NativeHandle<::HDC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \typedef handle_t - Native handle type
    using handle_t = ::HDC;

    // ----------------------------------- REPRESENTATION -----------------------------------
  
    ::HDC      Handle;      //!< Handle
    AllocType  Method;      //!< Allocation method
    ::HWND     Window;      //!< Owner window
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    /////////////////////////////////////////////////////////////////////////////////////////
    // NativeHandle<::HDC>::NativeHandle
    //! Create without owner window
    //! 
    //! \param[in] dc - Device context
    //! \param[in] at - Allocation type
    /////////////////////////////////////////////////////////////////////////////////////////
    NativeHandle(::HDC dc, AllocType at) : Handle(dc), Method(at), Window(0)
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // NativeHandle<::HDC>::NativeHandle
    //! Store owner window
    //! 
    //! \param[in] dc - Device context
    //! \param[in] wnd - Owner window
    //! \param[in] at - Allocation type
    /////////////////////////////////////////////////////////////////////////////////////////
    NativeHandle(::HDC dc, ::HWND wnd, AllocType at) : Handle(dc), Method(at), Window(wnd)
    {}
  
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
    ENABLE_COPY(NativeHandle);       //!< Can be shallow copied
    ENABLE_MOVE(NativeHandle);       //!< Can be moved
    ENABLE_POLY(NativeHandle);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias HDeviceContext - Shared device context handle
  /////////////////////////////////////////////////////////////////////////////////////////
  using HDeviceContext = Handle<::HDC>;

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<::HDC> - Encapsulates device context handle allocation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<::HDC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \var npos - Invalid handle sentinel value
    static constexpr ::HDC npos = defvalue<::HDC>(); 
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(handle_alloc);     //!< Cannot instantiate

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(handle_alloc);     //!< Cannot instantiate
    DISABLE_MOVE(handle_alloc);     //!< Cannot instantiate
    DISABLE_DTOR(handle_alloc);     //!< Cannot instantiate

    // ----------------------------------- STATIC METHODS -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HDC>::create
    //! Accquire client area device context 
    //! 
    //! \param[in] wnd - Window handle
    //! \return NativeHandle<::HDC> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static NativeHandle<::HDC> create(::HWND wnd) 
    { 
      // Load menu 
      if (::HDC dc = ::GetDC(wnd))
        return { dc, wnd, AllocType::Accquire };

      // Error: Failed  
      throw platform_error(HERE, "Unable to accquire device context");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HDC>::clone
    //! Clone handle
    //! 
    //! \param[in] dc - Device context handle
    //! \return NativeHandle<::HDC> - Duplicate of handle
    //! 
    //! \throw wtl::platform_error - Failed to clone handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static NativeHandle<::HDC> clone(NativeHandle<::HDC> dc);

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HDC>::destroy noexcept
    //! Release Menu handle
    //! 
    //! \param[in] dc - Device context handle
    //! \return bool - True iff closed successfully
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool destroy(NativeHandle<::HDC> dc) noexcept
    {
      // Delete without checking if handle is valid
      switch (dc.Method)
      {
      case AllocType::Accquire: return ::ReleaseDC(dc.Window, dc.Handle) != False;
      case AllocType::Create:   return ::DeleteDC(dc.Handle) != False;
      case AllocType::WeakRef:  return true;
      }
      return false;
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };


} //namespace wtl
#endif // WTL_DEVICE_CONTEXT_TRAITS_HPP

