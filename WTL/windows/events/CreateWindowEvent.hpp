//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\CreateWindowEvent.hpp
//! \brief Encapsulates the WM_CREATE message in the 'CreateWindow' event
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CREATE_WINDOW_EVENT_HPP
#define WTL_CREATE_WINDOW_EVENT_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<ENC,WindowMessage::CREATE> - Event arguments for Win32 message 'WM_CREATE'
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::CREATE> 
  { 
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias CreateStruct - Define window creation data type
    using CreateStruct = getType<char_t,::CREATESTRUCTA,::CREATESTRUCTW>;
    
    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WindowMessage::CREATE;
    
    //! \var unhandled - Define unhandled result
    static constexpr ::LRESULT  unhandled = unhandled_result<message>::value;

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs::EventArgs
    //! Decode the arguments for win32 message 'WM_CREATE' 
    //! 
    //! \param[in] w - Not used
    //! \param[in] l - Pointer to ::CREATESTRUCT 
    /////////////////////////////////////////////////////////////////////////////////////////
    EventArgs(::WPARAM w, ::LPARAM l) : Data(*opaque_cast<CreateStruct>(l)),
                                        Menu(Data.hMenu, AllocType::WeakRef),
                                        Ident(static_cast<WindowId>(LOWORD(Data.hMenu))),
                                        Rect(PointL(Data.x,Data.y), SizeL(Data.cx,Data.cy)),
                                        Style(enum_cast<WindowStyle>(Data.style)),
                                        StyleEx(enum_cast<WindowStyleEx>(Data.dwExStyle)),
                                        Class(Data.lpszClass),
                                        Name(Data.lpszName)
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs::~EventArgs
    //! Saves changes to the window data
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~EventArgs() 
    {
      // Save properties
      Data.style = enum_cast(Style); 
      Data.dwExStyle = enum_cast(StyleEx);
      Data.x = Rect.left;
      Data.y = Rect.top;
      Data.cx = Rect.width();
      Data.cy = Rect.height();
      Data.lpszClass = Class.toString();
      Data.lpszName = Name;

      // Set menu or child id
      //Data.hMenu = (Style & WindowStyle::Child ? reinterpret_cast<::HMENU>(Ident) : Menu.get());
    }
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    CreateStruct&   Data;        //!< Window creation data  (NB: Must be initialised first)

  public:
    HMenu           Menu;        //!< [¬Child] Menu handle
    WindowId        Ident;       //!< [Child] Child window Id
    RectL           Rect;        //!< Window rectangle
    WindowStyle     Style;       //!< Window style
    WindowStyleEx   StyleEx;     //!< Extended window style
    resource_t      Class;       //!< Window class id
    const char_t*   Name;        //!< Window name
  };


  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CreateWindowEvent - Defines 'CreateWindow' event (ie. WM_CREATE)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CreateWindowEvent = MessageEvent<ENC, WindowMessage::CREATE>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CreateWindowEventArgs - Arguments for 'CreateWindow' Event (ie. WM_CREATE)
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CreateWindowEventArgs = EventArgs<ENC,WindowMessage::CREATE>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CreateWindowEventHandler - Handler for 'CreateWindow' event (ie. WM_CREATE)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CreateWindowEventHandler = MessageEventHandler<ENC,WindowMessage::CREATE>;

}

#endif // WTL_CREATE_WINDOW_EVENT_HPP
