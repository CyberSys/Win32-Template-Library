//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\WindowFontProperty.cpp
//! \brief Implementation for 'WindowFont' property (avoids circular reference regarding WindowBase template)
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////

#include "wtl/WTL.hpp"
#include "wtl/casts/BooleanCast.hpp"                         //!< BooleanCast
#include "wtl/windows/properties/WindowFontProperty.hpp"     //!< WindowFontProperty
#include "wtl/windows/WindowBase.hpp"                        //!< WindowBase

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // WindowFontPropertyImpl::get const
  //! Get the window font
  //! 
  //! \return value_t - Current font if window exists, otherwise 'initial' font
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename WindowFontPropertyImpl<ENC>::value_t  WindowFontPropertyImpl<ENC>::get() const 
  {
    // Return shared handle
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // WindowFontPropertyImpl::set 
  //! Set the current window font iff window exists. If the window does not exist, this has no affect.
  //! 
  //! \param[in] font - Window font
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  WindowFontPropertyImpl<ENC>::set(value_t font) 
  {
    static constexpr bool redraw = true;

    // [EXISTS] Operation is ignored if ¬exists
    if (this->Window.exists())
    {
      // Set window font & redraw
      this->Window.send<WindowMessage::SETFONT>((uintptr_t)font.get(), boolean_cast(redraw)); 

      // Updated ref-counted shared handle
      base::set(font);
    }
  }

      
} // namespace wtl

