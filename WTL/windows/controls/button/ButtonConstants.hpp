//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\button\ButtonConstants.hpp
//! \brief Defines button control window styles and notifications
//! \date 24 February 2016
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BUTTON_CONSTANTS_HPP
#define WTL_BUTTON_CONSTANTS_HPP

#include <wtl/WTL.hpp>
#include <wtl/traits/EnumTraits.hpp>                //!< is_attribute, is_contiguous
#include <wtl/traits/MessageTraits.hpp>             //!< message_traits
#include <wtl/utils/Default.hpp>                    //!< default_t

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //! \enum ButtonStyle - Defines standard Button window styles
  enum class ButtonStyle : ulong32_t
  {
    PushButton      = 0x00000000L, 			  //!< 
    DefPushButton   = 0x00000001L, 		    //!< 
    CheckBox        = 0x00000002L, 			  //!< 
    AutoCheckBox    = 0x00000003L, 		 	  //!< 
    RadioButton     = 0x00000004L, 			  //!< 
    TriState        = 0x00000005L, 			  //!< 
    AutoTriState    = 0x00000006L, 			  //!< 
    GroupBox        = 0x00000007L, 			  //!< 
    UserButton      = 0x00000008L, 			  //!< 
    AutoRadioButton = 0x00000009L, 		    //!< 
    PushBox         = 0x0000000AL, 			  //!< 
    OwnerDraw       = 0x0000000BL, 			  //!< 
    TypeMask        = 0x0000000FL, 			  //!< 

    LeftText        = 0x00000020L, 			  //!< 
    RightButton     = LeftText,           //!< 
    Text            = 0x00000000L, 			  //!< 
    Icon            = 0x00000040L, 			  //!< 
    Bitmap          = 0x00000080L, 			  //!< 
    Left            = 0x00000100L, 			  //!< 
    Right           = 0x00000200L, 			  //!< 
    Centre          = 0x00000300L, 			  //!< 
    Top             = 0x00000400L, 			  //!<  
    Bottom          = 0x00000800L, 			  //!< 
    VCenter         = Top|Bottom, 			  //!< 
    PushLike        = 0x00001000L, 			  //!< 
    MultiLine       = 0x00002000L, 			  //!< 
    Notify          = 0x00004000L, 			  //!< 
    Flat            = 0x00008000L, 			  //!< 
  };
  
  //! Define traits: Non-contiguous Attribute
  template <> struct is_attribute<ButtonStyle>  : std::true_type  {};
  template <> struct is_contiguous<ButtonStyle> : std::false_type {};
  template <> struct default_t<ButtonStyle>     : std::integral_constant<ButtonStyle,ButtonStyle::PushButton>   {};

  // --------------------------------------------------------------------------------------------------------------
  
  //! \enum ButtonNotification - Defines standard Button notifications
  enum class ButtonNotification : ulong32_t
  {
    Click         = BN_CLICKED,			          //!< [Windows 3.11] 
    Paint         = BN_PAINT,			            //!< [Windows 3.11] 
    Pushed        = BN_PUSHED,			          //!< [Windows 3.11] 
    Unpushed      = BN_UNPUSHED,			        //!< [Windows 3.11] 
    Disabled      = BN_DISABLE,			          //!< [Windows 3.11] 
    DoubleClick   = BN_DOUBLECLICKED,			    //!< [Windows 3.11] 
    SetFocus      = BN_SETFOCUS,			        //!< [Explorer 3.00] 
    KillFocus     = BN_KILLFOCUS,			        //!< [Explorer 3.00] 
    Highlight     = Pushed,			              //!< [Explorer 3.00] 
    UnHighlight   = Unpushed,			            //!< [Explorer 3.00] 

#if _WIN32_WINNT >= _WIN32_WINNT_WINXP
    HotItemChange = BCN_HOTITEMCHANGE,        //!< [Windows 5.01]
#endif
#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
    DropDown      = BCN_DROPDOWN,             //!< [Windows 6.00]
#endif
  };

  //! Define traits: Contiguous enumeration
  template <> struct is_attribute<ButtonNotification>  : std::false_type  {};
  template <> struct is_contiguous<ButtonNotification> : std::true_type   {};
  template <> struct default_t<ButtonNotification>     : std::integral_constant<ButtonNotification,ButtonNotification::Click>   {};
  
  // --------------------------------------------------------------------------------------------------------------
  
  //! \enum ButtonMessage - Defines standard Button messages
  enum class ButtonMessage : ulong32_t
  {
    GetCheck		 = BM_GETCHECK,			    //!< [Windows 3.11] 
    SetCheck		 = BM_SETCHECK,			    //!< [Windows 3.11] 
    GetState		 = BM_GETSTATE,			    //!< [Windows 3.11] 
    SetState		 = BM_SETSTATE,			    //!< [Windows 3.11] 
    SetStyle		 = BM_SETSTYLE,			    //!< [Windows 3.11] 

#if _WIN32_WINNT >= _WIN32_WINNT_NT4
    Click		     = BM_CLICK,			      //!< [Windows 4.00] 
    GetImage		 = BM_GETIMAGE,			    //!< [Windows 4.00] 
    SetImage		 = BM_SETIMAGE,			    //!< [Windows 4.00] 
#endif
#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
    SetDontClick = BM_SETDONTCLICK,			//!< [Windows 6.00] 
#endif
  };

  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<ButtonMessage>  : std::false_type  {};
  template <> struct is_contiguous<ButtonMessage> : std::false_type  {};
  template <> struct default_t<ButtonMessage>     : std::integral_constant<ButtonMessage,ButtonMessage::GetCheck>   {};
  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct message_traits<ButtonMessage> - Provides traits for button messages
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct message_traits<ButtonMessage>
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
    static MsgRoute routing(ButtonMessage msg, RESULT res) 
    {
      // TODO: Routing logic
      return MsgRoute::Handled;
    }
  };

  // --------------------------------------------------------------------------------------------------------------
  
  //! \enum ButtonState - Defines Button control states
  enum class ButtonState : ulong32_t
  {
    Checked		      = BST_CHECKED,		      //!< [Windows 4.00] The button is checked.
    Unchecked		    = BST_UNCHECKED,		    //!< [Windows 4.00] No special state. Equivalent to zero.
    Indeterminate		= BST_INDETERMINATE,		//!< [Windows 4.00] The state of the button is indeterminate. Applies only if the button has the BS_3STATE or BS_AUTO3STATE style.
    Pushed		      = BST_PUSHED,		        //!< [Windows 4.00] The button is being shown in the pushed state.
    Focus		        = BST_FOCUS,		        //!< [Windows 4.00] The button has the keyboard focus.

#if _WIN32_WINNT >= _WIN32_WINNT_WINXP
    Hot		          = BST_HOT,		          //!< [Windows 5.01] The button is hot; that is, the mouse is hovering over it.
#endif
#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
    DropDownPushed	= BST_DROPDOWNPUSHED,		//!< [Windows 6.00] Windows Vista. The button is in the drop-down state. Applies only if the button has the TBSTYLE_DROPDOWN style.
#endif
  };

  //! Define traits: Non-contiguous attribute enumeration
  template <> struct is_attribute<ButtonState>  : std::true_type  {};
  template <> struct is_contiguous<ButtonState> : std::false_type {};
  template <> struct default_t<ButtonState>     : std::integral_constant<ButtonState,ButtonState::Unchecked>   {};
  

}

#endif  // WTL_BUTTON_CONSTANTS_HPP
