//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\event\PositionChangedEvent.hpp
//! \brief Encapsulates the WM_WINDOWPOSCHANGED message in the 'PositionChanged' event
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_POSITION_CHANGED_EVENT
#define WTL_POSITION_CHANGED_EVENT

#include "wtl/WTL.hpp"
#include "wtl/casts/OpaqueCast.hpp"                //!< OpaqueCast
#include "wtl/windows/MessageEvent.hpp"            //!< MessageEvent
#include "wtl/utils/Rectangle.hpp"                 //!< Rect
#include "wtl/traits/WindowTraits.hpp"             //!< HWnd

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<WindowMessage::WINDOWPOSCHANGED> - Event arguments for Win32 message 'WM_WINDOWPOSCHANGED'
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::WINDOWPOSCHANGED>
  {  
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    //! \var encoding - Define message character encoding 
    static constexpr Encoding  encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage  message = WindowMessage::WINDOWPOSCHANGED;
    
    //! \var unhandled - Define unhandled result
    static constexpr ::LRESULT  unhandled = unhandled_result<message>::value;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    ::WINDOWPOS&  Data;     //!< Message data, must be initialized first

  public:
    HWnd   Previous,        //!< Preceeding window in the Z-order
           Window;          //!< Window
    RectL  Rect;            //!< New window rectangle

    // ------------------------------------- CONSTRUCTION -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs<WindowMessage::WINDOWPOSCHANGED>::EventArgs
    //! Creates arguments for 'PositionChanged' Event (ie. WM_WINDOWPOSCHANGED)
    //! 
    //! \param[in] w - Not used
    //! \param[in] l - Not used
    /////////////////////////////////////////////////////////////////////////////////////////
    EventArgs(::WPARAM w, ::LPARAM l) : Data(*opaque_cast<::WINDOWPOS>(l)), 
                                        Previous(Data.hwndInsertAfter, AllocType::WeakRef),
                                        Window(Data.hwnd, AllocType::WeakRef),
                                        Rect(PointL(Data.x,Data.y), SizeL(Data.cx,Data.cy))
                                                         
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY  -------------------------------

    ENABLE_COPY(EventArgs);      //!< Can be shallow copied
    ENABLE_MOVE(EventArgs);      //!< Can be moved
    ENABLE_POLY(EventArgs);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };
  
  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias PositionChangedEventArgs - Defines arguments type for the 'PositionChanged' event 
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using PositionChangedEventArgs = EventArgs<ENC,WindowMessage::WINDOWPOSCHANGED>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias PositionChangedEvent - Defines the signature of 'PositionChanged' event handlers  [Pass by value]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using PositionChangedEvent = Event<LResult, PositionChangedEventArgs<ENC>>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias PositionChangedEventHandler - Defines the delegate type for the 'PositionChanged' event
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using PositionChangedEventHandler = handler_t<PositionChangedEvent<ENC>>;
  
} // namespace wtl

#endif // WTL_POSITION_CHANGED_EVENT
