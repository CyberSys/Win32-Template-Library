//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\event\OwnerMeasureMenuEvent.hpp
//! \brief Encapsulates the WM_MEASUREITEM message (when sent by Menus) in the 'OwnerMeasure' event
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_OWNER_MEASURE_MENU_EVENT_HPP
#define WTL_OWNER_MEASURE_MENU_EVENT_HPP

#include "wtl/WTL.hpp"
#include "wtl/casts/EnumCast.hpp"               //!< EnumCast
#include "wtl/casts/OpaqueCast.hpp"             //!< OpaqueCast
#include "wtl/utils/Handle.hpp"                 //!< Handle
#include "wtl/utils/Default.hpp"                //!< Default
#include "wtl/gdi/DeviceContext.hpp"            //!< DeviceContext
#include "wtl/windows/MessageEvent.hpp"         //!< MessageEvent
#include "wtl/platform/CommandId.hpp"           //!< CommandId
#include "wtl/utils/Rectangle.hpp"              //!< Rect

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<WindowMessage::MEASUREITEM> - Event arguments for Win32 message 'WM_MEASUREITEM'
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::MEASUREITEM>
  {  
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;

    //! \alias MeasureData - Define measure data type
    using MeasureData = ::MEASUREITEMSTRUCT;
    
    //! \var encoding - Define character encoding
    static constexpr Encoding encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage message = WindowMessage::MEASUREITEM;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    MeasureData&      Data;             //!< Encapsulates the input measure data (NB: Must be initialized before following fields)

  public:
    DeviceContext     Graphics;         //!< Device context
    CommandId         Ident;            //!< Command identifier
    SizeL             Size;             //!< Item size
    
    // ------------------------------------- CONSTRUCTION -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs<MEASUREITEM>::EventArgs
    //! Creates arguments for 'OwnerMeasure' Event (ie. WM_MEASUREITEM)
    //! 
    //! \param[in] const& wnd - Menu owner window
    //! \param[in] w - Originator window id
    //! \param[in] l - Measure data
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Event is not for a control
    /////////////////////////////////////////////////////////////////////////////////////////
    EventArgs(const HWnd& wnd, ::WPARAM w, ::LPARAM l) : Data(*opaque_cast<MeasureData>(l)), 
                                                         Graphics(HDeviceContext(wnd)),
                                                         Ident(static_cast<CommandId>(Data.itemID)),
                                                         Size(Data.itemWidth, Data.itemHeight)
    {
      PARAM_INVARIANT(Data,enum_cast<OwnerDrawControl>(Data.CtlType) == OwnerDrawControl::Menu);
    }

	  // -------------------------------- COPY, MOVE & DESTROY  -------------------------------
  
    ENABLE_COPY(EventArgs);      //!< Can be shallow copied
    ENABLE_MOVE(EventArgs);      //!< Can be moved
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs::~EventArgs
    //! Save values
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~EventArgs()
    {
      Data.itemWidth = Size.width;
      Data.itemHeight = Size.height;
    }
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerMeasureMenuEventArgs - Defines arguments type for the 'OwnerMeasureMenu' event 
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerMeasureMenuEventArgs = EventArgs<ENC,WindowMessage::MEASUREITEM>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerMeasureMenuEvent - Defines the signature of 'OwnerMeasureMenu' event handlers [Pass by reference]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerMeasureMenuEvent = Event<LResult, OwnerMeasureMenuEventArgs<ENC>& >;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerMeasureMenuEventHandler - Defines the delegate type for the 'OwnerMeasureMenu' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerMeasureMenuEventHandler = handler_t<OwnerMeasureMenuEvent<ENC>>;
  
} // namespace wtl

#endif // WTL_OWNER_MEASURE_MENU_EVENT_HPP
