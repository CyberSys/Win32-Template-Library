////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\WindowFlags.hpp
//! \brief Defines flags for windowing related Win32 API functions
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_FLAGS_HPP
#define WTL_WINDOW_FLAGS_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  // ----------------------------------- ::FormatMessage(..) Flags ----------------------------------

  //! \enum FormatMessageFlags - 
  enum class FormatMessageFlags : ulong32
  { 
    AllocateBuffer = 0x00000100, 
    IgnoreInserts = 0x00000200, 
    FromString = 0x00000400, 
    FromHModule = 0x00000800, 
    FromSystem = 0x00001000, 
    ArgumentArray = 0x00002000, 
    MaxWidthMask = 0x000000FF
  };

  //! Define traits: Non-contiguous Attribute
  template <> struct is_attribute<FormatMessageFlags>  : std::true_type  {};
  template <> struct is_contiguous<FormatMessageFlags> : std::false_type {};
  
  //! Define limits traits
  template <> struct max_value<FormatMessageFlags>     : std::integral_constant<FormatMessageFlags,FormatMessageFlags::MaxWidthMask>  {};
  template <> struct min_value<FormatMessageFlags>     : std::integral_constant<FormatMessageFlags,FormatMessageFlags::ArgumentArray> {};


  // ----------------------------------- ::MultiByteToWideChar(..) Flags ----------------------------------


  //! \enum MultiByteFlags - Defines narrow character conversion flags
  enum class MultiByteFlags : ulong32
  {
    PreComposed = 0x00000001,         // use precomposed chars
    Composite = 0x00000002,           // use composite chars
    UseGlyphChars = 0x00000004,       // use glyph chars, not ctrl chars
    ErrInvalidChars = 0x00000008,   // error for invalid chars
  };
  
  //! Define traits: Non-contiguous Attribute
  template <> struct is_attribute<MultiByteFlags>  : std::true_type  {};
  template <> struct is_contiguous<MultiByteFlags> : std::false_type {};

  //! Define limits traits
  template <> struct max_value<MultiByteFlags>     : std::integral_constant<MultiByteFlags,MultiByteFlags::PreComposed>      {};
  template <> struct min_value<MultiByteFlags>     : std::integral_constant<MultiByteFlags,MultiByteFlags::ErrInvalidChars>  {};


  // ----------------------------------- ::WideCharToMultiByte(..) Flags ----------------------------------


  //! \enum WideCharFlags - Defines wide character conversion flags
  enum class WideCharFlags : ulong32
  {
    CompositeCheck = 0x00000200,  // convert composite to precomposed
    DiscardNs = 0x00000010,       // discard non-spacing chars
    SepChars = 0x00000020,        // generate separate chars
    DefaultChar = 0x00000040,     // replace w/ default char
    NoBestFitChars = 0x00000400,  // do not use best fit chars
  };


  //! Define traits: Non-contiguous Attribute
  template <> struct is_attribute<WideCharFlags>  : std::true_type  {};
  template <> struct is_contiguous<WideCharFlags> : std::false_type {};

  //! Define limits traits
  template <> struct max_value<WideCharFlags>     : std::integral_constant<WideCharFlags,WideCharFlags::CompositeCheck>   {};
  template <> struct min_value<WideCharFlags>     : std::integral_constant<WideCharFlags,WideCharFlags::NoBestFitChars>   {};
  


  // ----------------------------------- ::MessageBox(..) Flags ----------------------------------
  

  //! \enum MessageBoxFlags - Defines MessageBox() flags
  enum class MessageBoxFlags : ulong32
  {
    Ok = 0x00000000l,			                  //!< 
    OkCancel = 0x00000001l,			            //!< 
    AbortRetryIgnore = 0x00000002l,			    //!< 
    YesNoCancel = 0x00000003l,			        //!< 
    YesNo = 0x00000004l,			              //!< 
    RetryCancel = 0x00000005l,			        //!< 
    CancelTryContinue = 0x00000006l, 			  //!< [Windows 5.00] 

    IconError = 0x00000010l,			          //!< 
    IconQuestion = 0x00000020l,			        //!< 
    IconWarning = 0x00000030l,			        //!< 
    IconInformation = 0x00000040l,			    //!< 
    UserIcon = 0x00000080l,			            //!< 

    DefButton1 = 0x00000000l,			          //!< 
    DefButton2 = 0x00000100l,			          //!< 
    DefButton3 = 0x00000200l,			          //!< 
    DefButton4 = 0x00000300l,			          //!< 

    Applmodal = 0x00000000l,			          //!< 
    Systemmodal = 0x00001000l,			        //!< 
    Taskmodal = 0x00002000l,			          //!< 
    Help = 0x00004000l,			                //!< 
    Nofocus = 0x00008000l,			            //!< 
    Setforeground = 0x00010000l,			      //!< 
    DefaultDesktopOnly = 0x00020000l,		    //!< 
    Topmost = 0x00040000l,			            //!< 
    Right = 0x00080000l,			              //!< 
    RtlReading = 0x00100000l,			          //!< 
    
    ServiceNotification = 0x00200000l,		  //!< 
    ServiceNotificationNT3x = 0x00040000l,  //!< 

    TypeMask = 0x0000000fl,			    //!< 
    IconMask = 0x000000f0l,			    //!< 
    DefMask = 0x00000f00l,			    //!< 
    ModeMask = 0x00003000l,			    //!< 
    MiscMask = 0x0000C000L,			    //!< 
  };


  //! Define traits: Non-Contiguous enumeration
  template <> struct is_attribute<MessageBoxFlags>  : std::true_type   {};
  template <> struct is_contiguous<MessageBoxFlags> : std::false_type  {};

  //! Define limits traits
  template <> struct max_value<MessageBoxFlags>     : std::integral_constant<MessageBoxFlags,MessageBoxFlags::RtlReading>   {};
  template <> struct min_value<MessageBoxFlags>     : std::integral_constant<MessageBoxFlags,MessageBoxFlags::Ok>           {};

  
  // ----------------------------------- ::ShowWindow(..) Flags ----------------------------------
  


  //! \enum ShowWindowFlags - Defines flags for the Win32 API function ::ShowWindow(..) 
  enum class ShowWindowFlags : int32
  {
    Hide = 0,			          //!< 
    ShowNormal = 1,			    //!< 
    Normal = ShowNormal,		//!< 
    ShowMinimized = 2,			//!< 
    ShowMaximized = 3,			//!< 
    Maximize = 3,			      //!< 
    ShowNoActivate = 4,			//!< 
    Show = 5,			          //!< 
    Minimize = 6,			      //!< 
    ShowMinNoActive = 7,		//!< 
    ShowNA = 8,			        //!< 
    Restore = 9,			      //!< 
    ShowDefault = 10,			  //!< 
    ForceMinimize = 11,			//!< 
  };


  //! Define traits: Contiguous enumeration
  template <> struct is_attribute<ShowWindowFlags>  : std::false_type  {};
  template <> struct is_contiguous<ShowWindowFlags> : std::true_type   {};

  //! Define limits traits
  template <> struct max_value<ShowWindowFlags>     : std::integral_constant<ShowWindowFlags,ShowWindowFlags::ForceMinimize>   {};
  template <> struct min_value<ShowWindowFlags>     : std::integral_constant<ShowWindowFlags,ShowWindowFlags::Hide>            {};
  
  

  // ----------------------------------- WM_SHOWWINDOW arguments ----------------------------------
  

  //! \enum ShowWindowType - Defines flags for the Win32 message 'WM_SHOWWINDOW'
  enum class ShowWindowType : ulong32
  {
    ParentClosing = 1, 			//!< The window is being uncovered because a maximize window was restored or minimized.
    OtherZoom = 2, 			    //!< The window is being covered by another window that has been maximized.
    ParentOpening = 3, 			//!< The window's owner window is being minimized.
    OtherUnZoom = 4, 			  //!< The window's owner window is being restored.
  };


  //! Define traits: Contiguous enumeration
  template <> struct is_attribute<ShowWindowType>  : std::false_type  {};
  template <> struct is_contiguous<ShowWindowType> : std::true_type   {};

  //! Define limits traits
  template <> struct max_value<ShowWindowType>     : std::integral_constant<ShowWindowType,ShowWindowType::OtherUnZoom>     {};
  template <> struct min_value<ShowWindowType>     : std::integral_constant<ShowWindowType,ShowWindowType::ParentClosing>   {};
  
  
  

  // ----------------------------------- SYSTEM WINDOW CLASSES ----------------------------------
  

  
  //! \enum SystemClass - Defines system window classes
  enum class SystemClass : uint16
  {
    // Common Controls
    Animate,                //!< Creates animation controls. These controls silently display an audio video interleaved (AVI) clip.
    DateTime,               //!< Creates date and time picker controls. These controls provide a simple interface to exchange date and time information with a user.
    HotKey,                 //!< Creates hot key controls. These controls make it easy for the user to define hot keys.
    Calendar,               //!< Creates month calendar controls. These controls provide a simple way for a user to select a date from a familiar interface.
    ProgressBar,            //!< Creates progress bars. These controls indicate the progress of a lengthy operation.
    CoolBar,                //!< Creates rebar controls. These controls act as a container for child windows.
    StatusBar,              //!< Creates status windows. These controls display status information in a horizontal window.
    ToolBar,                //!< Creates toolbars. These controls contain buttons that carry out menu commands.
    ToolTip,                //!< Creates tooltip controls. These controls display a small pop-up window containing a line of text that describes the purpose of a tool in an application.
    TrackBar,               //!< Creates trackbars. These controls let the user select from a range of values by moving a slider.
    Spin,                   //!< Creates up-down controls. These controls combine a pair of arrows with an edit control. Clicking the arrows increments or decrements the value in the edit control.

    // User controls
    Button = 0x0080,        //!< Creates button controls. These controls enable a user to initiate an action.
    Edit = 0x0081,          //!< Creates edit controls. These controls contain editable text.
    Static = 0x0082,			  //!< Creates static controls. These controls contain noneditable text.
    ListBox = 0x0083,       //!< Creates listbox controls. These controls contain lists of items.
    ScrollBar = 0x0084,			//!< Creates scroll bar controls. These controls enable the user to scroll the contents of a window.
    ComboBox = 0x0085,      //!< Creates combobox controls. These controls contain a drop-down list from which a single item can be selected.
    ComboBoxEx,             //!< Creates ComboBoxEx controls. These controls provide an extension of the combo box control that provides native support for item images.
    Header,                 //!< Creates header controls. These controls display headings at the top of columns of information and let the user sort the information by clicking the headings.
    IpAddress,			        //!< Creates IP address controls. These controls are similar to an edit control, but they enable you to enter a numeric address in Internet protocol (IP) format.
    Link,			              //!< Creates SysLink controls. These controls contain hypertext links.
    ListView,			          //!< Creates list-view controls. These controls display a collection of items, each consisting of an icon and a label, and provide several ways to arrange the items.
    NativeFont,			        //!< Creates native font controls. These are invisible controls that work in the background to enable a dialog box's predefined controls to display the current system language.
    PageScroller,			      //!< Creates pager controls. These controls are used to contain and scroll another window.
    TreeView,			          //!< Creates tree-view controls. These controls display a hierarchical list of items. Each item consists of a label and an optional bitmap.
    Tab,			              //!< Creates tab controls. These controls define multiple pages for the same area of a window or dialog box. Each page consists of a set of information or a group of controls that an application displays when the user selects the corresponding tab.
    
    // Undocumented
    DDEMLEvent,             //!< Dynamic Data Exchange Management Library (DDEML) events.
    ComboLBox,              //!< List box contained in a combo box.
    MessageOnly,            //!< Message-only window.
    Menu         = 32768,	  //!< Menu
    Desktop      = 32769,	  //!< Desktop window
    SystemDialog = 32770,	  //!< System dialog box
    TaskSwitch   = 32771,	  //!< task switch window.
    IconTitle    = 32772,	  //!< Icon titles.
  };


  //! Define traits: Contiguous enumeration
  template <> struct is_attribute<SystemClass>  : std::false_type  {};
  template <> struct is_contiguous<SystemClass> : std::true_type   {};
  
  //! Define limits traits
  template <> struct max_value<SystemClass>     : std::integral_constant<SystemClass,SystemClass::IconTitle>  {};
  template <> struct min_value<SystemClass>     : std::integral_constant<SystemClass,SystemClass::Animate>    {};


}

#endif  // WTL_WINDOW_FLAGS_HPP
