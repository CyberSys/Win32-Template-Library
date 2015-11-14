//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\MouseMoveEvent.hpp
//! \brief Encapsulates the WM_MOUSEMOVE message in the 'MouseMove' event
//! \date 13 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MOUSEMOVE_EVENT_HPP
#define WTL_MOUSEMOVE_EVENT_HPP

#include "wtl/WTL.hpp"
#include "wtl/casts/EnumCast.hpp"             //!< EnumCast
#include "wtl/windows/EventArgs.hpp"          //!< EventArgs
#include "wtl/platform/MouseFlags.hpp"        //!< MouseKeyFlag

//! \namespace wtl - Windows template library
namespace wtl
{

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<WindowMessage::MOUSEMOVE> - Arguments decoder mouse movement
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::MOUSEMOVE> 
  {  
    // ---------------------------------- TYPES & CONSTANTS ----------------------------------
    
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WindowMessage::MOUSEMOVE;
    
    //! \var unhandled - Define unhandled result
    static constexpr ::LRESULT  unhandled = unhandled_result<message>::value;
    
    // ------------------------------------- CONSTRUCTION -----------------------------------
	
    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs<WindowMessage::MOUSEMOVE>::EventArgs
    //! Decode arguments for win32 message 'WM_MOUSEMOVE' 
    //! 
    //! \param[in] w - Key/Mouse-button flags
    //! \param[in] l - Co-ordinates encoding in the LO and HO words
    /////////////////////////////////////////////////////////////////////////////////////////
    EventArgs(::WPARAM w, ::LPARAM l) : Position(GET_X_LPARAM(l), GET_Y_LPARAM(l)), 
                                        State(enum_cast<MouseKeyFlag>(w))
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY  -------------------------------
  
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
  //! \alias MouseMoveEventArgs - Defines arguments type for the 'MouseMove' Event 
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using MouseMoveEventArgs = EventArgs<ENC,WindowMessage::MOUSEMOVE>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias MouseMoveEvent - Defines the signature of 'MouseMove' event handlers  [Pass by value]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using MouseMoveEvent = Event<LResult,MouseMoveEventArgs<ENC>>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias MouseMoveEventHandler - Defines the delegate type for the 'MouseMove' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using MouseMoveEventHandler = handler_t<MouseMoveEvent<ENC>>;

  
} // namespace wtl

#endif // WTL_MOUSEMOVE_EVENT_HPP
