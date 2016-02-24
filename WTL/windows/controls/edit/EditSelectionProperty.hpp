//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\edit\EditSelectionProperty.hpp
//! \brief Separate implementation for 'SelectedRange' window property (resolves circular dependency)
//! \date 6 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EDIT_SELECTION_PROPERTY_HPP
#define WTL_EDIT_SELECTION_PROPERTY_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/OpaqueCast.hpp>                                   //!< opaque_cast
#include <wtl/windows/controls/edit/EditSelectionProperty.h>          //!< EditSelectionProperty
#include <wtl/windows/controls/edit/Edit.hpp>                         //!< Edit

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // EditSelectionPropertyImpl::get const
  //! Get the current text selection range
  //! 
  //! \return value_t - Current selection range
  //! 
  //! \throw wtl::platform_error - Unable to retrieve window text
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename EditSelectionPropertyImpl<ENC>::value_t  EditSelectionPropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Query window text
    if (this->Window.exists())
    {
      value_t range;
      // Query 
      this->Window.send(EditMessage::GetSel, opaque_cast(&range.Start), opaque_cast(&range.Finish));
      return range;
    }
        
    // [OFFLINE] Return empty
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // EditSelectionPropertyImpl::set 
  //! Set the current selection range iff window exists
  //! 
  //! \param[in] range - Text selection range
  //! 
  //! \throw wtl::platform_error - Unable to set text selection range
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  EditSelectionPropertyImpl<ENC>::set(value_t range) 
  {
    // [EXISTS] Set text selection range
    if (this->Window.exists())
      this->Window.send(EditMessage::SetSel, opaque_cast(&range.Start), opaque_cast(&range.Finish));
      //throw platform_error(HERE, "Unable to set window text");

    // Store value
    base::set(range);
  }

      
} // namespace wtl

#endif  // WTL_EDIT_SELECTION_PROPERTY_HPP

