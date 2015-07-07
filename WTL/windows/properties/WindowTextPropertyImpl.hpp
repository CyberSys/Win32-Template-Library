//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\WindowTextPropertyImpl.hpp
//! \brief Implementation for window text property accessors/mutators (resolves circular dependency)
//! \remarks Poor naming scheme not to be confused with the PIMPL pattern used by Property templates! 
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_TEXT_PROPERTY_IMPL_HPP
#define WTL_WINDOW_TEXT_PROPERTY_IMPL_HPP

#include "wtl/WTL.hpp"
#include "wtl/casts/EnumCast.hpp"                            //!< EnumCast
#include "wtl/windows/properties/WindowTextProperty.hpp"     //!< WindowTextProperty
#include "wtl/windows/WindowBase.hpp"                        //!< WindowBase

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // WindowTextPropertyImpl::get const
  //! Get the current text if window exists, otherwise 'initial' text
  //! 
  //! \return value_t - Dynamic string containing current Window text (using window character encoding)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename WindowTextPropertyImpl<ENC>::value_t  WindowTextPropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Query window text
    if (this->Window.exists())
    {
      int32_t length = getFunc<encoding>(::GetWindowTextLengthA,::GetWindowTextLengthW)(this->Window);    //!< Length in chars

      // [EMPTY] Return epsilon string
      if (!length)
        return {};

      // [SMALL] 99% of window classes do not require a gigabyte text buffer
      if (length < 256)
      {
        char_t  buffer[256];

        // Get window text
        if (!getFunc<encoding>(::GetWindowTextA,::GetWindowTextW)(this->Window, &buffer[0], 256))
          throw platform_error(HERE, "Unable to retrieve window text");

        // Generate dynamic string
        return { &buffer[0], &buffer[length] };
      }
      // [LARGE] Remaining classes (Text, RichText, etc.) require dynamic storage
      else
      {
        std::vector<char_t>  buffer(length+1);    //!< Encapsulates 'buffer = new char_t[length+1]'

        // Get window text
        if (!getFunc<encoding>(::GetWindowTextA,::GetWindowTextW)(this->Window, buffer.data(), length+1))
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
  // WindowTextPropertyImpl::set 
  //! Set the current window text iff window exists, otherwise 'initial' text
  //! 
  //! \param[in] text - Window text
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  WindowTextPropertyImpl<ENC>::set(value_t text) 
  {
    // [EXISTS] Set window text
    if (this->Window.exists() && !getFunc<encoding>(::SetWindowTextA,::SetWindowTextW)(this->Window, text.c_str()))
      throw platform_error(HERE, "Unable to set window text");

    // Store value
    base::set(text);
  }

      
} // namespace wtl

#endif  // WTL_WINDOW_TEXT_PROPERTY_IMPL_HPP

