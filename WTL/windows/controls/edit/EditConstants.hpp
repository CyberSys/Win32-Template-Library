//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\edit\EditConstants.hpp
//! \brief Defines edit control window styles and notifications
//! \date 24 February 2016
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EDIT_CONSTANTS_HPP
#define WTL_EDIT_CONSTANTS_HPP

#include <wtl/WTL.hpp>
#include <wtl/traits/EnumTraits.hpp>                //!< is_attribute, is_contiguous
#include <wtl/traits/MessageTraits.hpp>             //!< message_traits
#include <wtl/utils/Default.hpp>                    //!< default_t

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //! \enum EditStyle - Defines standard Edit window styles
  enum class EditStyle : ulong32_t
  {
    Left = 0x0000l,			      //!<  
    Center = 0x0001l,			    //!< 
    Right = 0x0002l,			    //!< 
    Multiline = 0x0004l,			//!< 
    Uppercase = 0x0008l,			//!< 
    Lowercase = 0x0010l,			//!< 
    Password = 0x0020l,			  //!< 
    AutoVScroll = 0x0040l,		//!< 
    AutoHScroll = 0x0080l,		//!< 
    NoHideSel = 0x0100l,			//!< 
    OemConvert = 0x0400l,			//!< 
    ReadOnly = 0x0800l,			  //!< 
    WantReturn = 0x1000l,			//!< 
    Number = 0x2000l,			    //!< 
  };
  
  //! Define traits: Non-contiguous attribute
  template <> struct is_attribute<EditStyle>  : std::true_type   {};
  template <> struct is_contiguous<EditStyle> : std::false_type  {};
  template <> struct default_t<EditStyle>     : std::integral_constant<EditStyle,EditStyle::Left>   {};

  // --------------------------------------------------------------------------------------------------------------
  
  //! \enum EditNotification - Defines standard Edit notifications
  enum class EditNotification : uint16_t
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
  template <> struct is_attribute<EditNotification>  : std::false_type  {};
  template <> struct is_contiguous<EditNotification> : std::false_type  {};
  template <> struct default_t<EditNotification>     : std::integral_constant<EditNotification,EditNotification::SetFocus>   {};
  
  // --------------------------------------------------------------------------------------------------------------
  
  //! \enum EditMessage - Defines standard Edit messages
  enum class EditMessage : uint16_t
  {
		CanUndo		          = EM_CANUNDO,			          //!< [Windows 3.11] 
		EmptyUndoBuffer		  = EM_EMPTYUNDOBUFFER,	      //!< [Windows 3.11] 
		FormatLines         = EM_FMTLINES,			        //!< [Windows 3.11] 
		GetFirstVisibleLine	= EM_GETFIRSTVISIBLELINE,		//!< [Windows 3.11] 
		GetHandle		        = EM_GETHANDLE,			        //!< [Windows 3.11] 
		GetLine		          = EM_GETLINE,			          //!< [Windows 3.11] 
		GetLineCount		    = EM_GETLINECOUNT,		      //!< [Windows 3.11] 
		GetModify		        = EM_GETMODIFY,			        //!< [Windows 3.11] 
		GetPasswordChar		  = EM_GETPASSWORDCHAR,	      //!< [Windows 3.11] 
		GetRect		          = EM_GETRECT,			          //!< [Windows 3.11] 
		GetSel		          = EM_GETSEL,			          //!< [Windows 3.11] 
		GetThumb		        = EM_GETTHUMB,			        //!< [Windows 3.11] 
		GetWordBreakProc	  = EM_GETWORDBREAKPROC,      //!< [Windows 3.11] 
		LimitText		        = EM_LIMITTEXT,			        //!< [Windows 3.11] 
		LineFromChar		    = EM_LINEFROMCHAR,		      //!< [Windows 3.11] 
		LineIndex		        = EM_LINEINDEX,			        //!< [Windows 3.11] 
		LineLength		      = EM_LINELENGTH,			      //!< [Windows 3.11] 
		LineScroll		      = EM_LINESCROLL,			      //!< [Windows 3.11] 
		ReplaceSel		      = EM_REPLACESEL,			      //!< [Windows 3.11] 
		Scroll		          = EM_SCROLL,			          //!< [Windows 3.11] 
		ScrollCaret		      = EM_SCROLLCARET,			      //!< [Windows 3.11] 
		SetHandle		        = EM_SETHANDLE,			        //!< [Windows 3.11] 
		SetModify		        = EM_SETMODIFY,			        //!< [Windows 3.11] 
		SetPasswordChar		  = EM_SETPASSWORDCHAR,	      //!< [Windows 3.11] 
		SetReadOnly		      = EM_SETREADONLY,			      //!< [Windows 3.11] 
		SetRect		          = EM_SETRECT,			          //!< [Windows 3.11] 
		SetRectNp		        = EM_SETRECTNP,			        //!< [Windows 3.11] 
		SetSel		          = EM_SETSEL,			          //!< [Windows 3.11] 
		SetTabStops		      = EM_SETTABSTOPS,			      //!< [Windows 3.11] 
		SetWordBreakProc	  = EM_SETWORDBREAKPROC,      //!< [Windows 3.11] 
		Undo		            = EM_UNDO,			            //!< [Windows 3.11] 

#if _WIN32_WINNT >= _WIN32_WINNT_NT4
    CharFromPos		      = EM_CHARFROMPOS,			      //!< [Windows 4.00] 
		GetLimitText		    = EM_GETLIMITTEXT,		      //!< [Windows 4.00] 
		GetMargins		      = EM_GETMARGINS,			      //!< [Windows 4.00] 
		PosFromChar		      = EM_POSFROMCHAR,			      //!< [Windows 4.00] 
		SetLimitText		    = EM_SETLIMITTEXT,		      //!< [Windows 4.00] 
		SetMargins		      = EM_SETMARGINS,			      //!< [Windows 4.00] 
#endif
#if _WIN32_WINNT >= _WIN32_WINNT_WIN2K
    GetImeStatus		    = EM_GETIMESTATUS,		      //!< [Windows 5.00] 
    SetImeStatus		    = EM_SETIMESTATUS,		      //!< [Windows 5.00] 
#endif
#if _WIN32_WINNT >= _WIN32_WINNT_WINXP
    GetCueBanner		    = EM_GETCUEBANNER,		      //!< [Windows 5.01] 
    HideBalloonTip		  = EM_HIDEBALLOONTIP,	      //!< [Windows 5.01] 
    SetCueBanner		    = EM_SETCUEBANNER,		      //!< [Windows 5.01] 
    ShowBalloonTip		  = EM_SHOWBALLOONTIP,        //!< [Windows 5.01] 
#endif
#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
    TakeFocus		        = EM_TAKEFOCUS,			        //!< [Windows 6.00] 
    NoSetFocus		      = EM_NOSETFOCUS,		        //!< [Windows 6.00] 
    GetHighlight		    = EM_GETHILITE,			        //!< [Windows 6.00] 
		SetHighlight        = EM_SETHILITE,			        //!< [Windows 6.00] 
#endif
  };

  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<EditMessage>  : std::false_type  {};
  template <> struct is_contiguous<EditMessage> : std::false_type  {};
  template <> struct default_t<EditMessage>     : std::integral_constant<EditMessage,EditMessage::CanUndo>   {};
  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct message_traits<EditMessage> - Provides traits for edit messages
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct message_traits<EditMessage>
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
    static MsgRoute routing(EditMessage msg, RESULT res) 
    {
      // TODO: Routing logic
      return MsgRoute::Handled;
    }
  };

}

#endif  // WTL_EDIT_CONSTANTS_HPP
