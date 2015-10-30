//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\MessageBox.hpp
//! \brief Encapsulates Win32 message boxes
//! \date 30 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MESSAGE_BOX_HPP
#define WTL_MESSAGE_BOX_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EncodingTraits.hpp"      //!< choose()
#include "wtl/utils/CharArray.hpp"            //!< CharArray
#include "wtl/platform/WindowFlags.hpp"       //!< MessageBoxFlags
#include <stdexcept>                          //!< std::Exception

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::messageBox
  //! Displays a 'Message Box' containing custom text and title, custom icon, and custom buttons.
  //! 
  //! \tparam ENC - Character Encoding 
  //! \tparam CAPTION - Title buffer capacity
  //! \tparam TEXT - Text buffer capacity
  //! 
  //! \param[in] parent - Parent window handle
  //! \param[in] const& title - Message box title
  //! \param[in] const& text - Message box text
  //! \param[in] flags - Flags defining buttons, icon, and appearance
  //! \return WindowId - Id of Button selected by user
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, unsigned CAPTION, unsigned TEXT>
  WindowId messageBox(HWND parent, const CharArray<ENC,CAPTION>& title, const CharArray<ENC,TEXT>& text, MessageBoxFlags flags)
  {
    return static_cast<WindowId>( choose<ENC>(::MessageBoxA,::MessageBoxW)(parent, text, title, enum_cast(flags)) );
  }


  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::errorBox
  //! Display an 'Error Box' 
  //! 
  //! \tparam ENC - Character Encoding 
  //! \tparam CAPTION - Title buffer capacity
  //! \tparam TEXT - Text buffer capacity
  //! 
  //! \param[in] parent - Parent window handle
  //! \param[in] const& title - Error box title
  //! \param[in] const& text - Error box text
  //! \param[in] buttons - [optional] Error box buttons ('Ok' if unspecified)
  //! \return WindowId - Id of Button selected by user
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, unsigned CAPTION, unsigned TEXT>
  WindowId errorBox(HWND parent, const CharArray<ENC,CAPTION>& title, const CharArray<ENC,TEXT>& text, MessageBoxFlags buttons = MessageBoxFlags::Ok)
  {
    return messageBox(parent, title, text, buttons|MessageBoxFlags::IconError);
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::errorBox
  //! Display an 'Error Box' displaying the contents of an exception
  //! 
  //! \tparam ENC - Character Encoding 
  //! \tparam CAPTION - Title buffer capacity
  //! 
  //! \param[in] parent - Parent window handle
  //! \param[in] const& ex - Exception
  //! \param[in] buttons - [optional] Error box buttons ('Ok' if unspecified)
  //! \return WindowId - Id of Button selected by user
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  WindowId errorBox(HWND parent, const std::exception& ex, MessageBoxFlags buttons = MessageBoxFlags::Ok)
  {
    return errorBox<ENC>(parent, CharArray<ENC,32>("Program Error"), CharArray<ENC,2048>(ex.what()), buttons|MessageBoxFlags::IconError);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::infoBox
  //! Display an 'Information Box' with an 'OK' button and an 'info' icon
  //! 
  //! \tparam ENC - Character Encoding 
  //! \tparam CAPTION - Title buffer capacity
  //! \tparam TEXT - Text buffer capacity
  //! 
  //! \param[in] parent - Parent window handle
  //! \param[in] const& title - Information box title
  //! \param[in] const& text - Information box text
  //! \param[in] buttons - [optional] Information box buttons ('Ok' if unspecified)
  //! \return WindowId - WindowId::Ok
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, unsigned CAPTION, unsigned TEXT>
  WindowId infoBox(HWND parent, const CharArray<ENC,CAPTION>& title, const CharArray<ENC,TEXT>& text, MessageBoxFlags buttons = MessageBoxFlags::Ok)
  {
    return messageBox(parent, title, text, buttons|MessageBoxFlags::IconInformation);
  }


  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::questionBox
  //! Display a 'Question Box' with 'Yes'/'No' buttons and a question mark icon
  //! 
  //! \tparam ENC - Character Encoding 
  //! \tparam CAPTION - Title buffer capacity
  //! \tparam TEXT - Text buffer capacity
  //! 
  //! \param[in] parent - Parent window handle
  //! \param[in] const& title - Question box title
  //! \param[in] const& text - Question box text
  //! \param[in] buttons - [optional] Question box buttons ('Yes'/'No' if unspecified)
  //! \return WindowId - WindowId::Yes or WindowId::No
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, unsigned CAPTION, unsigned TEXT>
  WindowId questionBox(HWND parent, const CharArray<ENC,CAPTION>& title, const CharArray<ENC,TEXT>& text, MessageBoxFlags buttons = MessageBoxFlags::YesNo)
  {
    return messageBox(parent, title, text, buttons|MessageBoxFlags::IconQuestion);
  }
        
} // namespace wtl

#endif // WTL_MESSAGE_BOX_HPP
