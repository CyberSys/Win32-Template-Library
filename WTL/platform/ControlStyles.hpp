//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\ControlStyles.hpp
//! \brief Defines standard and common control window styles and notifications
//! \date 26 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CONTROL_STYLES_HPP
#define WTL_CONTROL_STYLES_HPP

#include <wtl/WTL.hpp>
#include <wtl/traits/EnumTraits.hpp>                //!< is_attribute, is_contiguous
#include <wtl/utils/Default.hpp>                    //!< default_t

//! \namespace wtl - Windows template library
namespace wtl
{
  
  // --------------------------------------------------------------------------------------------------------------
  // ------------------------------------------------ LIST-BOX CONTROLS -------------------------------------------
  // --------------------------------------------------------------------------------------------------------------

  //! \enum ListBoxNotification - Defines standard ListBox notifications
  enum class ListBoxNotification : int16_t
  {
    ErrSpace = -0x0002,			//!< [Windows 3.11]
    SelChange = 0x0001,			//!< [Windows 3.11]
    DblClk = 0x0002,			  //!< [Windows 3.11]
    SelCancel = 0x0003,			//!< [Windows 3.11]
    SetFocus = 0x0004,			//!< [Windows 3.11]
    KillFocus = 0x0005,			//!< [Windows 3.11]
  };
  
  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<ListBoxNotification>  : std::false_type  {};
  template <> struct is_contiguous<ListBoxNotification> : std::false_type  {};
  template <> struct default_t<ListBoxNotification>     : std::integral_constant<ListBoxNotification,ListBoxNotification::ErrSpace>   {};

  // --------------------------------------------------------------------------------------------------------------

  //! \enum ListBoxStyle - Defines standard ListBox window styles
  enum class ListBoxStyle : ulong32_t
  {
    Notify = 0x0001L,			            //!< [Windows 3.11]
    Sort = 0x0002L,			              //!< [Windows 3.11]
    NoRedraw = 0x0004L,			          //!< [Windows 3.11]
    MultipleSel = 0x0008L,			      //!< [Windows 3.11]
    OwnerDrawFixed = 0X0010L,			    //!< [Windows 3.11]
    OwnerDrawVariable = 0X0020L,			//!< [Windows 3.11]
    HasStrings = 0x0040L,			        //!< [Windows 3.11]
    UseTabStops = 0x0080L,			      //!< [Windows 3.11]
    NoIntegralHeight = 0X0100L,			  //!< [Windows 3.11]
    MultiColumn = 0x0200L,			      //!< [Windows 3.11]
    WantKeyboardInput = 0x0400L,			//!< [Windows 3.11]
    ExtendedSel = 0x0800L,			      //!< [Windows 3.11]
    DisableNoScroll = 0x1000L,			  //!< [Windows 3.11]
    NoData = 0x2000L,			            //!< [Windows 3.11]
    NoSel = 0x4000L,			            //!< [Windows 4.00] 
    ComboBox = 0x8000L,			          //!< 

    Standard = Notify|Sort /*|WindowStyle::VScroll|WindowStyle::Border*/,    //!<
  };
  
  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<ListBoxStyle>  : std::false_type  {};
  template <> struct is_contiguous<ListBoxStyle> : std::false_type  {};
  template <> struct default_t<ListBoxStyle>     : std::integral_constant<ListBoxStyle,ListBoxStyle::Notify>   {};
  
  // --------------------------------------------------------------------------------------------------------------
  // ----------------------------------------------- LIST-VIEW CONTROLS -------------------------------------------
  // --------------------------------------------------------------------------------------------------------------

  //! \enum ListViewNotification - Defines standard ListView notifications
  enum class ListViewNotification : int16_t
  {
    First               = int16_t(LVN_FIRST),           //!< First notification
    ItemChanging 				= First - 0,			            //!< [Windows 4.00] 
    ItemChanged 				= First - 1,			            //!< [Windows 4.00] 
    InsertItem 				  = First - 2,			            //!< [Windows 4.00] 
    DeleteItem 				  = First - 3,			            //!< [Windows 4.00] 
    DeleteAllItems 			= First - 4,			            //!< [Windows 4.00] 
    BeginLabelEditA 		= First - 5,		              //!< [Windows 4.00] 
    EndLabelEditA 			= First - 6,			            //!< [Windows 4.00] 
    ColumnClick 				= First - 8,			            //!< [Windows 4.00] 
    BeginDrag 				  = First - 9,			            //!< [Windows 4.00] 
    BeginRDrag 				  = First - 11,			            //!< [Windows 4.00] 
    OdCacheHint 				= First - 13,			            //!< [Windows 4.00] 
    ItemActivate 				= First - 14,			            //!< [Windows 4.00] 
    OdStateChanged 			= First - 15,			            //!< [Windows 4.00] 
    HotTrack 				    = First - 21,			            //!< [Windows 4.00] 

    GetDispInfoA 				= First - 50,			            //!< [Windows 4.00] 
    SetDispInfoA 				= First - 51,			            //!< [Windows 4.00] 
    OdFindItemA 				= First - 52,			            //!< [Windows 4.00] 
    KeyDown 				    = First - 55,			            //!< [Windows 4.00] 
    BeginMarquee 				= First - 56,			            //!< 
    GetInfoTipA 				= First - 57,			            //!< [Windows 4.00] 
    GetInfoTipW 				= First - 58,			            //!< [Windows 4.00] 
    IncrementalSearchA 	= First - 62,			            //!< 
    IncrementalSearchW 	= First - 63,			            //!< 
    ColumnDropDown 			= First - 64,			            //!< [Windows 6.00] 
    ColumnOverflowClick = First - 66,			            //!< [Windows 6.00] 
    BeginLabelEditW 		= First - 75,		              //!< [Windows 4.00] 
    EndLabelEditW 			= First - 76,			            //!< [Windows 4.00] 
    GetDispInfoW 				= First - 77,			            //!< [Windows 4.00] 
    SetDispInfoW 				= First - 78,			            //!< [Windows 4.00] 
    OdFindItemW 				= First - 79,			            //!< [Windows 4.00] 
    
    BeginScroll 				= First - 80,			            //!< [Windows 5.00] 
    EndScroll 				  = First - 81,			            //!< [Windows 5.00] 
    LinkClick 				  = First - 84,			            //!< [Windows 6.00] 
    GetEmptyMarkup 			= First - 87,			            //!< 

    //BeginLabelEdit = LVN_BEGINLABELEDIT,	//!< 
    //EndLabelEdit = LVN_ENDLABELEDIT,			//!< 
    //IncrementalSearch = LVN_INCREMENTALSEARCH,	//!< 
    //OdFindItem = LVN_ODFINDITEM,			    //!< 
    //GetInfoTip = LVN_GETINFOTIP,			    //!< 
    //GetDispInfo = LVN_GETDISPINFO,			  //!< 
    //SetDispInfo = LVN_SETDISPINFO,			  //!< 

    CustomDraw = (int16_t)NotifyMessage::CustomDraw, //!< Library extension
  };
  
  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<ListViewNotification>  : std::false_type  {};
  template <> struct is_contiguous<ListViewNotification> : std::false_type  {};
  template <> struct default_t<ListViewNotification>     : std::integral_constant<ListViewNotification,ListViewNotification::First>   {};

  // --------------------------------------------------------------------------------------------------------------

  //! \enum ListViewStyle - Defines standard ListView window styles
  enum class ListViewStyle : ulong32_t
  {
    Icon = 0x0000,			          //!< [Explorer 3.00] This style specifies icon view. 
    Report = 0x0001,			        //!< [Explorer 3.00] 
    SmallIcon = 0x0002,			      //!< [Explorer 3.00] 
    List = 0x0003,			          //!< [Explorer 3.00] This style specifies list view. 
    TypeMask = 0x0003,			      //!< [Explorer 3.00] 
    SingleSel = 0x0004,			      //!< [Explorer 3.00] 
    ShowSelAlways = 0x0008,			  //!< [Explorer 3.00] 
    SortAscending = 0x0010,			  //!< [Explorer 3.00] 
    SortDescending = 0x0020,			//!< [Explorer 3.00] 
    ShareImageLists = 0x0040,			//!< [Explorer 3.00] 
    NoLabelWrap = 0x0080,			    //!< [Explorer 3.00] Item text is displayed on a single line in icon view
    AutoArrange = 0x0100,			    //!< [Explorer 3.00] 
    EditLabels = 0x0200,			    //!< [Explorer 3.00] Item text can be edited in place
    OwnerData = 0x1000,			      //!< [Explorer 3.01]
    NoScroll = 0x2000,			      //!< [Explorer 3.00] 

    AlignTop = 0x0000,			      //!< [Explorer 3.00] Items are aligned with the top of the list-view control in icon and small icon view. 
    AlignLeft = 0x0800,			      //!< [Explorer 3.00] Items are left-aligned in icon and small icon view. 
    AlignMask = 0x0c00,			      //!< [Explorer 3.00] The control's current alignment. 

    OwnerDrawFixed = 0x0400,			//!< [Explorer 3.00] 
    NoColumnHeader = 0x4000,			//!< [Explorer 3.00] Column headers are not displayed in report view
    NoSortHeader = 0x8000,			  //!< [Explorer 3.00] 

    TypeStyleMask = 0xfc00,			  //!< [Explorer 3.00] 
  };
  
  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<ListViewStyle>  : std::false_type  {};
  template <> struct is_contiguous<ListViewStyle> : std::false_type  {};
  template <> struct default_t<ListViewStyle>     : std::integral_constant<ListViewStyle,ListViewStyle::Icon>   {};
  
  // --------------------------------------------------------------------------------------------------------------

  //! \enum ListViewStyleEx - Defines standard ListView extended window styles
  enum class ListViewStyleEx : ulong32_t
  {
    GridLines             = 0x00000001,			    //!< [Explorer 3.00] Displays gridlines around items and subitems
    SubItemImages         = 0x00000002,		      //!< [Explorer 3.00] Allows images to be displayed for subitems
    CheckBoxes            = 0x00000004,			    //!< [Explorer 3.01] Enables check boxes for items in a list-view control
                                                //!< [Windows XP] Check boxes are visible and functional with all list view modes except the tile view
    TrackSelect           = 0x00000008,			    //!< [Explorer 3.00] Enables hot-track selection in a list-view control.
    HeaderDragDrop        = 0x00000010,			    //!< [Explorer 3.00] Enables drag-and-drop reordering of columns in a list-view control
    FullRowSelect         = 0x00000020,		      //!< [Explorer 3.00] Applies to report mode only
    OneClickActivate      = 0x00000040,		  	  //!< [Explorer 3.00] The list-view control sends an LVN_ITEMACTIVATE notification code to the parent window when the user clicks an item
    TwoClickActivate      = 0x00000080,			    //!< [Explorer 3.00] The list-view control sends an LVN_ITEMACTIVATE notification code to the parent window when the user double-clicks an item.
    FlatSb                = 0x00000100,	        //!< [Explorer 3.00] Enables flat scroll bars in the list view
    Regional              = 0x00000200,			    //!< [Explorer 4.00/5.00 Only] 
    InfoTip               = 0x00000400,			    //!< [Explorer 3.00] Listview does InfoTips for you
    UnderlineHot          = 0x00000800,			    //!< [Explorer 3.00] 
    UnderlineCold         = 0x00001000,			    //!< [Explorer 3.00] 
    MultiWorkAreas        = 0x00002000,			    //!< [Explorer 3.00] If the list-view control has the LVS_AUTOARRANGE style, the control will not autoarrange its icons until one or more work areas are defined
    LabelTip              = 0x00004000,			    //!< [Explorer 3.00] Listview unfolds partly hidden labels if it does not have infotip text
    BorderSelect          = 0x00008000,			    //!< [Explorer 4.00] Border selection style instead of highlight
    
    DoubleBuffer          = 0x00010000,			    //!< [Windows XP] Paints via double-buffering, which reduces flicker
    HideLabels            = 0x00020000,			    //!< [Windows XP] Hides the labels in icon and small icon view.
    SingleRow             = 0x00040000,			    //!< [Windows XP] Not used
    SnapToGrid            = 0x00080000,			    //!< [Windows XP] Icons automatically snap to grid.
    SimpleSelect          = 0x00100000,			    //!< [Windows XP] Also changes overlay rendering to top right for icon mode.
    
    JustifyColumns        = 0x00200000,			    //!< [Windows Vista] Icons are lined up in columns that use up the whole view area.
    TransparentBkgnd      = 0x00400000,			    //!< [Windows Vista] Background is painted by the parent via WM_PRINTCLIENT
    TransparentShadowText = 0x00800000,		      //!< [Windows Vista] Enable shadow text on transparent backgrounds only (useful with bitmaps)
    AutoAutoArrange       = 0x01000000,			    //!< [Windows Vista] Icons automatically arrange if no icon positions have been set
    HeaderInAllViews      = 0x02000000,			    //!< [Windows Vista] Display column header in all view modes
    AutoCheckSelect       = 0x08000000,			    //!< [Windows Vista] 
    AutoSizeColumns       = 0x10000000,			    //!< [Windows Vista] 
    ColumnSnapPoints      = 0x40000000,			    //!< [Windows Vista] 
    ColumnOverflow        = 0x80000000,			    //!< [Windows Vista] 
  };
  
  //! Define traits: Non-contiguous attribute
  template <> struct is_attribute<ListViewStyleEx>  : std::true_type   {};
  template <> struct is_contiguous<ListViewStyleEx> : std::false_type  {};
  template <> struct default_t<ListViewStyleEx>     : std::integral_constant<ListViewStyleEx,ListViewStyleEx::GridLines>   {};
  
  // --------------------------------------------------------------------------------------------------------------
  // ----------------------------------------------- TOOLTIPS CONTROLS --------------------------------------------
  // --------------------------------------------------------------------------------------------------------------

  //! \enum ToolTipNotification - Defines standard ToolTip notifications
  enum class ToolTipNotification : int16_t
  {
    First         = int16_t(TTN_FIRST), //!< First notification

    GetDispInfoA  = First - 0,			  //!< [Explorer 3.00] Retrieve information needed to display a tooltip window
    GetDispInfoW  = First - 10,		    //!< [Explorer 3.00] Retrieve information needed to display a tooltip window
    Show          = First - 1,			  //!< [Explorer 3.00] Notifies the owner window that a tooltip control is about to be displayed
    Pop           = First - 2,			  //!< [Explorer 3.00] Notifies the owner window that a tooltip is about to be hidden
    LinkClick     = First - 3,			  //!< [Explorer 3.00] Sent when a text link inside a balloon tooltip is clicked
    NeedTextA     = GetDispInfoA,     //!< [Explorer 3.00] Retrieve information needed to display a tooltip window
    NeedTextW     = GetDispInfoW,     //!< [Explorer 3.00] Retrieve information needed to display a tooltip window
  };
  
  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<ToolTipNotification>  : std::false_type  {};
  template <> struct is_contiguous<ToolTipNotification> : std::false_type  {};
  template <> struct default_t<ToolTipNotification>     : std::integral_constant<ToolTipNotification,ToolTipNotification::First>   {};
  
  // --------------------------------------------------------------------------------------------------------------

  //! \enum ToolTipStyle - Defines standard ToolTip window styles
  enum class ToolTipStyle : ulong32_t
  {
    AlwaysTip      = 0x01,			//!< [Explorer 3.00] Indicates that the tooltip control appears when the cursor is on a tool, even if the tooltip control's owner window is inactive.
    NoPrefix       = 0x02,			//!< [Explorer 3.00] Prevents the system from stripping ampersand characters from a string or terminating a string at a tab character
    NoAnimate      = 0x10,			//!< [Explorer 5.00] Disables sliding tooltip animation on Windows 98 and Windows 2000 systems.
    NoFade         = 0x20,			//!< [Explorer 5.00] Disables fading tooltip animation. 
    Balloon        = 0x40,			//!< [Explorer 5.00] Indicates that the tooltip control has the appearance of a cartoon balloon
    Close          = 0x80,			//!< [Explorer 3.00] Displays a Close button on the tooltip.
    UseVisualStyle = 0x100,			//!< [Windows Vista] Use themed hyperlinks
  };
  
  //! Define traits: Non-contiguous attribute
  template <> struct is_attribute<ToolTipStyle>  : std::true_type   {};
  template <> struct is_contiguous<ToolTipStyle> : std::false_type  {};
  template <> struct default_t<ToolTipStyle>     : std::integral_constant<ToolTipStyle,ToolTipStyle::AlwaysTip>   {};
  
  // --------------------------------------------------------------------------------------------------------------
  // ----------------------------------------------- TREEVIEW CONTROLS --------------------------------------------
  // --------------------------------------------------------------------------------------------------------------

  //! \enum TreeViewNotification - Defines standard TreeView notifications
  enum class TreeViewNotification : int16_t
  {
    First           = int16_t(TVN_FIRST), //!< First notification
    
    SelChangingA    = First - 1,			//!< [Explorer 3.00] 
    SelChangingW    = First - 50,			//!< [Explorer 3.00] 
    SelChangedA     = First - 2,			//!< [Explorer 3.00] 
    SelChangedW     = First - 51,			//!< [Explorer 3.00] 

    GetDispInfoA    = First - 3,		  //!< [Explorer 3.00] 
    GetDispInfoW    = First - 52,			//!< [Explorer 3.00] 
    SetDispInfoA    = First - 4,		  //!< [Explorer 3.00] 
    SetDispInfoW    = First - 53,			//!< [Explorer 3.00] 
    
    ItemExpandingA  = First - 5,			//!< [Explorer 3.00] 
    ItemExpandingW  = First - 54,	  	//!< [Explorer 3.00] 
    ItemExpandedA   = First - 6,			//!< [Explorer 3.00] 
    ItemExpandedW   = First - 55,			//!< [Explorer 3.00] 
    BeginDragA      = First - 7,		  //!< [Explorer 3.00] 
    BeginDragW      = First - 56,		  //!< [Explorer 3.00] 
    BeginRDragA     = First - 8,			//!< [Explorer 3.00] 
    BeginRDragW     = First - 57,		  //!< [Explorer 3.00] 
    DeleteItemA     = First - 9,		  //!< [Explorer 3.00] 
    DeleteItemW     = First - 58,		  //!< [Explorer 3.00] 
    BeginLabelEditA = First - 10,		  //!< [Explorer 3.00] 
    BeginLabelEditW = First - 59,		  //!< [Explorer 3.00] 
    EndLabelEditA   = First - 11,			//!< [Explorer 3.00] 
    EndLabelEditW   = First - 60,			//!< [Explorer 3.00] 
    KeyDown         = First - 12,			//!< [Explorer 3.00] 

    GetInfoTipA     = First - 13,			//!< [Explorer 3.00] Sent by a tree-view control that has the TVS_INFOTIP style.
    GetInfoTipW     = First - 14,			//!< [Explorer 3.00] 
    SingleExpand    = First - 15,			//!< [Explorer 3.00] 

    ItemChangingA   = First - 16,			//!< [Explorer 6.00] 
    ItemChangingW   = First - 17,			//!< [Explorer 6.00] 
    ItemChangedA    = First - 18,			//!< [Explorer 6.00] 
    ItemChangedW    = First - 19,			//!< [Explorer 6.00] 
    AsyncDraw       = First - 20,			//!< [Explorer 6.00] 

    CustomDraw      = int16_t(NotifyMessage::CustomDraw), //!< Library extension
  };
  
  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<TreeViewNotification>  : std::false_type  {};
  template <> struct is_contiguous<TreeViewNotification> : std::false_type  {};
  template <> struct default_t<TreeViewNotification>     : std::integral_constant<TreeViewNotification,TreeViewNotification::First>   {};

  // --------------------------------------------------------------------------------------------------------------
  
  //! \enum TreeViewStyle - Defines standard TreeView window styles
  enum class TreeViewStyle : ulong32_t
  {
    HasButtons      = 0x0001,			//!< [Explorer 3.00] Displays plus (+) and minus (-) buttons next to parent items
    HasLines        = 0x0002,			//!< [Explorer 3.00] Uses lines to show the hierarchy of items.
    LinesAtRoot     = 0x0004,			//!< [Explorer 3.00] Uses lines to link items at the root of the tree-view control
    EditLabels      = 0x0008,			//!< [Explorer 3.00] Allows the user to edit the labels of tree-view items. 
    DisableDragDrop = 0x0010,		  //!< [Explorer 3.00] Prevents the tree-view control from sending TVN_BEGINDRAG notification codes
    ShowSelAlways   = 0x0020,			//!< [Explorer 3.00] Causes a selected item to remain selected when the tree-view control loses focus
    
    RtlReading      = 0x0040,			//!< [Explorer 3.01] Causes text to be displayed from right-to-left 
    NoTooltips      = 0x0080,			//!< [Explorer 3.01] Disables tooltips. 
    CheckBoxes      = 0x0100,			//!< [Explorer 3.01] Enables item check boxes (Requires image property)
                                  //!< [Explorer 5.00] Enables item check boxes (Independent of image property)
    TrackSelect     = 0x0200,			//!< [Explorer 3.01] Enables hot tracking in a tree-view control. 

    SingleExpand    = 0x0400,			//!< [Explorer 4.00] Collapses previous node when selection changes
    InfoTip         = 0x0800,			//!< [Explorer 4.00] Obtains tooltip information by sending the TVN_GETINFOTIP notification
    FullRowSelect   = 0x1000,			//!< [Explorer 4.00] Enables full-row selection in the tree view
    NoScroll        = 0x2000,			//!< [Explorer 4.00] Disables both horizontal and vertical scrolling in the control
    NonEvenHeight   = 0x4000,			//!< [Explorer 4.00] Sets the height of the items to an odd height with the TVM_SETITEMHEIGHT message
    NoHScroll       = 0x8000,			//!< [Explorer 5.00] Disables horizontal scrolling in the control.
  };
  
  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<TreeViewStyle>  : std::false_type  {};
  template <> struct is_contiguous<TreeViewStyle> : std::false_type  {};
  template <> struct default_t<TreeViewStyle>     : std::integral_constant<TreeViewStyle,TreeViewStyle::HasButtons>   {};
  
  // --------------------------------------------------------------------------------------------------------------

  //! \enum TreeViewStyleEx - Defines standard TreeView extended window styles
  enum class TreeViewStyleEx : ulong32_t
  {    
    NoSingleCollapse    = 0x0001,			//!< [Windows XP] (Internal) Do not collapse the previously selected tree-view item unless it has the same parent as the new selection.
    MultiSelect         = 0x0002,			//!< [Windows Vista] Not supported. Do not use.
    DoubleBuffer        = 0x0004,			//!< [Windows Vista] Specifies how the background is erased or filled.
    NoIndentState       = 0x0008,			//!< [Windows Vista] Do not indent the tree view for the expando buttons.
    RichTooltip         = 0x0010,			//!< [Windows Vista] Allow rich tooltips in the tree view (custom drawn with icon and text).
    AutoHScroll         = 0x0020,			//!< [Windows Vista] Remove the horizontal scroll bar and auto-scroll depending on mouse position.
    FadeInOutExpandos   = 0x0040,			//!< [Windows Vista] Fade expando buttons in or out when the mouse moves away or into a state of hovering over the control.
    PartialCheckBoxes   = 0x0080,			//!< [Windows Vista] Include partial checkbox state if the control has the TVS_CHECKBOXES style.
    ExclusionCheckBoxes = 0x0100,			//!< [Windows Vista] Include exclusion checkbox state if the control has the TVS_CHECKBOXES style.
    DimmedCheckBoxes    = 0x0200,			//!< [Windows Vista] Include dimmed checkbox state if the control has the TVS_CHECKBOXES style.
    DrawImageAsync      = 0x0400,			//!< [Windows Vista] Retrieves calendar grid information.

  };
  
  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<TreeViewStyleEx>  : std::false_type  {};
  template <> struct is_contiguous<TreeViewStyleEx> : std::false_type  {};
  template <> struct default_t<TreeViewStyleEx>     : std::integral_constant<TreeViewStyleEx,TreeViewStyleEx::NoSingleCollapse>   {};


}

#endif  // WTL_CONTROL_STYLES_HPP
