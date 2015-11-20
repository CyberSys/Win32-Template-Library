//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\Colours.hpp
//! \brief Defines WTL and Win32 API colours
//! \date 26 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_COLOURS_HPP
#define WTL_COLOURS_HPP

#include <wtl/WTL.hpp>
#include <wtl/traits/EnumTraits.hpp>      //!< is_attribute, is_contiguous
#include <wtl/utils/Default.hpp>          //!< default_t

//! \namespace wtl - Windows template library
namespace wtl
{
  // ----------------------------------- LIBRARY COLOURS ----------------------------------


  //! \enum Colour - Defines common RGB colours
  enum class Colour : ulong32_t
  {
    Black = RGB(0x00,0x00,0x00),      //!< 
    
    Blue = RGB(0x00,0x4B,0xff),       //!< 
    DarkBlue = RGB(0x00,0x05,0xff),   //!< 
    SkyBlue = RGB(0x00,0x63,0xff),    //!< 
    
    Cyan = RGB(0x00,0xfa,0xff),       //!< 
    Teal = RGB(0x00,0xff,0x7d),       //!< 
    Lime = RGB(0x00,0xff,0x00),       //!< 
    Green = RGB(0x32,0xcd,0x32),      //!< 
    Leaves = RGB(0x22,0x8b,0x22),     //!< 
    Forest = RGB(0x00,0x64,0x00),     //!< 

    Yellow = RGB(0xbe,0xff,0x00),     //!< 
    Gold = RGB(0xff,0xff,0x00),       //!< 
    Orange = RGB(0xff,0xc3,0x00),     //!< 
    Honey = RGB(0xff,0x4b,0x00),      //!< 
    Brown = RGB(0x66,0x33,0x00),      //!< 

    Red = RGB(0xff,0x00,0x05),        //!< 
    Rose = RGB(0xff,0x00,0x73),       //!< 
    Pink = RGB(0xff,0x00,0xcd),       //!< 
    Purple = RGB(0xff,0x00,0xff),     //!< 
    Magenta = RGB(0x64,0x00,0xff),    //!< 

    Beige = RGB(0xf5,0xf5,0xdc),      //!< 
    Wheat = RGB(0xf5,0xde,0xb3),      //!< 
    Snow = RGB(0xff,0xfa,0xfa),       //!< 
    White = RGB(0xff,0xff,0xff),      //!< 

    Invalid = CLR_INVALID,            //!< Sentinel value for invalid colour
  };
  
  //! Define traits: Contiguous Enumeration
  template <> struct is_attribute<Colour>  : std::false_type  {};
  template <> struct is_contiguous<Colour> : std::true_type   {};
  template <> struct default_t<Colour>     : std::integral_constant<Colour,Colour::Black>   {};
  
}

#endif  // WTL_COLOURS_HPP
