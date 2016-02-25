//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\WTL.cpp
//! \brief Provides common explicit instantiations
//! \date 25 February 2016
//! \author Nick Crowley
//! \copyright © Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////

#include "WTL.hpp"
#include <WTL/windows/Window.hpp>                             //!< Window
#include <WTL/windows/MessageWindow.hpp>                      //!< MessageWindow
#include <WTL/windows/controls/button/Button.hpp>             //!< Button
#include <WTL/windows/controls/checkbox/CheckBox.hpp>         //!< CheckBox
#include <WTL/windows/controls/combobox/ComboBox.hpp>         //!< ComboBox
#include <WTL/windows/controls/edit/Edit.hpp>                 //!< Button
#include <WTL/windows/controls/radiobutton/RadioButton.hpp>   //!< RadioButton
#include <WTL/windows/controls/richedit/RichEdit.hpp>         //!< RichEdit
#include <WTL/windows/controls/static/Static.hpp>             //!< Static

//! \namespace wtl - Windows template library
namespace wtl
{
  
   
  //! Explicitly instantiate
  template struct Window<Encoding::ANSI>;
  template struct Window<Encoding::UTF16>;

  template struct MessageWindow<Encoding::ANSI>;
  template struct MessageWindow<Encoding::UTF16>;

  // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
  // ----------------------------------- REPRESENTATION -----------------------------------
  
  // ------------------------------------ CONSTRUCTION ------------------------------------
	
  // ----------------------------------- STATIC METHODS -----------------------------------

  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  // ----------------------------------- MUTATOR METHODS ----------------------------------
}


