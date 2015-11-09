/////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\gdi\Theme.hpp
//! \brief Supports Visual styles
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_THEME_H
#define WTL_THEME_H

#include "wtl/WTL.hpp"
#include "wtl/gdi/DeviceContext.hpp"               //!< HDeviceContext
#include "wtl/traits/ThemeTraits.hpp"              //!< HTheme
#include "wtl/platform/HResult.hpp"                //!< HResult
#include <vsstyle.h>                               //!< Parts and States
#include <Vssym32.h>                               //!< Properties

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Theme - 
  //!
  //! \remarks See "Parts & States" at https://msdn.microsoft.com/en-us/library/windows/desktop/bb773210(v=vs.85).aspx
  /////////////////////////////////////////////////////////////////////////////////////////
  struct Theme 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
     
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
    // Theme::isActive
    //! Tests if a visual style for the current application is active.
    //!
    //! \return bool - True iff visual style active
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool isActive()
    {
      return boolean_cast(::IsThemeActive());
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Theme::drawBackground const
    //! Draws the border and fill defined by the visual style for the specified control part.
    //!
    //! \param[in] const& dc - Target device context
    //! \param[in] part - Part to draw
    //! \param[in] state - State of specified part
    //! \param[in] const& rc - Drawing rectangle
    //! 
    //! \throw wtl::platform_error - Unable to draw background
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PART, typename STATE>
    void drawBackground(const DeviceContext& dc, PART part, STATE state, const RectL& rc) const
    {
      if (!HResult(::DrawThemeBackground(Handle, dc.handle(), part, state, const_cast<RectL&>(rc), nullptr)))
        throw platform_error(HERE, "Unable to draw themed control background");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Theme::drawBackground const
    //! Draws the border and fill defined by the visual style for the specified control part.
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
    void drawBackground(const DeviceContext& dc, PART part, STATE state, const RectL& rc, const RectL& clip) const
    {
      if (!HResult(::DrawThemeBackground(Handle, dc.handle(), part, state, const_cast<RectL&>(rc), const_cast<RectL&>(clip))))
        throw platform_error(HERE, "Unable to draw themed control background");
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Theme::getContentRect const
    //! Retrieves the size of the content area for the background defined by the visual style.
    //!
    //! \param[in] const& dc - Target device context
    //! \param[in] part - Part to query
    //! \param[in] state - State of specified part
    //! \param[in] const& rc - Background rectangle
    //! \param[in,out] & clip - On return this contains the content rectangle
    //! 
    //! \throw wtl::platform_error - Unable to query content rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PART, typename STATE>
    void getContentRect(const DeviceContext& dc, PART part, STATE state, const RectL& rc, RectL& content) const
    {
      if (!HResult(::GetThemeBackgroundContentRect(Handle, dc.handle(), part, state, const_cast<RectL&>(rc), content)))
        throw platform_error(HERE, "Unable to query themed control content rectangle");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Theme::getMargins const
    //! Retrieves the margins of a theme component
    //!
    //! \param[in] const& dc - Target device context
    //! \param[in] part - Part to query
    //! \param[in] state - State of specified part
    //! \param[in] prop - Margins identifier
    //! \param[in,out] &margins - On return this contains the margins
    //! 
    //! \throw wtl::platform_error - Unable to query margins
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PART, typename STATE, typename PROPERTY>
    void getMargins(const DeviceContext& dc, PART part, STATE state, PROPERTY prop, ::MARGINS& margins) const
    {
      if (!HResult(::GetThemeMargins(Handle, dc.handle(), part, state, prop, nullptr, &margins)))
        throw platform_error(HERE, "Unable to query themed control margins");
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
    void write(const DeviceContext& dc, PART part, STATE state, const String<Encoding::UTF16>& str, const RectL& rc, DrawTextFlags flags) const
    {
      if (!HResult(::DrawThemeText(Handle, dc.handle(), part, state, str.c_str(), str.size(), enum_cast(flags), 0, const_cast<RectL&>(rc))))
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

} // namespace wtl
#endif // WTL_THEME_H

