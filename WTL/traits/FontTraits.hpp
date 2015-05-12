//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\FontTraits.hpp
//! \brief Defines font traits + helper global operators
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_FONT_TRAITS_HPP
#define WTL_FONT_TRAITS_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/Handle.hpp"               //!< Handle
#include "wtl/utils/CharArray.hpp"            //!< CharArray
#include "wtl/platform/DrawingFlags.hpp"      //!< StockObject, FontStyles
#include "wtl/casts/EnumCast.hpp"             //!< EnumCast

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias HFont - Shared font handle
  /////////////////////////////////////////////////////////////////////////////////////////
  using HFont = Handle<::HFONT>;
  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<::HFONT> - Encapsulates creating device context fonts
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<::HFONT>
  {
    //! \var npos - Invalid handle sentinel value
    static constexpr ::HFONT npos = default<::HFONT>(); 

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HFONT>::create
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
    //! \return HAlloc<::HFONT> - Created handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, unsigned LEN>
    static HAlloc<::HFONT> create(const CharArray<ENC,LEN>& name, 
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
      if (::HFONT font = createFont(height, width, escape, orient, enum_cast(weight), italic ? TRUE : FALSE, underline ? TRUE : FALSE, strike ? TRUE : FALSE, 
                                  enum_cast(charSet), enum_cast(precision), enum_cast(clipping), enum_cast(quality), enum_cast(family), name))
        return { font, AllocType::Create };

      // Error: Failed  
      throw platform_error(HERE, "Unable to create font");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HFONT>::create
    //! Accquire stock font handle
    //! 
    //! \param[in] obj - Stock object
    //! \return HAlloc<::HFONT> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static HAlloc<::HFONT> create(StockObject obj) 
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
        if (::HFONT pen = (::HFONT)::GetStockObject(enum_cast(obj)))
          return { pen, AllocType::WeakRef };
      }
      
      // Error: Failed  
      throw platform_error(HERE, "Unable to accquire stock font");
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HFONT>::clone
    //! Clone handle
    //! 
    //! \param[in] font - Handle
    //! \return HAlloc<::HFONT> - Duplicate of handle
    //! 
    //! \throw wtl::platform_error - Failed to clone handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static HAlloc<::HFONT> clone(HAlloc<::HFONT> font);

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HFONT>::destroy noexcept
    //! Release handle 
    //! 
    //! \param[in] font - Handle
    //! \return bool - True iff closed successfully
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool destroy(HAlloc<::HFONT> font) noexcept
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

  
} //namespace wtl
#endif // WTL_FONT_TRAITS_HPP

