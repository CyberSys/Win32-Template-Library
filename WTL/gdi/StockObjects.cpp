//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\gdi\StockObjects.cpp
//! \brief Provides extended stock drawing objects
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////

#include "StockObjects.hpp"
#include "wtl/gdi/DeviceContext.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //! Stock brushes
  const HBrush  StockBrush::Black(Colour::Black);             //!< Black brush
  const HBrush  StockBrush::Blue(Colour::Blue);               //!< Blue brush
  const HBrush  StockBrush::DarkBlue(Colour::DarkBlue);       //!< DarkBlue brush
  const HBrush  StockBrush::SkyBlue(Colour::SkyBlue);         //!< SkyBlue brush
  const HBrush  StockBrush::Cyan(Colour::Cyan);               //!< Cyan brush
  const HBrush  StockBrush::Teal(Colour::Teal);               //!< Teal brush
  const HBrush  StockBrush::Lime(Colour::Lime);               //!< Lime brush
  const HBrush  StockBrush::Green(Colour::Green);             //!< Green brush
  const HBrush  StockBrush::Leaves(Colour::Leaves);           //!< Leaves brush
  const HBrush  StockBrush::Forest(Colour::Forest);           //!< Forest brush
  const HBrush  StockBrush::Yellow(Colour::Yellow);           //!< Yellow brush
  const HBrush  StockBrush::Gold(Colour::Gold);               //!< Gold brush
  const HBrush  StockBrush::Orange(Colour::Orange);           //!< Orange brush
  const HBrush  StockBrush::Honey(Colour::Honey);             //!< Honey brush
  const HBrush  StockBrush::Brown(Colour::Brown);             //!< Brown brush
  const HBrush  StockBrush::Red(Colour::Red);                 //!< Red brush
  const HBrush  StockBrush::Rose(Colour::Rose);               //!< Rose brush
  const HBrush  StockBrush::Pink(Colour::Pink);               //!< Pink brush
  const HBrush  StockBrush::Purple(Colour::Purple);           //!< Purple brush
  const HBrush  StockBrush::Magenta(Colour::Magenta);         //!< Magenta brush
  const HBrush  StockBrush::Beige(Colour::Beige);             //!< Beige brush
  const HBrush  StockBrush::Wheat(Colour::Wheat);             //!< Wheat brush
  const HBrush  StockBrush::Snow(Colour::Snow);               //!< Snow brush
  const HBrush  StockBrush::White(Colour::White);             //!< White brush
  const HBrush  StockBrush::Null(StockObject::NullBrush);     //!< Special: Null brush

  //! Stock system colour brushes
  const HBrush  StockBrush::ScrollBar(SystemColour::ScrollBar);			                          //!< Scroll bar gray area.
  const HBrush  StockBrush::Background(SystemColour::Background);			                        //!< Desktop.
  const HBrush  StockBrush::ActiveCaption(SystemColour::ActiveCaption);			                  //!< Active window title bar. 
  const HBrush  StockBrush::InactiveCaption(SystemColour::InactiveCaption);			              //!< Inactive window caption. 
  const HBrush  StockBrush::Menu(SystemColour::Menu);			                                    //!< Menu background.
  const HBrush  StockBrush::Window(SystemColour::Window);			                                //!< Window background.
  const HBrush  StockBrush::WindowFrame(SystemColour::WindowFrame);			                      //!< Window frame.
  const HBrush  StockBrush::MenuText(SystemColour::MenuText);			                            //!< Text in menus.
  const HBrush  StockBrush::WindowText(SystemColour::WindowText);			                        //!< Text in windows.
  const HBrush  StockBrush::CaptionText(SystemColour::CaptionText);			                      //!< Text in caption, size box, and scroll bar arrow box.
  const HBrush  StockBrush::ActiveBorder(SystemColour::ActiveBorder);			                    //!< Active window border.
  const HBrush  StockBrush::InactiveBorder(SystemColour::InactiveBorder);			                //!< Inactive window border.
  const HBrush  StockBrush::AppWorkspace(SystemColour::AppWorkspace);			                    //!< Background color of multiple document interface (MDI) applications.
  const HBrush  StockBrush::Highlight(SystemColour::Highlight);			                          //!< Item(s) selected in a control.
  const HBrush  StockBrush::HighlightText(SystemColour::HighlightText);			                  //!< Text of item(s) selected in a control.
  const HBrush  StockBrush::BtnFace(SystemColour::BtnFace);			                              //!< Face color for three-dimensional display elements and for dialog box backgrounds.
  const HBrush  StockBrush::BtnShadow(SystemColour::BtnShadow);			                          //!< Shadow color for three-dimensional display elements (for edges facing away from the light source).
  const HBrush  StockBrush::GrayText(SystemColour::GrayText);			                            //!< Grayed (disabled) text.
  const HBrush  StockBrush::BtnText(SystemColour::BtnText);			                              //!< Text on push buttons.
  const HBrush  StockBrush::InactiveCaptionText(SystemColour::InactiveCaptionText);			      //!< Color of text in an inactive caption.
  const HBrush  StockBrush::BtnHighlight(SystemColour::BtnHighlight);			                    //!< Highlight color for three-dimensional display elements (for edges facing the light source.)

  const HBrush  StockBrush::BtnDkShadow(SystemColour::BtnDkShadow);			                      //!< Dark shadow for three-dimensional display elements.
  const HBrush  StockBrush::BtnLight(SystemColour::BtnLight);			                            //!< Light color for three-dimensional display elements (for edges facing the light source.)
  const HBrush  StockBrush::InfoText(SystemColour::InfoText);			                            //!< Text color for tooltip controls.
  const HBrush  StockBrush::InfoBk(SystemColour::InfoBk);			                                //!< Background color for tooltip controls.
    
  const HBrush  StockBrush::Hotlight(SystemColour::Hotlight);			                            //!< [Windows 5.00] Color for a hyperlink or hot-tracked item.
  const HBrush  StockBrush::GradientActiveCaption(SystemColour::GradientActiveCaption);			  //!< [Windows 5.00] Right side color in the color gradient of an active window's title bar. 
  const HBrush  StockBrush::GradientInactiveCaption(SystemColour::GradientInactiveCaption);		//!< [Windows 5.00] Right side color in the color gradient of an inactive window's title bar. 
  const HBrush  StockBrush::MenuHilight(SystemColour::MenuHilight);			                      //!< [Windows 5.01] The color used to highlight menu items when the menu appears as a flat menu
  const HBrush  StockBrush::MenuBar(SystemColour::MenuBar);			                              //!< [Windows 5.01] The background color for the menu bar when menus appear as flat menus           

  //! Stock fonts
  const HFont  StockFont::System(StockObject::SystemFixedFont);
  const HFont  StockFont::Tooltip(c_str(L"MS Shell Dlg 2"), DeviceContext::ScreenDC.getFontHeight(8));
  const HFont  StockFont::Window(StockObject::DefaultGuiFont);
  
  //! Stock pens
  const HPen  StockPen::Black(PenStyle::Solid, 1, Colour::Black);         //!< Black pen
  const HPen  StockPen::Blue(PenStyle::Solid, 1, Colour::Blue);           //!< Blue pen
  const HPen  StockPen::DarkBlue(PenStyle::Solid, 1, Colour::DarkBlue);   //!< DarkBlue pen
  const HPen  StockPen::SkyBlue(PenStyle::Solid, 1, Colour::SkyBlue);     //!< SkyBlue pen
  const HPen  StockPen::Cyan(PenStyle::Solid, 1, Colour::Cyan);           //!< Cyan pen
  const HPen  StockPen::Teal(PenStyle::Solid, 1, Colour::Teal);           //!< Teal pen
  const HPen  StockPen::Lime(PenStyle::Solid, 1, Colour::Lime);           //!< Lime pen
  const HPen  StockPen::Green(PenStyle::Solid, 1, Colour::Green);         //!< Green pen
  const HPen  StockPen::Leaves(PenStyle::Solid, 1, Colour::Leaves);       //!< Leaves pen
  const HPen  StockPen::Forest(PenStyle::Solid, 1, Colour::Forest);       //!< Forest pen
  const HPen  StockPen::Yellow(PenStyle::Solid, 1, Colour::Yellow);       //!< Yellow pen
  const HPen  StockPen::Gold(PenStyle::Solid, 1, Colour::Gold);           //!< Gold pen
  const HPen  StockPen::Orange(PenStyle::Solid, 1, Colour::Orange);       //!< Orange pen
  const HPen  StockPen::Honey(PenStyle::Solid, 1, Colour::Honey);         //!< Honey pen
  const HPen  StockPen::Brown(PenStyle::Solid, 1, Colour::Brown);         //!< Brown pen
  const HPen  StockPen::Red(PenStyle::Solid, 1, Colour::Red);             //!< Red pen
  const HPen  StockPen::Rose(PenStyle::Solid, 1, Colour::Rose);           //!< Rose pen
  const HPen  StockPen::Pink(PenStyle::Solid, 1, Colour::Pink);           //!< Pink pen
  const HPen  StockPen::Purple(PenStyle::Solid, 1, Colour::Purple);       //!< Purple pen
  const HPen  StockPen::Magenta(PenStyle::Solid, 1, Colour::Magenta);     //!< Magenta pen
  const HPen  StockPen::Beige(PenStyle::Solid, 1, Colour::Beige);         //!< Beige pen
  const HPen  StockPen::Wheat(PenStyle::Solid, 1, Colour::Wheat);         //!< Wheat pen
  const HPen  StockPen::Snow(PenStyle::Solid, 1, Colour::Snow);           //!< Snow pen
  const HPen  StockPen::White(PenStyle::Solid, 1, Colour::White);         //!< White pen
  const HPen  StockPen::Null(StockObject::NullPen);                       //!< Special: Null pen
  
}

