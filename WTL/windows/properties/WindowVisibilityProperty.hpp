//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\WindowVisibilityProperty.hpp
//! \brief Encapsulates the visibility of a window in an appropriate enumeration property
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_VISIBLE_PROPERTY_HPP
#define WTL_WINDOW_VISIBLE_PROPERTY_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EncodingTraits.hpp"                  //!< Encoding
#include "wtl/platform/WindowFlags.hpp"                   //!< ShowWindowFlags
#include "wtl/windows/properties/WindowProperty.hpp"      //!< WindowPropertyImpl

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct WindowVisibilityPropertyImpl - Encapsulates the visibility of a window in a read/write enumeration property.
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks When the window does not exist this provides the initial value used during window creation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct WindowVisibilityPropertyImpl : WindowPropertyImpl<ENC,ShowWindowFlags,PropertyAccess::ReadWrite>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = WindowVisibilityPropertyImpl;

    //! \alias base - Define base type
    using base = WindowPropertyImpl<ENC,ShowWindowFlags,PropertyAccess::ReadWrite>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowVisibilityPropertyImpl::WindowVisibilityPropertyImpl
    //! Create with initial value
    //! 
    //! \param[in,out] &wnd - Owner window
    //! \param[in] init - Initial visibility
    /////////////////////////////////////////////////////////////////////////////////////////
    WindowVisibilityPropertyImpl(WindowBase<ENC>& wnd, ShowWindowFlags init) : base(wnd, init)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowVisibilityPropertyImpl::get const
    //! Get the window visibility
    //! 
    //! \return value_t - Current visibility if window exists, otherwise 'initial' visibility
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowVisibilityPropertyImpl::set 
    //! Set the current window visibility iff window exists, otherwise 'initial' visibility
    //! 
    //! \param[in] visibility - Window visibility
    /////////////////////////////////////////////////////////////////////////////////////////
    void set(value_t visibility);
  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias WindowVisibilityProperty - Define window visibliity property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using WindowVisibilityProperty = Property<WindowVisibilityPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_WINDOW_VISIBLE_PROPERTY_HPP
