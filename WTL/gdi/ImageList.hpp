/////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\gdi\ImageList.hpp
//! \brief Encapsulates a Win32 ImageList
//! \date 6 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_IMAGE_LIST_H
#define WTL_IMAGE_LIST_H

#include <wtl/WTL.hpp>
#include <wtl/gdi/DeviceContext.hpp>               //!< HDeviceContext
#include <wtl/traits/ImageListTraits.hpp>          //!< HImageList
#include <wtl/platform/HResult.hpp>                //!< HResult

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ImageList - 
  //!
  //! \remarks See "Parts & States" at https://msdn.microsoft.com/en-us/library/windows/desktop/bb773210(v=vs.85).aspx
  /////////////////////////////////////////////////////////////////////////////////////////
  struct ImageList 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
     
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    HImageList   Handle;     //!< ImageList handle
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ImageList::ImageList
    //! Create theme from class name
    //!
    //! \param[in] const& sz - Image size
    //! \param[in] flags - Creation flags
    //! \param[in] count - Number of initial images
    //! \param[in] grow - Number of extra elements to add when the capacity is reached
    //! 
    //! \throw wtl::platform_error - Failed to acquire theme handle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    ImageList(const Size<T>& sz, ImageListType flags, int32_t count, int32_t grow) : Handle(wnd, names)
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    // ----------------------------------- STATIC METHODS -----------------------------------
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ImageList::drawBackground const
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
      if (!HResult(::DrawImageListBackground(Handle, dc.handle(), part, state, const_cast<RectL&>(rc), nullptr)))
        throw platform_error(HERE, "Unable to draw themed control background");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ImageList::drawBackground const
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
      if (!HResult(::DrawImageListBackground(Handle, dc.handle(), part, state, const_cast<RectL&>(rc), const_cast<RectL&>(clip))))
        throw platform_error(HERE, "Unable to draw themed control background");
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ImageList::drawText const
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
    void drawText(const DeviceContext& dc, PART part, STATE state, const String<Encoding::UTF16>& str, const RectL& rc, DrawTextFlags flags) const
    {
      if (!HResult(::DrawImageListText(Handle, dc.handle(), part, state, str.c_str(), str.size(), enum_cast(flags), 0, const_cast<RectL&>(rc))))
        throw platform_error(HERE, "Unable to draw themed control text");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ImageList::getContentRect const
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
      if (!HResult(::GetImageListBackgroundContentRect(Handle, dc.handle(), part, state, const_cast<RectL&>(rc), content)))
        throw platform_error(HERE, "Unable to query themed control content rectangle");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ImageList::getMargins const
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
      if (!HResult(::GetImageListMargins(Handle, dc.handle(), part, state, prop, nullptr, &margins)))
        throw platform_error(HERE, "Unable to query themed control margins");
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
#endif // WTL_IMAGE_LIST_H

