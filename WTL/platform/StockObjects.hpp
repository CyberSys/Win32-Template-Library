//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\StockObjects.hpp
//! \brief Provides extended stock drawing objects
//! \date 5 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_STOCK_OBJECTS_HPP
#define WTL_STOCK_OBJECTS_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Handle.hpp>
#include <wtl/traits/BrushTraits.hpp>
#include <wtl/traits/FontTraits.hpp>
#include <wtl/traits/PenTraits.hpp>

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct StockBrush - Provides stock brushes
  /////////////////////////////////////////////////////////////////////////////////////////
  struct StockBrush final
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    // Define solid brushes
    static const HBrush Black,       //!< Black brush
                        Blue,        //!< Blue brush
                        DarkBlue,    //!< DarkBlue brush
                        SkyBlue,     //!< SkyBlue brush
                        Cyan,        //!< Cyan brush
                        Teal,        //!< Teal brush
                        Lime,        //!< Lime brush
                        Green,       //!< Green brush
                        Leaves,      //!< Leaves brush
                        Forest,      //!< Forest brush
                        Yellow,      //!< Yellow brush
                        Gold,        //!< Gold brush
                        Orange,      //!< Orange brush
                        Honey,       //!< Honey brush
                        Brown,       //!< Brown pen
                        Red,         //!< Red brush
                        Rose,        //!< Rose brush
                        Pink,        //!< Pink brush
                        Purple,      //!< Purple brush
                        Magenta,     //!< Magenta brush
                        Beige,       //!< Beige brush
                        Wheat,       //!< Wheat brush
                        Snow,        //!< Snow brush
                        White,       //!< White brush
                        Null;        //!< Special: Null brush

    // Define system brushes
    static const HBrush ActiveBorder,			          //!< Active window border.
                        ActiveCaption,			        //!< Active window title bar. 
                        AppWorkspace,			          //!< Background color of multiple document interface (MDI) applications.
                        Background,			            //!< Desktop.
                        ButtonFace,			            //!< Face color for three-dimensional display elements and for dialog box backgrounds.
                        ButtonShadow,			          //!< Shadow color for three-dimensional display elements (for edges facing away from the light source).
                        ButtonText,			            //!< Text on push buttons.
                        ButtonDkShadow,			        //!< Dark shadow for three-dimensional display elements.
                        ButtonLight,			          //!< Light color for three-dimensional display elements (for edges facing the light source.)
                        ButtonHighlight,			      //!< Highlight color for three-dimensional display elements (for edges facing the light source.)
                        CaptionText,			          //!< Text in caption, size box, and scroll bar arrow box.
                        GradientActiveCaption,			//!< [windows 5.00] Right side color in the color gradient of an active window's title bar. 
                        GradientInactiveCaption,		//!< [windows 5.00] Right side color in the color gradient of an inactive window's title bar. 
                        GrayText,			              //!< Grayed (disabled) text.
                        Highlight,			            //!< Item(s) selected in a control.
                        HighlightText,			        //!< Text of item(s) selected in a control.
                        Hotlight,			              //!< [windows 5.00] Color for a hyperlink or hot-tracked item.
                        InactiveCaption,			      //!< Inactive window caption. 
                        InactiveBorder,			        //!< Inactive window border.
                        InactiveCaptionText,			  //!< Color of text in an inactive caption.
                        InfoBk,			                //!< Background color for tooltip controls.
                        InfoText,			              //!< Text color for tooltip controls.
                        Menu,			                  //!< Menu background.
                        MenuBar,			              //!< [windows 5.01] The background color for the menu bar when menus appear as flat menus           
                        MenuHilight,			          //!< [windows 5.01] The color used to highlight menu items when the menu appears as a flat menu
                        MenuText,			              //!< Text in menus.
                        ScrollBar,			            //!< Scroll bar gray area.
                        Window,			                //!< Window background.
                        WindowFrame,			          //!< Window frame.
                        WindowText;			            //!< Text in windows.

    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(StockBrush);       //!< Cannot instantiate
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(StockBrush);       //!< Cannot instantiate
    DISABLE_MOVE(StockBrush);       //!< Cannot instantiate
    DISABLE_DTOR(StockBrush);       //!< Cannot instantiate
  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct StockFont - Provides stock fonts
  /////////////////////////////////////////////////////////////////////////////////////////
  struct StockFont final
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    // Define common fonts
    static const HFont  System,        //!< System font
                        Tooltip,       //!< Tooltip font
                        Window;        //!< Window font

    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(StockFont);       //!< Cannot instantiate
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(StockFont);       //!< Cannot instantiate
    DISABLE_MOVE(StockFont);       //!< Cannot instantiate
    DISABLE_DTOR(StockFont);       //!< Cannot instantiate
  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct StockPen - Provides stock pens
  /////////////////////////////////////////////////////////////////////////////////////////
  struct StockPen final
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    // Define solid pens
    static const HPen   Black,       //!< Black pen
                        Blue,        //!< Blue pen
                        DarkBlue,    //!< DarkBlue pen
                        SkyBlue,     //!< SkyBlue pen
                        Cyan,        //!< Cyan pen
                        Teal,        //!< Teal pen
                        Lime,        //!< Lime pen
                        Green,       //!< Green pen
                        Leaves,      //!< Leaves pen
                        Forest,      //!< Forest pen
                        Yellow,      //!< Yellow pen
                        Gold,        //!< Gold pen
                        Orange,      //!< Orange pen
                        Honey,       //!< Honey pen
                        Brown,       //!< Brown pen
                        Red,         //!< Red pen
                        Rose,        //!< Rose pen
                        Pink,        //!< Pink pen
                        Purple,      //!< Purple pen
                        Magenta,     //!< Magenta pen
                        Beige,       //!< Beige pen
                        Wheat,       //!< Wheat pen
                        Snow,        //!< Snow pen
                        White,       //!< White pen
                        Null;        //!< Special: Null pen

    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(StockPen);       //!< Cannot instantiate
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(StockPen);       //!< Cannot instantiate
    DISABLE_MOVE(StockPen);       //!< Cannot instantiate
    DISABLE_DTOR(StockPen);       //!< Cannot instantiate
  };

}

#endif // WTL_STOCK_OBJECTS_HPP
  