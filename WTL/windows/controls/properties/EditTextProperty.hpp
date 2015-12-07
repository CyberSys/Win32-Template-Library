//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\EditTextProperty.hpp
//! \brief Separate implementation for 'Text' window property (resolves circular dependency)
//! \date 6 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EDIT_TEXT_PROPERTY_HPP
#define WTL_EDIT_TEXT_PROPERTY_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/EnumCast.hpp>                    //!< EnumCast
#include <wtl/windows/properties/EditTextProperty.h>     //!< EditTextProperty
#include <wtl/windows/Window.hpp>                //!< Window

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // EditTextPropertyImpl::get const
  //! Get the current text if window exists, otherwise 'initial' text
  //! 
  //! \return value_t - Dynamic string containing current Window text (using window character encoding)
  //! 
  //! \throw wtl::platform_error - Unable to retrieve window text
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename EditTextPropertyImpl<ENC>::value_t  EditTextPropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Query window text
    if (this->Window.exists())
    {
      int32_t length = choose<base::encoding>(::GetWindowTextLengthA,::GetWindowTextLengthW)(this->Window);    //!< Length in chars

      // [EMPTY] Return epsilon string
      if (!length)
        return {};

      // [SMALL] 99% of window classes do not require a gigabyte text buffer
      if (length < 256)
      {
        char_t  buffer[256];

        // Get window text
        if (!choose<base::encoding>(::GetWindowTextA,::GetWindowTextW)(this->Window, &buffer[0], 256))
          throw platform_error(HERE, "Unable to retrieve window text");

        // Generate dynamic string
        return { &buffer[0], &buffer[length] };
      }
      // [LARGE] Remaining classes (Text, RichText, etc.) require dynamic storage
      else
      {
        std::vector<char_t>  buffer(length+1);    //!< Encapsulates 'buffer = new char_t[length+1]'

        // Get window text
        if (!choose<base::encoding>(::GetWindowTextA,::GetWindowTextW)(this->Window, buffer.data(), length+1))
          throw platform_error(HERE, "Unable to retrieve window text");

        // Convert to string
        return { buffer.begin(), buffer.end() };
      }
    }
        
    // [OFFLINE] Return cached
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // EditTextPropertyImpl::set 
  //! Set the current window text iff window exists, otherwise 'initial' text
  //! 
  //! \param[in] text - Window text
  //! 
  //! \throw wtl::platform_error - Unable to set window text
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  EditTextPropertyImpl<ENC>::set(value_t text) 
  {
    // [EXISTS] Set window text
    if (this->Window.exists() && !choose<base::encoding>(::SetWindowTextA,::SetWindowTextW)(this->Window, text.c_str()))
      throw platform_error(HERE, "Unable to set window text");

    // Store value
    base::set(text);
  }

      
} // namespace wtl

#endif  // WTL_EDIT_TEXT_PROPERTY_HPP

