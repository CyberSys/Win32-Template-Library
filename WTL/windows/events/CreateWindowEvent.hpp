//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\CreateWindowEvent.hpp
//! \brief Encapsulates the WM_CREATE message in the 'CreateWindow' event
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CREATE_WINDOW_EVENT_HPP
#define WTL_CREATE_WINDOW_EVENT_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/EventArgs.hpp>            //!< EventArgs

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<ENC,WindowMessage::Create> - Event arguments for Win32 message 'WM_CREATE'
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::Create> 
  { 
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias CreationData - Define window creation data type
    using CreationData = choose_t<ENC,::CREATESTRUCTA,::CREATESTRUCTW>;
    
    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WindowMessage::Create;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    CreationData&   Data;        //!< Window creation data  (NB: Must be initialised first)

  public:
    HMenu           Menu;        //!< [¬Child] Menu handle
    WindowId        Ident;       //!< [Child] Child window Id
    RectL           WndRect;     //!< Window rectangle
    WindowStyle     Style;       //!< Window style
    WindowStyleEx   StyleEx;     //!< Extended window style
    resource_t      Class;       //!< Window class id
    const char_t*   Name;        //!< Window name

    // ------------------------------------- CONSTRUCTION -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs::EventArgs
    //! Decode the arguments for win32 message 'WM_CREATE' 
    //! 
    //! \param[in] w - Not used
    //! \param[in] l - Pointer to ::CREATESTRUCT 
    /////////////////////////////////////////////////////////////////////////////////////////
    EventArgs(::WPARAM w, ::LPARAM l) : Data(*opaque_cast<CreationData>(l)),
                                        Menu(Data.hMenu, AllocType::WeakRef),
                                        Ident(static_cast<WindowId>(LOWORD(Data.hMenu))),
                                        WndRect(PointL(Data.x,Data.y), SizeL(Data.cx,Data.cy)),
                                        Style(enum_cast<WindowStyle>(Data.style)),
                                        StyleEx(enum_cast<WindowStyleEx>(Data.dwExStyle)),
                                        Class(Data.lpszClass),
                                        Name(Data.lpszName)
    {}

	  // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(EventArgs);      //!< Can be shallow copied
    ENABLE_MOVE(EventArgs);      //!< Can be moved
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs::~EventArgs
    //! Saves changes to the window data
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~EventArgs() 
    {
      // Save properties
      Data.style = enum_cast(Style); 
      Data.dwExStyle = enum_cast(StyleEx);
      Data.x = WndRect.Left;
      Data.y = WndRect.Top;
      Data.cx = WndRect.width();
      Data.cy = WndRect.height();
      Data.lpszClass = Class.toString();
      Data.lpszName = Name;

      // Set menu or child id
      //Data.hMenu = (Style & WindowStyle::Child ? reinterpret_cast<::HMENU>(Ident) : Menu.get());
    }
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };


  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CreateWindowEventArgs - Defines arguments type for the 'CreateWindow' event 
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CreateWindowEventArgs = EventArgs<ENC,WindowMessage::Create>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CreateWindowEvent - Defines the signature of the 'CreateWindow' event handler  [Pass by reference]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CreateWindowEvent = Event<LResult, CreateWindowEventArgs<ENC>& >;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CreateWindowEventHandler - Defines the delegate type for the 'CreateWindow' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CreateWindowEventHandler = handler_t<CreateWindowEvent<ENC>>;
  

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias WindowCreatedEvent - Defines the signature of the 'WindowCreated' event 
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using WindowCreatedEvent = Event<LResult>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias WindowCreatedEventHandler - Defines the delegate type for the 'WindowCreated' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using WindowCreatedEventHandler = handler_t<WindowCreatedEvent<ENC>>;
  
} // namespace wtl

#endif // WTL_CREATE_WINDOW_EVENT_HPP
