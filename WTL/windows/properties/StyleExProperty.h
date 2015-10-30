//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\StyleExProperty.hpp
//! \brief Separate class declaration for the 'StyleEx' window property
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EX_WINDOW_STYLE_PROPERTY_H
#define WTL_EX_WINDOW_STYLE_PROPERTY_H

#include "wtl/WTL.hpp"
#include "wtl/traits/EncodingTraits.hpp"     //!< Encoding
#include "wtl/platform/WindowFlags.hpp"      //!< WindowStyleEx
#include "wtl/windows/PropertyImpl.hpp"      //!< PropertyImpl

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct StyleExPropertyImpl - Provides the getters and setters for the 'StyleEx' window property
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks Provides initial 'extended' window-style during window creation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct StyleExPropertyImpl : PropertyImpl<ENC,WindowStyleEx>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = StyleExPropertyImpl;

    //! \alias base - Define base type
    using base = PropertyImpl<ENC,WindowStyleEx>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;
    
    //! \alias window_t - Inherit window type
    using window_t = typename base::window_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // StyleExPropertyImpl::StyleExPropertyImpl
    //! Create window property and set initial 'extended' window style
    //! 
    //! \param[in,out] &wnd - Owner window
    //! \param[in] init - Initial 'extended' window style
    /////////////////////////////////////////////////////////////////////////////////////////
    StyleExPropertyImpl(window_t& wnd, WindowStyleEx style) : base(wnd, style)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // StyleExPropertyImpl::get const
    //! Get the current 'extended' window style
    //! 
    //! \return value_t - Current 'extended' window style if window exists, otherwise initial 'extended' window style
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // StyleExPropertyImpl::set 
    //! Set the current 'extended' window style iff window exists, otherwise initial 'extended' window style
    //! 
    //! \param[in] style - Extended window style
    //! 
    //! \throw wtl::platform_error - Unable to set window style
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t style);
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias StyleExProperty - Define 'extended' window style property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using StyleExProperty = Property<StyleExPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_EX_WINDOW_STYLE_PROPERTY_H
