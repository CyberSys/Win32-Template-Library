//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\WindowTextLengthPropertyImpl.hpp
//! \brief Implementation for window text length property accessors/mutators (resolves circular dependency)
//! \remarks Poor naming scheme not to be confused with the PIMPL pattern used by Property templates! 
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_TEXT_LEN_PROPERTY_IMPL_HPP
#define WTL_WINDOW_TEXT_LEN_PROPERTY_IMPL_HPP

#include "wtl/WTL.hpp"
#include "wtl/windows/properties/WindowTextLengthProperty.hpp"    //!< WindowTextLengthProperty
#include "wtl/windows/WindowBase.hpp"                             //!< WindowBase

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // WindowTextLengthPropertyImpl::get const
  //! Get length of text, in characters
  //! 
  //! \return value_t - Length of current window text, in characters.  (Always zero when window doesn't exist)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename WindowTextLengthPropertyImpl<ENC>::value_t  WindowTextLengthPropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Query window text length
    if (this->Window.exists())
      return getFunc<encoding>(::GetWindowTextLengthA,::GetWindowTextLengthW)(this->Window);
        
    // Return cached
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------


      
} // namespace wtl

#endif // WTL_WINDOW_TEXT_LEN_PROPERTY_IMPL_HPP

