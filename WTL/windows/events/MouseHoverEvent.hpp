//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\MouseHoverEvent.hpp
//! \brief Encapsulates the WM_MOUSEHOVER message in the 'MouseHover' event
//! \date 15 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MOUSEHOVER_EVENT_HPP
#define WTL_MOUSEHOVER_EVENT_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/EnumCast.hpp>             //!< EnumCast
#include <wtl/windows/EventArgs.hpp>          //!< EventArgs
#include <wtl/platform/MouseFlags.hpp>        //!< MouseKeyFlag

//! \namespace wtl - Windows template library
namespace wtl
{

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<WindowMessage::MOUSEHOVER> - Arguments decoder mouse movement
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::MOUSEHOVER> 
  {  
    // ---------------------------------- TYPES & CONSTANTS ----------------------------------
    
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WindowMessage::MOUSEHOVER;
    
    //! \var unhandled - Define unhandled result
    static constexpr ::LRESULT  unhandled = unhandled_result<message>::value;
    
    // ------------------------------------- CONSTRUCTION -----------------------------------
	
    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs<WindowMessage::MOUSEHOVER>::EventArgs
    //! Decode arguments for win32 message 'WM_MOUSEHOVER' 
    //! 
    //! \param[in] w - Key/Mouse-button flags
    //! \param[in] l - Co-ordinates encoding in the LO and HO words
    /////////////////////////////////////////////////////////////////////////////////////////
    EventArgs(::WPARAM w, ::LPARAM l) : Position(GET_X_LPARAM(l), GET_Y_LPARAM(l)), 
                                        State(enum_cast<MouseKeyFlag>(w))
    {}
    
	  // -------------------------------- COPY, HOVER & DESTROY  -------------------------------
  
    ENABLE_COPY(EventArgs);      //!< Can be shallow copied
    ENABLE_MOVE(EventArgs);      //!< Can be moved
    ENABLE_POLY(EventArgs);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------

    PointL        Position;       //!< Cursor position [in client co-ordinates]
    MouseKeyFlag  State;          //!< State of keys and mouse buttons
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias MouseHoverEventArgs - Defines arguments type for the 'MouseHover' Event 
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using MouseHoverEventArgs = EventArgs<ENC,WindowMessage::MOUSEHOVER>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias MouseHoverEvent - Defines the signature of 'MouseHover' event handlers  [Pass by value]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using MouseHoverEvent = Event<LResult,MouseHoverEventArgs<ENC>>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias MouseHoverEventHandler - Defines the delegate type for the 'MouseHover' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using MouseHoverEventHandler = handler_t<MouseHoverEvent<ENC>>;

  
} // namespace wtl

#endif // WTL_MOUSEHOVER_EVENT_HPP
