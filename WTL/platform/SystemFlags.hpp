//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\DrawingFlags.hpp
//! \brief Defines flags for drawing related Win32 API functions
//! \date 26 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SYSTEM_FLAGS_HPP
#define WTL_SYSTEM_FLAGS_HPP

#include <wtl/WTL.hpp>
#include <wtl/traits/EnumTraits.hpp>                  //!< is_attribute, is_contiguous
#include <wtl/utils/Default.hpp>                      //!< default_t
#include <utility>

//! \namespace wtl - Windows template library
namespace wtl
{
  
  // ----------------------------------- COMMON CONTROL VERSION ----------------------------------

  
  //! \enum ControlVersion - Defines common control library (ComCtl32) versions
  enum class ControlVersion : ulong32_t
  { 
    Explorer30 = 0x0400,        //!< Internet Explorer 3.00  (Windows 95)
    Explorer31 = 0x0470,        //!< Internet Explorer 3.01  (Windows NT 4, Windows 95 OSR2)

    Explorer40 = 0x0471,        //!< Internet Explorer 4.00 
    Explorer41 = 0x0472,        //!< Internet Explorer 4.01  (Windows 98)

    Explorer50 = 0x0580,        //!< Internet Explorer 5.00  (Windows 98 SE)
    Explorer51 = 0x0581,        //!< Internet Explorer 5.01  (Windows 2000)

    Explorer60 = 0x0582,        //!< Internet Explorer 6.00  (Windows XP)
    
    WinXp      = 0x0600,        //!< <Not packaged with OS> (Windows XP)
    Vista      = 0x0610,        //!< <Not packaged with OS> (Windows Vista)
  };

  //! Define traits: Non-Contiguous enumeration
  template <> struct is_attribute<ControlVersion>  : std::false_type  {};
  template <> struct is_contiguous<ControlVersion> : std::false_type  {};
  template <> struct default_t<ControlVersion>     : std::integral_constant<ControlVersion,ControlVersion::Explorer30>   {};
  
  // ----------------------------------- DATE FORMAT FLAGS ----------------------------------

  //! \enum DateFlags - Define GetDateFormat() flags
  enum class DateFlags : long32_t
  {
    ShortDate = 0x00000001,			      //!< Use short date picture
    LongDate = 0x00000002,			      //!< Use long date picture
    AltCalendar = 0x00000004,		      //!< Use alternate calendar (if any)
    YearMonth = 0x00000008,			      //!< [Windows 5.00] Use year month picture
    LtrReading = 0x00000010,			    //!< [Windows 5.00] Add marks for left to right reading order layout
    RtlReading = 0x00000020,			    //!< [Windows 5.00] Add marks for right to left reading order layout
    AutoLayout = 0x00000040,			    //!< [Windows 6.01] add appropriate marks for left-to-right or right-to-left reading order layout
  };

  //! Define traits: Non-Contiguous attribute
  template <> struct is_attribute<DateFlags>  : std::true_type  {};
  template <> struct is_contiguous<DateFlags> : std::false_type {};
  template <> struct default_t<DateFlags>     : std::integral_constant<DateFlags,DateFlags::ShortDate>   {};

  // ----------------------------------- RESOURCE TYPES ----------------------------------
  
  
  //! \enum ResourceType - Defines resource types
  enum class ResourceType : uint32_t
  {
    Cursor = 1,           //!< 
    Bitmap = 2,           //!< 
    Icon = 3,             //!< 
    Menu = 4,             //!< 
    Dialog = 5,           //!< 
    String = 6,           //!< 
    FontDir = 7,          //!< 
    Font = 8,             //!< 
    Accelerator = 9,      //!< 
    RcData = 10,          //!< 
    MessageTable = 11,    //!< 
    GroupCursor = 12,     //!< 
    GroupIcon = 14,       //!< 
    Version = 16,         //!< 
    DlgInclude = 17,      //!< 
    PlugPlay = 19,        //!< [Windows 4.00] 
    Vxd = 20,             //!< [Windows 4.00] 
    AniCursor = 21,       //!< [Windows 4.00] 
    AniIcon = 22,         //!< [Windows 4.00] 
    Html = 23,            //!< 
    Manifest = 24,        //!< 
  };

  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<ResourceType>  : std::false_type {};
  template <> struct is_contiguous<ResourceType> : std::false_type {};
  template <> struct default_t<ResourceType>     : std::integral_constant<ResourceType,ResourceType::Cursor>   {};
  
  
  // ----------------------------------- SHELL VERSION ----------------------------------


  //! \enum ShellVersion - Defines shell library (Shell32) versions
  enum class ShellVersion : ulong32_t
  { 
    Win95      = 0x0400,        //!< Windows 95 / NT4 (Internet Explorer 3.0)
    Explorer40 = 0x0471,        //!< Windows 95 / NT4 (Internet Explorer 4.0)
    Win98      = 0x0472,        //!< Windows 98 / 98SE
    Win2000    = 0x0500,        //!< Windows 2000
    WinXp      = 0x0600,        //!< Windows XP / Vista
    Win7       = 0x0610,        //!< Windows 7
    Future,                     //!< Future
  };
  
  //! Define traits: Non-Contiguous enumeration
  template <> struct is_attribute<ShellVersion>  : std::false_type  {};
  template <> struct is_contiguous<ShellVersion> : std::false_type  {};
  template <> struct default_t<ShellVersion>     : std::integral_constant<ShellVersion,ShellVersion::Win95>   {};

  // ----------------------------------- SYSTEM CURSORS ----------------------------------
  

  //! \enum SystemCursor - System cursor Ids
  enum class SystemCursor
  {
    Arrow = 32512,			  //!< 
    IBeam = 32513,			  //!< 
    Wait = 32514,			    //!< 
    Cross = 32515,			  //!< 
    Uparrow = 32516,			//!< 
    Size = 32640,			    //!< obsolete: use SizeAll
    Icon = 32641,			    //!< obsolete: use Arrow
    SizeNWSE = 32642,			//!< 
    SizeNESW = 32643,			//!< 
    SizeWE = 32644,			  //!< 
    SizeNS = 32645,			  //!< 
    SizeAll = 32646,			//!< 
    No = 32648,			      //!< 
    Hand = 32649,			    //!< [Windows 5.00]
    AppStarting = 32650,	//!< 
    Help = 32651,			    //!< 
  };
  
  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<SystemCursor>  : std::false_type  {};
  template <> struct is_contiguous<SystemCursor> : std::false_type  {};
  template <> struct default_t<SystemCursor>     : std::integral_constant<SystemCursor,SystemCursor::Arrow>   {};


  // ----------------------------------- SYSTEM ICONS ----------------------------------
  
  //! \enum SystemIcon - System icon Ids
  enum class SystemIcon
  {
    Application = 32512,			    //!< 
    Hand = 32513,			            //!< 
    Question = 32514,			        //!< 
    Exclamation = 32515,			    //!< 
    Asterisk = 32516,			        //!< 
    WinLogo = 32517,			        //!< 
    Shield = 32518,               //!< [Windows 6.00]
    Warning = Exclamation,			  //!< 
    Error = Hand,			            //!< 
    Information = Asterisk,			  //!< 
  };
  
  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<SystemIcon>  : std::false_type  {};
  template <> struct is_contiguous<SystemIcon> : std::false_type  {};
  template <> struct default_t<SystemIcon>     : std::integral_constant<SystemIcon,SystemIcon::Application>   {};
  
  // ----------------------------------- SYSTEM COLOURS ----------------------------------
  
  //! \enum SystemColour - Defines system colours
  enum class SystemColour
  {
    ScrollBar = 0,			            //!< Scroll bar gray area.
    Background = 1,			            //!< Desktop.
    ActiveCaption = 2,			        //!< Active window title bar. 
    InactiveCaption = 3,			      //!< Inactive window caption. 
    Menu = 4,			                  //!< Menu background.
    Window = 5,			                //!< Window background.
    WindowFrame = 6,			          //!< Window frame.
    MenuText = 7,			              //!< Text in menus.
    WindowText = 8,			            //!< Text in windows.
    CaptionText = 9,			          //!< Text in caption, size box, and scroll bar arrow box.
    ActiveBorder = 10,			        //!< Active window border.
    InactiveBorder = 11,			      //!< Inactive window border.
    AppWorkspace = 12,			        //!< Background color of multiple document interface (MDI) applications.
    Highlight = 13,			            //!< Item(s) selected in a control.
    HighlightText = 14,			        //!< Text of item(s) selected in a control.
    BtnFace = 15,			              //!< Face color for three-dimensional display elements and for dialog box backgrounds.
    BtnShadow = 16,			            //!< Shadow color for three-dimensional display elements (for edges facing away from the light source).
    GrayText = 17,			            //!< Grayed (disabled) text.
    BtnText = 18,			              //!< Text on push buttons.
    InactiveCaptionText = 19,			  //!< Color of text in an inactive caption.
    BtnHighlight = 20,			        //!< Highlight color for three-dimensional display elements (for edges facing the light source.)

    BtnDkShadow = 21,			          //!< Dark shadow for three-dimensional display elements.
    BtnLight = 22,			            //!< Light color for three-dimensional display elements (for edges facing the light source.)
    InfoText = 23,			            //!< Text color for tooltip controls.
    InfoBk = 24,			              //!< Background color for tooltip controls.
    
    Hotlight = 26,			            //!< [windows 5.00] Color for a hyperlink or hot-tracked item.
    GradientActiveCaption = 27,			//!< [windows 5.00] Right side color in the color gradient of an active window's title bar. 
    GradientInactiveCaption = 28,		//!< [windows 5.00] Right side color in the color gradient of an inactive window's title bar. 
    MenuHilight = 29,			          //!< [windows 5.01] The color used to highlight menu items when the menu appears as a flat menu
    MenuBar = 30,			              //!< [windows 5.01] The background color for the menu bar when menus appear as flat menus 
    
    Desktop = Background,			      //!< 
  };

  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<SystemColour>  : std::false_type  {};
  template <> struct is_contiguous<SystemColour> : std::false_type  {};
  template <> struct default_t<SystemColour>     : std::integral_constant<SystemColour,SystemColour::ScrollBar>   {};

  // ----------------------------------- SYSTEM METRICS ----------------------------------
  
  //! \enum SystemMetric - System metrics
  enum class SystemMetric
  {
    Arrange = SM_ARRANGE,				                //!< The flags that specify how the system arranged minimized windows. For more information, see the Remarks section in this topic.
    CleanBoot = SM_CLEANBOOT,				            //!< The value that specifies how the system is started: A fail-safe boot (also called SafeBoot, Safe Mode, or Clean Boot) bypasses the user startup files.
    cMonitors = SM_CMONITORS,				            //!< The number of display monitors on a desktop. For more information, see the Remarks section in this topic. 
    cMouseButtons = SM_CMOUSEBUTTONS,				    //!< The number of buttons on a mouse, or zero if no mouse is installed.
    //ConvertibleSlateMode = SM_CONVERTIBLESLATEMODE,  //!<	Reflects the state of the laptop or slate mode, 0 for Slate Mode and non-zero otherwise. When this system metric changes, the system sends a broadcast message via WM_SETTINGCHANGE with "ConvertibleSlateMode" in the LPARAM. Note that this system metric doesn't apply to desktop PCs. In that case, use GetAutoRotationState.

    cxBorder = SM_CXBORDER,		                  //!< The width of a window border, in pixels. This is equivalent to the SM_CXEDGE value for windows with the 3-D look.
    cxCursor = SM_CXCURSOR,			                //!< The width of a cursor, in pixels. The system cannot create cursors of other sizes.
    cxDialogFrame = SM_CXDLGFRAME,				      //!< This value is the same as SM_CXFIXEDFRAME.
    cxDoubleClick = SM_CXDOUBLECLK,			        //!< The width of the rectangle around the location of a first click in a double-click sequence, in pixels. The second click must occur within the rectangle that is defined by SM_CXDOUBLECLK and SM_CYDOUBLECLK for the system to consider the two clicks a double-click. The two clicks must also occur within a specified time. To set the width of the double-click rectangle, call SystemParametersInfo with SPI_SETDOUBLECLKWIDTH.
    cxDrag = SM_CXDRAG,				                  //!< The number of pixels on either side of a mouse-down point that the mouse pointer can move before a drag operation begins. This allows the user to click and release the mouse button easily without unintentionally starting a drag operation. If this value is negative, it is subtracted from the left of the mouse-down point and added to the right of it.
    cxEdge = SM_CXEDGE,				                  //!< The width of a 3-D border, in pixels. This metric is the 3-D counterpart of SM_CXBORDER.
    cxFixedFrame = SM_CXFIXEDFRAME,			        //!< The thickness of the frame around the perimeter of a window that has a caption but is not sizable, in pixels. SM_CXFIXEDFRAME is the height of the horizontal border, and SM_CYFIXEDFRAME is the width of the vertical border. This value is the same as SM_CXDLGFRAME.
    cxFocusBorder = SM_CXFOCUSBORDER,		        //!< [Win XP] The width of the left and right edges of the focus rectangle that the DrawFocusRect draws. This value is in pixels. Windows 2000:  This value is not supported.
    cxFrame = SM_CXFRAME,			                  //!< This value is the same as SM_CXSIZEFRAME.
    cxFullScreen = SM_CXFULLSCREEN,			        //!< The width of the client area for a full-screen window on the primary display monitor, in pixels. To get the coordinates of the portion of the screen that is not obscured by the system taskbar or by application desktop toolbars, call the SystemParametersInfo function with the SPI_GETWORKAREA value.
    cxHScroll = SM_CXHSCROLL,				            //!< The width of the arrow bitmap on a horizontal scroll bar, in pixels.
    cxHThumb = SM_CXHTHUMB,				              //!< The width of the thumb box in a horizontal scroll bar, in pixels.
    cxIcon = SM_CXICON,				                  //!< The default width of an icon, in pixels. The LoadIcon function can load only icons with the dimensions that SM_CXICON and SM_CYICON specifies.
    cxIconSpacing = SM_CXICONSPACING,				    //!< The width of a grid cell for items in large icon view, in pixels. Each item fits into a rectangle of size SM_CXICONSPACING by SM_CYICONSPACING when arranged. This value is always greater than or equal to SM_CXICON.
    cxMaximized = SM_CXMAXIMIZED,				        //!< The default width, in pixels, of a maximized top-level window on the primary display monitor.
    cxMaxTrack = SM_CXMAXTRACK,				          //!< The default maximum width of a window that has a caption and sizing borders, in pixels. This metric refers to the entire desktop. The user cannot drag the window frame to a size larger than these dimensions. A window can override this value by processing the WM_GETMINMAXINFO message.
    cxMenuCheck = SM_CXMENUCHECK,				        //!< The width of the default menu check-mark bitmap, in pixels.
    cxMenuSize = SM_CXMENUSIZE,				          //!< The width of menu bar buttons, such as the child window close button that is used in the multiple document interface, in pixels.
    cxMinWindow = SM_CXMIN,				              //!< The minimum width of a window, in pixels.
    cxMinimized = SM_CXMINIMIZED,				        //!< The width of a minimized window, in pixels.
    cxMinSpacing = SM_CXMINSPACING,				      //!< The width of a grid cell for a minimized window, in pixels. Each minimized window fits into a rectangle this size when arranged. This value is always greater than or equal to SM_CXMINIMIZED.
    cxMinTrack = SM_CXMINTRACK,				          //!< The minimum tracking width of a window, in pixels. The user cannot drag the window frame to a size smaller than these dimensions. A window can override this value by processing the WM_GETMINMAXINFO message.
    //cxPaddedBorder = SM_CXPADDEDBORDER,				//!< [¬XP/2000] The amount of border padding for captioned windows, in pixels. Windows XP/2000:  This value is not supported.
    cxScreen = SM_CXSCREEN,				              //!< The width of the screen of the primary display monitor, in pixels. This is the same value obtained by calling GetDeviceCaps as follows: GetDeviceCaps( hdcPrimaryMonitor, HORZRES).
    cxSizeButton = SM_CXSIZE,				            //!< The width of a button in a window caption or title bar, in pixels.
    cxSizeFrame = SM_CXSIZEFRAME,				        //!< The thickness of the sizing border around the perimeter of a window that can be resized, in pixels. SM_CXSIZEFRAME is the width of the horizontal border, and SM_CYSIZEFRAME is the height of the vertical border. This value is the same as SM_CXFRAME.
    cxSmallIcon = SM_CXSMICON,				          //!< The recommended width of a small icon, in pixels. Small icons typically appear in window captions and in small icon view.
    cxSmallButton = SM_CXSMSIZE,				        //!< The width of small caption buttons, in pixels.
    cxVirtualScreen = SM_CXVIRTUALSCREEN,				//!< The width of the virtual screen, in pixels. The virtual screen is the bounding rectangle of all display monitors. The SM_XVIRTUALSCREEN metric is the coordinates for the left side of the virtual screen. 
    cxVScroll = SM_CXVSCROLL,				            //!< The width of a vertical scroll bar, in pixels.

    cyBorder = SM_CYBORDER,				              //!< The height of a window border, in pixels. This is equivalent to the SM_CYEDGE value for windows with the 3-D look.
    cyCaption = SM_CYCAPTION,				            //!< The height of a caption area, in pixels.
    cyCursor = SM_CYCURSOR,				              //!< The height of a cursor, in pixels. The system cannot create cursors of other sizes.
    cyDialogFrame = SM_CYDLGFRAME,				      //!< This value is the same as SM_CYFIXEDFRAME.
    cyDoubleClick = SM_CYDOUBLECLK,				      //!< The height of the rectangle around the location of a first click in a double-click sequence, in pixels. The second click must occur within the rectangle defined by SM_CXDOUBLECLK and SM_CYDOUBLECLK for the system to consider the two clicks a double-click. The two clicks must also occur within a specified time. To set the height of the double-click rectangle, call SystemParametersInfo with SPI_SETDOUBLECLKHEIGHT.
    cyDrag = SM_CYDRAG,				                  //!< The number of pixels above and below a mouse-down point that the mouse pointer can move before a drag operation begins. This allows the user to click and release the mouse button easily without unintentionally starting a drag operation. If this value is negative, it is subtracted from above the mouse-down point and added below it. 
    cyEdge = SM_CYEDGE,				                  //!< The height of a 3-D border, in pixels. This is the 3-D counterpart of SM_CYBORDER.
    cyFixedFrame = SM_CYFIXEDFRAME,				      //!< The thickness of the frame around the perimeter of a window that has a caption but is not sizable, in pixels. SM_CXFIXEDFRAME is the height of the horizontal border, and SM_CYFIXEDFRAME is the width of the vertical border.This value is the same as SM_CYDLGFRAME.
    cyFocusBorder = SM_CYFOCUSBORDER,				    //!< [¬Win 2000] The height of the top and bottom edges of the focus rectangle drawn by DrawFocusRect. This value is in pixels. Windows 2000:  This value is not supported.
    cyFrame = SM_CYFRAME,			                  //!< This value is the same as SM_CYSIZEFRAME.
    cyFullScreen = SM_CYFULLSCREEN,				      //!< The height of the client area for a full-screen window on the primary display monitor, in pixels. To get the coordinates of the portion of the screen not obscured by the system taskbar or by application desktop toolbars, call the SystemParametersInfo function with the SPI_GETWORKAREA value.
    cyHScroll = SM_CYHSCROLL,				            //!< The height of a horizontal scroll bar, in pixels.
    cyIcon = SM_CYICON,				                  //!< The default height of an icon, in pixels. The LoadIcon function can load only icons with the dimensions SM_CXICON and SM_CYICON.
    cyIconSpacing = SM_CYICONSPACING,				    //!< The height of a grid cell for items in large icon view, in pixels. Each item fits into a rectangle of size SM_CXICONSPACING by SM_CYICONSPACING when arranged. This value is always greater than or equal to SM_CYICON.
    cyKanjiWindow = SM_CYKANJIWINDOW,				    //!< For double byte character set versions of the system, this is the height of the Kanji window at the bottom of the screen, in pixels.
    cyMaximized = SM_CYMAXIMIZED,				        //!< The default height, in pixels, of a maximized top-level window on the primary display monitor.
    cyMaxTrack = SM_CYMAXTRACK,				          //!< The default maximum height of a window that has a caption and sizing borders, in pixels. This metric refers to the entire desktop. The user cannot drag the window frame to a size larger than these dimensions. A window can override this value by processing the WM_GETMINMAXINFO message.
    cyMenu = SM_CYMENU,				                  //!< The height of a single-line menu bar, in pixels.
    cyMenuCheck = SM_CYMENUCHECK,				        //!< The height of the default menu check-mark bitmap, in pixels.
    cyMenuSize = SM_CYMENUSIZE,				          //!< The height of menu bar buttons, such as the child window close button that is used in the multiple document interface, in pixels.
    cyMinWindow = SM_CYMIN,				              //!< The minimum height of a window, in pixels.
    cyMinimized = SM_CYMINIMIZED,				        //!< The height of a minimized window, in pixels.
    cyMinSpacing = SM_CYMINSPACING,			        //!< The height of a grid cell for a minimized window, in pixels. Each minimized window fits into a rectangle this size when arranged. This value is always greater than or equal to SM_CYMINIMIZED.
    cyMinTrack = SM_CYMINTRACK,				          //!< The minimum tracking height of a window, in pixels. The user cannot drag the window frame to a size smaller than these dimensions. A window can override this value by processing the WM_GETMINMAXINFO message.
    cyScreen = SM_CYSCREEN,	                  	//!< The height of the screen of the primary display monitor, in pixels. This is the same value obtained by calling GetDeviceCaps as follows: GetDeviceCaps( hdcPrimaryMonitor, VERTRES).
    cySizeButton = SM_CYSIZE,				            //!< The height of a button in a window caption or title bar, in pixels.
    cySizeFrame = SM_CYSIZEFRAME,				        //!< The thickness of the sizing border around the perimeter of a window that can be resized, in pixels. SM_CXSIZEFRAME is the width of the horizontal border, and SM_CYSIZEFRAME is the height of the vertical border. This value is the same as SM_CYFRAME.
    cySmallCaption = SM_CYSMCAPTION,				    //!< The height of a small caption, in pixels.
    cySmallIcon = SM_CYSMICON,				          //!< The recommended height of a small icon, in pixels. Small icons typically appear in window captions and in small icon view.
    cySmallButton = SM_CYSMSIZE,				        //!< The height of small caption buttons, in pixels.
    cyVirtualScreen = SM_CYVIRTUALSCREEN,				//!< The height of the virtual screen, in pixels. The virtual screen is the bounding rectangle of all display monitors. The SM_YVIRTUALSCREEN metric is the coordinates for the top of the virtual screen.
    cyVScroll = SM_CYVSCROLL,				            //!< The height of the arrow bitmap on a vertical scroll bar, in pixels.
    cyVThumb = SM_CYVTHUMB,		                  //!< The height of the thumb box in a vertical scroll bar, in pixels.

    DbcsEnabled = SM_DBCSENABLED,	              //!< Nonzero if User32.dll supports DBCS; otherwise, 0. 
    Debug = SM_DEBUG,				                    //!< Nonzero if the debug version of User.exe is installed; otherwise, 0.
    //Digitizer = SM_DIGITIZER,				          //!< Nonzero if the current operating system is Windows 7 or Windows Server 2008 R2 and the Tablet PC Input service is started; otherwise, 0. The return value is a bitmask that specifies the type of digitizer input supported by the device. For more information, see Remarks. Windows Server 2008, Windows Vista, and Windows XP/2000:  This value is not supported.
    ImeEnabled = SM_IMMENABLED,				          //!< Nonzero if Input Method Manager/Input Method Editor features are enabled; otherwise, 0. SM_IMMENABLED indicates whether the system is ready to use a Unicode-based IME on a Unicode application. To ensure that a language-dependent IME works, check SM_DBCSENABLED and the system ANSI code page. Otherwise the ANSI-to-Unicode conversion may not be performed correctly, or some components like fonts or registry settings may not be present.
    //MAXIMUMTOUCHES = SM_MAXIMUMTOUCHES,				//!< Nonzero if there are digitizers in the system; otherwise, 0. SM_MAXIMUMTOUCHES returns the aggregate maximum of the maximum number of contacts supported by every digitizer in the system. If the system has only single-touch digitizers, the return value is 1. If the system has multi-touch digitizers, the return value is the number of simultaneous contacts the hardware can provide. Windows Server 2008, Windows Vista, and Windows XP/2000:  This value is not supported.
  };
  
  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<SystemMetric>  : std::false_type  {};
  template <> struct is_contiguous<SystemMetric> : std::false_type  {};
  template <> struct default_t<SystemMetric>     : std::integral_constant<SystemMetric,SystemMetric::Arrange>   {};

  // ----------------------------------- WINDOWS OS VERSION ----------------------------------
  

  //! \enum WindowVersion - Defines operating system (Kernel32) versions
  enum class WindowVersion : ulong32_t
  { 
    WinNT    = 0x0400,      //!< Windows 9x / NT4
    Win2000  = 0x0500,      //!< Windows 2000
    WinXp    = 0x0501,      //!< Windows XP 
    Win2003  = 0x0502,      //!< Windows Server 2003
    Vista    = 0x0600,      //!< Windows Vista
    Win7     = 0x0601,      //!< Windows 7
    Win8     = 0x0602,      //!< Windows 8
    Win81    = 0x0603,      //!< Windows 8.1
    Future,                 //!< Future
  };
   
  //! Define traits: Non-Contiguous enumeration
  template <> struct is_attribute<WindowVersion>  : std::false_type  {};
  template <> struct is_contiguous<WindowVersion> : std::false_type  {};
  template <> struct default_t<WindowVersion>     : std::integral_constant<WindowVersion,WindowVersion::WinNT>   {};


  

}

#endif  // WTL_SYSTEM_FLAGS_HPP
