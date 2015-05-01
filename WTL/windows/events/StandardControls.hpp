////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\StandardControls.hpp
//! \brief Provides argument/delegate/handler types for events from standard controls 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CONTROL_EVENTS_HPP
#define WTL_CONTROL_EVENTS_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ButtonEvent - Defines a button control event raised by WM_COMMAND
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam CODE - Notification message 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, ButtonNotification CODE>
  using ButtonEvent = Event<LResult, ControlEventArgs<ENC,WindowMessage::COMMAND,ButtonNotification,CODE>&>;

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ButtonEventArgs - Defines arguments for button control events raised via WM_COMMAND 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam CODE - Notification message 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, ButtonNotification CODE>
  using ButtonEventArgs = ControlEventArgs<ENC,WindowMessage::COMMAND,ButtonNotification,CODE>;
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ButtonEventHandler - Defines handlers for button control events raised via WM_COMMAND 
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam CODE - Notification message 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, ButtonNotification CODE>
  using ButtonEventHandler = typename ButtonEvent<ENC,CODE>::delegate_t;

  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ButtonClickEvent... - Defines button 'Click' event/arguments/handler types
  //! 
  //! \tparam ENC - Window character encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC> using ButtonClickEvent = ButtonEvent<ENC,ButtonNotification::Click>;
  template <Encoding ENC> using ButtonClickEventArgs = ButtonEventArgs<ENC,ButtonNotification::Click>;
  template <Encoding ENC> using ButtonClickEventHandler = ButtonEventHandler<ENC,ButtonNotification::Click>;

  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ButtonGainFocusEvent... - Defines button 'GainFocus' event/arguments/handler types
  //! 
  //! \tparam ENC - Window character encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC> using ButtonGainFocusEvent = ButtonEvent<ENC,ButtonNotification::SetFocus>;
  template <Encoding ENC> using ButtonGainFocusEventArgs = ButtonEventArgs<ENC,ButtonNotification::SetFocus>;
  template <Encoding ENC> using ButtonGainFocusEventHandler = ButtonEventHandler<ENC,ButtonNotification::SetFocus>;

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ButtonLoseFocusEvent... - Defines button 'LoseFocus' event/arguments/handler types
  //! 
  //! \tparam ENC - Window character encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC> using ButtonLoseFocusEvent = ButtonEvent<ENC,ButtonNotification::KillFocus>;
  template <Encoding ENC> using ButtonLoseFocusEventArgs = ButtonEventArgs<ENC,ButtonNotification::KillFocus>;
  template <Encoding ENC> using ButtonLoseFocusEventHandler = ButtonEventHandler<ENC,ButtonNotification::KillFocus>;




  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ButtonEventHander - Defines a handler type for a Button event (ie. WM_COMMAND -> {BN_xxxx} )
  //! 
  //! \tparam ENC - Window character encoding 
  //! \tparam SENDER - Originator window type
  //! \tparam CODE - Notification
  ///////////////////////////////////////////////////////////////////////////////
  //template <Encoding ENC, typename SENDER, ButtonEvent CODE> using ButtonEventHandler = CtrlEventHandler<ENC,SENDER,ButtonEvent,CODE>;

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias Button...EventHander - Defines handler types for Button events (ie. WM_COMMAND -> {BN_xxxx} )
  //! 
  //! \tparam ENC - Window character encoding 
  //! \tparam SENDER - Originator window type
  ///////////////////////////////////////////////////////////////////////////////
  /*template <Encoding ENC, typename SENDER> using ButtonClickEventHandler       = ButtonEventHandler<ENC,SENDER,ButtonEvent::Click>;
  template <Encoding ENC, typename SENDER> using ButtonDisabledEventHandler    = ButtonEventHandler<ENC,SENDER,ButtonEvent::Disabled>;
  template <Encoding ENC, typename SENDER> using ButtonDoubleClickEventHandler = ButtonEventHandler<ENC,SENDER,ButtonEvent::DoubleClick>;
  template <Encoding ENC, typename SENDER> using ButtonPaintEventHandler       = ButtonEventHandler<ENC,SENDER,ButtonEvent::Paint>;   
  template <Encoding ENC, typename SENDER> using ButtonPushedEventHandler      = ButtonEventHandler<ENC,SENDER,ButtonEvent::Pushed>;
  template <Encoding ENC, typename SENDER> using ButtonUnpushedEventHandler    = ButtonEventHandler<ENC,SENDER,ButtonEvent::Unpushed>;
  template <Encoding ENC, typename SENDER> using ButtonLoseFocusEventHandler   = ButtonEventHandler<ENC,SENDER,ButtonEvent::KillFocus>;
  template <Encoding ENC, typename SENDER> using ButtonGainFocusEventHandler   = ButtonEventHandler<ENC,SENDER,ButtonEvent::SetFocus>;*/
  
  

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ListViewEventHander - Defines a handler type for a ListView event (ie. WM_NOTIFY -> {LVN_xxxx} )
  //! 
  //! \tparam ENC - Window character encoding 
  //! \tparam SENDER - Originator window type
  //! \tparam CODE - Notification
  ///////////////////////////////////////////////////////////////////////////////
  //template <Encoding ENC, typename SENDER, ListViewEvent CODE> using ListViewEventHandler = NotifyEventHandler<ENC,SENDER,ListViewEvent,CODE>;

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ListView...EventHander - Defines handler types for ListView events (ie. WM_NOTIFY -> {LVN_xxxx} )
  //! 
  //! \tparam ENC - Window character encoding 
  //! \tparam SENDER - Originator window type
  ///////////////////////////////////////////////////////////////////////////////
  /*template <Encoding ENC, typename SENDER> using ListViewCustomDrawEventHandler     = ListViewEventHandler<ENC,SENDER,ListViewEvent::CustomDraw>;
  template <Encoding ENC, typename SENDER> using ListViewColumnDropDownEventHandler = ListViewEventHandler<ENC,SENDER,ListViewEvent::ColumnDropDown>; 			
  template <Encoding ENC, typename SENDER> using ListViewColumnOverflowClickEventHandler = ListViewEventHandler<ENC,SENDER,ListViewEvent::ColumnOverflowClick>; 
  template <Encoding ENC, typename SENDER> using ListViewColumnClickEventHandler    = ListViewEventHandler<ENC,SENDER,ListViewEvent::ColumnClick>; 		
  template <Encoding ENC, typename SENDER> using ListViewDeleteItemEventHandler     = ListViewEventHandler<ENC,SENDER,ListViewEvent::DeleteItem>; 			
  template <Encoding ENC, typename SENDER> using ListViewDeleteAllItemsEventHandler = ListViewEventHandler<ENC,SENDER,ListViewEvent::DeleteAllItems>; 	
  template <Encoding ENC, typename SENDER> using ListViewBeginDragEventHandler      = ListViewEventHandler<ENC,SENDER,ListViewEvent::BeginDrag>; 			
  template <Encoding ENC, typename SENDER> using ListViewBeginRDragEventHandler     = ListViewEventHandler<ENC,SENDER,ListViewEvent::BeginRDrag>; 			
  template <Encoding ENC, typename SENDER> using ListViewBeginLabelEditEventHandler = ListViewEventHandler<ENC,SENDER,getValue_t<ENC>(ListViewEvent::BeginLabelEditA,ListViewEvent::BeginLabelEditW)>; 
  template <Encoding ENC, typename SENDER> using ListViewBeginMarqueeEventHandler   = ListViewEventHandler<ENC,SENDER,ListViewEvent::BeginMarquee>; 				
  template <Encoding ENC, typename SENDER> using ListViewBeginScrollEventHandler    = ListViewEventHandler<ENC,SENDER,ListViewEvent::BeginScroll>; 				
  template <Encoding ENC, typename SENDER> using ListViewEndLabelEditEventHandler   = ListViewEventHandler<ENC,SENDER,getValue_t<ENC>(ListViewEvent::EndLabelEditA,ListViewEvent::EndLabelEditW)>; 	
  template <Encoding ENC, typename SENDER> using ListViewEndScrollEventHandler      = ListViewEventHandler<ENC,SENDER,ListViewEvent::EndScroll>; 				  
  template <Encoding ENC, typename SENDER> using ListViewKeyDownEventHandler        = ListViewEventHandler<ENC,SENDER,ListViewEvent::KeyDown>; 				    
  template <Encoding ENC, typename SENDER> using ListViewInsertItemEventHandler     = ListViewEventHandler<ENC,SENDER,ListViewEvent::InsertItem>; 			
  template <Encoding ENC, typename SENDER> using ListViewItemActivateEventHandler   = ListViewEventHandler<ENC,SENDER,ListViewEvent::ItemActivate>; 		
  template <Encoding ENC, typename SENDER> using ListViewItemChangingEventHandler   = ListViewEventHandler<ENC,SENDER,ListViewEvent::ItemChanging>;
  template <Encoding ENC, typename SENDER> using ListViewItemChangedEventHandler    = ListViewEventHandler<ENC,SENDER,ListViewEvent::ItemChanged>;
  template <Encoding ENC, typename SENDER> using ListViewGetDispInfoEventHandler    = ListViewEventHandler<ENC,SENDER,getValue_t<ENC>(ListViewEvent::GetDispInfoA,ListViewEvent::GetDispInfoW)>;	
  template <Encoding ENC, typename SENDER> using ListViewGetEmptyMarkupEventHandler = ListViewEventHandler<ENC,SENDER,ListViewEvent::GetEmptyMarkup>; 			
  template <Encoding ENC, typename SENDER> using ListViewGetInfoTipEventHandler     = ListViewEventHandler<ENC,SENDER,getValue_t<ENC>(ListViewEvent::GetInfoTipA,ListViewEvent::GetInfoTipW)>; 				
  template <Encoding ENC, typename SENDER> using ListViewHotTrackEventHandler       = ListViewEventHandler<ENC,SENDER,ListViewEvent::HotTrack>; 				
  template <Encoding ENC, typename SENDER> using ListViewIncrementalSearchEventHandler = ListViewEventHandler<ENC,SENDER,getValue_t<ENC>(ListViewEvent::IncrementalSearchA,ListViewEvent::IncrementalSearchW)>; 	
  template <Encoding ENC, typename SENDER> using ListViewLinkClickEventHandler      = ListViewEventHandler<ENC,SENDER,ListViewEvent::LinkClick>; 				  
  template <Encoding ENC, typename SENDER> using ListViewSetDispInfoEventHandler    = ListViewEventHandler<ENC,SENDER,getValue_t<ENC>(ListViewEvent::SetDispInfoA,ListViewEvent::SetDispInfoW)>; */

  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias VirtualListView...EventHander - Defines handler types for events from virtual/owner-data ListView controls (ie. WM_NOTIFY -> {LVN_xxxx} ) 
  //! 
  //! \tparam ENC - Window character encoding 
  //! \tparam SENDER - Originator window type
  ///////////////////////////////////////////////////////////////////////////////
  /*template <Encoding ENC, typename SENDER> using VirtualListViewCacheHintEventHandler    = ListViewEventHandler<ENC,SENDER,ListViewEvent::OdCacheHint>; 		
  template <Encoding ENC, typename SENDER> using VirtualListViewFindItemEventHandler     = ListViewEventHandler<ENC,SENDER,getValue_t<ENC>(ListViewEvent::OdFindItemA,ListViewEvent::OdFindItemW)>;
  template <Encoding ENC, typename SENDER> using VirtualListViewStateChangedEventHandler = ListViewEventHandler<ENC,SENDER,ListViewEvent::OdStateChanged>; 	*/

}

#endif // WTL_CONTROL_EVENTS_HPP
