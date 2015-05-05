//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\event\NotifyEventData.hpp
//! \brief Defines the native data structures sent with WM_NOTIFY messages
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_NOTIFY_EVENT_DATA_HPP
#define WTL_NOTIFY_EVENT_DATA_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias notify_data - Defines the data-structure sent with each Win32 WM_NOTIFY message
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam EVENT - Notification identifier type
  //! \tparam CODE - Notification
  /////////////////////////////////////////////////////////////////////////////////////////
  /*template <Encoding ENC, typename EVENT, EVENT CODE>
  struct notify_data { using type = ::NMHDR; };*/
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias notify_data_t - Type accessor for notify message data-types
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam EVENT - Notification identifier type
  //! \tparam CODE - Notification
  /////////////////////////////////////////////////////////////////////////////////////////
  /*template <Encoding ENC, typename EVENT, EVENT CODE>
  using notify_data_t = typename notify_data<ENC,EVENT,CODE>::type;*/


  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias notify_data<NotifyMessage> - Define the data-structures used with standard WM_NOTIFY messages
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  /*template <Encoding ENC> struct notify_data<ENC,NotifyMessage,NotifyMessage::Click>       { using type = ::NMCLICK;      };
  template <Encoding ENC> struct notify_data<ENC,NotifyMessage,NotifyMessage::RClick>      { using type = ::NMCLICK;      };
  template <Encoding ENC> struct notify_data<ENC,NotifyMessage,NotifyMessage::NcHitTest>   { using type = ::NMMOUSE;      };
  template <Encoding ENC> struct notify_data<ENC,NotifyMessage,NotifyMessage::SetCursor>   { using type = ::NMCLICK;      };
  template <Encoding ENC> struct notify_data<ENC,NotifyMessage,NotifyMessage::KeyDown>     { using type = ::NMKEY;        };
  template <Encoding ENC> struct notify_data<ENC,NotifyMessage,NotifyMessage::Char>        { using type = ::NMCHAR;       };
  template <Encoding ENC> struct notify_data<ENC,NotifyMessage,NotifyMessage::CustomDraw>  { using type = ::NMCUSTOMDRAW;     };
  template <Encoding ENC> struct notify_data<ENC,ListViewEvent,ListViewEvent::CustomDraw>  { using type = ::NMLVCUSTOMDRAW;   };*/


}

#endif // WTL_NOTIFY_EVENT_DATA_HPP
