//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\event\OwnerDrawCtrlEvent.hpp
//! \brief Encapsulates the WM_DRAWITEM message (when sent for controls) in the 'OwnerDrawCtrl' event.
//! \remarks Notice that a separate event, 'OwnerDrawMenuEvent' handles WM_DRAWITEM messages sent for menus
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_OWNER_DRAW_CONTROL_EVENT_HPP
#define WTL_OWNER_DRAW_CONTROL_EVENT_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/EnumCast.hpp>               //!< EnumCast
#include <wtl/casts/OpaqueCast.hpp>             //!< OpaqueCast
#include <wtl/utils/Handle.hpp>                 //!< Handle
#include <wtl/utils/Default.hpp>                //!< Default
#include <wtl/gdi/DeviceContext.hpp>            //!< DeviceContext
#include <wtl/windows/ControlEventArgs.hpp>     //!< ControlEventArgs
#include <wtl/utils/Rectangle.hpp>              //!< Rect
#include <wtl/platform/CommonApi.hpp>           //!< send_message

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ControlEventArgs<WindowMessage::DrawItem> - Event arguments for Win32 message 'WM_DRAWITEM' raised from controls
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ControlEventArgs<ENC,WindowMessage::DrawItem,uint16_t,defvalue<uint16_t>()>
  {  
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;

    //! \alias PaintData - Define paint data type
    using PaintData = ::DRAWITEMSTRUCT;
    
    //! \var encoding - Define character encoding
    static constexpr Encoding encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage message = WindowMessage::DrawItem;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    PaintData&        Data;             //!< Encapsulates the input paint data (NB: Must be initialized before following fields)

  public:
    OwnerDrawAction   Action;           //!< Type of drawing requested
    OwnerDrawControl  CtrlType;         //!< Control type
    DeviceContext     Graphics;         //!< Device context clipped to non-client area 
    WindowId          Ident;            //!< Control id
    int32_t           Item;             //!< Zero-based item index
    RectL             Rect;             //!< Drawing/update rectangle
    HWnd              Sender;           //!< Control handle
    OwnerDrawState    State;            //!< State of item being drawn
    
    // ------------------------------------- CONSTRUCTION -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // ControlEventArgs<DRAWITEM>::ControlEventArgs
    //! Creates arguments for 'OwnerDraw' event from controls (ie. WM_DRAWITEM)
    //! 
    //! \param[in] w - Originator window id
    //! \param[in] l - Paint data
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Event is not for a control
    /////////////////////////////////////////////////////////////////////////////////////////
    ControlEventArgs(::WPARAM w, ::LPARAM l) : Data(*opaque_cast<PaintData>(l)), 
                                               Action(enum_cast<OwnerDrawAction>(Data.itemAction)), 
                                               CtrlType(enum_cast<OwnerDrawControl>(Data.CtlType)), 
                                               Graphics(Data.hDC),
                                               Rect(Data.rcItem),
                                               Item(Data.itemID), 
                                               Ident(static_cast<WindowId>(Data.CtlID)), 
                                               Sender(Data.hwndItem, AllocType::WeakRef),
                                               State(static_cast<OwnerDrawState>(Data.itemState))
    {
      PARAM_INVARIANT(Data,enum_cast<OwnerDrawControl>(Data.CtlType) != OwnerDrawControl::Menu);
    }

	  // -------------------------------- COPY, MOVE & DESTROY  -------------------------------
  
    DISABLE_COPY(ControlEventArgs);     //!< DeviceContext is move-only type
    ENABLE_MOVE(ControlEventArgs);      //!< Can be moved
    ENABLE_POLY(ControlEventArgs);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ControlEventArgs<DRAWITEM>::reflect const
    //! Reflects the event back to the originator control
    //! 
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult reflect() const
    {
      // Reflect message
      return send_message<encoding,message+WindowMessage::Reflect>(Sender, opaque_cast(Ident), opaque_cast(Data));
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerDrawCtrlEventArgs - Defines arguments type for the 'OwnerDrawCtrl' event 
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerDrawCtrlEventArgs = ControlEventArgs<ENC,WindowMessage::DrawItem>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerDrawCtrlEvent - Defines the signature of 'OwnerDrawCtrl' event handlers  [Pass by value]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerDrawCtrlEvent = Event<LResult,OwnerDrawCtrlEventArgs<ENC>&>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerDrawCtrlEventHandler - Defines the delegate type for the 'OwnerDrawCtrl' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerDrawCtrlEventHandler = handler_t<OwnerDrawCtrlEvent<ENC>>;
  
} // namespace wtl

#endif // WTL_OWNER_DRAW_CONTROL_EVENT_HPP
