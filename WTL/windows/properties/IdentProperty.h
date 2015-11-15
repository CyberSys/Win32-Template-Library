//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\IdentProperty.hpp
//! \brief Separate class declaration for the 'Ident' window property
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_ID_PROPERTY_H
#define WTL_WINDOW_ID_PROPERTY_H

#include <wtl/WTL.hpp>
#include <wtl/traits/EncodingTraits.hpp>     //!< Encoding
#include <wtl/platform/WindowFlags.hpp>      //!< WindowId
#include <wtl/windows/PropertyImpl.hpp>      //!< PropertyImpl

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct IdentPropertyImpl - Provides the getters and setters for the 'Ident' window property
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks Provides the initial value used during window creation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct IdentPropertyImpl : PropertyImpl<ENC,WindowId>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = IdentPropertyImpl;

    //! \alias base - Define base type
    using base = PropertyImpl<ENC,WindowId>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;
    
    //! \alias window_t - Inherit window type
    using window_t = typename base::window_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // IdentPropertyImpl::IdentPropertyImpl
    //! Create window property and set initial window-id
    //! 
    //! \param[in,out] &wnd - Owner window
    //! \param[in] init - [optional] Initial window id  (Default is zero)
    /////////////////////////////////////////////////////////////////////////////////////////
    IdentPropertyImpl(window_t& wnd, WindowId init = zero<WindowId>()) : base(wnd, init)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // IdentPropertyImpl::get const
    //! Get the current window id
    //! 
    //! \return value_t - Current window-id if window exists, otherwise initial window-id
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const; 

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // IdentPropertyImpl::set 
    //! Set the current window id iff window exists, otherwise initial window-id
    //! 
    //! \param[in] id - Window id
    //! 
    //! \throw wtl::platform_error - Unable to set window-id
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t id);
  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias IdentProperty - Define window id property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using IdentProperty = Property<IdentPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_WINDOW_ID_PROPERTY_H
