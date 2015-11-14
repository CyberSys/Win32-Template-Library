//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\MouseFlags.hpp
//! \brief Defines flags for mouse related Win32 API functions
//! \date 13 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MOUSE_FLAGS_HPP
#define WTL_MOUSE_FLAGS_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EnumTraits.hpp"                  //!< is_attribute, is_contiguous

//! \namespace wtl - Windows template library
namespace wtl
{
  // --------------------------------------------------------------------------------------------------------------
  // ----------------------------------------------- DEVICE CONTEXT -----------------------------------------------
  // --------------------------------------------------------------------------------------------------------------
  
   //! \enum MouseKeyFlag - Defines keys state masks for mouse messages
  enum class MouseKeyFlag : uint16_t
  {
    LeftButton = MK_LBUTTON,			//!< [Windows 4.00]  
    RightButton = MK_RBUTTON,			//!< [Windows 4.00] 
    Shift = MK_SHIFT,			        //!< [Windows 4.00] 
    Control = MK_CONTROL,			    //!< [Windows 4.00] 
    MiddleButton = MK_MBUTTON,		//!< [Windows 4.00] 
#if(_WIN32_WINNT >= 0x0500)
    XButton1 = MK_XBUTTON1,			  //!< [Windows 5.00] 
    XButton2 = MK_XBUTTON2,			  //!< [Windows 5.00] 
#endif 
  };
  
  //! Define traits: Non-contiguous attribute 
  template <> struct is_attribute<MouseKeyFlag>  : std::true_type   {};
  template <> struct is_contiguous<MouseKeyFlag> : std::false_type  {};
  template <> struct default_t<MouseKeyFlag>     : std::integral_constant<MouseKeyFlag,MouseKeyFlag::LeftButton>   {};

  // --------------------------------------------------------------------------------------------------------------
  
}

#endif  // WTL_MOUSE_FLAGS_HPP
