//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\DrawingFlags.hpp
//! \brief Defines flags for drawing related Win32 API functions
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_DRAWING_FLAGS_HPP
#define WTL_DRAWING_FLAGS_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EnumTraits.hpp"                  //!< is_attribute, is_contiguous
#include "wtl/utils/Default.hpp"                      //!< default_t

//! \namespace wtl - Windows template library
namespace wtl
{
  // --------------------------------------------------------------------------------------------------------------
  // ----------------------------------------------- DEVICE CONTEXT -----------------------------------------------
  // --------------------------------------------------------------------------------------------------------------

  //! \enum DrawTextFlags - Defines DrawText flags
  enum class DrawTextFlags : ulong32_t
  {
    Top = 0x00000000, 			            //!< 
    Left = 0x00000000, 			            //!< 
    Centre = 0x00000001, 			          //!< 
    Right = 0x00000002, 			          //!< 
    VCentre = 0x00000004, 			        //!< 
    Bottom = 0x00000008, 			          //!< 
    WordBreak = 0x00000010, 			      //!< 
    SingleLine = 0x00000020, 			      //!< 
    ExpandTabs = 0x00000040, 			      //!< 
    TabStop = 0x00000080, 			        //!< 
    NoClip = 0x00000100, 			          //!< 
    ExternalLeading = 0x00000200, 	    //!< 
    CalcRect = 0x00000400, 			        //!< 
    NoPrefix = 0x00000800, 			        //!< 
    Internal = 0x00001000, 			        //!< 
    EditControl = 0x00002000, 			    //!< 
    PathEllipsis = 0x00004000, 		      //!< 
    EndElipsis = 0x00008000, 			      //!< 
    ModifyString = 0x00010000, 			    //!< 
    RtlReading = 0x00020000, 			      //!< 
    WordElipsis = 0x00040000, 		      //!< 
    NoFullWidthCharBreak = 0x00080000, 	//!< [windows 5.00]
    HidePrefix = 0x00100000, 			      //!< [windows 5.00]
    PrefixOnly = 0x00200000, 			      //!< [windows 5.00]
  };
  
  //! Define traits: Non-contiguous Attribute
  template <> struct is_attribute<DrawTextFlags>  : std::true_type  {};
  template <> struct is_contiguous<DrawTextFlags> : std::false_type {};
  template <> struct default_t<DrawTextFlags>     : std::integral_constant<DrawTextFlags,DrawTextFlags::Top>   {};

  // --------------------------------------------------------------------------------------------------------------
  
  //! \enum DrawObjectType - Defines drawing object types 
  enum class DrawObjectType : uint32_t
  {
    Pen = 1, 			      //!< 
    Brush = 2, 	        //!< 
    Dc = 3, 			      //!< 
    MetaDc = 4, 	      //!< 
    Pal = 5, 			      //!< 
    Font = 6, 		      //!< 
    Bitmap = 7, 	      //!< 
    Region = 8, 	      //!< 
    MetaFile = 9,       //!< 
    MemDc = 10, 			  //!< 
    ExtPen = 11, 			  //!< 
    EnhMetaDc = 12, 		//!< 
    EnhMetaFile = 13, 	//!< 
    ColorSpace = 14, 		//!< 
  };
  
  //! Define traits: Contiguous Enumeration
  template <> struct is_attribute<DrawObjectType>  : std::false_type  {};
  template <> struct is_contiguous<DrawObjectType> : std::true_type   {};
  template <> struct default_t<DrawObjectType>     : std::integral_constant<DrawObjectType,DrawObjectType::Pen>   {};
  
  // --------------------------------------------------------------------------------------------------------------
  
  //! \enum DrawingMode - Defines background drawing modes
  enum class DrawingMode : uint32_t
  {
    Transparent = 1, 			//!< Transparent background
    Opaque = 2, 	        //!< Opaque background
  };
  
  //! Define traits: Contiguous Enumeration
  template <> struct is_attribute<DrawingMode>  : std::false_type  {};
  template <> struct is_contiguous<DrawingMode> : std::true_type   {};
  template <> struct default_t<DrawingMode>     : std::integral_constant<DrawingMode,DrawingMode::Transparent>   {};

  // --------------------------------------------------------------------------------------------------------------
  // -----------------------------------------------      FONTS     -----------------------------------------------
  // --------------------------------------------------------------------------------------------------------------

  //! \enum FontWeight - Defines font weights
  enum class FontWeight : ulong32_t
  { 
    DontCare = 0, 			        //!< 
    Thin = 100, 			          //!< 
    ExtraLight = 200, 			    //!< 
    Light = 300, 			          //!< 
    Normal = 400, 			        //!< 
    Medium = 500, 			        //!< 
    SemiBold = 600, 			      //!< 
    Bold = 700, 			          //!< 
    ExtraBold = 800, 			      //!< 
    Heavy = 900, 			          //!< 

    UltraKight = ExtraLight, 		//!< 
    Regular = Normal, 			    //!< 
    Demibold = SemiBold, 			  //!< 
    Ultrabold = ExtraBold, 			//!< 
    Black = Heavy, 			        //!< 
  };
  
  //! Define traits: Non-Contiguous enumeration
  template <> struct is_attribute<FontWeight>  : std::false_type  {};
  template <> struct is_contiguous<FontWeight> : std::false_type  {};
  template <> struct default_t<FontWeight>     : std::integral_constant<FontWeight,FontWeight::DontCare>   {};
 
  // --------------------------------------------------------------------------------------------------------------

  //! \enum FontCharSet - Defines font character sets
  enum class FontCharSet : ulong32_t
  { 
    Ansi = 0,			      //!<
    Default = 1,			  //!<
    Symbol = 2,			    //!<
    Shiftjis = 128,		  //!<
    Hangeul = 129,			//!<
    Hangul = 129,			  //!<
    Gb2312 = 134,			  //!<
    Chinesebig5 = 136,	//!<
    Oem = 255,			    //!<
    Johab = 130,			  //!<
    Hebrew = 177,			  //!<
    Arabic = 178,			  //!<
    Greek = 161,			  //!<
    Turkish = 162,			//!<
    Vietnamese = 163,	  //!<
    Thai = 222,			    //!<
    Easteurope = 238,	  //!<
    Russian = 204,			//!<
    Mac = 77,			      //!<
    Baltic = 186,			  //!<
  };
  
  //! Define traits: Non-Contiguous enumeration
  template <> struct is_attribute<FontCharSet>  : std::false_type  {};
  template <> struct is_contiguous<FontCharSet> : std::false_type  {};
  template <> struct default_t<FontCharSet>     : std::integral_constant<FontCharSet,FontCharSet::Ansi>   {};

  // --------------------------------------------------------------------------------------------------------------

  //! \enum FontQuality - Defines font quality
  enum class FontQuality : ulong32_t
  {
    Default = 0,			      //!<
    Draft = 1,			        //!<
    Proof = 2,			        //!<
    NonAntiAliased = 3,			//!<
    AntiAliased = 4,			  //!<
    ClearType = 5,			    //!< [Windows 5.01]
    ClearTypeNatural = 6,	  //!< [Windows 5.01]
  };
  
  //! Define traits: Contiguous enumeration
  template <> struct is_attribute<FontQuality>  : std::false_type  {};
  template <> struct is_contiguous<FontQuality> : std::true_type   {};
  template <> struct default_t<FontQuality>     : std::integral_constant<FontQuality,FontQuality::Default>   {};

  // --------------------------------------------------------------------------------------------------------------

  //! \enum FontPrecision - Defines font output precision
  enum class FontPrecision : ulong32_t
  {
    Default = 0, 			  //!< 
    String = 1, 			  //!< 
    Character = 2, 			//!< 
    Stroke = 3, 			  //!< 
    TT = 4, 			      //!< 
    Device = 5, 			  //!< 
    Raster = 6, 			  //!< 
    TT_Only = 7, 			  //!< 
    Outline = 8, 			  //!< 
    ScreenOutline = 9,  //!< 
    PS_Only = 10, 			//!< 
  };
    
  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<FontPrecision>  : std::false_type  {};
  template <> struct is_contiguous<FontPrecision> : std::false_type  {};
  template <> struct default_t<FontPrecision>     : std::integral_constant<FontPrecision,FontPrecision::Default>   {};

  // --------------------------------------------------------------------------------------------------------------

  //! \enum FontClipping - Defines font output precision
  enum class FontClipping : ulong32_t
  {
    Default = 0, 			        //!< 
    Character = 1, 			      //!< 
    Stroke = 2, 			        //!< 
    Mask = 0xf, 			        //!< 
    LH_Angles = (1 << 4), 		//!< 
    TT_Always = (2 << 4), 		//!< 
    DFA_Disable = (4 << 4), 	//!< [Windows 6.00]
    Embedded = (8 << 4), 			//!< 
  };
  
  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<FontClipping>  : std::false_type  {};
  template <> struct is_contiguous<FontClipping> : std::false_type  {};
  template <> struct default_t<FontClipping>     : std::integral_constant<FontClipping,FontClipping::Default>   {};

  // --------------------------------------------------------------------------------------------------------------
  
  //! \enum FontFamily - Defines font families
  enum class FontFamily : ulong32_t
  { 
    Default = 0,          //!< Default pitch
    Fixed = 1,            //!< Fixed pitch
    Variable = 2,         //!< Variable pitch
    MonoFont = 8,         //!< Default pitch
    DontCare = (0<<4),    //!< Don't care or don't know
    Roman = (1<<4),       //!< Variable stroke width, serifed. Times Roman, Century Schoolbook, etc.
    Swiss = (2<<4),       //!< Variable stroke width, sans-serifed. Helvetica, Swiss, etc. 
    Modern = (3<<4),      //!< Constant stroke width, serifed or sans-serifed. Pica, Elite, Courier, etc. 
    Script = (4<<4),      //!< Cursive, etc. 
    Decorative = (5<<4),  //!< Old English, etc. 
  };
  
  //! Define traits: Non-contiguous attribute
  template <> struct is_attribute<FontFamily>  : std::false_type  {};
  template <> struct is_contiguous<FontFamily> : std::false_type  {};
  template <> struct default_t<FontFamily>     : std::integral_constant<FontFamily,FontFamily::Default>   {};

  // --------------------------------------------------------------------------------------------------------------
  // -----------------------------------------------   OWNER DRAW   -----------------------------------------------
  // --------------------------------------------------------------------------------------------------------------

  //! \enum OwnerDrawAction - Defines owner drawing requests
  enum class OwnerDrawAction : uint32_t
  {
    Entire = ODA_DRAWENTIRE, 			//!< Entire control
    Focus = ODA_FOCUS, 	          //!< Focus rectange
    Select = ODA_SELECT, 	        //!< Selection status has changed
  };
  
  //! Define traits: Non-Contiguous attribute
  template <> struct is_attribute<OwnerDrawAction>  : std::true_type  {};
  template <> struct is_contiguous<OwnerDrawAction> : std::false_type {};
  template <> struct default_t<OwnerDrawAction>     : std::integral_constant<OwnerDrawAction,OwnerDrawAction::Entire>   {};
  
  // --------------------------------------------------------------------------------------------------------------
  
  //! \enum OwnerDrawControl - Defines controls that support owner draw
  enum class OwnerDrawControl : uint32_t
  {
    Button = ODT_BUTTON,      //!< Button control
    ComboBox = ODT_COMBOBOX,  //!< ComboBox control
    ListBox = ODT_LISTBOX,    //!< ListBox control
    ListView = ODT_LISTVIEW,  //!< ListView control
    Menu = ODT_MENU,          //!< Window Menu 
    Static = ODT_STATIC,      //!< Static control
    Tab = ODT_TAB,            //!< Tab control
  };
  
  //! Define traits: Non-Contiguous attribute
  template <> struct is_attribute<OwnerDrawControl>  : std::true_type  {};
  template <> struct is_contiguous<OwnerDrawControl> : std::false_type {};
  template <> struct default_t<OwnerDrawControl>     : std::integral_constant<OwnerDrawControl,OwnerDrawControl::Button>   {};
  
  // --------------------------------------------------------------------------------------------------------------
  
  //! \enum OwnerDrawState - Defines controls that support owner draw
  enum class OwnerDrawState : uint16_t
  {
    Checked = ODS_CHECKED,              //!< The menu item is to be checked. This bit is used only in a menu.
    ComboBoxEdit = ODS_COMBOBOXEDIT,    //!< The drawing takes place in the selection field (edit control) of an owner-drawn combo box.
    Default = ODS_DEFAULT,              //!< The item is the default item.
    Disabled = ODS_DISABLED,            //!< The item is to be drawn as disabled.
    Focus = ODS_FOCUS,                  //!< The item has the keyboard focus.
    Grayed = ODS_GRAYED,                //!< The item is to be grayed. This bit is used only in a menu.
    Hotlight = ODS_HOTLIGHT,            //!< The item is being hot-tracked, that is, the item will be highlighted when the mouse is on the item.
    Inactive = ODS_INACTIVE,            //!< The item is inactive and the window associated with the menu is inactive.
    NoAccel = ODS_NOACCEL,              //!< The control is drawn without the keyboard accelerator cues.
    NoFocusRect = ODS_NOFOCUSRECT,      //!< The control is drawn without focus indicator cues.
    Selected = ODS_SELECTED,            //!< The menu item's status is selected.
  };
  
  //! Define traits: Non-Contiguous attribute
  template <> struct is_attribute<OwnerDrawState>  : std::true_type  {};
  template <> struct is_contiguous<OwnerDrawState> : std::false_type {};
  template <> struct default_t<OwnerDrawState>     : std::integral_constant<OwnerDrawState,OwnerDrawState::Checked>   {};
  
  // --------------------------------------------------------------------------------------------------------------
  // -----------------------------------------------     BRUSHES    -----------------------------------------------
  // --------------------------------------------------------------------------------------------------------------

  //! \enum HatchStyle - Defines brush hatch styles
  enum class HatchStyle : uint32_t
  {
    Horizontal = 0, 			  //!<        /* ----- */
    Vertical = 1, 			    //!<        /* ||||| */
    ForwardDiagonal = 2, 		//!<        /* \\\\\ */
    BackwardDiagonal = 3, 	//!<        /* ///// */
    Cross = 4, 			        //!<        /* +++++ */
    CrossDiagonal = 5, 			//!<        /* xxxxx */
  };
  
  //! Define traits: Non-Contiguous Enumeration
  template <> struct is_attribute<HatchStyle>  : std::false_type  {};
  template <> struct is_contiguous<HatchStyle> : std::false_type  {};
  template <> struct default_t<HatchStyle>     : std::integral_constant<HatchStyle,HatchStyle::Horizontal>   {};

  // --------------------------------------------------------------------------------------------------------------
  // -----------------------------------------------       PENS     -----------------------------------------------
  // --------------------------------------------------------------------------------------------------------------

  //! \enum PenStyle - Defines cosmetic and geometric pen styles
  enum class PenStyle : int32_t
  {
    Solid = 0, 			                //!< 
    Dash = 1, 			                //!<        /* -------  */
    Dot = 2, 			                  //!<        /* .......  */
    DashDot = 3, 			              //!<        /* _._._._  */
    DashDotDot = 4, 			          //!<        /* _.._.._  */
    Null = 5, 			                //!< 
    InsideFrame = 6, 			          //!< 
    UserStyle = 7, 			            //!< 
    Alternate = 8, 			            //!< 
    StyleMask = 0x0000000f, 			  //!< 

    EndcapRound = 0x00000000, 			//!< 
    EndcapSquare = 0x00000100, 			//!< 
    EndcapFlat = 0x00000200, 			  //!< 
    EndcapMask = 0x00000f00, 			  //!< 

    JoinRound = 0x00000000, 			  //!< 
    JoinBevel = 0x00001000, 			  //!< 
    JoinMiter = 0x00002000, 			  //!< 
    JoinMask = 0x0000f000, 			    //!< 

    Cosmetic = 0x00000000, 			    //!< 
    Geometric = 0x00010000, 			  //!< 
    TypeMask  = 0x000f0000, 			  //!< 
  };


  //! Define traits: Non-contiguous Attribute
  template <> struct is_attribute<PenStyle>  : std::true_type  {};
  template <> struct is_contiguous<PenStyle> : std::false_type {};
  template <> struct default_t<PenStyle>     : std::integral_constant<PenStyle,PenStyle::Solid>   {};
  
  // --------------------------------------------------------------------------------------------------------------
  // ------------------------------------------------ STOCK OBJECTS -----------------------------------------------
  // --------------------------------------------------------------------------------------------------------------

   //! \enum StockObject - Defines stock drawing objects
  enum class StockObject : int32_t
  {
    WhiteBrush = 0, 			    //!< 
    LtGreyBrush = 1, 			    //!< 
    GreyBrush = 2, 			      //!< 
    DkBreyBrush = 3, 			    //!< 
    BlackBrush = 4, 			    //!< 
    NullBrush = 5, 			      //!< 
    HollowBrush = NullBrush,
    WhitePen = 6, 			      //!< 
    BlackPen = 7, 			      //!< 
    NullPen = 8, 			        //!< 
    OemFixedFont = 10, 			  //!< 
    AnsiFixedFont = 11, 			//!< 
    AnsiVarFont = 12, 			  //!< 
    SystemFont = 13, 			    //!< 
    DeviceDefaultFont = 14, 	//!< 
    DefaultPalette = 15, 			//!< 
    SystemFixedFont = 16, 		//!< 
    DefaultGuiFont = 17, 			//!< 
    DcBrush = 18, 			      //!< [windows 2000]
    DcPen = 19, 			        //!< [windows 2000]
  };
  
  //! Define traits: Contiguous Enumeration
  template <> struct is_attribute<StockObject>  : std::false_type  {};
  template <> struct is_contiguous<StockObject> : std::true_type   {};
  template <> struct default_t<StockObject>     : std::integral_constant<StockObject,StockObject::WhiteBrush>   {};

}

#endif  // WTL_DRAWING_FLAGS_HPP
