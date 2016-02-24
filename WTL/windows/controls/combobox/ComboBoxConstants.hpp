//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\combobox\ComboBoxConstants.hpp
//! \brief Defines combobox control window styles and notifications
//! \date 24 February 2016
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_COMBOBOX_CONSTANTS_HPP
#define WTL_COMBOBOX_CONSTANTS_HPP

#include <wtl/WTL.hpp>
#include <wtl/traits/EnumTraits.hpp>                //!< is_attribute, is_contiguous
#include <wtl/traits/MessageTraits.hpp>             //!< message_traits
#include <wtl/utils/Default.hpp>                    //!< default_t

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //! \enum ComboBoxNotification - Defines standard ComboBox notification
  enum class ComboBoxNotification : int16_t
  {
    ErrSpace = -0x0001,			  //!< [Explorer 3.00]
    SelChange = 0x0001,			  //!< [Explorer 3.00]
    DblClk = 0x0002,			    //!< [Explorer 3.00]
    SetFocus = 0x0003,			  //!< [Explorer 3.00]
    KillFocus = 0x0004,			  //!< [Explorer 3.00]
    EditChange = 0x0005,			//!< [Explorer 3.00]
    EditUpdate = 0x0006,			//!< [Explorer 3.00]
    DropDown = 0x0007,			  //!< [Explorer 3.00]
    CloseUp = 0x0008,			    //!< [Explorer 3.00]
    SelAndOk = 0x0009,			  //!< [Explorer 3.00]
    SelAndCancel = 0x00010,		//!< [Explorer 3.00]
  };
  
  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<ComboBoxNotification>  : std::false_type  {};
  template <> struct is_contiguous<ComboBoxNotification> : std::false_type  {};
  template <> struct default_t<ComboBoxNotification>     : std::integral_constant<ComboBoxNotification,ComboBoxNotification::ErrSpace>   {};
  
  // --------------------------------------------------------------------------------------------------------------

  //! \enum ComboBoxStyle - Defines standard ComboBox window styles
  enum class ComboBoxStyle : ulong32_t
  {
    Simple = 0x0001L,			          //!< [Explorer 3.00]
    DropDown = 0x0002L,			        //!< [Explorer 3.00]
    DropDownList = 0x0003L,			    //!< [Explorer 3.00]
    OwnerDrawFixed = 0x0010L,			  //!< [Explorer 3.00]
    OwnerDrawVariable = 0x0020L,		//!< [Explorer 3.00]
    AutoHScroll = 0x0040L,			    //!< [Explorer 3.00]
    OemConvert = 0x0080L,			      //!< [Explorer 3.00]
    Sort = 0x0100l,			            //!< [Explorer 3.00]
    HasStrings = 0x0200L,			      //!< [Explorer 3.00]
    NoIntegralHeight = 0x0400L,			//!< [Explorer 3.00]
    DisableNoScroll = 0x0800L,			//!< [Explorer 3.00]
    UpperCase = 0x2000L,			      //!< [Explorer 3.00] 
    LowerCase = 0x4000L,			      //!< [Explorer 3.00] 
  };
  
  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<ComboBoxStyle>  : std::false_type  {};
  template <> struct is_contiguous<ComboBoxStyle> : std::false_type  {};
  template <> struct default_t<ComboBoxStyle>     : std::integral_constant<ComboBoxStyle,ComboBoxStyle::Simple>   {};
  

}

#endif  // WTL_COMBOBOX_CONSTANTS_HPP
