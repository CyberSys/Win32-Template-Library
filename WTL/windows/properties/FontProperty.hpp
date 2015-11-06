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
  //! \return value_t - Current font if window exists, otherwise 'initial' font
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
  //! Set the current window font iff window exists. If the window does not exist, this has no affect.
  //! 
  //! \param[in] font - Window font
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  FontPropertyImpl<ENC>::set(value_t font) 
  {
    static constexpr bool redraw = true;

    // [EXISTS] Operation is ignored if ¬exists
    if (this->Window.exists())
    {
      // Set window font & redraw
      this->Window.send<WindowMessage::SETFONT>(opaque_cast(font.get()), boolean_cast(redraw)); 

      // Updated ref-counted shared handle
      base::set(font);
    }
  }

      
} // namespace wtl

#endif // WTL_WINDOW_FONT_PROPERTY_HPP
