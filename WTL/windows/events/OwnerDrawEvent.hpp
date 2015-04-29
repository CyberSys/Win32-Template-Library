////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\event\OwnerDrawEvent.hpp
//! \brief Encapsulates the WM_DRAWITEM message in the 'OwnerDraw' event
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_DRAWITEM_WINDOW_EVENT_HPP
#define WTL_DRAWITEM_WINDOW_EVENT_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<WindowMessage::DRAWITEM> - Event arguments for Win32 message 'WM_DRAWITEM'
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::DRAWITEM>
  {  
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;

    //! \alias PaintData - Define paint data type
    using PaintData = ::DRAWITEMSTRUCT;
    
    //! \var encoding - Define character encoding
    static constexpr Encoding encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage message = WindowMessage::DRAWITEM;
    
    // -------------------- REPRESENTATION ---------------------
  protected:
    PaintData&        Data;             //!< Encapsulates the input paint data (NB: Must be initialized before following fields)

  public:
    DeviceContext     Graphics;         //!< Device context clipped to non-client area 
    RectL             Rect;             //!< Drawing/update rectangle
    int32             Item;             //!< Zero-based item index
    WindowId          Ident;            //!< Control id
    OwnerDrawAction   Action;           //!< Type of drawing requested
    OwnerDrawControl  CtrlType;         //!< Control type
    HWnd              Ctrl;             //!< [CONTROL] Control handle
    HMenu             Menu;             //!< [MENU]    Menu handle

    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs<DRAWITEM>::EventArgs
    //! Creates arguments for 'OwnerDraw' Event (ie. WM_DRAWITEM)
    //! 
    //! \param[in] w - Not used
    //! \param[in] l - Not used
    ///////////////////////////////////////////////////////////////////////////////
    EventArgs(::WPARAM w, ::LPARAM l) : Data(*opaque_cast<PaintData>(l)), 
                                        Graphics(Data.hDC),
                                        Rect(Data.rcItem),
                                        Item(Data.itemID), 
                                        Ident(static_cast<WindowId>(Data.CtlID)), 
                                        Action(enum_cast<OwnerDrawAction>(Data.itemAction)), 
                                        CtrlType(enum_cast<OwnerDrawControl>(Data.CtlType)), 
                                        Ctrl(CtrlType != OwnerDrawControl::Menu ? Data.hwndItem : handle_alloc<::HWND>::npos, AllocType::WeakRef), 
                                        Menu(CtrlType == OwnerDrawControl::Menu ? (HMENU)Data.hwndItem : handle_alloc<::HMENU>::npos, AllocType::WeakRef)
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs::~EventArgs
    //! Ends the paint cycle
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~EventArgs()
    {}
    
    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			
    
    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs<DRAWITEM>::reflect const
    //! Reflects the event back to the originator control
    //! 
    //! \return LResult - Message result and routing
    ///////////////////////////////////////////////////////////////////////////////
    LResult reflect() const
    {
      // Reflect message
      return send_message<encoding,message+WindowMessage::REFLECT>(Ctrl, opaque_cast(Ident), opaque_cast(Data));
    }
    
    // ----------------------- MUTATORS ------------------------
  };
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerDrawEvent - Defines 'OwnerDraw' event (ie. WM_DRAWITEM)
  //! 
  //! \tparam ENC - Window character encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerDrawEvent = MessageEvent<ENC, WindowMessage::DRAWITEM>;
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerDrawEventArgs - Arguments for 'OwnerDraw' Event (ie. WM_DRAWITEM)
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerDrawEventArgs = EventArgs<ENC,WindowMessage::DRAWITEM>;

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerDrawEventHandler - Handler for 'OwnerDraw' event (ie. WM_DRAWITEM)
  //! 
  //! \tparam ENC - Window character encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerDrawEventHandler = typename OwnerDrawEvent<ENC>::delegate_t;

}

#endif // WTL_DRAWITEM_WINDOW_EVENT_HPP
