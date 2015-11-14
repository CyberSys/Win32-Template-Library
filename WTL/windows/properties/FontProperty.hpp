//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\FontProperty.hpp
//! \brief Separate implementation for 'Font' window property (resolves circular dependency)
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_FONT_PROPERTY_HPP
#define WTL_WINDOW_FONT_PROPERTY_HPP

#include "wtl/WTL.hpp"
#include "wtl/casts/BooleanCast.hpp"                 //!< BooleanCast
#include "wtl/windows/properties/FontProperty.h"     //!< FontPropertyImpl
#include "wtl/windows/WindowBase.hpp"                //!< WindowBase

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // FontPropertyImpl::get const
  //! Get the window font
  //! 
  //! \return value_t - Current font if window exists, otherwise initial font
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename FontPropertyImpl<ENC>::value_t  FontPropertyImpl<ENC>::get() const 
  {
    // Return shared handle
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // FontPropertyImpl::set 
  //! Set the current window font iff window exists, otherwise the initial font
  //! 
  //! \param[in] font - Window font
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  FontPropertyImpl<ENC>::set(value_t font) 
  {
    static constexpr bool redraw = true;

    // [EXISTS] Set font iff window exists
    if (this->Window.exists())
      this->Window.template send<WindowMessage::SETFONT>(opaque_cast(font.get()), boolean_cast(redraw)); 

    // Updated ref-counted shared handle
    base::set(font);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // FontPropertyImpl::onCreate 
  //! Called during window creation to set the initial font
  //! 
  //! \param[in,out] &args - Message arguments 
  //! \return LResult - Returns 0 to accept window creation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  LResult FontPropertyImpl<ENC>::onCreate(CreateWindowEventArgs<ENC>& args)
  {
    static constexpr bool redraw = true;

    // [EXISTS] Set font iff window exists
    if (this->Window.exists())
      this->Window.template send<WindowMessage::SETFONT>(opaque_cast(this->Value.get()), boolean_cast(redraw)); 

    // [Accept window creation]
    return 0;
  }
} // namespace wtl

#endif // WTL_WINDOW_FONT_PROPERTY_HPP
