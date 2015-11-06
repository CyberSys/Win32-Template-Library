//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\event\ShowWindowEvent.hpp
//! \brief Encapsulates the WM_SHOWWINDOW message in the 'ShowWindow' event
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SHOW_WINDOW_EVENT_HPP
#define WTL_SHOW_WINDOW_EVENT_HPP

#include "wtl/WTL.hpp"
#include "wtl/windows/EventArgs.hpp"              //!< EventArgs
#include "wtl/casts/OpaqueCast.hpp"               //!< OpaqueCast
#include "wtl/platform/WindowFlags.hpp"           //!< ShowWindowType

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<ENC,WindowMessage::SHOWWINDOW> - Event arguments for Win32 message 'WM_SHOWWINDOW'
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::SHOWWINDOW>
  {  
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WindowMessage::SHOWWINDOW;
    
    //! \var unhandled - Define unhandled result
    static constexpr ::LRESULT  unhandled = unhandled_result<message>::value;
    
    // ----------------------------------- REPRESENTATION -----------------------------------

    bool            Showing;      //!< Whether window is being shown or hidden
    ShowWindowType  Status;       //!< Status of window
    
    // ------------------------------------- CONSTRUCTION -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs::EventArgs
    //! Decode arguments for win32 message 'WM_SHOWWINDOW' 
    //! 
    //! \param[in] w - Whether showing or hiding
    //! \param[in] l - Window status
    /////////////////////////////////////////////////////////////////////////////////////////
    EventArgs(::WPARAM w, ::LPARAM l) : Showing(w != False), 
                                        Status(opaque_cast<ShowWindowType>(l))
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY  -------------------------------

    ENABLE_COPY(EventArgs);      //!< Can be shallow copied
    ENABLE_MOVE(EventArgs);      //!< Can be moved
    ENABLE_POLY(EventArgs);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };
  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ShowWindowEventArgs - Defines arguments type for the 'ShowWindow' event 
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ShowWindowEventArgs = EventArgs<ENC,WindowMessage::SHOWWINDOW>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ShowWindowEvent - Defines the signature of 'ShowWindow' event handlers  [Pass by value]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ShowWindowEvent = Event<LResult,ShowWindowEventArgs<ENC>>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ShowWindowEventHandler - Defines the delegate type for the 'ShowWindow' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ShowWindowEventHandler = handler_t<ShowWindowEvent<ENC>>;
  
} // namespace wtl

#endif // WTL_SHOW_WINDOW_EVENT_HPP
