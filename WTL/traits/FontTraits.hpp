////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\FontTraits.hpp
//! \brief Defines font traits + helper global operators
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_FONT_TRAITS_HPP
#define WTL_FONT_TRAITS_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \enum FontWeight - Defines font weights
  enum class FontWeight : ulong32
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

  //! Define limits traits
  template <> struct max_value<FontWeight>     : std::integral_constant<FontWeight,FontWeight::Heavy>      {};
  template <> struct min_value<FontWeight>     : std::integral_constant<FontWeight,FontWeight::DontCare>   {};
 

  //! \enum FontCharSet - Defines font character sets
  enum class FontCharSet : ulong32
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

  //! Define limits traits
  template <> struct max_value<FontCharSet>     : std::integral_constant<FontCharSet,FontCharSet::Oem>    {};
  template <> struct min_value<FontCharSet>     : std::integral_constant<FontCharSet,FontCharSet::Ansi>   {};
  


  //! \enum FontQuality - Defines font quality
  enum class FontQuality : ulong32
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

  //! Define limits traits
  template <> struct max_value<FontQuality>     : std::integral_constant<FontQuality,FontQuality::ClearTypeNatural>    {};
  template <> struct min_value<FontQuality>     : std::integral_constant<FontQuality,FontQuality::Default>             {};



  //! \enum FontPrecision - Defines font output precision
  enum class FontPrecision : ulong32
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

  //! Define limits traits
  template <> struct max_value<FontPrecision>     : std::integral_constant<FontPrecision,FontPrecision::PS_Only>   {};
  template <> struct min_value<FontPrecision>     : std::integral_constant<FontPrecision,FontPrecision::Default>   {};



  //! \enum FontClipping - Defines font output precision
  enum class FontClipping : ulong32
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

  //! Define limits traits
  template <> struct max_value<FontClipping>     : std::integral_constant<FontClipping,FontClipping::Embedded>    {};
  template <> struct min_value<FontClipping>     : std::integral_constant<FontClipping,FontClipping::Default>     {};

  
  //! \enum FontFamily - Defines font families
  enum class FontFamily : ulong32
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

  //! Define limits traits
  template <> struct max_value<FontFamily>     : std::integral_constant<FontFamily,FontFamily::Decorative>  {};
  template <> struct min_value<FontFamily>     : std::integral_constant<FontFamily,FontFamily::Default>     {};


  ///////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<HFONT> - Encapsulates creating device context fonts
  ///////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<HFONT>
  {
    //! \var npos - Invalid handle sentinel value
    static const HFONT npos; 

    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HFONT>::create
    //! Create font handle
    //! 
    //! \tparam ENC - Encoding type
    //! \tparam LEN - Name buffer capacity
    //! 
    //! \param[in] name - Font name
    //! \param[in] height - Height in twips
    //! \param[in] weight - Weight
    //! \param[in] italic - Italics
    //! \param[in] underline - Underline
    //! \param[in] strike - Strikeout
    //! \param[in] charSet - Character set
    //! \param[in] quality - Quality
    //! \param[in] family - Font pitch and family
    //! \param[in] precision - Output precision
    //! \param[in] clipping - Clipping precision
    //! \param[in] width - Width in twips
    //! \param[in] escape - Escapement
    //! \param[in] orient - Orientation
    //! \return HAlloc<HFONT> - Created handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, unsigned LEN>
    static HAlloc<HFONT> create(const CharArray<ENC,LEN>& name, 
                                int32       height,  
                                FontWeight  weight = FontWeight::Normal, 
                                bool        italic = false, 
                                bool        underline = false, 
                                bool        strike = false, 
                                FontCharSet charSet = FontCharSet::Default, 
                                FontQuality quality = FontQuality::Default,
                                FontFamily  family = FontFamily::Default, 
                                FontPrecision precision = FontPrecision::Default, 
                                FontClipping clipping = FontClipping::Default,
                                int32       width = 0,  
                                int32       escape = 0, 
                                int32       orient = 0) 
    { 
      //! \var createFont - Functor for CreateFont
      static const auto createFont = getFunc<ENC>(::CreateFontA,::CreateFontW);

      // Create font 
      if (HFONT font = createFont(height, width, escape, orient, enum_cast(weight), italic ? TRUE : FALSE, underline ? TRUE : FALSE, strike ? TRUE : FALSE, 
                                  enum_cast(charSet), enum_cast(precision), enum_cast(clipping), enum_cast(quality), enum_cast(family), name))
        return { font, AllocType::Create };

      // Error: Failed  
      throw platform_error(HERE, "Unable to create font");
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HFONT>::create
    //! Accquire stock font handle
    //! 
    //! \param[in] obj - Stock object
    //! \return HAlloc<HFONT> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    ///////////////////////////////////////////////////////////////////////////////
    static HAlloc<HFONT> create(StockObject obj) 
    { 
      switch (obj)
      {
      case StockObject::OemFixedFont:
      case StockObject::AnsiFixedFont:
      case StockObject::AnsiVarFont:
      case StockObject::SystemFont:
      case StockObject::DeviceDefaultFont:
      case StockObject::SystemFixedFont:
      case StockObject::DefaultGuiFont:
        // Accquire stock font
        if (HFONT pen = (HFONT)::GetStockObject(enum_cast(obj)))
          return { pen, AllocType::WeakRef };
      }
      
      // Error: Failed  
      throw platform_error(HERE, "Unable to accquire stock font");
    }

    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HFONT>::clone
    //! Clone handle
    //! 
    //! \param[in] font - Handle
    //! \return HAlloc<HFONT> - Duplicate of handle
    //! 
    //! \throw wtl::platform_error - Failed to clone handle
    ///////////////////////////////////////////////////////////////////////////////
    static HAlloc<HFONT> clone(HAlloc<HFONT> font);

    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HFONT>::destroy noexcept
    //! Release handle 
    //! 
    //! \param[in] font - Handle
    //! \return bool - True iff closed successfully
    ///////////////////////////////////////////////////////////////////////////////
    static bool destroy(HAlloc<HFONT> font) noexcept
    {
      // Delete without checking if handle is valid
      switch (font.Method)
      {
      case AllocType::Accquire: return ::DeleteObject(font.Handle) != FALSE;
      case AllocType::Create:   return false;
      case AllocType::WeakRef:  return true;
      }
      return false;
    }
  };

  
  //! \alias HFont - Shared font handle
  using HFont = Handle<HFONT>;
  
} //namespace wtl
#endif // WTL_FONT_TRAITS_HPP

