////////////////////////////////////////////////////////////////////////////////
//! \file wtl\library\Traits.cpp
//! \brief Provides static storage for traits for compilers that dont fully support constexpr
//! \date 7 March 2015
//! \author Nick Crowley
//! \copyright © Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////

#include "WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ////////////////////////////////////////////////////////////////////////////////
  // Console::flush 
  //! Flushes the log-file output to disc
  ////////////////////////////////////////////////////////////////////////////////
  void Console::flush()
  {
    // Flush log-file
    //logFile.flush();
  }

  ////////////////////////////////////////////////////////////////////////////////
  // Console::log
  //! Write string to log with current attributes
  //! 
  //! \param[in] *str - String
  ////////////////////////////////////////////////////////////////////////////////
  void Console::log(const char* str)
  {
    // Write to log-file
    //logFile.write(str, getAttributes());
  }
  
  //! \var cdebug - Debug output
  Console cdebug;
}

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \var npos - Define 'Invalid handle' sentinel values
  const ::ATOM        handle_alloc<::ATOM>::npos = INVALID_ATOM;
  const ::HACCEL      handle_alloc<::HACCEL>::npos = nullptr;
  const ::HBRUSH      handle_alloc<::HBRUSH>::npos = nullptr;
  const ::HDC         handle_alloc<::HDC>::npos = nullptr;
  const ::HFILESEARCH handle_alloc<::HFILESEARCH>::npos = (const ::HFILESEARCH)INVALID_HANDLE_VALUE;
  const ::HFONT       handle_alloc<::HFONT>::npos = nullptr;
  const ::HGLOBAL     handle_alloc<::HGLOBAL>::npos = nullptr;
  const ::HICON       handle_alloc<::HICON>::npos = nullptr;
  const ::HMENU       handle_alloc<::HMENU>::npos = nullptr;
  const ::HMODULE     handle_alloc<::HMODULE>::npos = nullptr;
  const ::HPEN        handle_alloc<::HPEN>::npos = nullptr;
  const ::HRSRC       handle_alloc<::HRSRC>::npos = nullptr;
  const ::HWND        handle_alloc<::HWND>::npos = nullptr;
}

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \var enum_values<...>::values - Defines character encodings
  const char* enum_names<Encoding>::values[] = { "ANSI", "ASCII", "OEM", "MAC", "THREAD_ANSI", "SYMBOL", "UTF7", "UTF8", "UTF16" };

  //! \var enum_values<...>::values - Defines HatchStyle values
  const Encoding enum_values<Encoding>::values[] = { Encoding::ANSI, Encoding::ASCII, Encoding::OEM, Encoding::MAC, Encoding::THREAD_ANSI, 
                                                     Encoding::SYMBOL, Encoding::UTF7, Encoding::UTF8, Encoding::UTF16 };
  
  //! \var enum_values<...>::value - Defines Colour values
  const Colour  enum_values<Colour>::values[26] = {Colour::Black, Colour::Blue, Colour::DarkBlue, Colour::SkyBlue, Colour::Cyan, Colour::Teal, Colour::Lime, Colour::Teal, Colour::Green,
                                                   Colour::Leaves, Colour::Forest, Colour::Yellow, Colour::Gold, Colour::Orange, Colour::Honey, Colour::Brown, Colour::Red, Colour::Rose,
                                                   Colour::Pink, Colour::Purple, Colour::Magenta, Colour::Beige, Colour::Wheat, Colour::Snow, Colour::White, Colour::Invalid };
  
  //! \var enum_values<...>::value - Defines HatchStyle values
  const HatchStyle  enum_values<HatchStyle>::values[6] = { HatchStyle::Horizontal, HatchStyle::Vertical, HatchStyle::ForwardDiagonal, 
                                                           HatchStyle::BackwardDiagonal, HatchStyle::Cross, HatchStyle::CrossDiagonal };

}


//! \namespace wtl - Windows template library
namespace wtl
{
  //! \var DeviceContext::ScreenDC - Screen device context
  DeviceContext  ScreenDC = HDeviceContext::npos;

  //! Stock brushes
  const HBrush  StockBrush::Black(Colour::Black);       //!< Black brush
  const HBrush  StockBrush::Blue(Colour::Blue);        //!< Blue brush
  const HBrush  StockBrush::DarkBlue(Colour::DarkBlue);    //!< DarkBlue brush
  const HBrush  StockBrush::SkyBlue(Colour::SkyBlue);     //!< SkyBlue brush
  const HBrush  StockBrush::Cyan(Colour::Cyan);        //!< Cyan brush
  const HBrush  StockBrush::Teal(Colour::Teal);        //!< Teal brush
  const HBrush  StockBrush::Lime(Colour::Lime);        //!< Lime brush
  const HBrush  StockBrush::Green(Colour::Green);       //!< Green brush
  const HBrush  StockBrush::Leaves(Colour::Leaves);       //!< Leaves brush
  const HBrush  StockBrush::Forest(Colour::Forest);       //!< Forest brush
  const HBrush  StockBrush::Yellow(Colour::Yellow);      //!< Yellow brush
  const HBrush  StockBrush::Gold(Colour::Gold);        //!< Gold brush
  const HBrush  StockBrush::Orange(Colour::Orange);      //!< Orange brush
  const HBrush  StockBrush::Honey(Colour::Honey);       //!< Honey brush
  const HBrush  StockBrush::Brown(Colour::Brown);       //!< Brown brush
  const HBrush  StockBrush::Red(Colour::Red);         //!< Red brush
  const HBrush  StockBrush::Rose(Colour::Rose);        //!< Rose brush
  const HBrush  StockBrush::Pink(Colour::Pink);        //!< Pink brush
  const HBrush  StockBrush::Purple(Colour::Purple);      //!< Purple brush
  const HBrush  StockBrush::Magenta(Colour::Magenta);     //!< Magenta brush
  const HBrush  StockBrush::Beige(Colour::Beige);       //!< Beige brush
  const HBrush  StockBrush::Wheat(Colour::Wheat);       //!< Wheat brush
  const HBrush  StockBrush::Snow(Colour::Snow);       //!< Snow brush
  const HBrush  StockBrush::White(Colour::White);       //!< White brush
  const HBrush  StockBrush::Null(StockObject::NullBrush);       //!< Special: Null brush

  //! Stock fonts
  const HFont  StockFont::System(StockObject::SystemFixedFont);
  const HFont  StockFont::Tooltip(c_arr("MS Shell Dlg 2"), ScreenDC.getFontHeight(8));
  const HFont  StockFont::Window(StockObject::DefaultGuiFont);
  
  //! Stock pens
  const HPen  StockPen::Black(PenStyle::Solid, 1, Colour::Black);       //!< Black pen
  const HPen  StockPen::Blue(PenStyle::Solid, 1, Colour::Blue);        //!< Blue pen
  const HPen  StockPen::DarkBlue(PenStyle::Solid, 1, Colour::DarkBlue);    //!< DarkBlue pen
  const HPen  StockPen::SkyBlue(PenStyle::Solid, 1, Colour::SkyBlue);     //!< SkyBlue pen
  const HPen  StockPen::Cyan(PenStyle::Solid, 1, Colour::Cyan);        //!< Cyan pen
  const HPen  StockPen::Teal(PenStyle::Solid, 1, Colour::Teal);        //!< Teal pen
  const HPen  StockPen::Lime(PenStyle::Solid, 1, Colour::Lime);        //!< Lime pen
  const HPen  StockPen::Green(PenStyle::Solid, 1, Colour::Green);       //!< Green pen
  const HPen  StockPen::Leaves(PenStyle::Solid, 1, Colour::Leaves);       //!< Leaves pen
  const HPen  StockPen::Forest(PenStyle::Solid, 1, Colour::Forest);       //!< Forest pen
  const HPen  StockPen::Yellow(PenStyle::Solid, 1, Colour::Yellow);      //!< Yellow pen
  const HPen  StockPen::Gold(PenStyle::Solid, 1, Colour::Gold);        //!< Gold pen
  const HPen  StockPen::Orange(PenStyle::Solid, 1, Colour::Orange);      //!< Orange pen
  const HPen  StockPen::Honey(PenStyle::Solid, 1, Colour::Honey);       //!< Honey pen
  const HPen  StockPen::Brown(PenStyle::Solid, 1, Colour::Brown);       //!< Brown pen
  const HPen  StockPen::Red(PenStyle::Solid, 1, Colour::Red);         //!< Red pen
  const HPen  StockPen::Rose(PenStyle::Solid, 1, Colour::Rose);        //!< Rose pen
  const HPen  StockPen::Pink(PenStyle::Solid, 1, Colour::Pink);        //!< Pink pen
  const HPen  StockPen::Purple(PenStyle::Solid, 1, Colour::Purple);      //!< Purple pen
  const HPen  StockPen::Magenta(PenStyle::Solid, 1, Colour::Magenta);     //!< Magenta pen
  const HPen  StockPen::Beige(PenStyle::Solid, 1, Colour::Beige);       //!< Beige pen
  const HPen  StockPen::Wheat(PenStyle::Solid, 1, Colour::Wheat);       //!< Wheat pen
  const HPen  StockPen::Snow(PenStyle::Solid, 1, Colour::Snow);       //!< Snow pen
  const HPen  StockPen::White(PenStyle::Solid, 1, Colour::White);       //!< White pen
  const HPen  StockPen::Null(StockObject::NullPen);                                     //!< Special: Null pen
  
  //! \var LanguageId::Neutral - Define neutral language id
  const LanguageId  LanguageId::Neutral(LANG_NEUTRAL,SUBLANG_NEUTRAL);

  //! \var Resource::npos - Define 'Resource not found' sentinel value
  const Resource  Resource::npos;
}


