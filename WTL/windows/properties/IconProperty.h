//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\IconProperty.hpp
//! \brief Separate class declaration for the Button control 'Icon' property
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_ICON_PROPERTY_H
#define WTL_WINDOW_ICON_PROPERTY_H

#include <wtl/WTL.hpp>
#include <wtl/traits/EncodingTraits.hpp>     //!< Encoding
#include <wtl/traits/IconTraits.hpp>         //!< HIcon
#include <wtl/windows/PropertyImpl.hpp>      //!< PropertyImpl

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  //! Forward declaration
  template <Encoding ENC>
  struct Button;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct IconPropertyImpl - Provides the getters and setters for the 'Icon' window property
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks All windows are created using the default system font, therefore this property does not define the 'initial' font.
  //! \remarks The font is stored as a shared-handle which is not necessarily released when the window is destroyed.
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct IconPropertyImpl : PropertyImpl<ENC,HIcon,Button<ENC>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = IconPropertyImpl;

    //! \alias base - Define base type
    using base = PropertyImpl<ENC,HIcon,Button<ENC>>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;
    
    //! \alias window_t - Inherit window type
    using window_t = typename base::window_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // IconPropertyImpl::IconPropertyImpl
    //! Create window property 
    //! 
    //! \param[in,out] &wnd - Owner window
    /////////////////////////////////////////////////////////////////////////////////////////
    IconPropertyImpl(window_t& wnd) : base(wnd)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // IconPropertyImpl::exist const
    //! Query whether button has an icon
    //! 
    //! \return bool - True iff icon assigned
    /////////////////////////////////////////////////////////////////////////////////////////
    bool exists() const
    {
      return this->Value.exists();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // IconPropertyImpl::get const
    //! Get the button icon
    //! 
    //! \return value_t - Current icon if button exists, otherwise initial icon
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // IconPropertyImpl::set 
    //! Set the icon iff button exists, otherwise sets the initial icon
    //! 
    //! \param[in] icon - Button icon
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t font);
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // IconPropertyImpl::set 
    //! Set the previously assigned icon
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set();

  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias IconProperty - Define button icon property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using IconProperty = Property<IconPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_WINDOW_ICON_PROPERTY_H
