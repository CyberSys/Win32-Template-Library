//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\EncodingTraits.cpp
//! \brief Provides storage for encoding traits 
//! \date 7 March 2015
//! \author Nick Crowley
//! \copyright © Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////

#include "wtl/WTL.hpp"
#include "wtl/traits/EncodingTraits.hpp"

// Debug
#include "wtl/windows/Window.hpp"
static wtl::MessageWindow<> wnd;

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \var enum_values<...>::values - Defines character encodings
  const char*  enum_names<Encoding>::values[] = { "ANSI", "ASCII", "OEM", "MAC", "THREAD_ANSI", "SYMBOL", "UTF7", "UTF8", "UTF16" };

  //! \var enum_values<...>::values - Defines HatchStyle values
  const Encoding  enum_values<Encoding>::values[] = { Encoding::ANSI, Encoding::ASCII, Encoding::OEM, Encoding::MAC, Encoding::THREAD_ANSI, 
                                                      Encoding::SYMBOL, Encoding::UTF7, Encoding::UTF8, Encoding::UTF16 };
  
}


