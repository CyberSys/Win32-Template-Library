//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\MouseEnterEvent.hpp
//! \brief Encapsulates the WM_MOUSEENTER message in the 'MouseEnter' event
//! \date 15 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MOUSEENTER_EVENT_HPP
#define WTL_MOUSEENTER_EVENT_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/EventArgs.hpp>                    //!< EventArgs
#include <wtl/windows/events/MouseMoveEvent.hpp>        //!< MouseKeyFlag

//! \namespace wtl - Windows template library
namespace wtl
{

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<WindowMessage::MouseEnter> - Arguments decoder mouse movement
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::MouseEnter> 
  {  
    // ---------------------------------- TYPES & CONSTANTS ----------------------------------
    
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WindowMessage::MouseEnter;
    
    //! \var unhandled - Define unhandled result
    static constexpr ::LRESULT  unhandled = unhandled_result<message>::value;
    
    // ------------------------------------- CONSTRUCTION -----------------------------------
	
    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs<WindowMessage::MouseEnter>::EventArgs
    //! Decode arguments for win32 message 'WM_MOUSEENTER' 
    //! 
    //! \param[in] const& args - MouseMove event arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    EventArgs(const EventArgs<ENC,WindowMessage::MouseMove>& args) : Position(args.Position), 
                                                                     State(args.State)
    {}
    
	  // -------------------------------- COPY, ENTER & DESTROY  -------------------------------
  
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
  //! \alias MouseEnterEventArgs - Defines arguments type for the 'MouseEnter' Event 
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using MouseEnterEventArgs = EventArgs<ENC,WindowMessage::MouseEnter>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias MouseEnterEvent - Defines the signature of 'MouseEnter' event handlers  [Pass by value]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using MouseEnterEvent = Event<LResult,MouseEnterEventArgs<ENC>>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias MouseEnterEventHandler - Defines the delegate type for the 'MouseEnter' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using MouseEnterEventHandler = handler_t<MouseEnterEvent<ENC>>;

  
} // namespace wtl

#endif // WTL_MOUSEENTER_EVENT_HPP
