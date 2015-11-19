//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\VisibilityProperty.h
//! \brief Separate class declaration for the 'Visibility' window property
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_VISIBLE_PROPERTY_H
#define WTL_WINDOW_VISIBLE_PROPERTY_H

#include <wtl/WTL.hpp>
#include <wtl/casts/BaseCast.hpp>            //!< BaseCast
#include <wtl/utils/Clear.hpp>               //!< Clear
#include <wtl/traits/EncodingTraits.hpp>     //!< Encoding
#include <wtl/platform/WindowFlags.hpp>      //!< ShowWindowFlags
#include <wtl/windows/PropertyImpl.hpp>      //!< PropertyImpl

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
    //! \alias base - Define base type
    using base = ::WINDOWPLACEMENT;

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowPlacement::WindowPlacement
    //! Initailizes the 'Length' field and zero-initializes the remainder
    /////////////////////////////////////////////////////////////////////////////////////////
    WindowPlacement() 
    {
      clear(*base_cast(this));
      this->length = sizeof(base);
    }
  };
  

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct VisibilityPropertyImpl - Provides the getters and setters for the 'Visible' window property
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks Provides initial visibity during window creation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct VisibilityPropertyImpl : PropertyImpl<ENC,bool>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = VisibilityPropertyImpl;

    //! \alias base - Define base type
    using base = PropertyImpl<ENC,bool>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;
    
    //! \alias window_t - Inherit window type
    using window_t = typename base::window_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // VisibilityPropertyImpl::VisibilityPropertyImpl
    //! Create window property and set initial window visibility
    //! 
    //! \param[in,out] &wnd - Owner window
    //! \param[in] init - Initial visibility
    /////////////////////////////////////////////////////////////////////////////////////////
    VisibilityPropertyImpl(window_t& wnd, bool init) : base(wnd, init)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // VisibilityPropertyImpl::get const
    //! Get the window visibility
    //! 
    //! \return value_t - Current visibility if window exists, otherwise initial window visibility
    //! 
    //! \throw wtl::platform_error - Unable to query window visibility
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // VisibilityPropertyImpl::set 
    //! Set the current window visibility iff window exists, otherwise sets initial window visibility
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

#endif // WTL_WINDOW_VISIBLE_PROPERTY_H
