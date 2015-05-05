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

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct HAlloc<::HDC> - Associates window handle, DC handle, and allocation method
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct HAlloc<::HDC>
  {
    /////////////////////////////////////////////////////////////////////////////////////////
    // HAlloc<::HDC>::HAlloc
    //! Create without owner window
    //! 
    //! \param[in] dc - Device context
    //! \param[in] at - Allocation type
    /////////////////////////////////////////////////////////////////////////////////////////
    HAlloc(::HDC dc, AllocType at) : Handle(dc), Method(at), Window(0)
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // HAlloc<::HDC>::HAlloc
    //! Store owner window
    //! 
    //! \param[in] dc - Device context
    //! \param[in] wnd - Owner window
    //! \param[in] at - Allocation type
    /////////////////////////////////////////////////////////////////////////////////////////
    HAlloc(::HDC dc, ::HWND wnd, AllocType at) : Handle(dc), Method(at), Window(wnd)
    {}

    ::HDC      Handle;      //!< Handle
    AllocType  Method;      //!< Allocation method
    ::HWND     Window;      //!< Owner window
  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<::HDC> - Encapsulates device context handle allocation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<::HDC>
  {
    //! \var npos - Invalid handle sentinel value
    static const ::HDC npos; 

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HDC>::create
    //! Accquire client area device context 
    //! 
    //! \param[in] wnd - Window handle
    //! \return HAlloc<::HDC> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static HAlloc<::HDC> create(::HWND wnd) 
    { 
      // Load menu 
      if (::HDC dc = GetDC(wnd))
        return { dc, wnd, AllocType::Accquire };

      // Error: Failed  
      throw platform_error(HERE, "Unable to accquire device context");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HDC>::clone
    //! Clone handle
    //! 
    //! \param[in] dc - Device context handle
    //! \return HAlloc<::HDC> - Duplicate of handle
    //! 
    //! \throw wtl::platform_error - Failed to clone handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static HAlloc<::HDC> clone(HAlloc<::HDC> dc);

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HDC>::destroy noexcept
    //! Release Menu handle
    //! 
    //! \param[in] dc - Device context handle
    //! \return bool - True iff closed successfully
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool destroy(HAlloc<::HDC> dc) noexcept
    {
      // Delete without checking if handle is valid
      switch (dc.Method)
      {
      case AllocType::Accquire: return ::ReleaseDC(dc.Window, dc.Handle) != FALSE;
      case AllocType::Create:   return ::DeleteDC(dc.Handle) != FALSE;
      case AllocType::WeakRef:  return true;
      }
      return false;
    }
  };


  //! \alias HDeviceContext - Shared device context handle
  using HDeviceContext = Handle<::HDC>;

  
} //namespace wtl
#endif // WTL_DEVICE_CONTEXT_TRAITS_HPP

