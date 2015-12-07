//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\properties\RichEditBackColourProperty.hpp
//! \brief Separate implementation for the Button control 'Icon' property (resolves circular dependency)
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RICH_EDIT_BACK_COLOUR_PROPERTY_HPP
#define WTL_RICH_EDIT_BACK_COLOUR_PROPERTY_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/controls/properties/RichEditBackColourProperty.h>    //!< RichEditBackColourPropertyImpl
#include <wtl/windows/controls/RichEdit.hpp>                               //!< RichEdit

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // RichEditBackColourPropertyImpl::onCreate 
  //! Called during control creation to set the initial background colour
  //! 
  //! \param[in,out] &args - Message arguments 
  //! \return LResult - Returns 0 to accept window creation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  LResult  RichEditBackColourPropertyImpl<ENC>::onCreate(CreateWindowEventArgs<ENC>& args)
  {
    // Set colour iff previously set and window exists
    if (base::get() != Colour::Invalid)
      this->Window.send(RichEditMessage::SetBackColour, 0, enum_cast(base::get())); 

    // Accept window creation
    return 0;
  }
    
  /////////////////////////////////////////////////////////////////////////////////////////
  // RichEditBackColourPropertyImpl::set 
  //! Set the current background colour iff control exists, otherwise sets the initial colour
  //! 
  //! \param[in] col - Background colour
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  RichEditBackColourPropertyImpl<ENC>::set(value_t col) 
  {
    // [EXISTS] Set  background colour iff window exists
    if (this->Window.exists())
      this->Window.send(RichEditMessage::SetBackColour, 0, enum_cast(col)); 
    
    // Update 'initial' value
    base::set(col);
  }
  
    
} // namespace wtl

#endif // WTL_RICH_EDIT_BACK_COLOUR_PROPERTY_HPP
