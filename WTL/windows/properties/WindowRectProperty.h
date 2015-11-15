//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\WindowRectProperty.h
//! \brief Separate class declaration for the 'WindowRect' window property
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_RECT_PROPERTY_H
#define WTL_WINDOW_RECT_PROPERTY_H

#include <wtl/WTL.hpp>
#include <wtl/utils/Rectangle.hpp>           //!< RectL
#include <wtl/traits/EncodingTraits.hpp>     //!< Encoding
#include <wtl/windows/PropertyImpl.hpp>      //!< PropertyImpl

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct WindowRectPropertyImpl - Provides the getters and setters for the 'WindowRect' window property
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks [WINDOW NOT EXISTS] Values derived from 'Size' and 'Position' window properties
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct WindowRectPropertyImpl : PropertyImpl<ENC,RectL>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = WindowRectPropertyImpl<ENC>;

    //! \alias base - Define base type
    using base = PropertyImpl<ENC,RectL>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;
    
    //! \alias window_t - Inherit window type
    using window_t = typename base::window_t;
    
    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowRectPropertyImpl::WindowRectPropertyImpl
    //! Create window property
    //! 
    //! \param[in,out] &wnd - Owner window
    //!
    //! \remarks Initial value is determined from offline 'Size' and 'Position'
    /////////////////////////////////////////////////////////////////////////////////////////
    WindowRectPropertyImpl(window_t& wnd) : base(wnd, defvalue<value_t>())
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowRectPropertyImpl::get const
    //! Get the window rectangle
    //! 
    //! \return value_t - Returns the current window rectangle if window exists, otherwise return the initial window rectangle
    //!
    //! \throw wtl::logic_error - Window is using default size or location
    //! \throw wtl::platform_error - Unable to query window rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowRectPropertyImpl::set 
    //! Set the current window rectangle iff window exists, otherwise the initial window rectangle
    //! 
    //! \param[in] rectangle - Window rectangle
    //! 
    //! \throw wtl::platform_error - Unable to set window position
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t rectangle);
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias WindowRectProperty - Define window rectangle property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using WindowRectProperty = Property<WindowRectPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_WINDOW_RECT_PROPERTY_H
