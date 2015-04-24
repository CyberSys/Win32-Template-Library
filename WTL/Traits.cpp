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
  //const ::HFILESEARCH handle_alloc<::HFILESEARCH>::npos = (const ::HFILESEARCH)INVALID_HANDLE_VALUE;
  const ::ATOM        handle_alloc<::ATOM>::npos = INVALID_ATOM;
  const ::HACCEL      handle_alloc<::HACCEL>::npos = (const ::HACCEL)INVALID_HANDLE_VALUE;
  const ::HBRUSH      handle_alloc<::HBRUSH>::npos = (const ::HBRUSH)INVALID_HANDLE_VALUE;
  const ::HDC         handle_alloc<::HDC>::npos = (const ::HDC)INVALID_HANDLE_VALUE;
  const ::HFONT       handle_alloc<::HFONT>::npos = (const ::HFONT)INVALID_HANDLE_VALUE;
  const ::HGLOBAL     handle_alloc<::HGLOBAL>::npos = (const ::HGLOBAL)INVALID_HANDLE_VALUE;
  const ::HICON       handle_alloc<::HICON>::npos = (const ::HICON)INVALID_HANDLE_VALUE;
  const ::HMENU       handle_alloc<::HMENU>::npos = (const ::HMENU)INVALID_HANDLE_VALUE;
  const ::HMODULE     handle_alloc<::HMODULE>::npos = (const ::HMODULE)INVALID_HANDLE_VALUE;
  const ::HPEN        handle_alloc<::HPEN>::npos = (const ::HPEN)INVALID_HANDLE_VALUE;
  const ::HRSRC       handle_alloc<::HRSRC>::npos = (const ::HRSRC)INVALID_HANDLE_VALUE;
  const ::HWND        handle_alloc<::HWND>::npos = (const ::HWND)INVALID_HANDLE_VALUE;
  
  //! \var handle_alloc<T>::npos - 'Invalid handle' sentinel value 
  /*template <typename T>
  const T handle_alloc<T>::npos = (const T)INVALID_HANDLE_VALUE;*/

  
  //! \var npos - Define 'No handle' sentinel values
  /*template <typename T>
  const Handle<T>   Handle<T>::npos = Handle<T>(handle_alloc<T>::npos, AllocType::WeakRef);*/

  /*const HAccelerator   HAccelerator::npos = HAccelerator(handle_alloc<::HACCEL>::npos, AllocType::WeakRef);
  const HAtom          HAtom::npos = HAtom(handle_alloc<::ATOM>::npos, AllocType::WeakRef);
  const HBrush         HBrush::npos = HBrush(handle_alloc<::HBRUSH>::npos, AllocType::WeakRef);
  const HDeviceContext HDeviceContext::npos = HDeviceContext(handle_alloc<::HDC>::npos, AllocType::WeakRef);
  const HFont          HFont::npos = HFont(handle_alloc<::HFONT>::npos, AllocType::WeakRef);
  const HGlobal        HGlobal::npos = HGlobal(handle_alloc<::HGLOBAL>::npos, AllocType::WeakRef);
  const HIcon          HIcon::npos = HIcon(handle_alloc<::HICON>::npos, AllocType::WeakRef);
  const HMenu          HMenu::npos = HMenu(handle_alloc<::HMENU>::npos, AllocType::WeakRef);
  const HModule        HModule::npos = HModule(handle_alloc<::HMODULE>::npos, AllocType::WeakRef);
  const HPen           HPen::npos = HPen(handle_alloc<::HPEN>::npos, AllocType::WeakRef);
  const HResource      HResource::npos = HResource(handle_alloc<::HRSRC>::npos, AllocType::WeakRef);
  const HWnd           HWnd::npos = HWnd(handle_alloc<::HWND>::npos, AllocType::WeakRef);*/
}

//! \if CONSTEXPR_CAP - Define here if compiler does not support static storage
#ifndef CONSTEXPR_CAP
//! \namespace wtl - Windows template library
namespace wtl
{
  //! \var enum_values<...>::values - Defines HatchStyle values
  const char* enum_names<Encoding>::values[] = { "ANSI", "ASCII", "OEM", "MAC", "THREAD_ANSI", "SYMBOL", "UTF7", "UTF8", "UTF16" };

  //! \var enum_values<...>::values - Defines HatchStyle values
  const Encoding enum_values<Encoding>::values[] = { Encoding::ANSI, Encoding::ASCII, Encoding::OEM, Encoding::MAC, Encoding::THREAD_ANSI, 
                                                      Encoding::SYMBOL, Encoding::UTF7, Encoding::UTF8, Encoding::UTF16 };

  //! \var format_spec<...>::value - Defines narrow string formatting type specifications
  const char format_spec<char,double>::value[] = "%llf";
  const char format_spec<char,uint64>::value[] = "%llu";
  const char format_spec<char,int64>::value[] = "%lld";
  const char format_spec<char,float>::value[] = "%lf";
  const char format_spec<char,uint32>::value[] = "%lu";
  const char format_spec<char,int32>::value[] = "%ld";
  const char format_spec<char,ulong32>::value[] = "%lu";
  const char format_spec<char,long32>::value[] = "%ld";
  const char format_spec<char,uint16>::value[] = "%hu";
  const char format_spec<char,int16>::value[] = "%hd";
  const char format_spec<char,uint8>::value[] = "%cu";
  const char format_spec<char,int8>::value[] = "%c";
  const char format_spec<char,char>::value[] = "%c";
  const char format_spec<char,char*>::value[] = "%s";
  const char format_spec<char,uint8*>::value[] = "%s";
  const char format_spec<char,const char*>::value[] = "%s";
  const char format_spec<char,const uint8*>::value[] = "%s";

  //! \var format_spec<...>::value - Defines wide string formatting type specifications
  const wchar_t format_spec<wchar_t,double>::value[] = L"%llf";
  const wchar_t format_spec<wchar_t,uint64>::value[] = L"%llu";
  const wchar_t format_spec<wchar_t,int64>::value[] = L"%lld";
  const wchar_t format_spec<wchar_t,float>::value[] = L"%lf";
  const wchar_t format_spec<wchar_t,uint32>::value[] = L"%lu";
  const wchar_t format_spec<wchar_t,int32>::value[] = L"%ld";
  const wchar_t format_spec<wchar_t,ulong32>::value[] = L"%lu";
  const wchar_t format_spec<wchar_t,long32>::value[] = L"%ld";
  const wchar_t format_spec<wchar_t,uint16>::value[] = L"%hu";
  const wchar_t format_spec<wchar_t,int16>::value[] = L"%hd";
  const wchar_t format_spec<wchar_t,uint8>::value[] = L"%cu";
  const wchar_t format_spec<wchar_t,int8>::value[] = L"%c";
  const wchar_t format_spec<wchar_t,wchar_t>::value[] = L"%c";
  const wchar_t format_spec<wchar_t,wchar_t*>::value[] = L"%s";
  const wchar_t format_spec<wchar_t,uint16*>::value[] = L"%s";
  const wchar_t format_spec<wchar_t,const wchar_t*>::value[] = L"%s";
  const wchar_t format_spec<wchar_t,const uint16*>::value[] = L"%s";
}
#endif


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


//! \namespace wtl - Windows template library
namespace wtl
{
  //! \var system_class<...>::value - Defines standard controls window class names
  const wchar_t system_class<wchar_t,SystemClass::Animate>::name[] = ANIMATE_CLASSW;
  const wchar_t system_class<wchar_t,SystemClass::DateTime>::name[] = DATETIMEPICK_CLASSW;
  const wchar_t system_class<wchar_t,SystemClass::HotKey>::name[] = HOTKEY_CLASSW;
  const wchar_t system_class<wchar_t,SystemClass::Calendar>::name[] = MONTHCAL_CLASSW;
  const wchar_t system_class<wchar_t,SystemClass::ProgressBar>::name[] = PROGRESS_CLASSW;
  const wchar_t system_class<wchar_t,SystemClass::CoolBar>::name[] = REBARCLASSNAMEW;
  const wchar_t system_class<wchar_t,SystemClass::StatusBar>::name[] = STATUSCLASSNAMEW;
  const wchar_t system_class<wchar_t,SystemClass::ToolBar>::name[] = TOOLBARCLASSNAMEW;
  const wchar_t system_class<wchar_t,SystemClass::ToolTip>::name[] = TOOLTIPS_CLASSW;
  const wchar_t system_class<wchar_t,SystemClass::TrackBar>::name[] = TRACKBAR_CLASSW;
  const wchar_t system_class<wchar_t,SystemClass::Spin>::name[] = UPDOWN_CLASSW;
  
  //! \var system_class<...>::value - Defines common control window class names
  const wchar_t system_class<wchar_t,SystemClass::Button>::name[] = WC_BUTTONW;
  const wchar_t system_class<wchar_t,SystemClass::ComboBox>::name[] = WC_COMBOBOXW;
  const wchar_t system_class<wchar_t,SystemClass::ComboBoxEx>::name[] = WC_COMBOBOXEXW;
  const wchar_t system_class<wchar_t,SystemClass::Edit>::name[] = WC_EDITW;
  const wchar_t system_class<wchar_t,SystemClass::Header>::name[] = WC_HEADERW;
  const wchar_t system_class<wchar_t,SystemClass::ListBox>::name[] = WC_LISTBOXW;
  const wchar_t system_class<wchar_t,SystemClass::IpAddress>::name[] = WC_IPADDRESSW;
  const wchar_t system_class<wchar_t,SystemClass::Link>::name[] = L"SysLink";
  const wchar_t system_class<wchar_t,SystemClass::ListView>::name[] = WC_LISTVIEWW;
  const wchar_t system_class<wchar_t,SystemClass::NativeFont>::name[] = WC_NATIVEFONTCTLW;
  const wchar_t system_class<wchar_t,SystemClass::PageScroller>::name[] = WC_PAGESCROLLERW;
  const wchar_t system_class<wchar_t,SystemClass::ScrollBar>::name[] = WC_SCROLLBARW;
  const wchar_t system_class<wchar_t,SystemClass::Static>::name[] = WC_STATICW;
  const wchar_t system_class<wchar_t,SystemClass::Tab>::name[] = WC_TABCONTROLW;
  const wchar_t system_class<wchar_t,SystemClass::TreeView>::name[] = WC_TREEVIEWW;
}

