//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\DrawingFlags.cpp
//! \brief Provides storage for drawing flags
//! \date 7 March 2015
//! \author Nick Crowley
//! \copyright © Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////

#include "wtl/WTL.hpp"
#include "wtl/platform/DrawingFlags.hpp"
#include "wtl/platform/Colours.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \var enum_values<...>::value - Defines Colour values
  const Colour  enum_values<Colour>::values[26] = {Colour::Black, Colour::Blue, Colour::DarkBlue, Colour::SkyBlue, Colour::Cyan, Colour::Teal, Colour::Lime, Colour::Teal, Colour::Green,
                                                   Colour::Leaves, Colour::Forest, Colour::Yellow, Colour::Gold, Colour::Orange, Colour::Honey, Colour::Brown, Colour::Red, Colour::Rose,
                                                   Colour::Pink, Colour::Purple, Colour::Magenta, Colour::Beige, Colour::Wheat, Colour::Snow, Colour::White, Colour::Invalid };
  
  //! \var enum_values<...>::value - Defines HatchStyle values
  const HatchStyle  enum_values<HatchStyle>::values[6] = { HatchStyle::Horizontal, HatchStyle::Vertical, HatchStyle::ForwardDiagonal, 
                                                           HatchStyle::BackwardDiagonal, HatchStyle::Cross, HatchStyle::CrossDiagonal };

}


