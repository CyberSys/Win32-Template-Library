////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\event\CreateWindowEvent.hpp
//! \brief Encapsulates the WM_CREATE message in the 'CreateWindow' event
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CREATE_WINDOW_EVENT_HPP
#define WTL_CREATE_WINDOW_EVENT_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<ENC,WindowMessage::CREATE> - Event arguments for Win32 message 'WM_CREATE'
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::CREATE> 
  { 
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias CreateStruct - Define window creation data type
    using CreateStruct = getType<encoding_char_t<ENC>,::CREATESTRUCTA,::CREATESTRUCTW>;

    //! \var message - Define message identifier
    static constexpr WindowMessage message = WindowMessage::CREATE;

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs::EventArgs
    //! Decode the arguments for win32 message 'WM_CREATE' 
    //! 
    //! \param[in] const& wnd - Window receiving message
    //! \param[in] w - Nothing
    //! \param[in] l - Pointer to ::CREATESTRUCT 
    ///////////////////////////////////////////////////////////////////////////////
    EventArgs(const HWnd& wnd, ::WPARAM w, ::LPARAM l) : Data(*opaque_cast<CreateStruct>(l)),
                                                         Menu(Data.hMenu, AllocType::WeakRef),
                                                         Rect(PointL(Data.x,Data.y), SizeL(Data.cx,Data.cy)),
                                                         Style(enum_cast<WindowStyle>(Data.style)),
                                                         StyleEx(enum_cast<WindowStyleEx>(Data.dwExStyle)),
                                                         Class(Data.lpszClass),
                                                         Name(Data.lpszName)
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs::~EventArgs
    //! Saves changes to the window data
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~EventArgs() 
    {
      /*Data.hMenu = Menu;
      Data.style = enum_cast(Style); 
      Data.dwExStyle = enum_cast(StyleEx);
      Data.x = Rect.left;
      Data.y = Rect.top;
      Data.cx = Rect.width();
      Data.cy = Rect.height();
      Data.lpszClass = Class.toString();
      Data.lpszName = Name;*/
    }
    
    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------

    // -------------------- REPRESENTATION ---------------------
  protected:
    CreateStruct&  Data;         //!< Window creation data  (NB: Must be initialised first)

  public:
    HMenu           Menu;        //!< Menu handle
    RectL           Rect;        //!< Window rectangle
    WindowStyle     Style;       //!< Window style
    WindowStyleEx   StyleEx;     //!< Extended window style
    ResourceId<ENC> Class;       //!< Window class id
    const char_t*   Name;        //!< Window name
  };


  ///////////////////////////////////////////////////////////////////////////////
  //! \alias CreateWindowEventArgs - Arguments for 'CreateWindow' Event (ie. WM_CREATE)
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CreateWindowEventArgs = EventArgs<ENC,WindowMessage::CREATE>;

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias CreateWindowEventDelegate - Delegate for 'CreateWindow' event (ie. WM_CREATE)
  //! 
  //! \tparam ENC - Window character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CreateWindowEventDelegate = EventArgsDelegate<ENC,WindowMessage::CREATE>;

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias CreateWindowEventHandler - Handler for 'CreateWindow' event (ie. WM_CREATE)
  //! 
  //! \tparam ENC - Window character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CreateWindowEventHandler = EventHandler<ENC,WindowMessage::CREATE,CreateWindowEventDelegate<ENC>>;
  
}

#endif // WTL_CREATE_WINDOW_EVENT_HPP
