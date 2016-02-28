//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\combobox\ComboBoxMinVisibleProperty.hpp
//! \brief Separate implementation for the ComboBox control 'MinVisible' property (resolves circular dependency)
//! \date 28 February 2016
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_COMBOBOX_MINVISIBLE_PROPERTY_HPP
#define WTL_COMBOBOX_MINVISIBLE_PROPERTY_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/BooleanCast.hpp>                                    //!< boolean_cast
#include <wtl/windows/controls/combobox/ComboBoxMinVisibleProperty.h>   //!< ComboBoxMinVisiblePropertyImpl
#include <wtl/windows/controls/combobox/ComboBox.hpp>                   //!< ComboBox

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // ComboBoxMinVisiblePropertyImpl::get const
  //! Get the current 'MinVisible' state
  //! 
  //! \return value_t - Minimum number of items to display if control exists, otherwise 0
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename ComboBoxMinVisiblePropertyImpl<ENC>::value_t  ComboBoxMinVisiblePropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Query state iff control exists
    if (this->Window.exists())
      return ComboBox_GetMinVisible(this->Window.handle());

    // Return false
    return 0;
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // ComboBoxMinVisiblePropertyImpl::set 
  //! Set the current 'MinVisible' state iff ComboBox exists, otherwise this has no effect
  //! 
  //! \param[in] num - Minimum number of items to display
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  ComboBoxMinVisiblePropertyImpl<ENC>::set(value_t num) 
  {
    // [EXISTS] Set state iff control exists
    if (this->Window.exists())
      ComboBox_SetMinVisible(this->Window.handle(), num);
  }
  
      
} // namespace wtl

#endif // WTL_COMBOBOX_MINVISIBLE_PROPERTY_HPP
