//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\StyleProperty.hpp
//! \brief Encapsulates the basic window style in an appropriate enumeration property
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_STYLE_PROPERTY_HPP
#define WTL_WINDOW_STYLE_PROPERTY_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EncodingTraits.hpp"                  //!< Encoding
#include "wtl/platform/WindowFlags.hpp"                   //!< WindowStyle
#include "wtl/windows/properties/WindowProperty.hpp"      //!< WindowPropertyImpl

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct StylePropertyImpl - Encapsulates the basic-window-style in a read/write property.
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks When the window does not exist this provides the initial value used during window creation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct StylePropertyImpl : WindowPropertyImpl<ENC,WindowStyle,PropertyAccess::ReadWrite>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = StylePropertyImpl;

    //! \alias base - Define base type
    using base = WindowPropertyImpl<ENC,WindowStyle,PropertyAccess::ReadWrite>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // StylePropertyImpl::StylePropertyImpl
    //! Create with initial value
    //! 
    //! \param[in,out] &wnd - Owner window
    //! \param[in] init - Initial window style
    /////////////////////////////////////////////////////////////////////////////////////////
    StylePropertyImpl(WindowBase<ENC>& wnd, WindowStyle style) : base(wnd, style)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // StylePropertyImpl::get const
    //! Get the window style
    //! 
    //! \return value_t - Current style if window exists, otherwise 'initial' style
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // StylePropertyImpl::set 
    //! Set the current window style iff window exists, otherwise 'initial' style
    //! 
    //! \param[in] style - Window style
    //! 
    //! \throw wtl::platform_error - Unable to set window style
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t style);
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias StyleProperty - Define window style property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using StyleProperty = Property<StylePropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_WINDOW_STYLE_PROPERTY_HPP