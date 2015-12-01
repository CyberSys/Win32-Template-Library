//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\event\PaintWindowEvent.hpp
//! \brief Encapsulates the WM_PAINT message in the 'PaintWindow' event
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_PAINT_WINDOW_EVENT_HPP
#define WTL_PAINT_WINDOW_EVENT_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/EventArgs.hpp>              //!< EventArgs
#include <wtl/gdi/DeviceContext.hpp>              //!< DeviceContext
#include <wtl/utils/Rectangle.hpp>                //!< Rect
#include <wtl/traits/WindowTraits.hpp>            //!< HWnd

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<WindowMessage::Paint> - Event arguments for Win32 message 'WM_PAINT'
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::Paint>
  {  
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WindowMessage::Paint;
    
    //! \struct PaintCycle - Encapsulates the paint cycle for a window's client area
    struct PaintCycle : PAINTSTRUCT
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
      //! \alias base - Define base type
      using base = PAINTSTRUCT;
      
      // ----------------------------------- REPRESENTATION -----------------------------------
  
      // ------------------------------------- CONSTRUCTION -----------------------------------
	
      /////////////////////////////////////////////////////////////////////////////////////////
      // PaintCycle::PaintCycle
      //! Initiates the paint cycle for a window
      //! 
      //! \param[in] const& wnd - Window being painted
      /////////////////////////////////////////////////////////////////////////////////////////
      PaintCycle(const HWnd& wnd) : Window(wnd)
      {
        // Begin paint cycle
        ::BeginPaint(Window, static_cast<base*>(this));
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      // PaintCycle::~PaintCycle
      //! Ends the paint cycle for a window
      /////////////////////////////////////////////////////////////////////////////////////////
      virtual ~PaintCycle()
      {
        // End paint cycle (Validate update region)
        ::EndPaint(Window, static_cast<base*>(this));
      }
      
	    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

      const HWnd&   Window;     //!< Handle of window being painted
    };
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    PaintCycle     Data;             //!< Encapsulates the paint cycle (NB: Must be initialized before following fields)

  public:
    DeviceContext  Graphics;         //!< Device context clipped to non-client area 
    RectL          Rect;             //!< Drawing/update rectangle
    bool           EraseBackground;  //!< Whether to erase window background 
    
  protected:
    bool           IncUpdate,        //!< Reserved
                   Restore;          //!< Reserved

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs<WindowMessage::Paint>::EventArgs
    //! Creates arguments for 'PaintWindow' Event (ie. WM_PAINT)
    //! 
    //! \param[in] const& wnd - Window receiving message
    //! \param[in] w - Not used
    //! \param[in] l - Not used
    /////////////////////////////////////////////////////////////////////////////////////////
    EventArgs(const HWnd& wnd, ::WPARAM w, ::LPARAM l) : Data(wnd), 
                                                         Graphics(Data.hdc),
                                                         Rect(Data.rcPaint),
                                                         EraseBackground(Data.fErase != False), 
                                                         IncUpdate(Data.fIncUpdate != False), 
                                                         Restore(Data.fRestore != False)
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(EventArgs);      //!< Can be shallow copied
    ENABLE_MOVE(EventArgs);      //!< Can be moved
    ENABLE_POLY(EventArgs);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };
  
  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias PaintWindowEventArgs - Defines arguments type for the 'PaintWindow' event 
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using PaintWindowEventArgs = EventArgs<ENC,WindowMessage::Paint>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias PaintWindowEvent - Defines the signature of 'PaintWindow' event handlers  [Pass by reference]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using PaintWindowEvent = Event<LResult, PaintWindowEventArgs<ENC>& >;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias PaintWindowEventHandler - Defines the delegate type for the 'PaintWindow' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using PaintWindowEventHandler = handler_t<PaintWindowEvent<ENC>>;
  
} // namespace wtl

#endif // WTL_PAINT_WINDOW_EVENT_HPP
