//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\WTL.cpp
//! \brief Provides common explicit instantiations in order to reduce compile times
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
#include <WTL/utils/String.hpp>                               //!< String
#include <WTL/utils/Rectangle.hpp>                            //!< Rect
#include <WTL/utils/Point.hpp>                                //!< Point
#include <WTL/utils/Size.hpp>                                 //!< Size

//! \namespace wtl - Windows template library
namespace wtl
{
  // ----------------------------------- UTILS -----------------------------------

  //! Explicitly instantiate common encodings
  extern template String<Encoding::ANSI>;
  extern template String<Encoding::UTF16>;
  
  //! Explicitly instantiate common field types
  extern template Point<long32_t>;
  extern template Point<int32_t>;
  
  //! Explicitly instantiate common field types
  extern template Rect<long32_t>;
  extern template Rect<int32_t>;

  //! Explicitly instantiate common field types
  extern template Size<long32_t>;
  extern template Size<int32_t>;

  // ----------------------------------- WINDOWS -----------------------------------

  //! Explicitly instantiate common encodings
  extern template Window<Encoding::ANSI>;
  extern template Window<Encoding::UTF16>;
  
  //! Explicitly instantiate common encodings
  extern template MessageWindow<Encoding::ANSI>;
  extern template MessageWindow<Encoding::UTF16>;
  
  // ----------------------------------- CONTROLS -----------------------------------

  //! Explicitly instantiate common encodings
  extern template RichEdit<Encoding::ANSI>;
  extern template RichEdit<Encoding::UTF16>;

  //! Explicitly instantiate common encodings
  extern template CheckBox<Encoding::ANSI>;
  extern template CheckBox<Encoding::UTF16>;
  
  //! Explicitly instantiate common encodings
  extern template ComboBox<Encoding::ANSI>;
  extern template ComboBox<Encoding::UTF16>;
  
  //! Explicitly instantiate common encodings
  extern template Edit<Encoding::ANSI>;
  extern template Edit<Encoding::UTF16>;

  //! Explicitly instantiate common encodings
  extern template RadioButton<Encoding::ANSI>;
  extern template RadioButton<Encoding::UTF16>;

  //! Explicitly instantiate common encodings
  extern template Static<Encoding::ANSI>;
  extern template Static<Encoding::UTF16>;
  
}


