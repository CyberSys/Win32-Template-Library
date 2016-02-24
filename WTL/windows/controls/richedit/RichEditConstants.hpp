//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\richedit\RichEditConstants.hpp
//! \brief Defines richedit control window styles and notifications
//! \date 24 February 2016
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RICHEDIT_CONSTANTS_HPP
#define WTL_RICHEDIT_CONSTANTS_HPP

#include <wtl/WTL.hpp>
#include <wtl/traits/EnumTraits.hpp>                //!< is_attribute, is_contiguous
#include <wtl/traits/MessageTraits.hpp>             //!< message_traits
#include <wtl/utils/Default.hpp>                    //!< default_t
#include <Richedit.h>                               //!< RichEdit

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //! \enum RichEditStyle - Defines standard RichEdit window styles
  enum class RichEditStyle : ulong32_t
  {
    AutoHScroll = ES_AUTOHSCROLL,		//!<  	Automatically scrolls text to the right by 10 characters when the user types a character at the end of the line. When the user presses the ENTER key, the control scrolls all text back to position zero.
    AutoVScroll = ES_AUTOVSCROLL,		//!<  	Automatically scrolls text up one page when the user presses the ENTER key on the last line.
    Centre = ES_CENTER,			        //!<  	Centers text in a single-line or multiline edit control.
    Left = ES_LEFT,			            //!<  	Left aligns text.
    Multiline = ES_MULTILINE,			  //!<  	Designates a multiline edit control. The default is single-line edit control.
    NoHideSel = ES_NOHIDESEL,			  //!<  	Negates the default behavior for an edit control. The default behavior hides the selection when the control loses the input focus and inverts the selection when the control receives the input focus. If you specify ES_NOHIDESEL, the selected text is inverted, even if the control does not have the focus.
    Number = ES_NUMBER,			        //!<  	Allows only digits to be entered into the edit control.
    Password = ES_PASSWORD,			    //!<  	Displays an asterisk (*) for each character typed into the edit control. This style is valid only for single-line edit controls.
    Readonly = ES_READONLY,			    //!<  	Prevents the user from typing or editing text in the edit control.
    Right = ES_RIGHT,			          //!<  	Right aligns text in a single-line or multiline edit control.
    WantReturn = ES_WANTRETURN,			//!<  	Specifies that a carriage return be inserted when the user presses the ENTER key while entering text into a multiline edit control in a dialog box. If you do not specify this style, pressing the ENTER key has the same effect as pressing the dialog box's default push button. This style has no effect on a single-line edit control.
#if 0
    DisableNoScroll = ES_DISABLENOSCROLL,			//!<	Disables scroll bars instead of hiding them when they are not needed.
    NoCallOleInit = ES_EX_NOCALLOLEINIT,			//!<	Prevents the control from calling the OleInitialize function when created. This window style is useful only in dialog templates because CreateWindowEx does not accept this style.
    NoIme = ES_NOIME,			                    //!<	Disables the IME operation. This style is available for Asian language support only.
    NoOleDragDrop = ES_NOOLEDRAGDROP,			    //!<	Disables support for drag-drop of OLE objects.
    SaveSel = ES_SAVESEL,			                //!<	Preserves the selection when the control loses the focus. By default, the entire contents of the control are selected when it regains the focus.
    SelectionBar = ES_SELECTIONBAR,			      //!<	Adds space to the left margin where the cursor changes to a right-up arrow, allowing the user to select full lines of text.
    SelfIme = ES_SELFIME,			                //!<	Directs the rich edit control to allow the application to handle all IME operations. This style is available for Asian language support only.
    Sunken = ES_SUNKEN,			                  //!<	Displays the control with a sunken border style so that the rich edit control appears recessed into its parent window.
    Vertical = ES_VERTICAL,			              //!<	Draws text and objects in a vertical direction. This style is available for Asian-language support only.
#endif
  };
  
  //! Define traits: Non-contiguous attribute
  template <> struct is_attribute<RichEditStyle>  : std::true_type   {};
  template <> struct is_contiguous<RichEditStyle> : std::false_type  {};
  template <> struct default_t<RichEditStyle>     : std::integral_constant<RichEditStyle,RichEditStyle::Left>   {};

  // --------------------------------------------------------------------------------------------------------------
  
  //! \enum CharFormatEffect - Defines RichEdit character format effects
  enum class CharFormatEffect : uint32_t
  {
    AutoColour = CFE_AUTOCOLOR,       //!<	The text color is the return value of GetSysColor(COLOR_WINDOWTEXT).
    Bold = CFE_BOLD,                  //!<	Characters are bold.
    Disabled = CFE_DISABLED,          //!<	RichEdit 2.0 and later: Characters are displayed with a shadow that is offset by 3/4 point or one pixel, whichever is larger.
    Italic = CFE_ITALIC,              //!<	Characters are italic.
    Strikeout = CFE_STRIKEOUT,        //!<	Characters are struck.
    Underline = CFE_UNDERLINE,        //!<	Characters are underlined.
    Protected = CFE_PROTECTED,        //!<	Characters are protected; an attempt to modify them will cause an EN_PROTECTED notification code.
  };

  //! Define traits: Non-contiguous attribute
  template <> struct is_attribute<CharFormatEffect>  : std::true_type  {};
  template <> struct is_contiguous<CharFormatEffect> : std::false_type  {};
  template <> struct default_t<CharFormatEffect>     : std::integral_constant<CharFormatEffect,CharFormatEffect::AutoColour>   {};
  
  //! \enum CharFormatMask - Defines RichEdit character format masks
  enum class CharFormatMask : uint32_t
  {
    All = CFM_ALL,              //!< Windows 8: A combination of the following values: CFM_EFFECTS | CFM_SIZE | CFM_FACE | CFM_OFFSET | CFM_CHARSET
    Bold = CFM_BOLD,            //!< The CFE_BOLD value of the dwEffects member is valid.
    Charset = CFM_CHARSET,      //!< The bCharSet member is valid.
    Colour = CFM_COLOR,         //!< The crTextColor member and the CFE_AUTOCOLOR value of the dwEffects member are valid.
    Effects = CFM_EFFECTS,      //!< Windows 8: A combination of the following values: CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_COLOR | CFM_STRIKEOUT | CFE_PROTECTED | CFM_LINK
    Face = CFM_FACE,            //!< The szFaceName member is valid.
    Italic = CFM_ITALIC,        //!< The CFE_ITALIC value of the dwEffects member is valid.
    Offset = CFM_OFFSET,        //!< The yOffset member is valid.
    Protected = CFM_PROTECTED,  //!< The CFE_PROTECTED value of the dwEffects member is valid.
    Size = CFM_SIZE,            //!< The yHeight member is valid.
    Strikeout = CFM_STRIKEOUT,  //!< The CFE_STRIKEOUT value of the dwEffects member is valid.
    Underline = CFM_UNDERLINE,  //!< The CFE_UNDERLINE value of the dwEffects member is valid.
  };

  //! Define traits: Non-contiguous attribute
  template <> struct is_attribute<CharFormatMask>  : std::true_type  {};
  template <> struct is_contiguous<CharFormatMask> : std::false_type  {};
  template <> struct default_t<CharFormatMask>     : std::integral_constant<CharFormatMask,CharFormatMask::All>   {};
  
  // --------------------------------------------------------------------------------------------------------------
  
  //! \enum RichEditMessage - Defines standard RichEdit messages
  enum class RichEditMessage : uint16_t
  {
    GetSelText         = EM_GETSELTEXT,			            //!< [Windows 4.00] 
    GetTextEx          = EM_GETTEXTEX, 			            //!< [Windows 4.00] 
    GetTextLengthEx    = EM_GETTEXTLENGTHEX,			      //!< [Windows 4.00] 
		SetBackColour	     = EM_SETBKGNDCOLOR,			        //!< [Windows 4.00] 
    SetCharFormat      = EM_SETCHARFORMAT,			        //!< [Windows 4.00] 
    SetTextEx          = EM_SETTEXTEX,			            //!< [Windows 4.00] 

#if _WIN32_WINNT >= _WIN32_WINNT_NT4
    
#endif
#if _WIN32_WINNT >= _WIN32_WINNT_WIN2K
    
#endif
#if _WIN32_WINNT >= _WIN32_WINNT_WINXP
    
#endif
#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
    
#endif
  };

  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<RichEditMessage>  : std::false_type  {};
  template <> struct is_contiguous<RichEditMessage> : std::false_type  {};
  template <> struct default_t<RichEditMessage>     : std::integral_constant<RichEditMessage,RichEditMessage::SetBackColour>   {};
  
  // --------------------------------------------------------------------------------------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct message_traits<RichEditMessage> - Provides traits for RichEdit messages
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct message_traits<RichEditMessage>
  {
    /////////////////////////////////////////////////////////////////////////////////////////
    // wtl::routing constexpr
    //! Determine whether a message was handled from its result
    //!
    //! \param[in] message - Window message 
    //! \param[in] res - Message result 
    //! \return bool - True iff result indicates message was unhandled
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename RESULT> constexpr
    static MsgRoute routing(RichEditMessage msg, RESULT res) 
    {
      return res != 0 ? MsgRoute::Handled : MsgRoute::Unhandled;
    }
  };

  // --------------------------------------------------------------------------------------------------------------
  
  //! \enum RichEditNotification - Defines standard RichEdit notifications
  enum class RichEditNotification : uint16_t
  {
    SetFocus = 0x0100,			        //!< [Explorer 3.00]
    KillFocus = 0x0200,			        //!< [Explorer 3.00]
    Change = 0x0300,			          //!< [Explorer 3.00]
    Update = 0x0400,			          //!< [Explorer 3.00]
    ErrSpace = 0x0500,			        //!< [Explorer 3.00]
    MaxText = 0x0501,			          //!< [Explorer 3.00]
    HScroll = 0x0601,			          //!< [Explorer 3.00]
    VScroll = 0x0602,			          //!< [Explorer 3.00]

#if _WIN32_WINNT >= _WIN32_WINNT_WINXP
    AlignLeftToRight = 0x0700,			//!< [Explorer 5.01] 
    AlignRightToLeft = 0x0701,			//!< [Explorer 5.01] 
#endif
  };

  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<RichEditNotification>  : std::false_type  {};
  template <> struct is_contiguous<RichEditNotification> : std::false_type  {};
  template <> struct default_t<RichEditNotification>     : std::integral_constant<RichEditNotification,RichEditNotification::SetFocus>   {};
  
  // --------------------------------------------------------------------------------------------------------------

  //! \enum RichEditVersion - Defines RichEdit library versions
  enum class RichEditVersion : uint16_t
  {
    v1_0,		//!< [Windows ?.00] Version 1.0
    v2_0,		//!< [Windows ?.00] Version 2.0
    v3_0,		//!< [Windows ?.00] Version 3.0
    v4_0,		//!< [Windows ?.00] Version 4.0
  };

  //! Define traits: Non-Contiguous enumeration
  template <> struct is_attribute<RichEditVersion>  : std::false_type  {};
  template <> struct is_contiguous<RichEditVersion> : std::false_type  {};
  template <> struct default_t<RichEditVersion>     : std::integral_constant<RichEditVersion,RichEditVersion::v1_0>   {};
  
  

}

#endif  // WTL_RICHEDIT_CONSTANTS_HPP
