//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\combobox\ComboBoxMinVisibleProperty.hpp
//! \brief Separate class declaration for the ComboBox control 'MinVisible' property
//! \date 28 February 2016
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_COMBOBOX_MINVISIBLE_PROPERTY_H
#define WTL_COMBOBOX_MINVISIBLE_PROPERTY_H

#include <wtl/WTL.hpp>
#include <wtl/traits/EncodingTraits.hpp>              //!< Encoding
#include <wtl/windows/PropertyImpl.hpp>               //!< PropertyImpl

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  //! Forward declaration
  template <Encoding ENC>
  struct ComboBox;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ComboBoxMinVisiblePropertyImpl - Provides the getters and setters for the ComboBox control 'MinVisible' property
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ComboBoxMinVisiblePropertyImpl : PropertyImpl<ENC,uint32_t,ComboBox<ENC>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = ComboBoxMinVisiblePropertyImpl;

    //! \alias base - Define base type
    using base = PropertyImpl<ENC,uint32_t,ComboBox<ENC>>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;
    
    //! \alias window_t - Inherit window type
    using window_t = typename base::window_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBoxMinVisiblePropertyImpl::ComboBoxMinVisiblePropertyImpl
    //! Create window property 
    //! 
    //! \param[in,out] &wnd - Owner window
    /////////////////////////////////////////////////////////////////////////////////////////
    ComboBoxMinVisiblePropertyImpl(window_t& wnd) : base(wnd, 30)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBoxMinVisiblePropertyImpl::get const
    //! Get the 'MinVisible' state
    //! 
    //! \return value_t - Current 'MinVisible' state if ComboBox exists, otherwise false
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // ComboBoxMinVisiblePropertyImpl::set 
    //! Set the current 'MinVisible' state iff ComboBox exists, otherwise this has no effect
    //! 
    //! \param[in] num - Minimum number of items to display
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t state);

  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ComboBoxMinVisibleProperty - Defines type of ComboBox control 'MinVisible' property
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ComboBoxMinVisibleProperty = Property<ComboBoxMinVisiblePropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_COMBOBOX_MINVISIBLE_PROPERTY_H
