////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\MessageHandler.hpp
//! \brief Encapsulates a collection of event handlers
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MESSAGE_HANDLER_HPP
#define WTL_MESSAGE_HANDLER_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  
  //! \struct Window - Window class
  template <Encoding ENC>
  struct IEventHandlers
  {
    // ------------------- TYPES & CONSTANTS -------------------
    
    //! Define convenience typedefs for standard event handlers
    using CreateWindowEventHandler_t = CreateWindowEventHandler<ENC>;
    using CreateWindowEventArgs_t = CreateWindowEventArgs<ENC>;
    using DestroyWindowEventHandler_t = DestroyWindowEventHandler<ENC>;
    using ShowWindowEventHandler_t = ShowWindowEventHandler<ENC>;
    using ShowWindowEventArgs_t = ShowWindowEventArgs<ENC>;
    using PaintWindowEventHandler_t = PaintWindowEventHandler<ENC>;
    using PaintWindowEventArgs_t = PaintWindowEventArgs<ENC>;

    //! \using Button_t - Define button type
    using Button_t = Button<ENC>;

    template <typename SENDER>
    using ButtonClickEventHandler_t = ButtonClickEventHandler<ENC,SENDER>;
  };



// Previous implementation
#if STALE_CODE
    virtual LResult  onInitDialog()                                                            { return { MsgRoute::Unhandled, -1 }; };
      
     Commands/Notification
    virtual LResult  onCommand(window_t* child, int16 msg)                                     { return unhandled<WindowMessage::COMMAND>::value; }
    virtual LResult  onNotify(window_t* child, int16 msg, NMHDR* header)                       { return unhandled<WindowMessage::NOTIFY>::value;  } 

    // Reflection
    virtual LResult  onCommandReflect(int16 msg)                                               { return unhandled<WindowMessage::COMMAND>::value; }
    virtual LResult  onNotifyReflect(int16 msg, NMHDR* header)                                 { return unhandled<WindowMessage::NOTIFY>::value;  }

    static  LResult  getInitialSize(::MINMAXINFO* info)                                        { return unhandled<WindowMessage::GETMINMAXINFO>::value;     }
    virtual LResult  onGetMinMaxInfo(::MINMAXINFO* info)                                       { return unhandled<WindowMessage::GETMINMAXINFO>::value;     }

    // General
    virtual LResult  onKillFocus(HWND  hNextWnd)                                               { return unhandled<WindowMessage::KILLFOCUS>::value; }
    virtual LResult  onSetFocus(HWND  hPrevWnd)                                                { return unhandled<WindowMessage::SETFOCUS>::value;  }
    virtual LResult  onTimer(const uint32  iID)                                                { return unhandled<WindowMessage::TIMER>::value;     }
    virtual LResult  onHorizontalScroll(const uint32  eType, const uint32  iPos, HWND  hCtrl)  { return unhandled<WindowMessage::HSCROLL>::value;   }
    virtual LResult  onVerticalScroll(const uint32  eType, const uint32  iPos, HWND  hCtrl)    { return unhandled<WindowMessage::VSCROLL>::value;   }
    virtual LResult  onSize(const uint32  eType, const uint32  iWidth, const uint32  iHeight)  { return unhandled<WindowMessage::SIZE>::value;      }

    // Keyboard
    virtual LResult  onKeyCharacter(const TCHAR  chChar)                                       { return unhandled<WindowMessage::CHAR>::value;      }
    virtual LResult  onKeyDown(const uint32  iVirtKey)                                         { return unhandled<WindowMessage::KEYDOWN>::value;   }
    virtual LResult  onKeyUp(const uint32  iVirtKey)                                           { return unhandled<WindowMessage::KEYUP>::value;     }

    // Mouse
    virtual LResult  onMouseHover(Point  oPoint, const uint32  iFlags)                         { return { MsgRoute::Unhandled }; };
    virtual LResult  onMouseMove(Point  oPoint, const uint32  iFlags)                          { return { MsgRoute::Unhandled }; };
    virtual LResult  onMouseLeave()                                                            { return { MsgRoute::Unhandled }; };

    virtual LResult  onMouseLeftClick(const uint32  iControlID, NMHDR*  pHeader)             { return { MsgRoute::Unhandled }; };
    virtual LResult  onMouseLeftDoubleClick(Point  oPoint, const uint32  iFlags)             { return { MsgRoute::Unhandled }; };
    virtual LResult  onMouseLeftDoubleClick(const uint32  iControlID, NMHDR*  pHeader)       { return { MsgRoute::Unhandled }; };
    virtual LResult  onMouseLeftDown(Point  oPoint, const uint32  iFlags)                    { return { MsgRoute::Unhandled }; };
    virtual LResult  onMouseLeftUp(Point  oPoint, const uint32  iFlags)                      { return { MsgRoute::Unhandled }; };

    virtual LResult  onMouseRightClick(const uint32  iControlID, NMHDR*  pHeader)            { return { MsgRoute::Unhandled }; };
    virtual LResult  onMouseRightDoubleClick(Point  oPoint, const uint32  iFlags)            { return { MsgRoute::Unhandled }; };
    virtual LResult  onMouseRightDoubleClick(const uint32  iControlID, NMHDR*  pHeader)      { return { MsgRoute::Unhandled }; };
    virtual LResult  onMouseRightDown(Point  oPoint, const uint32  iFlags)                   { return { MsgRoute::Unhandled }; };
    virtual LResult  onMouseRightUp(Point  oPoint, const uint32  iFlags)                     { return { MsgRoute::Unhandled }; };

    // Drag n Drop
    virtual LResult  onMouseLeftDrag(BaseWindow*  pSource, DropData*  pData)               { return { MsgRoute::Unhandled }; };
    virtual LResult  onMouseLeftDrop(BaseWindow*  pSource, DropData*  pData)               { return { MsgRoute::Unhandled }; };
    virtual LResult  onMouseRightDrag(BaseWindow*  pSource, DropData*  pData)              { return { MsgRoute::Unhandled }; };
    virtual LResult  onMouseRightDrop(BaseWindow*  pSource, DropData*  pData)              { return { MsgRoute::Unhandled }; };

    // Drawing
    virtual LResult  onCustomDraw(const uint32  iControlID, NMCUSTOMDRAW*  pHeader)          { return { MsgRoute::Unhandled }; };
    virtual LResult  onOwnerDraw(const uint32  iControlID, DRAWITEMSTRUCT*  pDrawItem);
    virtual LResult  onEraseBackground(HDC  hDC)                                           { return { MsgRoute::Unhandled }; };
    virtual LResult  onPaint()                                                             { return { MsgRoute::Unhandled }; };

    // Custom Message Handlers
    virtual LResult  onUserMessage(uint32  iMessage, WPARAM  wParam, LPARAM  lParam)         { return { MsgRoute::Unhandled }; };
    virtual LResult  onUnhandledMessage(uint32  iMessage, WPARAM  wParam, LPARAM  lParam)    { return { MsgRoute::Unhandled }; };
    virtual LResult  onSubclassed()                                                        { return { MsgRoute::Unhandled }; };
#endif
  
  
}

#endif // WTL_MESSAGE_HANDLER_HPP
