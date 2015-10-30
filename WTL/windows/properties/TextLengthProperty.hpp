//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\TextLengthProperty.hpp
//! \brief Separate implementation for 'TextLength' window property (resolves circular dependency)
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_TEXT_LENGTH_PROPERTY_HPP
#define WTL_WINDOW_TEXT_LENGTH_PROPERTY_HPP

#include "wtl/WTL.hpp"
#include "wtl/windows/properties/TextLengthProperty.h"        //!< TextLengthProperty
#include "wtl/windows/WindowBase.hpp"                         //!< WindowBase

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // TextLengthPropertyImpl::get const
  //! Get length of text, in characters
  //! 
  //! \return value_t - Length of current window text, in characters.  (Always zero when window doesn't exist)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename TextLengthPropertyImpl<ENC>::value_t  TextLengthPropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Query window text length
    if (this->Window.exists())
      return getFunc<base::encoding>(::GetWindowTextLengthA,::GetWindowTextLengthW)(this->Window);
        
    // Return cached
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------


      
} // namespace wtl

#endif // WTL_WINDOW_TEXT_LENGTH_PROPERTY_HPP

