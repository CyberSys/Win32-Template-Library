//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\LoseFocusEvent.hpp
//! \brief Encapsulates the WM_KILLFOCUS message in the 'LoseFocus' event
//! \date 13 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_KILLFOCUS_EVENT_HPP
#define WTL_KILLFOCUS_EVENT_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/EnumCast.hpp>             //!< EnumCast
#include <wtl/windows/EventArgs.hpp>          //!< EventArgs
#include <wtl/traits/WindowTraits.hpp>        //!< HWnd

//! \namespace wtl - Windows template library
namespace wtl
{

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<WindowMessage::KillFocus> - Arguments decoder mouse movement
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::KillFocus> 
  {  
    // ---------------------------------- TYPES & CONSTANTS ----------------------------------
    
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WindowMessage::KillFocus;
    
    // ----------------------------------- REPRESENTATION -----------------------------------

    HWnd    Window;       //!< Weak-reference to window that receives focus [may be empty]

    // ------------------------------------- CONSTRUCTION -----------------------------------
	
    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs<WindowMessage::KillFocus>::EventArgs
    //! Decode arguments for win32 message 'WM_KILLFOCUS' 
    //! 
    //! \param[in] w - Window that receives focus 
    //! \param[in] l - Unused
    /////////////////////////////////////////////////////////////////////////////////////////
    EventArgs(::WPARAM w, ::LPARAM l) : Window(reinterpret_cast<::HWND>(w), AllocType::WeakRef)
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  
    ENABLE_COPY(EventArgs);      //!< Can be shallow copied
    ENABLE_MOVE(EventArgs);      //!< Can be moved
    ENABLE_POLY(EventArgs);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias LoseFocusEventArgs - Defines arguments type for the 'LoseFocus' Event 
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using LoseFocusEventArgs = EventArgs<ENC,WindowMessage::KillFocus>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias LoseFocusEvent - Defines the signature of 'LoseFocus' event handlers  [Pass by const-reference]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using LoseFocusEvent = Event<LResult,const LoseFocusEventArgs<ENC>&>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias LoseFocusEventHandler - Defines the delegate type for the 'LoseFocus' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using LoseFocusEventHandler = handler_t<LoseFocusEvent<ENC>>;

  
} // namespace wtl

#endif // WTL_KILLFOCUS_EVENT_HPP
