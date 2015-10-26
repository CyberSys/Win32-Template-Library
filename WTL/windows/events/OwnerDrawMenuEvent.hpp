//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\event\OwnerDrawMenuEvent.hpp
//! \brief Encapsulates the WM_DRAWITEM message (when sent for menus) in the 'OwnerDrawMenu' event
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_OWNER_DRAW_EVENT_HPP
#define WTL_OWNER_DRAW_EVENT_HPP

#include "wtl/WTL.hpp"
#include "wtl/casts/EnumCast.hpp"               //!< EnumCast
#include "wtl/casts/OpaqueCast.hpp"             //!< OpaqueCast
#include "wtl/utils/Handle.hpp"                 //!< Handle
#include "wtl/utils/Default.hpp"                //!< Default
#include "wtl/gdi/DeviceContext.hpp"            //!< DeviceContext
#include "wtl/windows/MessageEvent.hpp"         //!< EventArgs
#include "wtl/utils/Rectangle.hpp"              //!< Rect

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<WindowMessage::DRAWITEM> - Event arguments for Win32 message 'WM_DRAWITEM' raised for menus
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::DRAWITEM>
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
    DeviceContext     Graphics;         //!< Device context clipped to menu area 
    CommandId         Ident;            //!< Command id
    RectL             Rect;             //!< Drawing/update rectangle
    HMenu             Menu;             //!< Control handle
    
    // ------------------------------------- CONSTRUCTION -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs<DRAWITEM>::EventArgs
    //! Creates arguments for 'OwnerDraw' event from controls (ie. WM_DRAWITEM)
    //! 
    //! \param[in] w - Originator window id
    //! \param[in] l - Paint data
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Event is not for a menu
    /////////////////////////////////////////////////////////////////////////////////////////
    EventArgs(::WPARAM w, ::LPARAM l) : Data(*opaque_cast<PaintData>(l)), 
                                        Action(enum_cast<OwnerDrawAction>(Data.itemAction)), 
                                        Graphics(Data.hDC),
                                        Ident(static_cast<CommandId>(Data.itemID)), 
                                        Menu((::HMENU)Data.hwndItem, AllocType::WeakRef),
                                        Rect(Data.rcItem)
    {
      PARAM_INVARIANT(Data,enum_cast<OwnerDrawControl>(Data.CtlType) == OwnerDrawControl::Menu);
    }
    
	  // -------------------------------- COPY, MOVE & DESTROY  -------------------------------
  
    ENABLE_COPY(EventArgs);      //!< Can be shallow copied
    ENABLE_MOVE(EventArgs);      //!< Can be moved
    ENABLE_POLY(EventArgs);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerDrawMenuEventArgs - Defines arguments type for the 'OwnerDrawMenu' event 
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerDrawMenuEventArgs = EventArgs<ENC,WindowMessage::DRAWITEM>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerDrawMenuEvent - Defines the signature of 'OwnerDrawMenu' event handlers  [Pass by value]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerDrawMenuEvent = Event<LResult, OwnerDrawMenuEventArgs<ENC>>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerDrawMenuEventHandler - Defines the delegate type for the 'OwnerDrawMenu' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerDrawMenuEventHandler = handler_t<OwnerDrawMenuEvent<ENC>>;

  
} // namespace wtl

#endif // WTL_OWNER_DRAW_EVENT_HPP
