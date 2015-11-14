//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\GainFocusEvent.hpp
//! \brief Encapsulates the WM_SETFOCUS message in the 'GainFocus' event
//! \date 13 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SETFOCUS_EVENT_HPP
#define WTL_SETFOCUS_EVENT_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/EnumCast.hpp>             //!< EnumCast
#include <wtl/windows/EventArgs.hpp>          //!< EventArgs
#include <wtl/traits/WindowTraits.hpp>        //!< HWnd

//! \namespace wtl - Windows template library
namespace wtl
{

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<WindowMessage::SETFOCUS> - Arguments decoder mouse movement
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::SETFOCUS> 
  {  
    // ---------------------------------- TYPES & CONSTANTS ----------------------------------
    
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WindowMessage::SETFOCUS;
    
    //! \var unhandled - Define unhandled result
    static constexpr ::LRESULT  unhandled = unhandled_result<message>::value;
    
    // ------------------------------------- CONSTRUCTION -----------------------------------
	
    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs<WindowMessage::SETFOCUS>::EventArgs
    //! Decode arguments for win32 message 'WM_SETFOCUS' 
    //! 
    //! \param[in] w - Window that lost focus 
    //! \param[in] l - Unused
    /////////////////////////////////////////////////////////////////////////////////////////
    EventArgs(::WPARAM w, ::LPARAM l) : Window(reinterpret_cast<::HWND>(w), AllocType::WeakRef)
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY  -------------------------------
  
    ENABLE_COPY(EventArgs);      //!< Can be shallow copied
    ENABLE_MOVE(EventArgs);      //!< Can be moved
    ENABLE_POLY(EventArgs);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------

    HWnd    Window;       //!< Weak-reference to window that lost focus [may be empty]
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias GainFocusEventArgs - Defines arguments type for the 'GainFocus' Event 
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using GainFocusEventArgs = EventArgs<ENC,WindowMessage::SETFOCUS>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias GainFocusEvent - Defines the signature of 'GainFocus' event handlers  [Pass by const-reference]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using GainFocusEvent = Event<LResult,const GainFocusEventArgs<ENC>&>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias GainFocusEventHandler - Defines the delegate type for the 'GainFocus' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using GainFocusEventHandler = handler_t<GainFocusEvent<ENC>>;

  
} // namespace wtl

#endif // WTL_SETFOCUS_EVENT_HPP
