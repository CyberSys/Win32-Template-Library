/////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\gdi\Theme.hpp
//! \brief Provides themed window drawing through Visual styles
//! \date 30 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_THEME_H
#define WTL_THEME_H

#include <wtl/WTL.hpp>
#include <wtl/casts/BooleanCast.hpp>              //!< boolean_cast
#include <wtl/utils/String.hpp>                   //!< String
#include <wtl/utils/Size.hpp>                     //!< Size
#include <wtl/utils/Rectangle.hpp>                //!< Rect
#include <wtl/gdi/DeviceContext.hpp>              //!< DeviceContext
#include <wtl/traits/ThemeTraits.hpp>             //!< HTheme
#include <wtl/traits/WindowTraits.hpp>            //!< HWnd
#include <wtl/platform/HResult.hpp>               //!< HResult
#include <wtl/platform/DrawingFlags.hpp>          //!< DrawTextFlags
#include <vsstyle.h>                              //!< Parts and States
#include <Vssym32.h>                              //!< Properties

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Theme - Provides themed window drawing
  //!
  //! \remarks See "Parts & States" at https://msdn.microsoft.com/en-us/library/windows/desktop/bb773210(v=vs.85).aspx
  /////////////////////////////////////////////////////////////////////////////////////////
  struct Theme 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  public:
    //! \enum ThemeSize - Identifies the type of size value to retrieve for a visual style part
    enum ThemeSize
    {
      MinSize = TS_MIN,     //!< Minimum size
      TrueSize = TS_TRUE,   //!< True size
      DrawSize = TS_DRAW,   //!< Draw size
    };

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    HTheme   Handle;     //!< Theme handle

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Theme::Theme
    //! Create theme from class name
    //!
    //! \param[in] const& wnd - Owner window
    //! \param[in] const& name - Wide-character string containing a semicolon-separated list of classes.
    //! 
    //! \throw wtl::platform_error - Failed to acquire theme handle
    /////////////////////////////////////////////////////////////////////////////////////////
    Theme(const HWnd& wnd, const String<Encoding::UTF16>& names) : Handle(wnd, names)
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    // ----------------------------------- STATIC METHODS -----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Theme::active
    //! Tests if a visual style for the current application is active.
    //!
    //! \return bool - True iff visual style active
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool active()
    {
      return boolean_cast(::IsThemeActive());
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Theme::content const
    //! Retrieves the size of the content area for the background defined by the visual style.
    //!
    //! \param[in] const& dc - Target device context
    //! \param[in] part - Part to query
    //! \param[in] state - State of specified part
    //! \param[in] const& rc - Background rectangle
    //! \return RectL - Desired content rectangle
    //! 
    //! \throw wtl::platform_error - Unable to query content rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PART, typename STATE>
    RectL content(const DeviceContext& dc, PART part, STATE state, const RectL& rc) const
    {
      RectL c;
      // Query content rectangle
      if (!HResult(::GetThemeBackgroundContentRect(Handle, dc.handle(), part, state, const_cast<RectL&>(rc), c)))
        throw platform_error(HERE, "Unable to query themed control content rectangle");
      return c;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Theme::fill const
    //! Draws the border and background defined by the visual style for the specified control part.
    //!
    //! \param[in] const& dc - Target device context
    //! \param[in] part - Part to draw
    //! \param[in] state - State of specified part
    //! \param[in] const& rc - Drawing rectangle
    //! 
    //! \throw wtl::platform_error - Unable to draw background
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PART, typename STATE>
    void fill(const DeviceContext& dc, PART part, STATE state, const RectL& rc) const
    {
      if (!HResult(::DrawThemeBackground(Handle, dc.handle(), part, state, const_cast<RectL&>(rc), nullptr)))
        throw platform_error(HERE, "Unable to draw themed control background");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Theme::fill const
    //! Draws the border and background defined by the visual style for the specified control part.
    //!
    //! \param[in] const& dc - Target device context
    //! \param[in] part - Part to draw
    //! \param[in] state - State of specified part
    //! \param[in] const& rc - Drawing rectangle
    //! \param[in] const& clip - Clipping rectangle
    //! 
    //! \throw wtl::platform_error - Unable to draw background
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PART, typename STATE>
    void fill(const DeviceContext& dc, PART part, STATE state, const RectL& rc, const RectL& clip) const
    {
      if (!HResult(::DrawThemeBackground(Handle, dc.handle(), part, state, const_cast<RectL&>(rc), const_cast<RectL&>(clip))))
        throw platform_error(HERE, "Unable to draw themed control background");
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Theme::margins const
    //! Retrieves the margins of a theme component
    //!
    //! \param[in] const& dc - Target device context
    //! \param[in] part - Part to query
    //! \param[in] state - State of specified part
    //! \param[in] prop - Margins identifier
    //! \return ::MARGINS - Margins of specified part
    //! 
    //! \throw wtl::platform_error - Unable to query margins
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PART, typename STATE, typename PROPERTY>
    ::MARGINS margins(const DeviceContext& dc, PART part, STATE state, PROPERTY prop) const
    {
      ::MARGINS m;
      // Query margins
      if (!HResult(::GetThemeMargins(Handle, dc.handle(), part, state, prop, nullptr, &m)))
        throw platform_error(HERE, "Unable to query themed control margins");
      return m;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Theme::measure const
    //! Retrieves the margins of a theme component
    //!
    //! \param[in] const& dc - Target device context
    //! \param[in] part - Part to query
    //! \param[in] state - State of specified part
    //! \param[in] type - [optional] Whether to query the true, minimimum, or draw size
    //! \return SizeL - Size of specified part
    //! 
    //! \throw wtl::platform_error - Unable to measure part
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PART, typename STATE>
    SizeL measure(const DeviceContext& dc, PART part, STATE state, ThemeSize type = ThemeSize::TrueSize) const
    {
      SizeL sz;
      // Query size
      if (!HResult(::GetThemePartSize(Handle, dc.handle(), part, state, nullptr, static_cast<THEMESIZE>(type), sz)))
        throw platform_error(HERE, "Unable to query size of themed control part");
      return sz;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Theme::measure const
    //! Measures the size required to draw a string 
    //!
    //! \param[in] const& dc - Target device context
    //! \param[in] part - Part to query
    //! \param[in] state - State of specified part
    //! \param[in] const& txt - Text to draw
    //! \param[in] flags - Drawing flags 
    //! \return SizeL - Extent of text
    //! 
    //! \throw wtl::platform_error - Unable to measure part
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PART, typename STATE>
    SizeL measure(const DeviceContext& dc, PART part, STATE state, const String<Encoding::UTF16>& str, DrawTextFlags flags = DrawTextFlags::VCentre|DrawTextFlags::SingleLine) const
    {
      RectL rc;
      // Query text rectangle
      if (!HResult(::GetThemeTextExtent(Handle, dc.handle(), part, state, str.c_str(), static_cast<int>(str.size()), enum_cast(flags), nullptr, rc)))
        throw platform_error(HERE, "Unable to measure themed control text");
      return rc.size();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Theme::measure const
    //! Measures the rectangle required to draw a string 
    //!
    //! \param[in] const& dc - Target device context
    //! \param[in] part - Part to query
    //! \param[in] state - State of specified part
    //! \param[in] const& txt - Text to draw
    //! \param[in] const& rc - Bounding rectangle
    //! \param[in] flags - Drawing flags 
    //! \return RectL - Required drawing rectangle
    //! 
    //! \throw wtl::platform_error - Unable to measure part
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PART, typename STATE>
    RectL measure(const DeviceContext& dc, PART part, STATE state, const String<Encoding::UTF16>& str, const RectL& rc, DrawTextFlags flags = DrawTextFlags::VCentre|DrawTextFlags::SingleLine) const
    {
      RectL out;
      // Query text rectangle
      if (!HResult(::GetThemeTextExtent(Handle, dc.handle(), part, state, str.c_str(), static_cast<int>(str.size()), enum_cast(flags), rc, out)))
        throw platform_error(HERE, "Unable to measure themed control text");
      return out;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Theme::write const
    //! Draws text using the color and font defined by the visual style.
    //!
    //! \param[in] const& dc - Target device context
    //! \param[in] part - Part to draw. If this value is 0, the text is drawn in the default font, or a font selected into the device context.
    //! \param[in] state - State of specified part
    //! \param[in] const& txt - Text to draw
    //! \param[in] const& rc - Drawing rectangle
    //! \param[in] flags - Drawing flags (Note: DT_CALCRECT is not supported)
    //! 
    //! \throw wtl::platform_error - Unable to draw text
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PART, typename STATE>
    void write(const DeviceContext& dc, PART part, STATE state, const String<Encoding::UTF16>& str, const RectL& rc, DrawTextFlags flags = DrawTextFlags::VCentre|DrawTextFlags::SingleLine) const
    {
      if (!HResult(::DrawThemeText(Handle, dc.handle(), part, state, str.c_str(), static_cast<int>(str.size()), enum_cast(flags), 0, const_cast<RectL&>(rc))))
        throw platform_error(HERE, "Unable to draw themed control text");
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator+=
  //! Enlarges a rectangle by the specified visual styles margins
  //!
  //! \param[in,out] & rc - Rectangle to enlarge
  //! \param[in] const& m - Margins
  //! \return Rect<T>& - Reference to updated 'rc'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  Rect<T>& operator+= (Rect<T>& rc, const ::MARGINS& m)
  {
    rc.Left -= m.cxLeftWidth;
    rc.Top -= m.cyTopHeight;
    rc.Right += m.cxRightWidth;
    rc.Bottom += m.cyBottomHeight;
    return rc;
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator+=
  //! Enlarges a 'size' by the specified visual styles margins
  //!
  //! \param[in,out] & sz - Size to enlarge
  //! \param[in] const& m - Margins
  //! \return Size<T>& - Reference to updated 'sz'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  Size<T>& operator+= (Size<T>& sz, const ::MARGINS& m)
  {
    sz.Width += (m.cxLeftWidth + m.cxRightWidth);
    sz.Height += (m.cyTopHeight + m.cyBottomHeight);
    return sz;
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator + constexpr
  //! Calculates the result of enlarging a size by a visual styles margin
  //!
  //! \param[in] const& sz - Size 
  //! \param[in] const& m - Margins
  //! \return Size<T> - Result of enlarging 'sz' by 'm'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> constexpr
  Size<T> operator + (const Size<T>& sz, const ::MARGINS& m)
  {
    return { sz.Width + m.cxLeftWidth + m.cxRightWidth, 
             sz.Height + m.cyTopHeight + m.cyBottomHeight };
  }

} // namespace wtl
#endif // WTL_THEME_H

