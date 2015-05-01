////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\event\OwnerMeasureEvent.hpp
//! \brief Encapsulates the WM_MEASUREITEM message in the 'OwnerMeasure' event
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_OWNER_MEASURE_EVENT_HPP
#define WTL_OWNER_MEASURE_EVENT_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct EventArgs<WindowMessage::MEASUREITEM> - Event arguments for Win32 message 'WM_MEASUREITEM'
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EventArgs<ENC,WindowMessage::MEASUREITEM>
  {  
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;

    //! \alias MeasureData - Define measure data type
    using MeasureData = ::MEASUREITEMSTRUCT;
    
    //! \var encoding - Define character encoding
    static constexpr Encoding encoding = ENC;
    
    //! \var message - Define message identifier
    static constexpr WindowMessage message = WindowMessage::MEASUREITEM;
    
    // -------------------- REPRESENTATION ---------------------
  protected:
    MeasureData&      Data;             //!< Encapsulates the input measure data (NB: Must be initialized before following fields)

  public:
    SizeL             Size;             //!< Item size
    int32             Item;             //!< Zero-based item index
    WindowId          Ident;            //!< [CONTROL] Control id
    CommandId         Action;           //!< [MENU] Command Id
    OwnerDrawControl  CtrlType;         //!< Control type

    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs<MEASUREITEM>::EventArgs
    //! Creates arguments for 'OwnerMeasure' Event (ie. WM_MEASUREITEM)
    //! 
    //! \param[in] w - Not used
    //! \param[in] l - Not used
    ///////////////////////////////////////////////////////////////////////////////
    EventArgs(::WPARAM w, ::LPARAM l) : Data(*opaque_cast<MeasureData>(l)), 
                                        Size(Data.itemWidth, Data.itemHeight),
                                        Item(Data.itemID), 
                                        Action(static_cast<CommandId>(Data.itemID)), 
                                        Ident(static_cast<WindowId>(Data.CtlID)), 
                                        CtrlType(enum_cast<OwnerDrawControl>(Data.CtlType))
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs::~EventArgs
    //! Save values
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~EventArgs()
    {
      Data.itemWidth = Size.width;
      Data.itemHeight = Size.height;
    }
    
    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			
    
    ///////////////////////////////////////////////////////////////////////////////
    // EventArgs<MEASUREITEM>::reflect const
    //! Reflects the event back to the originator control
    //! 
    //! \param[in] const& ctrl - Originator control
    //! \return LResult - Message result and routing
    ///////////////////////////////////////////////////////////////////////////////
    LResult reflect(const HWnd& ctrl) const
    {
      // Reflect message
      return send_message<encoding,message+WindowMessage::REFLECT>(ctrl, opaque_cast(Ident), opaque_cast(Data));
    }
    
    // ----------------------- MUTATORS ------------------------
  };
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerMeasureEvent - Defines 'OwnerMeasure' event (ie. WM_MEASUREITEM)
  //! 
  //! \tparam ENC - Window character encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerMeasureEvent = MessageEvent<ENC, WindowMessage::MEASUREITEM>;
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerMeasureEventArgs - Arguments for 'OwnerMeasure' Event (ie. WM_MEASUREITEM)
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerMeasureEventArgs = EventArgs<ENC,WindowMessage::MEASUREITEM>;

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias OwnerMeasureEventHandler - Handler for 'OwnerMeasure' event (ie. WM_MEASUREITEM)
  //! 
  //! \tparam ENC - Window character encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using OwnerMeasureEventHandler = typename OwnerMeasureEvent<ENC>::delegate_t;

}

#endif // WTL_OWNER_MEASURE_EVENT_HPP
