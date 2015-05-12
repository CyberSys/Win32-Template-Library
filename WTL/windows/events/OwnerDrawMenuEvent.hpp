//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\event\OwnerDrawMenuEvent.hpp
//! \brief Encapsulates the WM_DRAWITEM message for menus in the 'OwnerDrawMenu' event
//! \date 6 March 2015
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
#include "wtl/utils/Rectangle.hpp"           //!< Rect

//! \namespace wtl - Windows template library
namespace wtl {
//! \namespace events - WTL Window events
namespace events 
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
    CommandId         Ident;            //!< Action id
    RectL             Rect;             //!< Drawing/update rectangle
    HMenu             Menu;             //!< Control handle

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------

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

    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs::~EventArgs
    //! Virtual d-tor
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~EventArgs()
    {}
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerDrawMenuEvent - Defines 'OwnerDraw' event for menus (ie. WM_DRAWITEM)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerDrawMenuEvent = MessageEvent<ENC, WindowMessage::DRAWITEM>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerDrawMenuEventArgs - Arguments for 'OwnerDraw' event for menus (ie. WM_DRAWITEM)
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerDrawMenuEventArgs = EventArgs<ENC,WindowMessage::DRAWITEM>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerDrawMenuEventHandler - Handler for 'OwnerDraw' event for menus (ie. WM_DRAWITEM)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerDrawMenuEventHandler = MessageEventHandler<ENC,WindowMessage::DRAWITEM>;

  
} // namespace events
} // namespace wtl

#endif // WTL_OWNER_DRAW_EVENT_HPP