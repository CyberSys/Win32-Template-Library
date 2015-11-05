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

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Theme - 
  //! 
  //! \tparam ENC - Character encoding 
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
    Theme(const HWnd& wnd, const String<Encoding::UTF16>& names)  : Handle(names)
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };

} // namespace wtl
#endif // WTL_THEME_H

