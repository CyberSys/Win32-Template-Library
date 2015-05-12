//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\Traits.cpp
//! \brief Provides storage for static traits members
//! \date 7 March 2015
//! \author Nick Crowley
//! \copyright © Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////

#include "wtl/WTL.hpp"
#include "wtl/traits/EncodingTraits.hpp"
#include "wtl/platform/Colours.hpp"
#include "wtl/platform/DrawingFlags.hpp"

#include "wtl/windows/Window.hpp"

// Debug
static wtl::MessageWindow<> wnd;

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \var enum_values<...>::values - Defines character encodings
  const char*  enum_names<Encoding>::values[] = { "ANSI", "ASCII", "OEM", "MAC", "THREAD_ANSI", "SYMBOL", "UTF7", "UTF8", "UTF16" };

  //! \var enum_values<...>::values - Defines HatchStyle values
  const Encoding  enum_values<Encoding>::values[] = { Encoding::ANSI, Encoding::ASCII, Encoding::OEM, Encoding::MAC, Encoding::THREAD_ANSI, 
                                                      Encoding::SYMBOL, Encoding::UTF7, Encoding::UTF8, Encoding::UTF16 };
  
  //! \var enum_values<...>::value - Defines Colour values
  const Colour  enum_values<Colour>::values[26] = {Colour::Black, Colour::Blue, Colour::DarkBlue, Colour::SkyBlue, Colour::Cyan, Colour::Teal, Colour::Lime, Colour::Teal, Colour::Green,
                                                   Colour::Leaves, Colour::Forest, Colour::Yellow, Colour::Gold, Colour::Orange, Colour::Honey, Colour::Brown, Colour::Red, Colour::Rose,
                                                   Colour::Pink, Colour::Purple, Colour::Magenta, Colour::Beige, Colour::Wheat, Colour::Snow, Colour::White, Colour::Invalid };
  
  //! \var enum_values<...>::value - Defines HatchStyle values
  const HatchStyle  enum_values<HatchStyle>::values[6] = { HatchStyle::Horizontal, HatchStyle::Vertical, HatchStyle::ForwardDiagonal, 
                                                           HatchStyle::BackwardDiagonal, HatchStyle::Cross, HatchStyle::CrossDiagonal };

}


