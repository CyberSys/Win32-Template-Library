//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\edit\EditModifiedProperty.hpp
//! \brief Separate implementation for the edit control 'Modified' property (resolves circular dependency)
//! \date 22 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EDIT_MODIFIED_PROPERTY_HPP
#define WTL_EDIT_MODIFIED_PROPERTY_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/BooleanCast.hpp>                            //!< boolean_cast
#include <wtl/windows/controls/edit/EditModifiedProperty.h>     //!< EditModifiedPropertyImpl
#include <wtl/windows/controls/edit/Edit.hpp>                   //!< Edit

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // EditModifiedPropertyImpl::get const
  //! Get the current 'modified' state
  //! 
  //! \return value_t - Current 'modified' state if edit exists, otherwise false
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename EditModifiedPropertyImpl<ENC>::value_t  EditModifiedPropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Query state iff control exists
    if (this->Window.exists())
      return this->Window.send(EditMessage::GetModify).Result != FALSE; 

    // Return false
    return false;
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // EditModifiedPropertyImpl::set 
  //! Set the current 'Modified' state iff edit exists, otherwise this has no effect
  //! 
  //! \param[in] state - New 'Modified' state
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  EditModifiedPropertyImpl<ENC>::set(value_t state) 
  {
    // [EXISTS] Set state iff control exists
    if (this->Window.exists())
      this->Window.send(EditMessage::SetModify, boolean_cast(state)); 
  }
  
      
} // namespace wtl

#endif // WTL_EDIT_MODIFIED_PROPERTY_HPP
