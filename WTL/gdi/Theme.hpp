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

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Theme - 
  //! 
  //! \tparam ENC - Character encoding 
  //!
  //! \remarks See "Parts & States" at https://msdn.microsoft.com/en-us/library/windows/desktop/bb773210(v=vs.85).aspx
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct Theme 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    //! \var encoding - Define character encoding
    static constexpr Encoding encoding = ENC;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    HTheme      Handle;     //!< Theme handle

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
    Theme(const HWnd& wnd, const String<Encoding::UTF16>& names) : Handle(names)
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
    //! \param[in] const& hdc - Target device context
    //! \param[in] part - Part to draw
    //! \param[in] state - State of specified part
    //! \param[in] const& rc - Drawing rectangle
    //! 
    //! \throw wtl::platform_error - Unable to draw background
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PART, typename STATE>
    void drawBackground(const HDeviceContext& dc, PART part, STATE state, const RectL& rc) const
    {
      if (!HResult(::DrawThemeBackground(Handle, dc, part, state, const_cast<RectL&>(rc), nullptr)))
        throw platform_error(HERE, "Unable to draw themed control background");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Theme::drawBackground const
    //! Draws the border and fill defined by the visual style for the specified control part.
    //!
    //! \param[in] const& hdc - Target device context
    //! \param[in] part - Part to draw
    //! \param[in] state - State of specified part
    //! \param[in] const& rc - Drawing rectangle
    //! \param[in] const& clip - Clipping rectangle
    //! 
    //! \throw wtl::platform_error - Unable to draw background
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PART, typename STATE>
    void drawBackground(const HDeviceContext& dc, PART part, STATE state, const RectL& rc, const RectL& clip) const
    {
      if (!HResult(::DrawThemeBackground(Handle, dc, part, state, const_cast<RectL&>(rc), const_cast<RectL&>(clip))))
        throw platform_error(HERE, "Unable to draw themed control background");
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Theme::drawText const
    //! Draws text using the color and font defined by the visual style.
    //!
    //! \param[in] const& hdc - Target device context
    //! \param[in] part - Part to draw. If this value is 0, the text is drawn in the default font, or a font selected into the device context.
    //! \param[in] state - State of specified part
    //! \param[in] const& txt - Text to draw
    //! \param[in] const& rc - Drawing rectangle
    //! \param[in] flags - Drawing flags (Note: DT_CALCRECT is not supported)
    //! 
    //! \throw wtl::platform_error - Unable to draw text
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PART, typename STATE>
    void drawText(const HDeviceContext& dc, PART part, STATE state, const String<Encoding::UTF16>& str, const RectL& rc, DrawTextFlags flags) const
    {
      if (!HResult(::DrawThemeText(Handle, dc, part, state, str.c_str(), str.size(), enum_cast(flags), 0, const_cast<RectL&>(rc))))
        throw platform_error(HERE, "Unable to draw themed control text");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Theme::getContentRect const
    //! Retrieves the size of the content area for the background defined by the visual style.
    //!
    //! \param[in] const& hdc - Target device context
    //! \param[in] part - Part to query
    //! \param[in] state - State of specified part
    //! \param[in] const& rc - Background rectangle
    //! \param[in,out] & clip - On return this contains the content rectangle
    //! 
    //! \throw wtl::platform_error - Unable to query content rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename PART, typename STATE>
    void getContentRect(const HDeviceContext& dc, PART part, STATE state, const RectL& rc, RectL& content) const
    {
      if (!HResult(::GetThemeBackgroundContentRect(Handle, dc, part, state, const_cast<RectL&>(rc), content)))
        throw platform_error(HERE, "Unable to query themed control content rectangle");
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
  };

} // namespace wtl
#endif // WTL_THEME_H

