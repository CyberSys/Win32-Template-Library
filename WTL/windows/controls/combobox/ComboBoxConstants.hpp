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
  
  //! \enum ComboBoxMessage - Defines standard ComboBox messages
  enum class ComboBoxMessage : int16_t
  {
  	AddString = CB_ADDSTRING,				                    //!< [Explorer 3.00] Adds a string to the list box of a combo box. If the combo box does not have the CBS_SORT style, the string is added to the end of the list. Otherwise, the string is inserted into the list, and the list is sorted.
  	DeleteString = CB_DELETESTRING,				              //!< [Explorer 3.00] Deletes a string in the list box of a combo box.
  	Dir = CB_DIR,				                                //!< [Explorer 3.00] Adds names to the list displayed by the combo box. The message adds the names of directories and files that match a specified string and set of file attributes. 	DIR = CB_DIR,				//!< [Explorer 3.00]can also add mapped drive letters to the list.
  	FindString = CB_FINDSTRING,				                  //!< [Explorer 3.00] Searches the list box of a combo box for an item beginning with the characters in a specified string.
  	FindStringExact = CB_FINDSTRINGEXACT,				        //!< [Explorer 3.00] Finds the first list box string in a combo box that matches the string specified in the lParam parameter.
  	GetComboBoxInfo = CB_GETCOMBOBOXINFO,				        //!< [Explorer 3.00] Gets information about the specified combo box.
  	GetCount = CB_GETCOUNT,				                      //!< [Explorer 3.00] Gets the number of items in the list box of a combo box.
  	GetCueBanner = CB_GETCUEBANNER,				              //!< [Explorer 3.00] Gets the cue banner text displayed in the edit control of a combo box. Send this message explicitly or by using the ComboBox_GetCueBannerText macro.
  	GetCurSel = CB_GETCURSEL,				                    //!< [Explorer 3.00] Sent to retrieve the index of the currently selected item, if any, in the list box of a combo box.
  	GetDroppedControlRect = CB_GETDROPPEDCONTROLRECT,		//!< [Explorer 3.00] Sent to retrieve the screen coordinates of a combo box in its dropped-down state.
  	GetDroppedState = CB_GETDROPPEDSTATE,				        //!< [Explorer 3.00] Determines whether the list box of a combo box is dropped down.
  	GetDroppedWidth = CB_GETDROPPEDWIDTH,				        //!< [Explorer 3.00] Gets the minimum allowable width, in pixels, of the list box of a combo box with the CBS_DROPDOWN or CBS_DROPDOWNLIST style.
  	GetEditSel = CB_GETEDITSEL,				                  //!< [Explorer 3.00] Gets the starting and ending character positions of the current selection in the edit control of a combo box.
  	GetExtendedUI = CB_GETEXTENDEDUI,				            //!< [Explorer 3.00] Determines whether a combo box has the default user interface or the extended user interface.
  	GetHorizontalExtent = CB_GETHORIZONTALEXTENT,				//!< [Explorer 3.00] Gets the width, in pixels, that the list box can be scrolled horizontally (the scrollable width). This is applicable only if the list box has a horizontal scroll bar.
  	GetItemData = CB_GETITEMDATA,					              //!< [Explorer 3.00] Sent  to a combo box to retrieve the application-supplied value associated with the specified item in the combo box.
  	GetItemHeight = CB_GETITEMHEIGHT,				            //!< [Explorer 3.00] Determines the height of list items or the selection field in a combo box.
  	GetListBoxText = CB_GETLBTEXT,				              //!< [Explorer 3.00] Gets a string from the list of a combo box.
  	GetListBoxTextLen = CB_GETLBTEXTLEN,				        //!< [Explorer 3.00] Gets the length, in characters, of a string in the list of a combo box.
  	GetLocale = CB_GETLOCALE,				                    //!< [Explorer 3.00] Gets the current locale of the combo box. The locale is used to determine the correct sorting order of displayed text for combo boxes with the CBS_SORT style and text added by using the 	ADDSTRING = CB_ADDSTRING,				//!< [Explorer 3.00]message.
  	GetMinVisible = CB_GETMINVISIBLE,				            //!< [Explorer 3.00] Gets the minimum number of visible items in the drop-down list of a combo box.
  	GetTopIndex = CB_GETTOPINDEX,				                //!< [Explorer 3.00] Sent to retrieve the zero-based index of the first visible item in the list box portion of a combo box. Initially, the item with index 0 is at the top of the list box, but if the list box contents have been scrolled, another item may be at the top.
  	InitStorage = CB_INITSTORAGE,				                //!< [Explorer 3.00] Sent before adding a large number of items to the list box portion of a combo box. This message allocates memory for storing list box items.
  	InsertString = CB_INSERTSTRING,				              //!< [Explorer 3.00] Inserts a string or item data into the list of a combo box. Unlike the CB_ADDSTRING,	CB_INSERTSTRING, message does not cause a list with the CBS_SORT style to be sorted.
  	LimitText = CB_LIMITTEXT,				                    //!< [Explorer 3.00] Limits the length of the text the user may type into the edit control of a combo box.
  	ResetContent = CB_RESETCONTENT,				              //!< [Explorer 3.00] Removes all items from the list box and edit control of a combo box.
  	SelectString = CB_SELECTSTRING,				              //!< [Explorer 3.00] Searches the list of a combo box for an item that begins with the characters in a specified string. If a matching item is found, it is selected and copied to the edit control.
  	SetCueBanner = CB_SETCUEBANNER,				              //!< [Explorer 3.00] Sets the cue banner text that is displayed for the edit control of a combo box.
  	SetCurSel = CB_SETCURSEL,				                    //!< [Explorer 3.00] Sent to select a string in the list of a combo box. If necessary, the list scrolls the string into view. The text in the edit control of the combo box changes to reflect the new selection, and any previous selection in the list is removed.
  	SetDroppedWidth = CB_SETDROPPEDWIDTH,				        //!< [Explorer 3.00] Sent to set the maximum allowable width, in pixels, of the list box of a combo box with the CBS_DROPDOWN or CBS_DROPDOWNLIST style.
  	SetEditSel = CB_SETEDITSEL,				                  //!< [Explorer 3.00] Sent to select characters in the edit control of a combo box.
  	SetExtendedUI = CB_SETEXTENDEDUI,				            //!< [Explorer 3.00] Sent to select either the default UI or the extended UI for a combo box that has the CBS_DROPDOWN or CBS_DROPDOWNLIST style.
  	SetHorizontalExtent = CB_SETHORIZONTALEXTENT,				//!< [Explorer 3.00] Sent to set the width, in pixels, by which a list box can be scrolled horizontally (the scrollable width). If the width of the list box is smaller than this value, the horizontal scroll bar horizontally scrolls items in the list box. If the width of the list box is equal to or greater than this value, the horizontal scroll bar is hidden or, if the combo box has the CBS_DISABLENOSCROLL style, disabled.
  	SetItemData = CB_SETITEMDATA,					              //!< [Explorer 3.00] Sent to set the value associated with the specified item in a combo box.
  	SetItemHeight = CB_SETITEMHEIGHT,				            //!< [Explorer 3.00] Sent to set the height of list items or the selection field in a combo box.
  	SetLocale = CB_SETLOCALE,				                    //!< [Explorer 3.00] Sent to set the current locale of the combo box. If the combo box has the CBS_SORT style and strings are added using CB_ADDSTRING, the locale of a combo box affects how list items are sorted.
  	SetMinVisible = CB_SETMINVISIBLE,				            //!< [Explorer 3.00] Sent to set the minimum number of visible items in the drop-down list of a combo box.
  	SetTopIndex = CB_SETTOPINDEX,					              //!< [Explorer 3.00] Sent to ensure that a particular item is visible in the list box of a combo box. The system scrolls the list box contents so that either the specified item appears at the top of the list box or the maximum scroll range has been reached.
  	ShowDropDown = CB_SHOWDROPDOWN,				              //!< [Explorer 3.00] Sent to show or hide the list box of a combo box that has the CBS_DROPDOWN or CBS_DROPDOWNLIST style.
  };
  
  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<ComboBoxMessage>  : std::false_type  {};
  template <> struct is_contiguous<ComboBoxMessage> : std::false_type  {};
  template <> struct default_t<ComboBoxMessage>     : std::integral_constant<ComboBoxMessage,ComboBoxMessage::AddString>   {};
  
  // --------------------------------------------------------------------------------------------------------------

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
