//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\MessageBox.hpp
//! \brief Encapsulates Win32 message boxes
//! \date 30 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MESSAGE_BOX_HPP
#define WTL_MESSAGE_BOX_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/EnumCast.hpp>             //!< enum_cast
#include <wtl/traits/EncodingTraits.hpp>      //!< Encoding
#include <wtl/utils/String.hpp>               //!< String
#include <wtl/utils/Exception.hpp>            //!< caught_exception
#include <wtl/io/Console.hpp>                 //!< Console
#include <wtl/platform/WindowFlags.hpp>       //!< MessageBoxFlags
#include <wtl/windows/WindowId.hpp>           //!< WindowId
#include <stdexcept>                          //!< std::exception

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::messageBox
  //! Displays a 'Message Box' containing custom text and title, custom icon, and custom buttons.
  //! 
  //! \tparam ENC - Character Encoding 
  //! 
  //! \param[in] parent - Parent window handle
  //! \param[in] const& title - Message box title
  //! \param[in] const& text - Message box text
  //! \param[in] flags - Flags defining buttons, icon, and appearance
  //! \return WindowId - Id of Button selected by user
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  WindowId messageBox(::HWND parent, const String<ENC>& title, const String<ENC>& text, MessageBoxFlags flags)
  {
    return static_cast<WindowId>( WinAPI<ENC>::messageBox(parent, text.c_str(), title.c_str(), enum_cast(flags)) );
  }


  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::errorBox
  //! Display an 'Error Box' 
  //! 
  //! \tparam ENC - Character Encoding 
  //! 
  //! \param[in] parent - Parent window handle
  //! \param[in] const& title - Error box title
  //! \param[in] const& text - Error box text
  //! \param[in] buttons - [optional] Error box buttons ('Ok' if unspecified)
  //! \return WindowId - Id of Button selected by user
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  WindowId errorBox(::HWND parent, const String<ENC>& title, const String<ENC>& text, MessageBoxFlags buttons = MessageBoxFlags::Ok)
  {
    return messageBox(parent, title, text, buttons|MessageBoxFlags::IconError);
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::errorBox
  //! Display an 'Error Box' displaying the contents of an exception
  //! 
  //! \tparam ENC - Character Encoding 
  //! 
  //! \param[in] parent - Parent window handle
  //! \param[in] const& ex - Exception
  //! \param[in] buttons - [optional] Error box buttons ('Ok' if unspecified)
  //! \return WindowId - Id of Button selected by user
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  WindowId errorBox(::HWND parent, const caught_exception& ex, MessageBoxFlags buttons = MessageBoxFlags::Ok)
  {
    // Write to debug console
    cdebug << ex << std::flush;

    // Display error
    return errorBox<ENC>(parent, String<ENC>("Program Error"), String<ENC>(ex.message()), buttons|MessageBoxFlags::IconError);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::infoBox
  //! Display an 'Information Box' with an 'OK' button and an 'info' icon
  //! 
  //! \tparam ENC - Character Encoding 
  //! 
  //! \param[in] parent - Parent window handle
  //! \param[in] const& title - Information box title
  //! \param[in] const& text - Information box text
  //! \param[in] buttons - [optional] Information box buttons ('Ok' if unspecified)
  //! \return WindowId - WindowId::Ok
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  WindowId infoBox(::HWND parent, const String<ENC>& title, const String<ENC>& text, MessageBoxFlags buttons = MessageBoxFlags::Ok)
  {
    return messageBox(parent, title, text, buttons|MessageBoxFlags::IconInformation);
  }


  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::questionBox
  //! Display a 'Question Box' with 'Yes'/'No' buttons and a question mark icon
  //! 
  //! \tparam ENC - Character Encoding 
  //! 
  //! \param[in] parent - Parent window handle
  //! \param[in] const& title - Question box title
  //! \param[in] const& text - Question box text
  //! \param[in] buttons - [optional] Question box buttons ('Yes'/'No' if unspecified)
  //! \return WindowId - WindowId::Yes or WindowId::No
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  WindowId questionBox(::HWND parent, const String<ENC>& title, const String<ENC>& text, MessageBoxFlags buttons = MessageBoxFlags::YesNo)
  {
    return messageBox(parent, title, text, buttons|MessageBoxFlags::IconQuestion);
  }
        
} // namespace wtl

#endif // WTL_MESSAGE_BOX_HPP
