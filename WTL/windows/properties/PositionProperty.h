//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\PositionProperty.h
//! \brief Separate class declaration for the 'Position' window property
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_POSITION_PROPERTY_H
#define WTL_WINDOW_POSITION_PROPERTY_H

#include "wtl/WTL.hpp"
#include "wtl/utils/Point.hpp"                            //!< PointL
#include "wtl/traits/EncodingTraits.hpp"                  //!< Encoding
#include "wtl/windows/properties/WindowProperty.hpp"      //!< WindowPropertyImpl

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct PositionPropertyImpl - Provides the getters and setters for the 'Position' window property
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks [WINDOW NOT EXIST] Provides initial position during window creation. 
  //! \remarks [WINDOW EXISTS] Value derived from 'WindowRect' window property.
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct PositionPropertyImpl : WindowPropertyImpl<ENC,PointL,PropertyAccess::ReadWrite>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = PositionPropertyImpl;

    //! \alias base - Define base type
    using base = WindowPropertyImpl<ENC,PointL,PropertyAccess::ReadWrite>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;
    
    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // PositionPropertyImpl::PositionPropertyImpl
    //! Create with initial value
    //! 
    //! \param[in,out] &wnd - Owner window
    //! \param[in] init - Initial window position
    /////////////////////////////////////////////////////////////////////////////////////////
    PositionPropertyImpl(WindowBase<ENC>& wnd, value_t position) : base(wnd, position)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // PositionPropertyImpl::get const
    //! Get the window position
    //! 
    //! \return value_t - Current position if window exists, otherwise 'initial' position
    //! 
    //! \throw wtl::platform_error - [Exists] Unable to query window rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // PositionPropertyImpl::set 
    //! Set the current window position iff window exists, otherwise 'initial' position
    //! 
    //! \param[in] position - Window position
    //! 
    //! \throw wtl::platform_error - [Exists] Unable to set window rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t position);
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias PositionProperty - Define window position property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using PositionProperty = Property<PositionPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_WINDOW_POSITION_PROPERTY_H
