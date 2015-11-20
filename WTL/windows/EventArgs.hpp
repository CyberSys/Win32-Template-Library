//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\EventArgs.hpp
//! \brief Provides a basic Win32 message decoder
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EVENT_MESSAGE_HPP
#define WTL_EVENT_MESSAGE_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/Event.hpp>                        //!< Event
#include <wtl/resources/ResourceId.hpp>                  //!< ResourceId
#include <wtl/platform/WindowMessage.hpp>               //!< WindowMessage
#include <wtl/traits/EncodingTraits.hpp>                //!< Encoding

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs - Encapsulates decoding win32 message arguments
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam WM - Window message
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage WM> 
  struct EventArgs
  {    
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WM;
    
    //! \var unhandled - Define unhandled result
    static constexpr ::LRESULT  unhandled = unhandled_result<WM>::value;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // ------------------------------------- CONSTRUCTION -----------------------------------
	
    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs::EventArgs
    //! Create argument decoder for messages with zero arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    EventArgs()
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs::EventArgs
    //! Create from parameters
    //! 
    //! \param[in] w - First message parameter
    //! \param[in] l - Second message parameter
    /////////////////////////////////////////////////////////////////////////////////////////
    EventArgs(::WPARAM w, ::LPARAM l)
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY  -------------------------------
  public:
    ENABLE_COPY(EventArgs);      //!< Can be shallow copied
    ENABLE_MOVE(EventArgs);      //!< Can be moved
    ENABLE_POLY(EventArgs);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };
  

} // namespace wtl

#endif // WTL_EVENT_MESSAGE_HPP
