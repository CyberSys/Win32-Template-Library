//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\checkbox\EditReadOnlyProperty.hpp
//! \brief Separate implementation for the edit control 'ReadOnly' property (resolves circular dependency)
//! \date 22 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EDIT_READONLY_PROPERTY_HPP
#define WTL_EDIT_READONLY_PROPERTY_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/BooleanCast.hpp>                            //!< boolean_cast
#include <wtl/windows/controls/edit/EditReadOnlyProperty.h>     //!< EditReadOnlyPropertyImpl
#include <wtl/windows/controls/edit/Edit.hpp>                   //!< Edit

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // EditReadOnlyPropertyImpl::get const
  //! Query whether the text is read-only
  //! 
  //! \return value_t - Current 'readonly' state if edit exists, otherwise initial state
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename EditReadOnlyPropertyImpl<ENC>::value_t  EditReadOnlyPropertyImpl<ENC>::get() const 
  {
    // Query window style for ES_READONLY
    return this->Window.Style && EditStyle::ReadOnly;
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // EditReadOnlyPropertyImpl::set 
  //! Set the 'ReadOnly' state iff edit exists, otherwise sets the initial state
  //! 
  //! \param[in] state - New 'ReadOnly' state
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  EditReadOnlyPropertyImpl<ENC>::set(value_t state) 
  {
    // [EXISTS] Set state iff control exists
    if (this->Window.exists())
      this->Window.send(EditMessage::SetReadOnly, boolean_cast(state)); 

    // [¬EXISTS] Set/clear ES_READONLY window style
    else if (state)
      this->Window.Style |= EditStyle::ReadOnly;
    else
      this->Window.Style &= ~EditStyle::ReadOnly;
  }
  
      
} // namespace wtl

#endif // WTL_EDIT_READONLY_PROPERTY_HPP
