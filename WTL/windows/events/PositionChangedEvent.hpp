//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\event\PositionChangedEvent.hpp
//! \brief Encapsulates the WM_WINDOWPOSCHANGED message in the 'PositionChanged' event
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_POSITION_CHANGED_EVENT
#define WTL_POSITION_CHANGED_EVENT

#include "wtl/WTL.hpp"
#include "wtl/casts/OpaqueCast.hpp"                //!< OpaqueCast
#include "wtl/windows/MessageEvent.hpp"            //!< Event
#include "wtl/utils/Rectangle.hpp"              //!< Rect
#include "wtl/traits/WindowTraits.hpp"             //!< HWnd

//! \namespace wtl - Windows template library
namespace wtl {
//! \namespace events - WTL Window events
namespace events 
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
    
    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------

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

    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs::~EventArgs
    //! Virtual d-tor
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~EventArgs() = default;
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    ::WINDOWPOS&  Data;     //!< Message data, must be initialized first

  public:
    HWnd   Previous,        //!< Preceeding window in the Z-order
           Window;          //!< Window
    RectL  Rect;            //!< New window rectangle
  };
  
  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias PositionChangedEvent - Defines 'PositionChanged' event (ie. WM_WINDOWPOSCHANGED)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using PositionChangedEvent = MessageEvent<ENC,WindowMessage::WINDOWPOSCHANGED>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias PositionChangedEventArgs - Arguments for 'PositionChanged' Event (ie. WM_WINDOWPOSCHANGED)
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using PositionChangedEventArgs = EventArgs<ENC,WindowMessage::WINDOWPOSCHANGED>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias PositionChangedEventHandler - Handler for 'PositionChanged' event (ie. WM_WINDOWPOSCHANGED)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using PositionChangedEventHandler = MessageEventHandler<ENC,WindowMessage::WINDOWPOSCHANGED>;
  
} // namespace events
} // namespace wtl

#endif // WTL_POSITION_CHANGED_EVENT
