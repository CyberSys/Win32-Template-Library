//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\WindowMessage.hpp
//! \brief Defines window messages
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_MESSAGE_HPP
#define WTL_WINDOW_MESSAGE_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \enum WindowMessage - Windows message identifiers
  enum class WindowMessage : uint16_t
  {
    NONE = 0x0000,			//!< WM_NULL
    CREATE = 0x0001,			//!< 
    DESTROY = 0x0002,			//!< 
    MOVE = 0x0003,			//!< 
    SIZE = 0x0005,			//!< 

    ACTIVATE = 0x0006,			//!< 

    SETFOCUS = 0x0007,			//!< 
    KILLFOCUS = 0x0008,			//!< 
    ENABLE = 0x000A,			//!< 
    SETREDRAW = 0x000B,			//!< 
    SETTEXT = 0x000C,			//!< 
    GETTEXT = 0x000D,			//!< 
    GETTEXTLENGTH = 0x000E,			//!< 
    PAINT = 0x000F,			//!< 
    CLOSE = 0x0010,			//!< 
    QUERYENDSESSION = 0x0011,			//!< 
    QUERYOPEN = 0x0013,			//!< 
    ENDSESSION = 0x0016,			//!< 
    QUIT = 0x0012,			//!< 
    ERASEBKGND = 0x0014,			//!< 
    SYSCOLORCHANGE = 0x0015,			//!< 
    SHOWWINDOW = 0x0018,			//!< 
    SETTINGCHANGE = 0x001A,			//!< 
    DEVMODECHANGE = 0x001B,			//!< 
    ACTIVATEAPP = 0x001C,			//!< 
    FONTCHANGE = 0x001D,			//!< 
    TIMECHANGE = 0x001E,			//!< 
    CANCELMODE = 0x001F,			//!< 
    SETCURSOR = 0x0020,			//!< 
    MOUSEACTIVATE = 0x0021,			//!< 
    CHILDACTIVATE = 0x0022,			//!< 
    QUEUESYNC = 0x0023,			//!< 
    GETMINMAXINFO = 0x0024,			//!< 
    PAINTICON = 0x0026,			//!< 
    ICONERASEBKGND = 0x0027,			//!< 
    NEXTDLGCTL = 0x0028,			//!< 
    SPOOLERSTATUS = 0x002A,			//!< 
    DRAWITEM = 0x002B,			//!< 
    MEASUREITEM = 0x002C,			//!< 
    DELETEITEM = 0x002D,			//!< 
    VKEYTOITEM = 0x002E,			//!< 
    CHARTOITEM = 0x002F,			//!< 
    SETFONT = 0x0030,			//!< 
    GETFONT = 0x0031,			//!< 
    SETHOTKEY = 0x0032,			//!< 
    GETHOTKEY = 0x0033,			//!< 
    QUERYDRAGICON = 0x0037,			//!< 
    COMPAREITEM = 0x0039,			//!< 
    GETOBJECT = 0x003D,			//!< 
    COMPACTING = 0x0041,			//!< 
    COMMNOTIFY = 0x0044,			//!<   /* no longer suported */
    WINDOWPOSCHANGING = 0x0046,			//!< 
    WINDOWPOSCHANGED = 0x0047,			//!< 

    POWER = 0x0048,			//!< 

    NOTIFY = 0x004E,			//!< 
    INPUTLANGCHANGEREQUEST = 0x0050,			//!< 
    INPUTLANGCHANGE = 0x0051,			//!< 
    TCARD = 0x0052,			//!< 
    HELP = 0x0053,			//!< 
    USERCHANGED = 0x0054,			//!< 
    NOTIFYFORMAT = 0x0055,			//!< 

    CONTEXTMENU = 0x007B,			//!< 
    STYLECHANGING = 0x007C,			//!< 
    STYLECHANGED = 0x007D,			//!< 
    DISPLAYCHANGE = 0x007E,			//!< 
    GETICON = 0x007F,			//!< 
    SETICON = 0x0080,			//!< 

    NCCREATE = 0x0081,			//!< 
    NCDESTROY = 0x0082,			//!< 
    NCCALCSIZE = 0x0083,			//!< 
    NCHITTEST = 0x0084,			//!< 
    NCPAINT = 0x0085,			//!< 
    NCACTIVATE = 0x0086,			//!< 
    GETDLGCODE = 0x0087,			//!< 
    SYNCPAINT = 0x0088,			//!< 
    NCMOUSEMOVE = 0x00A0,			//!< 
    NCLBUTTONDOWN = 0x00A1,			//!< 
    NCLBUTTONUP = 0x00A2,			//!< 
    NCLBUTTONDBLCLK = 0x00A3,			//!< 
    NCRBUTTONDOWN = 0x00A4,			//!< 
    NCRBUTTONUP = 0x00A5,			//!< 
    NCRBUTTONDBLCLK = 0x00A6,			//!< 
    NCMBUTTONDOWN = 0x00A7,			//!< 
    NCMBUTTONUP = 0x00A8,			//!< 
    NCMBUTTONDBLCLK = 0x00A9,			//!< 
    NCXBUTTONDOWN = 0x00AB,			//!< 
    NCXBUTTONUP = 0x00AC,			//!< 
    NCXBUTTONDBLCLK = 0x00AD,			//!< 

    INPUT_DEVICE_CHANGE = 0x00FE,			//!< 
    INPUT = 0x00FF,			//!< 

    KEYFIRST = 0x0100,			//!< 
    KEYDOWN = 0x0100,			//!< 
    KEYUP = 0x0101,			//!< 
    CHAR = 0x0102,			//!< 
    DEADCHAR = 0x0103,			//!< 
    SYSKEYDOWN = 0x0104,			//!< 
    SYSKEYUP = 0x0105,			//!< 
    SYSCHAR = 0x0106,			//!< 
    SYSDEADCHAR = 0x0107,			//!< 
    UNICHAR = 0x0109,			//!< 
    KEYLAST = 0x0109,			//!< 

    IME_STARTCOMPOSITION = 0x010D,			//!< 
    IME_ENDCOMPOSITION = 0x010E,			//!< 
    IME_COMPOSITION = 0x010F,			//!< 
    IME_KEYLAST = 0x010F,			//!< 

    INITDIALOG = 0x0110,			//!< 
    COMMAND = 0x0111,			//!< 
    SYSCOMMAND = 0x0112,			//!< 
    TIMER = 0x0113,			//!< 
    HSCROLL = 0x0114,			//!< 
    VSCROLL = 0x0115,			//!< 
    INITMENU = 0x0116,			//!< 
    INITMENUPOPUP = 0x0117,			//!< 

    GESTURE = 0x0119,			//!< [Windows 6.01] 
    GESTURENOTIFY = 0x011A,			//!< [Windows 6.01] 

    MENUSELECT = 0x011F,			//!< 
    MENUCHAR = 0x0120,			//!< 
    ENTERIDLE = 0x0121,			//!< 

    MENURBUTTONUP = 0x0122,			//!< 
    MENUDRAG = 0x0123,			//!< 
    MENUGETOBJECT = 0x0124,			//!< 
    UNINITMENUPOPUP = 0x0125,			//!< 
    MENUCOMMAND = 0x0126,			//!< 

    CHANGEUISTATE = 0x0127,			//!< 
    UPDATEUISTATE = 0x0128,			//!< 
    QUERYUISTATE = 0x0129,			//!< 

    CTLCOLORMSGBOX = 0x0132,			//!< 
    CTLCOLOREDIT = 0x0133,			//!< 
    CTLCOLORLISTBOX = 0x0134,			//!< 
    CTLCOLORBTN = 0x0135,			//!< 
    CTLCOLORDLG = 0x0136,			//!< 
    CTLCOLORSCROLLBAR = 0x0137,			//!< 
    CTLCOLORSTATIC = 0x0138,			//!< 
    GETHMENU = 0x01E1,         //!< 

    MOUSEFIRST = 0x0200,			//!< 
    MOUSEMOVE = 0x0200,			//!< 
    LBUTTONDOWN = 0x0201,			//!< 
    LBUTTONUP = 0x0202,			//!< 
    LBUTTONDBLCLK = 0x0203,			//!< 
    RBUTTONDOWN = 0x0204,			//!< 
    RBUTTONUP = 0x0205,			//!< 
    RBUTTONDBLCLK = 0x0206,			//!< 
    MBUTTONDOWN = 0x0207,			//!< 
    MBUTTONUP = 0x0208,			//!< 
    MBUTTONDBLCLK = 0x0209,			//!< 

    MOUSEWHEEL = 0x020A,			//!< 

    XBUTTONDOWN = 0x020B,			//!< 
    XBUTTONUP = 0x020C,			//!< 
    XBUTTONDBLCLK = 0x020D,			//!< 

    MOUSEHWHEEL = 0x020E,					              //!< [Windows 6.00] 
    MOUSELAST = MOUSEHWHEEL,				            //!< [Windows 6.00] 

    PARENTNOTIFY = 0x0210,					            //!< 
    ENTERMENULOOP = 0x0211,					            //!< 
    EXITMENULOOP = 0x0212,					            //!< 
    NEXTMENU = 0x0213,			                    //!< 
    SIZING = 0x0214,					                  //!< 
    CAPTURECHANGED = 0x0215,				            //!< 
    MOVING = 0x0216,				            		    //!< 

    POWERBROADCAST = 0x0218,					          //!< 
    DEVICECHANGE = 0x0219,					            //!< 

    MDICREATE = 0x0220,					                //!< 
    MDIDESTROY = 0x0221,					              //!< 
    MDIACTIVATE = 0x0222,					              //!< 
    MDIRESTORE = 0x0223,					              //!< 
    MDINEXT = 0x0224,					                  //!< 
    MDIMAXIMIZE = 0x0225,					              //!< 
    MDITILE = 0x0226,					                  //!< 
    MDICASCADE = 0x0227,					              //!< 
    MDIICONARRANGE = 0x0228,			            	//!< 
    MDIGETACTIVE = 0x0229,					            //!< 
    MDISETMENU = 0x0230,				              	//!< 

    ENTERSIZEMOVE = 0x0231,			            		//!< 
    EXITSIZEMOVE = 0x0232,			            		//!< 
    DROPFILES = 0x0233,					            		//!< 
    MDIREFRESHMENU = 0x0234,			              //!< 
    TOUCH = 0x0240,					                    //!< [Windows 6.01] 
    
    IME_SETCONTEXT = 0x0281,				            //!< 
    IME_NOTIFY = 0x0282,				            	  //!< 
    IME_CONTROL = 0x0283,				            	  //!< 
    IME_COMPOSITIONFULL = 0x0284,	        		  //!< 
    IME_SELECT = 0x0285,				                //!< 
    IME_CHAR = 0x0286,					                //!< 
    IME_REQUEST = 0x0288,					              //!< 
    IME_KEYDOWN = 0x0290,					              //!< 
    IME_KEYUP = 0x0291,					                //!< 

    MOUSEHOVER = 0x02A1,					              //!< 
    MOUSELEAVE = 0x02A3,					              //!< 
    NCMOUSEHOVER = 0x02A0,					            //!< 
    NCMOUSELEAVE = 0x02A2,					            //!< 

    WTSSESSION_CHANGE = 0x02B1,			            //!< [Windows 5.01] 

    CUT = 0x0300,		                            //!< 
    COPY = 0x0301,                              //!< 
    PASTE = 0x0302,                             //!< 
    CLEAR = 0x0303,		                          //!< 
    UNDO = 0x0304,                            	//!< 

    RENDERFORMAT = 0x0305,		              	  //!< 
    RENDERALLFORMATS = 0x0306,	              	//!< 
    DESTROYCLIPBOARD = 0x0307,	              	//!< 
    DRAWCLIPBOARD = 0x0308,		              	  //!< 
    PAINTCLIPBOARD = 0x0309,		              	//!< 
    VSCROLLCLIPBOARD = 0x030A,	              	//!< 
    SIZECLIPBOARD = 0x030B,		              	  //!< 
    ASKCBFORMATNAME = 0x030C,			              //!< 
    CHANGECBCHAIN = 0x030D,			                //!< 
    HSCROLLCLIPBOARD = 0x030E,	              	//!< 
    QUERYNEWPALETTE = 0x030F,			              //!< 
    PALETTEISCHANGING = 0x0310,	              	//!< 
    PALETTECHANGED = 0x0311,			              //!< 
    HOTKEY = 0x0312,			                      //!< 

    PRINT = 0x0317,			                        //!< 
    PRINTCLIENT = 0x0318,			                  //!< 

    APPCOMMAND = 0x0319,			                  //!< [Windows 5.00] 
    THEMECHANGED = 0x031A,			                //!< [Windows 5.01] 
    CLIPBOARDUPDATE = 0x031D,			              //!< [Windows 5.01] 

    DWMCOMPOSITIONCHANGED = 0x031E,			        //!< [Windows 6.00] 
    DWMNCRENDERINGCHANGED = 0x031F,			        //!< [Windows 6.00] 
    DWMCOLORIZATIONCOLORCHANGED = 0x0320,			  //!< [Windows 6.00] 
    DWMWINDOWMAXIMIZEDCHANGE = 0x0321,			    //!< [Windows 6.00] 
    DWMSENDICONICTHUMBNAIL = 0x0323,			      //!< [Windows 6.01] 
    DWMSENDICONICLIVEPREVIEWBITMAP = 0x0326,		//!< [Windows 6.01] 
    GETTITLEBARINFOEX = 0x033F,			            //!< [Windows 6.00] 
    HANDHELDFIRST = 0x0358,					            //!< 
    HANDHELDLAST = 0x035F,					            //!< 
    AFXFIRST = 0x0360,					                //!< 
    AFXLAST = 0x037F,					                  //!< 
    PENWINFIRST = 0x0380,					              //!< 
    PENWINLAST = 0x038F,					              //!< 

    REFLECT = 0x2000,					            		  //!< [Custom] Reflection sentinel
    REFLECT_COMMAND = REFLECT + COMMAND,				//!< [Custom] Reflected COMMAND
    REFLECT_NOTIFY = REFLECT + NOTIFY,					//!< [Custom] Reflected NOTIFY
    REFLECT_DRAWITEM = REFLECT + DRAWITEM,			//!< [Custom] Reflected DRAWITEM
    REFLECT_MEASUREITEM = REFLECT + MEASUREITEM,//!< [Custom] Reflected MEASUREITEM

    APP = 0x8000,					            		      //!< 
  };
  
  //! Define traits: Non-contiguous Attribute
  template <> struct is_attribute<WindowMessage>  : std::false_type {};
  template <> struct is_contiguous<WindowMessage> : std::false_type {};
  template <> struct default_t<WindowMessage>     : std::integral_constant<WindowMessage,WindowMessage::NONE>   {};

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::window_msg constexpr
  //! Convert integral/enumeration type into a window message enumeration
  //!
  //! \tparam VALUE - Integral or enumeration type
  //! 
  //! \param[in] msg - Value representing a window message
  //! \return WindowMessage - WindowMessage representation of 'msg'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE, typename = enable_if_numeric_t<VALUE>> constexpr
  WindowMessage  window_msg(VALUE msg) 
  {
    // Convert into underlying type then cast to enumeration
    return enum_cast<WindowMessage>( static_cast<std::underlying_type_t<WindowMessage>>(msg) );
  }


  //! \enum MsgRoute - Defines how a message was handled
  enum class MsgRoute
  {
    Handled,      //!< Window handled message
    Reflected,    //!< Window reflected command/notification to child window
    Unhandled,    //!< Window ignored message
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct unhandled_result - Defines the 'Unhandled' result value for any window message
  //! 
  //! \tparam WM - Window message identifier
  /////////////////////////////////////////////////////////////////////////////////////////
  template <WindowMessage WM> 
  struct unhandled_result //: std::integral_constant<LRESULT,static_cast<LRESULT>(-1)> 
  {
    static constexpr ::LRESULT value = -1; 
  };

  /*template <WindowMessage WM> 
  struct unhandled_result<WM>::value = -1;*/

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct MsgResult - Encapsulates the result of any windows message 
  //! 
  //! \tparam RESULT - Message result type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename RESULT>
  struct MsgResult
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = MsgResult<RESULT>;
  
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    MsgRoute  Route;        //!< Message routing
    RESULT    Result;       //!< Message result

    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    /////////////////////////////////////////////////////////////////////////////////////////
    //! MsgResult::MsgResult
    //! Create unhandled result
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr 
    MsgResult() noexcept : Route(MsgRoute::Unhandled), Result(-1)
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    //! MsgResult::MsgResult
    //! Create handled result
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr 
    MsgResult(RESULT res) noexcept : Route(MsgRoute::Handled), Result(res)
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    //! MsgResult::MsgResult
    //! Create reflected result
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr 
    MsgResult(MsgRoute route, RESULT res = -1) noexcept : Route(route), Result(res)
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    CONSTEXPR_COPY_CTOR(MsgResult);      //!< Can be deep copied at compile-time
    CONSTEXPR_MOVE_CTOR(MsgResult);      //!< Can be moved at compile-time
    ENABLE_COPY_ASSIGN(MsgResult);       //!< Can be assigned
    ENABLE_MOVE_ASSIGN(MsgResult);       //!< Can be move-assigned
    DISABLE_POLY(MsgResult);             //!< Cannot be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
  };

  //! \alias LResult - Default window message return type
  using LResult = MsgResult<LRESULT>;
  

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct unhandled - Encapsulates unhandled message results
  //! 
  //! \tparam WM - Window message
  /////////////////////////////////////////////////////////////////////////////////////////
  template <WindowMessage WM> 
  struct unhandled
  {
    //! \var value - 'Unhandled' window message sentinel result 
    static const LResult value;  
  };

  
  //! \var unhandled<WM>::value - 'Unhandled' window message sentinel result 
  template <WindowMessage WM> 
  const LResult  unhandled<WM>::value = LResult(MsgRoute::Unhandled, unhandled_result<WM>::value);
  
} //namespace wtl
#endif // WTL_WINDOW_MESSAGE_HPP

