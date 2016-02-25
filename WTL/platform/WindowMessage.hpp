//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\WindowMessage.hpp
//! \brief Defines window messages
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_MESSAGE_HPP
#define WTL_WINDOW_MESSAGE_HPP

#include <wtl/WTL.hpp>

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \enum WindowMessage - Window message identifiers
  enum class WindowMessage : uint16_t
  {
    None = 0x0000,			        	              //!< WM_NULL
    Create = 0x0001,			     	                //!< 
    Destroy = 0x0002,			      	              //!< 
    Move = 0x0003,			        	              //!< 
    Size = 0x0005,			        	              //!< 
    Activate = 0x0006,			    	              //!< 
    SetFocus = 0x0007,			    	              //!< 
    KillFocus = 0x0008,			    	              //!< 
    Enable = 0x000A,			      	              //!< 
    SetRedraw = 0x000B,			    	              //!< 
    SetText = 0x000C,			      	              //!< 
    GetText = 0x000D,			      	              //!< 
    GetTextLength = 0x000E,				              //!< 
    Paint = 0x000F,			        	              //!< 
    Close = 0x0010,			       	                //!< 
    QueryEndSession = 0x0011,			              //!< 
    QueryOpen = 0x0013,			    	              //!< 
    EndSession = 0x0016,			  	              //!< 
    Quit = 0x0012,			        	              //!< 
    EraseBackground = 0x0014,			              //!< Formerly WM_ERASEBKND
    SysColourChange = 0x0015,			              //!< 
    ShowWindow = 0x0018,			 	                //!< 
    SettingChange = 0x001A,				              //!< 
    DevModeChange = 0x001B,				              //!< 
    ActivateApp = 0x001C,			  	              //!< 
    FontChange = 0x001D,			  	              //!< 
    TimeChange = 0x001E,			  	              //!< 
    CancelMode = 0x001F,			  	              //!< 
    SetCursor = 0x0020,			    	              //!< 
    MouseActivate = 0x0021,				              //!< 
    ChildActivate = 0x0022,				              //!< 
    QueueSync = 0x0023,			    	              //!< 
    GetMinMaxInfo = 0x0024,				              //!< 
    PaintIcon = 0x0026,			    	              //!< 
    IconEraseBackground = 0x0027,			          //!< Formerly WM_ICONERASEBKND
    NextDialogControl = 0x0028,			            //!< Formerly WM_NEXTDLGCTL
    SpoolerStatus = 0x002A,				              //!< 
    DrawItem = 0x002B,			    	              //!< 
    MeasureItem = 0x002C,			  	              //!< 
    DeleteItem = 0x002D,			  	              //!< 
    VKeyToItem = 0x002E,			  	              //!< 
    CharToItem = 0x002F,			  	              //!< 
    SetFont = 0x0030,			      	              //!< 
    GetFont = 0x0031,			      	              //!< 
    SetHotKey = 0x0032,			    	              //!< 
    GetHotKey = 0x0033,			    	              //!< 
    QueryDragIcon = 0x0037,				              //!< 
    CompareItem = 0x0039,			  	              //!< 
    GetComObject = 0x003D,				              //!< Formerly WM_GETOBJECT
    Compacting = 0x0041,			  	              //!< 
    CommNotify = 0x0044,					              //!<   /* no longer suported */
    WindowPositionChanging = 0x0046,		        //!< Formerly WM_WINDOWPOSCHANGING
    WindowPositionChanged = 0x0047,		          //!< Formerly WM_WINDOWPOSCHANGED
    Power = 0x0048,			        	              //!< 
    Notify = 0x004E,			      	              //!< 
    InputLangChangeRequest = 0x0050,			      //!< 
    InputLangChange = 0x0051,			              //!< 
    TCard = 0x0052,			        	              //!< 
    Held = 0x0053,			        	              //!< 
    UserChanged = 0x0054,			  	              //!< 
    NotifyFormat = 0x0055,				              //!< 
    ContextMenu = 0x007B,			  	              //!< 
    StyleChanging = 0x007C,				              //!< 
    StyleChanged = 0x007D,				              //!< 
    DisplayChange = 0x007E,				              //!< 
    GetIcon = 0x007F,			      	              //!< 
    SetIcon = 0x0080,			     	                //!< 
    NonClientCreate = 0x0081,			              //!< Formerly WM_NCCREATE
    NonClientDestroy = 0x0082,			            //!< Formerly WM_NCDESTROY
    NonClientCalcSize = 0x0083,			            //!< Formerly WM_NCCALCSIZE
    NonClientHitTest = 0x0084,			            //!< Formerly WM_NCHITTEST
    NonClientPaint = 0x0085,			              //!< Formerly WM_NCPAINT
    NonClientActivate = 0x0086,			            //!< Formerly WM_NCACTIVATE
    GetDlgCode = 0x0087,			                  //!< 
    SyncPaint = 0x0088,			                    //!< 
    NonClientMouseMove = 0x00A0,			          //!< Formerly WM_NCMOUSEMOVE
    NonClientLButtonDown = 0x00A1,			        //!< Formerly WM_NCLBUTTONDOWN
    NonClientLButtonUp = 0x00A2,			          //!< Formerly WM_NCLBUTTONUP
    NonClientLButtonDoubleClick = 0x00A3,			  //!< Formerly WM_NCLBUTTONDBLCLK
    NonClientRButtonDown = 0x00A4,			        //!< Formerly WM_NCRBUTTONDOWN
    NonClientRButtonUp = 0x00A5,			          //!< Formerly WM_NCRBUTTONUP
    NonClientRButtonDoubleClick = 0x00A6,			  //!< Formerly WM_NCRBUTTONDBLCLK
    NonClientMButtonDown = 0x00A7,			        //!< Formerly WM_NCMBUTTONDOWN
    NonClientMButtonUp = 0x00A8,			          //!< Formerly WM_NCMBUTTONUP
    NonClientMButtonDoubleClick = 0x00A9,			  //!< Formerly WM_NCMBUTTONDBLCLK
    NonClientXButtonDown = 0x00AB,			        //!< Formerly WM_NCXBUTTONDOWN
    NonClientXButtonUp = 0x00AC,			          //!< Formerly WM_NCXBUTTONUP
    NonClientXButtonDoubleClick = 0x00AD,			  //!< Formerly WM_NCXBUTTONDBLCLK
    InputDeviceChange = 0x00FE,			            //!< 
    Input = 0x00FF,			                        //!< 
    KeyFirst = 0x0100,			                    //!< 
    KeyDown = 0x0100,			                      //!< 
    KeyUp = 0x0101,			                        //!< 
    Char = 0x0102,			                        //!< 
    DeadChar = 0x0103,			                    //!< 
    SysKeyDown = 0x0104,			                  //!< 
    SysKeyUp = 0x0105,			                    //!< 
    SysChar = 0x0106,			                      //!< 
    SysDeadChar = 0x0107,			                  //!< 
    UniChar = 0x0109,			                      //!< 
    KeyLast = 0x0109,			                      //!< 
    ImeStartComposition = 0x010D,			          //!< 
    ImeEndComposition = 0x010E,			            //!< 
    ImeComposition = 0x010F,		                //!< 
    ImeKeyLast = 0x010F,			                  //!< 
    InitDialog = 0x0110,			                  //!< 
    Command = 0x0111,			                      //!< 
    SysCommand = 0x0112,			                  //!< 
    Timer = 0x0113,			                        //!< 
    HScroll = 0x0114,			                      //!< 
    VScroll = 0x0115,			                      //!< 
    InitMenu = 0x0116,			                    //!< 
    InitMenuPopup = 0x0117,			                //!< 
    
    MenuSelect = 0x011F,			                  //!< 
    MenuChar = 0x0120,			                    //!< 
    EnterIdle = 0x0121,			                    //!< 
    MenuRButtonUp = 0x0122,			                //!< 
    MenuDrag = 0x0123,			                    //!< 
    MenuGetObject = 0x0124,			                //!< 
    UnInitMenuPopup = 0x0125,			              //!< 
    MenuCommand = 0x0126,			                  //!< 
    ChangeUIState = 0x0127,			                //!< 
    UpdateUIState = 0x0128,			                //!< 
    QueryUIState = 0x0129,			                //!< 
    CtrlColourMsgBox = 0x0132,			            //!< 
    CtrlColourEdit = 0x0133,			              //!< An edit control that is not read-only or disabled sends the WM_CTLCOLOREDIT message to its parent window when the control is about to be drawn
    CtrlColourListbox = 0x0134,			            //!< 
    CtrlColourButton = 0x0135,			            //!< 
    CtrlColourDialog = 0x0136,			            //!< 
    CtrlColourScrollbar = 0x0137,			          //!< 
    CtrlColourStatic = 0x0138,			            //!< 
    GetHMenu = 0x01E1,                          //!< 
    MouseFirst = 0x0200,			                  //!< 
    MouseMove = 0x0200,			                    //!< 
    LButtonDown = 0x0201,			                  //!< 
    LButtonUp = 0x0202,			                    //!< 
    LButtonDoubleClick = 0x0203,			          //!< Formerly WM_LBUTTONDBLCLK
    RButtonDown = 0x0204,			                  //!< 
    RButtonUp = 0x0205,			                    //!< 
    RButtonDoubleClick = 0x0206,			          //!< Formerly WM_RBUTTONDBLCLK
    MButtonDown = 0x0207,			                  //!< 
    MButtonUp = 0x0208,			                    //!< 
    MButtonDoubleClick = 0x0209,			          //!< Formerly WM_MBUTTONDBLCLK
    MouseWheel = 0x020A,			                  //!< 
    XButtonDown = 0x020B,			                  //!< 
    XButtonUp = 0x020C,			                    //!< 
    XButtonDoubleClick = 0x020D,			          //!< Formerly WM_XBUTTONDBLCLK
    MouseHWheel = 0x020E,					              //!< [Windows 6.00] 
    MouseLast = MouseHWheel,				            //!< [Windows 6.00] 
    ParentNotify = 0x0210,					            //!< 
    EnterMenuLoop = 0x0211,					            //!< 
    ExitMenuLoop = 0x0212,					            //!< 
    NextMenu = 0x0213,			                    //!< 
    Sizing = 0x0214,					                  //!< 
    CaptureChanged = 0x0215,				            //!< 
    Moving = 0x0216,				            		    //!< 
    PowerBroadcast = 0x0218,					          //!< 
    DeviceChange = 0x0219,					            //!< 
    MdiCreate = 0x0220,					                //!< 
    MdiDestroy = 0x0221,					              //!< 
    MdiActivate = 0x0222,					              //!< 
    MdiRestore = 0x0223,					              //!< 
    MdiNext = 0x0224,					                  //!< 
    MdiMaximize = 0x0225,					              //!< 
    MdiTitle= 0x0226,					                  //!< 
    MdiCascase = 0x0227,					              //!< 
    MdiIconArrange = 0x0228,			            	//!< 
    MdiGetActive = 0x0229,					            //!< 
    MdiSetMenu = 0x0230,				              	//!< 
    EnterSizeMove = 0x0231,			            		//!< 
    ExitSizeMove = 0x0232,			            		//!< 
    DropFiles = 0x0233,					            		//!< 
    MdiRefreshMenu = 0x0234,			              //!< 
    
    ImeSetContext = 0x0281,				              //!< 
    ImeNotify = 0x0282,				            	    //!< 
    ImeControl = 0x0283,				            	  //!< 
    ImeCompositionFull = 0x0284,	        		  //!< 
    ImeSelect = 0x0285,				                  //!< 
    ImeChar = 0x0286,					                  //!< 
    ImeRequest = 0x0288,					              //!< 
    ImeKeyDown = 0x0290,					              //!< 
    ImeKeyUp = 0x0291,					                //!< 
    MouseHover = 0x02A1,					              //!< 
    MouseLeave = 0x02A3,					              //!< 
    NonClientMouseHover = 0x02A0,					      //!< 
    NonClientMouseLeave = 0x02A2,					      //!< 
    
    Cut = 0x0300,		                            //!< 
    Copy = 0x0301,                              //!< 
    Paste = 0x0302,                             //!< 
    Clear = 0x0303,		                          //!< 
    Undo = 0x0304,                            	//!< 
    RenderFormat = 0x0305,		              	  //!< 
    RenderAllFormats = 0x0306,	              	//!< 
    DestroyClipboard = 0x0307,	              	//!< 
    DrawClipboard = 0x0308,		              	  //!< 
    PaintClipboard = 0x0309,		              	//!< 
    VScrollClipboard = 0x030A,	              	//!< 
    SizeClipboard = 0x030B,		              	  //!< 
    AskCbFormatName = 0x030C,			              //!< 
    ChangeCbChain = 0x030D,			                //!< 
    HScrollClipboard = 0x030E,	              	//!< 
    QueryNewPalette = 0x030F,			              //!< 
    PaletteIsChanging = 0x0310,	              	//!< 
    PaletteChanged = 0x0311,			              //!< 
    HotKey = 0x0312,			                      //!< 
    Print = 0x0317,			                        //!< [Windows 4.00] 
    PrintClient = 0x0318,			                  //!< [Windows 4.00] 

#if _WIN32_WINNT >= _WIN32_WINNT_WIN2K
    AppCommand = 0x0319,			                  //!< [Windows 5.00] 
#endif

#if _WIN32_WINNT >= _WIN32_WINNT_WINXP
    WtsSession_Change = 0x02B1,			            //!< [Windows 5.01] 
    ThemeChanged = 0x031A,			                //!< [Windows 5.01] 
    ClipboardUpdate = 0x031D,			              //!< [Windows 5.01] 
#endif

#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
    DwmCompositionChanged = 0x031E,			        //!< [Windows 6.00] 
    DwmNcRenderingChanged = 0x031F,			        //!< [Windows 6.00] 
    DwmColourizationColourChanged = 0x0320,			//!< [Windows 6.00] 
    DwmWindowMaximizedChange = 0x0321,			    //!< [Windows 6.00] 
    GetTitleBarInfoEx = 0x033F,			            //!< [Windows 6.00] 
#endif

#if _WIN32_WINNT >= _WIN32_WINNT_WIN7
    Gesture = 0x0119,			                      //!< [Windows 6.01] 
    GestureNotify = 0x011A,			                //!< [Windows 6.01] 
    Touch = 0x0240,					                    //!< [Windows 6.01] 
    DwmSendIconIcThumbnail = 0x0323,			      //!< [Windows 6.01] 
    DwmSendIconICLivePreviewBitmap = 0x0326,		//!< [Windows 6.01] 
#endif

    HandHeldFirst = 0x0358,					            //!< [Windows 4.00] 
    HandHeldLast = 0x035F,					            //!< [Windows 4.00] 
    AfxFirst = 0x0360,					                //!< [Windows 4.00] 
    AfxLast = 0x037F,					                  //!< [Windows 4.00] 
    PenWinFirst = 0x0380,					              //!< 
    PenWinLast = 0x038F,					              //!< 
      
    Reflect = 0x2000,					            		  //!< [Custom] Reflection sentinel
    ReflectCommand = Reflect + Command,				  //!< [Custom] Reflected Command
    ReflectNotify = Reflect + Notify,					  //!< [Custom] Reflected Notify
    ReflectDrawItem = Reflect + DrawItem,			  //!< [Custom] Reflected DrawItem
    ReflectMeasureItem = Reflect + MeasureItem, //!< [Custom] Reflected MEASUREItem

    App = 0x8000,					            		      //!< [Windows 4.00] 
    MouseEnter,                                 //!< [Custom] Mouse entering window
    Socket,                                     //!< [Custom] Socket event
  };
  
  //! Define traits: Non-contiguous Attribute
  template <> struct is_attribute<WindowMessage>  : std::false_type {};
  template <> struct is_contiguous<WindowMessage> : std::false_type {};
  template <> struct default_t<WindowMessage>     : std::integral_constant<WindowMessage,WindowMessage::None>   {};

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::window_msg constexpr
  //! Convert integral/enumeration type into a window message enumeration
  //!
  //! \tparam VALUE - Integral or enumeration type
  //! 
  //! \param[in] msg - Value representing a window message
  //! \return WindowMessage - WindowMessage representation of 'msg'
  /////////////////////////////////////////////////////////////////////////////////////////
  //template <typename VALUE, typename = enable_if_numeric_t<VALUE>> constexpr
  //WindowMessage  window_msg(VALUE msg) 
  //{
  //  // Convert into underlying type then cast to enumeration
  //  return enum_cast<WindowMessage>( static_cast<std::underlying_type_t<WindowMessage>>(msg) );
  //}

  
  //! \enum NotifyMessage - Standard common control notifications
  enum class NotifyMessage : int16_t
  {
    First = NM_FIRST,
    OutOfMemory = (First - 1),			        //!< Control has run out of memory
    Click = (First - 2),			              //!< Control has been left clicked                (Uses ::NMCLICK)
    DblClk = (First - 3),			              //!< Control has been left double-clicked
    Return = (First - 4),			              //!< Control has received ENTER keypress
    RClick = (First - 5),			              //!< Control has been right clicked               (Uses ::NMCLICK)
    RDblClk = (First - 6),			            //!< Control has been right double-clicked
    SetFocus = (First - 7),			            //!< Control has received the input focus
    KillFocus = (First - 8),			          //!< Control has lost the input focus
    CustomDraw = (First - 12),			        //!< Control requesting custom drawing
    Hover = (First - 13),			              //!< Control is being hovered over
    NcHitTest = (First - 14),			          //!< Sent by Rebars in response to WM_NCHITTEST   (Uses ::NMMOUSE)
    KeyDown = (First - 15),			            //!< Control has received keypress                (Uses ::NMKEY)
    ReleasedCapture = (First - 16),	        //!< Control has released mouse capture
    SetCursor = (First - 17),			          //!< Control has set the cursor                   (Uses ::NMMOUSE)
    Char = (First - 18),			              //!< Control has received character press         (Uses ::NMCHAR)
    TooltipsCreated = (First - 19),	        //!< Notify when the tooltips window is create
    LDown = (First - 20),			              //!< Control has been left clicked 
    RDown = (First - 21),			              //!< Not supported
    ThemeChanged = (First - 22),		        //!< Sent when user theme changes

#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
    FontChanged = (First - 23),			        //!< [Windows 6.00] Control font has changed
    CustomText = (First - 24),			        //!< [Windows 6.00] Control performed custom text operation  (Uses ::NMCUSTOMTEXT)
    TvStateImageChanging = (First - 24),		//!< [Windows 6.00] TreeView state image is changing         (Uses ::NMTVStateImageChanging)
#endif
  };
  
  
  //! Define traits: Non-contiguous Attribute
  template <> struct is_attribute<NotifyMessage>  : std::true_type  {};
  template <> struct is_contiguous<NotifyMessage> : std::false_type {};
  template <> struct default_t<NotifyMessage>     : std::integral_constant<NotifyMessage,NotifyMessage::First>   {};


} //namespace wtl
#endif // WTL_WINDOW_MESSAGE_HPP

