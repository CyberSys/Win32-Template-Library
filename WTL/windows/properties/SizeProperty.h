//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\SizeProperty.h
//! \brief Separate class declaration for the 'Size' window property
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_SIZE_PROPERTY_H
#define WTL_WINDOW_SIZE_PROPERTY_H

#include <wtl/WTL.hpp>
#include <wtl/utils/Size.hpp>                //!< SizeL
#include <wtl/traits/EncodingTraits.hpp>     //!< Encoding
#include <wtl/windows/PropertyImpl.hpp>      //!< PropertyImpl

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct SizePropertyImpl - Provides the getters and setters for the 'Size' window property
  //! 
  //! \tparam ENC - Window encoding
  //! 
  //! \remarks [WINDOW NOT EXIST] Provides initial size during window creation. 
  //! \remarks [WINDOW EXISTS] Value derived from 'WindowRect' window property.
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct SizePropertyImpl : PropertyImpl<ENC,SizeL>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = SizePropertyImpl;

    //! \alias base - Define base type
    using base = PropertyImpl<ENC,SizeL>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;
    
    //! \alias window_t - Inherit window type
    using window_t = typename base::window_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // SizePropertyImpl::SizePropertyImpl
    //! Create property and set initial window size
    //! 
    //! \param[in,out] &wnd - Owner window
    //! \param[in] init - Initial window size
    /////////////////////////////////////////////////////////////////////////////////////////
    SizePropertyImpl(window_t& wnd, value_t size) : base(wnd, size)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // SizePropertyImpl::get const
    //! Get the window size
    //! 
    //! \return value_t - Current size if window exists, otherwise initial window size
    //! 
    //! \throw wtl::platform_error - [Exists] Unable to query window rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // SizePropertyImpl::set 
    //! Set the current window size iff window exists, otherwise initial window size
    //! 
    //! \param[in] size - Window size
    //! 
    //! \throw wtl::platform_error - [Exists] Unable to set window rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t size);
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias SizeProperty - Define window size property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using SizeProperty = Property<SizePropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_WINDOW_SIZE_PROPERTY_H
