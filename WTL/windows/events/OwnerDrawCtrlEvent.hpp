//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\event\OwnerDrawCtrlEvent.hpp
//! \brief Encapsulates the WM_DRAWITEM message sent for controls in the 'OwnerDrawCtrl' event
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_OWNER_DRAW_CONTROL_EVENT_HPP
#define WTL_OWNER_DRAW_CONTROL_EVENT_HPP

#include "wtl/WTL.hpp"
#include "wtl/casts/EnumCast.hpp"               //!< EnumCast
#include "wtl/casts/OpaqueCast.hpp"             //!< OpaqueCast
#include "wtl/utils/Handle.hpp"                 //!< Handle
#include "wtl/utils/Default.hpp"                //!< Default
#include "wtl/gdi/DeviceContext.hpp"            //!< DeviceContext
#include "wtl/windows/ControlEvent.hpp"         //!< ControlEventArgs
#include "wtl/windows/MessageEvent.hpp"         //!< MessageEvent
#include "wtl/utils/Rectangle.hpp"           //!< Rect
#include "wtl/platform/CommonApi.hpp"           //!< send_message

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ControlEventArgs<WindowMessage::DRAWITEM> - Event arguments for Win32 message 'WM_DRAWITEM' raised from controls
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ControlEventArgs<ENC,WindowMessage::DRAWITEM,uint16_t,default<uint16_t>()>
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
    static constexpr WindowMessage message = WindowMessage::DRAWITEM;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    PaintData&        Data;             //!< Encapsulates the input paint data (NB: Must be initialized before following fields)

  public:
    OwnerDrawAction   Action;           //!< Type of drawing requested
    OwnerDrawControl  CtrlType;         //!< Control type
    DeviceContext     Graphics;         //!< Device context clipped to non-client area 
    WindowId          Ident;            //!< Control id
    int32_t             Item;             //!< Zero-based item index
    RectL             Rect;             //!< Drawing/update rectangle
    HWnd              Sender;           //!< Control handle
    
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
                                               Sender(Data.hwndItem, AllocType::WeakRef)
    {
      PARAM_INVARIANT(Data,enum_cast<OwnerDrawControl>(Data.CtlType) != OwnerDrawControl::Menu);
    }

	  // -------------------------------- COPYING & DESTRUCTION -------------------------------
  
    ENABLE_COPY(ControlEventArgs);      //!< Can be shallow copied
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
      return send_message<encoding,message+WindowMessage::REFLECT>(Sender, opaque_cast(Ident), opaque_cast(Data));
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerDrawCtrlEvent - Defines 'OwnerDraw' event for controls (ie. WM_DRAWITEM)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerDrawCtrlEvent = ControlEvent<ENC, WindowMessage::DRAWITEM>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerDrawCtrlEventArgs - Arguments for 'OwnerDraw' event for controls (ie. WM_DRAWITEM)
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerDrawCtrlEventArgs = ControlEventArgs<ENC,WindowMessage::DRAWITEM>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerDrawCtrlEventHandler - Handler for 'OwnerDraw' event for controls (ie. WM_DRAWITEM)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerDrawCtrlEventHandler = ControlEventHandler<ENC,WindowMessage::DRAWITEM>;
  
} // namespace wtl

#endif // WTL_OWNER_DRAW_CONTROL_EVENT_HPP
