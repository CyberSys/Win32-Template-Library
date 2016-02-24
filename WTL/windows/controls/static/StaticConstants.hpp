//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\static\StaticConstants.hpp
//! \brief Defines static control window styles and notifications
//! \date 24 February 2016
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_STATIC_CONSTANTS_HPP
#define WTL_STATIC_CONSTANTS_HPP

#include <wtl/WTL.hpp>
#include <wtl/traits/EnumTraits.hpp>                //!< is_attribute, is_contiguous
#include <wtl/traits/MessageTraits.hpp>             //!< message_traits
#include <wtl/utils/Default.hpp>                    //!< default_t

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //! \enum StaticNotification - Defines standard Static notification
  enum class StaticNotification : int16_t
  {
    Clicked  = 0x00000000L,			//!< [Windows 3.11] (MuEx)
    DblClick = 0x00000001L,			//!< [Windows 3.11] (MuEx)
    Enable   = 0x00000002L,			//!< [Windows 3.11] (MuEx)
    Disable  = 0x00000003L,			//!< [Windows 3.11] (MuEx)
  };
  
  //! Define traits: Non-contiguous attribute
  template <> struct is_attribute<StaticNotification>  : std::true_type   {};
  template <> struct is_contiguous<StaticNotification> : std::false_type  {};
  template <> struct default_t<StaticNotification>     : std::integral_constant<StaticNotification,StaticNotification::Clicked>   {};

  // --------------------------------------------------------------------------------------------------------------

  //! \enum StaticStyle - Defines standard Static window styles
  enum class StaticStyle : ulong32_t
  {
    Left            = 0x00000000L,			//!< [Windows 3.11] (MuEx)
    Center          = 0x00000001L,			//!< [Windows 3.11] (MuEx)
    Right           = 0x00000002L,			//!< [Windows 3.11] (MuEx)
    Icon            = 0x00000003L,			//!< [Windows 3.11] (MuEx)
    BlackRect       = 0x00000004L,			//!< [Windows 3.11] (MuEx)
    GrayRect        = 0x00000005L,			//!< [Windows 3.11] (MuEx)
    WhiteRect       = 0x00000006L,			//!< [Windows 3.11] (MuEx)
    BlackFrame      = 0x00000007L,			//!< [Windows 3.11] (MuEx)
    GrayFrame       = 0x00000008L,			//!< [Windows 3.11] (MuEx)
    WhiteFrame      = 0x00000009L,			//!< [Windows 3.11] (MuEx)
    UserItem        = 0x0000000AL,			//!< [Windows 3.11] (MuEx)
    Simple          = 0x0000000BL,			//!< [Windows 3.11] (MuEx)
    LeftNoWordWrap  = 0x0000000CL,			//!< [Windows 3.11] (MuEx)
    OwnerDraw       = 0x0000000DL,			//!< [Explorer 3.00] (MuEx)
    Bitmap          = 0x0000000EL,			//!< [Explorer 3.00] (MuEx)
    EnhMetaFile     = 0x0000000FL,			//!< [Explorer 3.00] (MuEx)
    EtchedHorz      = 0x00000010L,			//!< [Explorer 3.00] 
    EtchedVert      = 0x00000011L,			//!< [Explorer 3.00] 
    EtchedFrame     = 0x00000012L,			//!< [Explorer 3.00] 
    TypeMask        = 0x0000001FL,			//!< [Explorer 3.00] 
    RealSizeControl = 0x00000040L,			//!< [Windows XP] 
    NoPrefix        = 0x00000080L,			//!< [Explorer 3.00] Don't do '&' character translation 
    Notify          = 0x00000100L,			//!< [Explorer 3.00] 
    CenterImage     = 0x00000200L,			//!< [Explorer 3.00] 
    RightJust       = 0x00000400L,			//!< [Explorer 3.00] 
    RealSizeImage   = 0x00000800L,			//!< [Explorer 3.00] 
    Sunken          = 0x00001000L,			//!< [Explorer 3.00] 
    EditControl     = 0x00002000L,			//!< [Explorer 3.00] 
    EndEllipsis     = 0x00004000L,			//!< [Explorer 3.00] 
    PathEllipsis    = 0x00008000L,			//!< [Explorer 3.00] 
    WordEllipsis    = 0x0000C000L,			//!< [Explorer 3.00] 
    EllipsisMask    = WordEllipsis,			//!< [Explorer 3.00] 
  };
  
  //! Define traits: Non-contiguous attribute
  template <> struct is_attribute<StaticStyle>  : std::true_type   {};
  template <> struct is_contiguous<StaticStyle> : std::false_type  {};
  template <> struct default_t<StaticStyle>     : std::integral_constant<StaticStyle,StaticStyle::Left>   {};


}

#endif  // WTL_STATIC_CONSTANTS_HPP
