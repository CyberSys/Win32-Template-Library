//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\events\ButtonEvents.hpp
//! \brief Provides argument/delegate/handler types for events for the standard button control
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BUTTON_EVENTS_HPP
#define WTL_BUTTON_EVENTS_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/ControlEventArgs.hpp>       //!< ControlEventArgs,Event
#include <wtl/platform/ControlStyles.hpp>         //!< Styles+Notifications

//! \namespace wtl - Windows template library
namespace wtl 
{
  namespace
  {
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \alias button_event_args_t - Defines argument types for button control events
    //! 
    //! \tparam ENC - Window character encoding
    //! \tparam MSG - Window message
    //! \tparam CODE - Notification message 
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, WindowMessage MSG, ButtonNotification CODE>
    using button_event_args_t = ControlEventArgs<ENC,MSG,ButtonNotification,CODE>;
  }

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ButtonClickEvent... - Defines arguments type, handle signature, and delegate type for Button 'Click' event 
  //! \remarks Encapsulates the WM_COMMAND message containing a BN_CLICKED notification.  [Arguments passed by value]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC> using ButtonClickEventArgs    = button_event_args_t<ENC,WindowMessage::Command,ButtonNotification::Click>;
  template <Encoding ENC> using ButtonClickEvent        = Event<LResult, ButtonClickEventArgs<ENC>>;
  template <Encoding ENC> using ButtonClickEventHandler = handler_t<ButtonClickEvent<ENC>>;

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ButtonGainFocusEvent... - Defines arguments type, handle signature, and delegate type for Button 'GainFocus' event
  //! \remarks Encapsulates the WM_COMMAND message containing a BN_SETFOCUS notification.  [Arguments passed by value]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC> using ButtonGainFocusEventArgs    = button_event_args_t<ENC,WindowMessage::Command,ButtonNotification::SetFocus>;
  template <Encoding ENC> using ButtonGainFocusEvent        = Event<LResult, ButtonGainFocusEventArgs<ENC>>;
  template <Encoding ENC> using ButtonGainFocusEventHandler = handler_t<ButtonGainFocusEvent<ENC>>;

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ButtonLoseFocusEvent... - Defines arguments type, handle signature, and delegate type for Button 'LoseFocus' event
  //! \remarks Encapsulates the WM_COMMAND message containing a BN_KILLFOCUS notification.  [Arguments passed by value]
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC> using ButtonLoseFocusEventArgs    = button_event_args_t<ENC,WindowMessage::Command,ButtonNotification::KillFocus>;
  template <Encoding ENC> using ButtonLoseFocusEvent        = Event<LResult, ButtonLoseFocusEventArgs<ENC>>;
  template <Encoding ENC> using ButtonLoseFocusEventHandler = handler_t<ButtonLoseFocusEvent<ENC>>;



  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias Button...EventHander - Defines handler types for Button events (ie. WM_COMMAND -> {BN_xxxx} )
  //! 
  //! \tparam ENC - Window character encoding 
  //! \tparam SENDER - Originator window type
  /////////////////////////////////////////////////////////////////////////////////////////
  /*template <Encoding ENC, typename SENDER> using ButtonClickEventHandler       = ButtonEventHandler<ENC,SENDER,ButtonEvent::Click>;
  template <Encoding ENC, typename SENDER> using ButtonDisabledEventHandler    = ButtonEventHandler<ENC,SENDER,ButtonEvent::Disabled>;
  template <Encoding ENC, typename SENDER> using ButtonDoubleClickEventHandler = ButtonEventHandler<ENC,SENDER,ButtonEvent::DoubleClick>;
  template <Encoding ENC, typename SENDER> using ButtonPaintEventHandler       = ButtonEventHandler<ENC,SENDER,ButtonEvent::Paint>;   
  template <Encoding ENC, typename SENDER> using ButtonPushedEventHandler      = ButtonEventHandler<ENC,SENDER,ButtonEvent::Pushed>;
  template <Encoding ENC, typename SENDER> using ButtonUnpushedEventHandler    = ButtonEventHandler<ENC,SENDER,ButtonEvent::Unpushed>;
  template <Encoding ENC, typename SENDER> using ButtonLoseFocusEventHandler   = ButtonEventHandler<ENC,SENDER,ButtonEvent::KillFocus>;
  template <Encoding ENC, typename SENDER> using ButtonGainFocusEventHandler   = ButtonEventHandler<ENC,SENDER,ButtonEvent::SetFocus>;*/
  
  

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ListViewEventHander - Defines a handler type for a ListView event (ie. WM_NOTIFY -> {LVN_xxxx} )
  //! 
  //! \tparam ENC - Window character encoding 
  //! \tparam SENDER - Originator window type
  //! \tparam CODE - Notification
  /////////////////////////////////////////////////////////////////////////////////////////
  //template <Encoding ENC, typename SENDER, ListViewEvent CODE> using ListViewEventHandler = NotifyEventHandler<ENC,SENDER,ListViewEvent,CODE>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ListView...EventHander - Defines handler types for ListView events (ie. WM_NOTIFY -> {LVN_xxxx} )
  //! 
  //! \tparam ENC - Window character encoding 
  //! \tparam SENDER - Originator window type
  /////////////////////////////////////////////////////////////////////////////////////////
  /*template <Encoding ENC, typename SENDER> using ListViewCustomDrawEventHandler     = ListViewEventHandler<ENC,SENDER,ListViewEvent::CustomDraw>;
  template <Encoding ENC, typename SENDER> using ListViewColumnDropDownEventHandler = ListViewEventHandler<ENC,SENDER,ListViewEvent::ColumnDropDown>; 			
  template <Encoding ENC, typename SENDER> using ListViewColumnOverflowClickEventHandler = ListViewEventHandler<ENC,SENDER,ListViewEvent::ColumnOverflowClick>; 
  template <Encoding ENC, typename SENDER> using ListViewColumnClickEventHandler    = ListViewEventHandler<ENC,SENDER,ListViewEvent::ColumnClick>; 		
  template <Encoding ENC, typename SENDER> using ListViewDeleteItemEventHandler     = ListViewEventHandler<ENC,SENDER,ListViewEvent::DeleteItem>; 			
  template <Encoding ENC, typename SENDER> using ListViewDeleteAllItemsEventHandler = ListViewEventHandler<ENC,SENDER,ListViewEvent::DeleteAllItems>; 	
  template <Encoding ENC, typename SENDER> using ListViewBeginDragEventHandler      = ListViewEventHandler<ENC,SENDER,ListViewEvent::BeginDrag>; 			
  template <Encoding ENC, typename SENDER> using ListViewBeginRDragEventHandler     = ListViewEventHandler<ENC,SENDER,ListViewEvent::BeginRDrag>; 			
  template <Encoding ENC, typename SENDER> using ListViewBeginLabelEditEventHandler = ListViewEventHandler<ENC,SENDER,choose<ENC>(ListViewEvent::BeginLabelEditA,ListViewEvent::BeginLabelEditW)>; 
  template <Encoding ENC, typename SENDER> using ListViewBeginMarqueeEventHandler   = ListViewEventHandler<ENC,SENDER,ListViewEvent::BeginMarquee>; 				
  template <Encoding ENC, typename SENDER> using ListViewBeginScrollEventHandler    = ListViewEventHandler<ENC,SENDER,ListViewEvent::BeginScroll>; 				
  template <Encoding ENC, typename SENDER> using ListViewEndLabelEditEventHandler   = ListViewEventHandler<ENC,SENDER,choose<ENC>(ListViewEvent::EndLabelEditA,ListViewEvent::EndLabelEditW)>; 	
  template <Encoding ENC, typename SENDER> using ListViewEndScrollEventHandler      = ListViewEventHandler<ENC,SENDER,ListViewEvent::EndScroll>; 				  
  template <Encoding ENC, typename SENDER> using ListViewKeyDownEventHandler        = ListViewEventHandler<ENC,SENDER,ListViewEvent::KeyDown>; 				    
  template <Encoding ENC, typename SENDER> using ListViewInsertItemEventHandler     = ListViewEventHandler<ENC,SENDER,ListViewEvent::InsertItem>; 			
  template <Encoding ENC, typename SENDER> using ListViewItemActivateEventHandler   = ListViewEventHandler<ENC,SENDER,ListViewEvent::ItemActivate>; 		
  template <Encoding ENC, typename SENDER> using ListViewItemChangingEventHandler   = ListViewEventHandler<ENC,SENDER,ListViewEvent::ItemChanging>;
  template <Encoding ENC, typename SENDER> using ListViewItemChangedEventHandler    = ListViewEventHandler<ENC,SENDER,ListViewEvent::ItemChanged>;
  template <Encoding ENC, typename SENDER> using ListViewGetDispInfoEventHandler    = ListViewEventHandler<ENC,SENDER,choose<ENC>(ListViewEvent::GetDispInfoA,ListViewEvent::GetDispInfoW)>;	
  template <Encoding ENC, typename SENDER> using ListViewGetEmptyMarkupEventHandler = ListViewEventHandler<ENC,SENDER,ListViewEvent::GetEmptyMarkup>; 			
  template <Encoding ENC, typename SENDER> using ListViewGetInfoTipEventHandler     = ListViewEventHandler<ENC,SENDER,choose<ENC>(ListViewEvent::GetInfoTipA,ListViewEvent::GetInfoTipW)>; 				
  template <Encoding ENC, typename SENDER> using ListViewHotTrackEventHandler       = ListViewEventHandler<ENC,SENDER,ListViewEvent::HotTrack>; 				
  template <Encoding ENC, typename SENDER> using ListViewIncrementalSearchEventHandler = ListViewEventHandler<ENC,SENDER,choose<ENC>(ListViewEvent::IncrementalSearchA,ListViewEvent::IncrementalSearchW)>; 	
  template <Encoding ENC, typename SENDER> using ListViewLinkClickEventHandler      = ListViewEventHandler<ENC,SENDER,ListViewEvent::LinkClick>; 				  
  template <Encoding ENC, typename SENDER> using ListViewSetDispInfoEventHandler    = ListViewEventHandler<ENC,SENDER,choose<ENC>(ListViewEvent::SetDispInfoA,ListViewEvent::SetDispInfoW)>; */

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias VirtualListView...EventHander - Defines handler types for events from virtual/owner-data ListView controls (ie. WM_NOTIFY -> {LVN_xxxx} ) 
  //! 
  //! \tparam ENC - Window character encoding 
  //! \tparam SENDER - Originator window type
  /////////////////////////////////////////////////////////////////////////////////////////
  /*template <Encoding ENC, typename SENDER> using VirtualListViewCacheHintEventHandler    = ListViewEventHandler<ENC,SENDER,ListViewEvent::OdCacheHint>; 		
  template <Encoding ENC, typename SENDER> using VirtualListViewFindItemEventHandler     = ListViewEventHandler<ENC,SENDER,choose<ENC>(ListViewEvent::OdFindItemA,ListViewEvent::OdFindItemW)>;
  template <Encoding ENC, typename SENDER> using VirtualListViewStateChangedEventHandler = ListViewEventHandler<ENC,SENDER,ListViewEvent::OdStateChanged>; 	*/
  
} // namespace wtl

#endif // WTL_BUTTON_EVENTS_HPP
