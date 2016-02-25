//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\ColourizeEvent.hpp
//! \brief Provides a decoder for back/text colour messages raised from controls 
//! \date 25 February 2016
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CONTROL_COLOUR_EVENT_HPP
#define WTL_CONTROL_COLOUR_EVENT_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Handle.hpp>                 //!< Handle
#include <wtl/traits/DeviceContextTraits.hpp>   //!< HDeviceContext
#include <wtl/traits/WindowTraits.hpp>          //!< HWnd
#include <wtl/windows/Event.hpp>                //!< Event
#include <wtl/platform/WindowFlags.hpp>         //!< WindowId
#include <wtl/platform/CommonApi.hpp>           //!< send_message

//! \namespace wtl - Windows template library
namespace wtl 
{
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias enable_if_colour_message_t - Defines an SFINAE expression requiring a control-colour window message
  //! 
  //! \tparam WM - Window message
  //! \tparam RET - [optional] Desired type if expression is valid   (Default is void)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <WindowMessage WM, typename RET = void>
  using enable_if_colour_message_t = std::enable_if_t<WM == WindowMessage::CtrlColourButton
                                                   || WM == WindowMessage::CtrlColourEdit
                                                   || WM == WindowMessage::CtrlColourListbox
                                                   || WM == WindowMessage::CtrlColourScrollbar
                                                   || WM == WindowMessage::CtrlColourStatic, RET>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ControlEventArgs<CtrlColour_____> - Arguments decoder for events from controls passed via 'WM_CTLCOLOR_____'
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam MESSAGE - Window message
  //! \tparam EVENT - Notification message type
  //! \tparam CODE - [optional] Notification message 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage MESSAGE, typename EVENT, EVENT CODE>
  struct ControlEventArgs<ENC,MESSAGE,EVENT,CODE,enable_if_colour_message_t<MESSAGE>> 
  {  
    // ---------------------------------- TYPES & CONSTANTS ----------------------------------
    
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias event_t - Define notification message type
    using event_t = EVENT;
    
    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = MESSAGE;
    
    // ----------------------------------- REPRESENTATION -----------------------------------

    HDeviceContext Graphics;      //!< Originator device context
    HWnd           Sender;        //!< Originator window handle
    
    // ------------------------------------- CONSTRUCTION -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // ControlEventArgs<CtrlColour_____>::ControlEventArgs
    //! Decode arguments for win32 message 'WM_CTLCOLOR____' 
    //! 
    //! \param[in] w - Originator device context handle
    //! \param[in] l - Originator window handle
    /////////////////////////////////////////////////////////////////////////////////////////
    ControlEventArgs(::WPARAM w, ::LPARAM l) : Graphics(reinterpret_cast<::HDC>(l), AllocType::WeakRef), 
                                               Sender(reinterpret_cast<::HWND>(l), AllocType::WeakRef)
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  
    ENABLE_COPY(ControlEventArgs);      //!< Can be shallow copied
    ENABLE_MOVE(ControlEventArgs);      //!< Can be moved
    ENABLE_POLY(ControlEventArgs);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ControlEventArgs<COMMAND>::reflect const
    //! Reflects the event back to the originator control
    //! 
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult reflect() const
    {
      // Reflect message
      return send_message<encoding>(message | WindowMessage::Reflect, Sender, opaque_cast(Graphics.get()), opaque_cast(Sender.get()));
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ColourizeEventArgs - Defines arguments type for the 'Colourize' event 
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam MESSAGE - [optional] Window message
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage MESSAGE = WindowMessage::CtrlColourButton>
  using ColourizeEventArgs = ControlEventArgs<ENC,MESSAGE>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ColourizeEvent - Defines the signature of 'Colourize' event handlers  [Pass by value]
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam MESSAGE - [optional] Window message
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage MESSAGE = WindowMessage::CtrlColourButton>
  using ColourizeEvent = Event<LResult,ColourizeEventArgs<ENC,MESSAGE>&>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ColourizeEventHandler - Defines the delegate type for the 'Colourize' event
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam MESSAGE - [optional] Window message
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, WindowMessage MESSAGE = WindowMessage::CtrlColourButton>
  using ColourizeEventHandler = handler_t<ColourizeEvent<ENC,MESSAGE>>;
  
  
} // namespace wtl

#endif // WTL_CONTROL_COMMAND_EVENT_HPP
