//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\VisibilityProperty.hpp
//! \brief Encapsulates the visibility of a window in an appropriate enumeration property
//! \date 25 October 2015
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
  //! \struct WindowPlacement - Window placement
  /////////////////////////////////////////////////////////////////////////////////////////
  struct WindowPlacement : ::WINDOWPLACEMENT
  {
    using base = ::WINDOWPLACEMENT;

    WindowPlacement() 
    {
      clear(*base_cast(this));
      this->length = sizeof(base);
    }
  };
  

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct VisibilityPropertyImpl - Encapsulates the visibility of a window in a read/write enumeration property.
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks When the window does not exist this provides the initial value used during window creation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct VisibilityPropertyImpl : WindowPropertyImpl<ENC,ShowWindowFlags,PropertyAccess::ReadWrite>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = VisibilityPropertyImpl;

    //! \alias base - Define base type
    using base = WindowPropertyImpl<ENC,ShowWindowFlags,PropertyAccess::ReadWrite>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // VisibilityPropertyImpl::VisibilityPropertyImpl
    //! Create with initial value
    //! 
    //! \param[in,out] &wnd - Owner window
    //! \param[in] init - Initial visibility
    /////////////////////////////////////////////////////////////////////////////////////////
    VisibilityPropertyImpl(WindowBase<ENC>& wnd, ShowWindowFlags init) : base(wnd, init)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // VisibilityPropertyImpl::get const
    //! Get the window visibility
    //! 
    //! \return value_t - Current visibility if window exists, otherwise 'initial' visibility
    //! 
    //! \throw wtl::platform_error - Unable to query window visibility
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // VisibilityPropertyImpl::set 
    //! Set the current window visibility iff window exists, otherwise 'initial' visibility
    //! 
    //! \param[in] visibility - Window visibility
    //! 
    //! \throw wtl::platform_error - Unable to set window visibility
    /////////////////////////////////////////////////////////////////////////////////////////
    void set(value_t visibility);
  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias VisibilityProperty - Define window visibliity property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using VisibilityProperty = Property<VisibilityPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_WINDOW_VISIBLE_PROPERTY_HPP
