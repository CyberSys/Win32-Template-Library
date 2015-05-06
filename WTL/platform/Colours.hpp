//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\Colours.hpp
//! \brief Defines WTL and Win32 API colours
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_COLOURS_HPP
#define WTL_COLOURS_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  // ----------------------------------- LIBRARY COLOURS ----------------------------------


  //! \enum Colour - Defines common RGB colours
  enum class Colour : ulong32
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

  //! Define limits traits
  template <> struct max_value<Colour>     : std::integral_constant<Colour,Colour::Invalid>   {};
  template <> struct min_value<Colour>     : std::integral_constant<Colour,Colour::Black>     {};
  
  //! Define names and values
  //template <> struct enum_names<Colour>  { static const char* names[];   };
  template <> struct enum_values<Colour> { static const Colour values[26]; };

  // ----------------------------------- SYSTEM COLOURS ----------------------------------
  

  //! \enum SystemColour - Defines system colours
  enum class SystemColour
  {
    ScrollBar = 0,			            //!< 
    Background = 1,			            //!< 
    ActiveCaption = 2,			        //!< 
    InactiveCaption = 3,			      //!< 
    Menu = 4,			                  //!< 
    Window = 5,			                //!< 
    WindowFrame = 6,			          //!< 
    MenuText = 7,			              //!< 
    WindowText = 8,			            //!< 
    CaptionText = 9,			          //!< 
    ActiveBorder = 10,			        //!< 
    InactiveBorder = 11,			      //!< 
    AppWorkspace = 12,			        //!< 
    Highlight = 13,			            //!< 
    HighlightText = 14,			        //!< 
    BtnFace = 15,			              //!< 
    BtnShadow = 16,			            //!< 
    GrayText = 17,			            //!< 
    BtnText = 18,			              //!< 
    InactiveCaptionText = 19,			  //!< 
    BtnHighlight = 20,			        //!< 

    BtnDkShadow = 21,			          //!< 
    BtnLight = 22,			            //!< 
    InfoText = 23,			            //!< 
    InfoBk = 24,			              //!< 
    
    Hotlight = 26,			            //!< [windows 5.00] 
    GradientActiveCaption = 27,			//!< [windows 5.00] 
    GradientInactiveCaption = 28,		//!< [windows 5.00] 
    MenuHilight = 29,			          //!< [windows 5.01] 
    MenuBar = 30,			              //!< [windows 5.01] 
    
    Desktop = Background,			      //!< 
  };

  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<SystemColour>  : std::false_type  {};
  template <> struct is_contiguous<SystemColour> : std::false_type  {};
  
  //! Define limits traits
  template <> struct max_value<SystemColour>     : std::integral_constant<SystemColour,SystemColour::ScrollBar>  {};
  template <> struct min_value<SystemColour>     : std::integral_constant<SystemColour,SystemColour::MenuBar>    {};




}

#endif  // WTL_COLOURS_HPP
