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
  

  // ----------------------------------- SYSTEM COLOURS ----------------------------------
  

  //! \enum SystemColour - Defines system colours
  enum class SystemColour
  {
    ScrollBar = 0,			            //!< Scroll bar gray area.
    Background = 1,			            //!< Desktop.
    ActiveCaption = 2,			        //!< Active window title bar. 
    InactiveCaption = 3,			      //!< Inactive window caption. 
    Menu = 4,			                  //!< Menu background.
    Window = 5,			                //!< Window background.
    WindowFrame = 6,			          //!< Window frame.
    MenuText = 7,			              //!< Text in menus.
    WindowText = 8,			            //!< Text in windows.
    CaptionText = 9,			          //!< Text in caption, size box, and scroll bar arrow box.
    ActiveBorder = 10,			        //!< Active window border.
    InactiveBorder = 11,			      //!< Inactive window border.
    AppWorkspace = 12,			        //!< Background color of multiple document interface (MDI) applications.
    Highlight = 13,			            //!< Item(s) selected in a control.
    HighlightText = 14,			        //!< Text of item(s) selected in a control.
    BtnFace = 15,			              //!< Face color for three-dimensional display elements and for dialog box backgrounds.
    BtnShadow = 16,			            //!< Shadow color for three-dimensional display elements (for edges facing away from the light source).
    GrayText = 17,			            //!< Grayed (disabled) text.
    BtnText = 18,			              //!< Text on push buttons.
    InactiveCaptionText = 19,			  //!< Color of text in an inactive caption.
    BtnHighlight = 20,			        //!< Highlight color for three-dimensional display elements (for edges facing the light source.)

    BtnDkShadow = 21,			          //!< Dark shadow for three-dimensional display elements.
    BtnLight = 22,			            //!< Light color for three-dimensional display elements (for edges facing the light source.)
    InfoText = 23,			            //!< Text color for tooltip controls.
    InfoBk = 24,			              //!< Background color for tooltip controls.
    
    Hotlight = 26,			            //!< [windows 5.00] Color for a hyperlink or hot-tracked item.
    GradientActiveCaption = 27,			//!< [windows 5.00] Right side color in the color gradient of an active window's title bar. 
    GradientInactiveCaption = 28,		//!< [windows 5.00] Right side color in the color gradient of an inactive window's title bar. 
    MenuHilight = 29,			          //!< [windows 5.01] The color used to highlight menu items when the menu appears as a flat menu
    MenuBar = 30,			              //!< [windows 5.01] The background color for the menu bar when menus appear as flat menus 
    
    Desktop = Background,			      //!< 
  };

  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<SystemColour>  : std::false_type  {};
  template <> struct is_contiguous<SystemColour> : std::false_type  {};
  template <> struct default_t<SystemColour>     : std::integral_constant<SystemColour,SystemColour::ScrollBar>   {};



}

#endif  // WTL_COLOURS_HPP
