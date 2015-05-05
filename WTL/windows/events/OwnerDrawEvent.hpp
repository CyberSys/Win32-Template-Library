//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\event\OwnerDrawEvent.hpp
//! \brief Encapsulates the WM_DRAWITEM message in the 'OwnerDraw' event
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_OWNER_DRAW_EVENT_HPP
#define WTL_OWNER_DRAW_EVENT_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ControlEventArgs<WindowMessage::DRAWITEM> - Event arguments for Win32 message 'WM_DRAWITEM' raised from controls
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ControlEventArgs<ENC,WindowMessage::DRAWITEM,uint16,default_t<uint16>::value>
  {  
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;

    //! \alias PaintData - Define paint data type
    using PaintData = ::DRAWITEMSTRUCT;
    
    //! \var encoding - Define character encoding
    static constexpr Encoding encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage message = WindowMessage::DRAWITEM;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    PaintData&        Data;             //!< Encapsulates the input paint data (NB: Must be initialized before following fields)

  public:
    OwnerDrawAction   Action;           //!< Type of drawing requested
    OwnerDrawControl  CtrlType;         //!< Control type
    DeviceContext     Graphics;         //!< Device context clipped to non-client area 
    WindowId          Ident;            //!< Control id
    int32             Item;             //!< Zero-based item index
    RectL             Rect;             //!< Drawing/update rectangle
    HWnd              Sender;           //!< Control handle

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // ControlEventArgs<DRAWITEM>::ControlEventArgs
    //! Creates arguments for 'OwnerDraw' event from controls (ie. WM_DRAWITEM)
    //! 
    //! \param[in] w - Originator window id
    //! \param[in] l - Paint data
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Event is not for a control
    /////////////////////////////////////////////////////////////////////////////////////////
    ControlEventArgs(::WPARAM w, ::LPARAM l) : Data(*opaque_cast<PaintData>(l)), 
                                               Action(enum_cast<OwnerDrawAction>(Data.itemAction)), 
                                               CtrlType(enum_cast<OwnerDrawControl>(Data.CtlType)), 
                                               Graphics(Data.hDC),
                                               Rect(Data.rcItem),
                                               Item(Data.itemID), 
                                               Ident(static_cast<WindowId>(Data.CtlID)), 
                                               Sender(Data.hwndItem, AllocType::WeakRef)
    {
      PARAM_INVARIANT(Data,enum_cast<OwnerDrawControl>(Data.CtlType) != OwnerDrawControl::Menu);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ControlEventArgs::~ControlEventArgs
    //! Virtual d-tor
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~ControlEventArgs()
    {}
    
    // --------------------------------------- STATIC ---------------------------------------

    // -------------------------------------- ACCESSORS --------------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ControlEventArgs<DRAWITEM>::reflect const
    //! Reflects the event back to the originator control
    //! 
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult reflect() const
    {
      // Reflect message
      return send_message<encoding,message+WindowMessage::REFLECT>(Sender, opaque_cast(Ident), opaque_cast(Data));
    }
    
    // --------------------------------------- MUTATORS --------------------------------------
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerDrawCtrlEvent - Defines 'OwnerDraw' event for controls (ie. WM_DRAWITEM)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerDrawCtrlEvent = ControlEvent<ENC, WindowMessage::DRAWITEM>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerDrawCtrlEventArgs - Arguments for 'OwnerDraw' event for controls (ie. WM_DRAWITEM)
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerDrawCtrlEventArgs = ControlEventArgs<ENC,WindowMessage::DRAWITEM>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerDrawCtrlEventHandler - Handler for 'OwnerDraw' event for controls (ie. WM_DRAWITEM)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerDrawCtrlEventHandler = ControlEventHandler<ENC,WindowMessage::DRAWITEM>;






  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<WindowMessage::DRAWITEM> - Event arguments for Win32 message 'WM_DRAWITEM' raised for menus
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::DRAWITEM>
  {  
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;

    //! \alias PaintData - Define paint data type
    using PaintData = ::DRAWITEMSTRUCT;
    
    //! \var encoding - Define character encoding
    static constexpr Encoding encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage message = WindowMessage::DRAWITEM;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    PaintData&        Data;             //!< Encapsulates the input paint data (NB: Must be initialized before following fields)

  public:
    OwnerDrawAction   Action;           //!< Type of drawing requested
    DeviceContext     Graphics;         //!< Device context clipped to menu area 
    CommandId         Ident;            //!< Action id
    RectL             Rect;             //!< Drawing/update rectangle
    HMenu             Menu;             //!< Control handle

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs<DRAWITEM>::EventArgs
    //! Creates arguments for 'OwnerDraw' event from controls (ie. WM_DRAWITEM)
    //! 
    //! \param[in] w - Originator window id
    //! \param[in] l - Paint data
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Event is not for a menu
    /////////////////////////////////////////////////////////////////////////////////////////
    EventArgs(::WPARAM w, ::LPARAM l) : Data(*opaque_cast<PaintData>(l)), 
                                        Action(enum_cast<OwnerDrawAction>(Data.itemAction)), 
                                        Graphics(Data.hDC),
                                        Ident(static_cast<CommandId>(Data.itemID)), 
                                        Menu((::HMENU)Data.hwndItem, AllocType::WeakRef),
                                        Rect(Data.rcItem)
    {
      PARAM_INVARIANT(Data,enum_cast<OwnerDrawControl>(Data.CtlType) == OwnerDrawControl::Menu);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // EventArgs::~EventArgs
    //! Virtual d-tor
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~EventArgs()
    {}
    
    // --------------------------------------- STATIC ---------------------------------------

    // -------------------------------------- ACCESSORS --------------------------------------			
    
    // --------------------------------------- MUTATORS --------------------------------------
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerDrawMenuEvent - Defines 'OwnerDraw' event for menus (ie. WM_DRAWITEM)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerDrawMenuEvent = MessageEvent<ENC, WindowMessage::DRAWITEM>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerDrawMenuEventArgs - Arguments for 'OwnerDraw' event for menus (ie. WM_DRAWITEM)
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerDrawMenuEventArgs = EventArgs<ENC,WindowMessage::DRAWITEM>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerDrawMenuEventHandler - Handler for 'OwnerDraw' event for menus (ie. WM_DRAWITEM)
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerDrawMenuEventHandler = MessageEventHandler<ENC,WindowMessage::DRAWITEM>;
}

#endif // WTL_OWNER_DRAW_EVENT_HPP
