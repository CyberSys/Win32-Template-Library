//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\PositionProperty.hpp
//! \brief Encapsulates the window position in a class-type property
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_POSITION_PROPERTY_HPP
#define WTL_WINDOW_POSITION_PROPERTY_HPP

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
  //! \struct PositionPropertyImpl - Encapsulates the window-position in a read/write class-type property.
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks When the window does NOT exist, this provides the initial position used during window creation.
  //! \remarks When the window DOES exist, the position is determined from the WindowRect, and vice versa when it does not exist.
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

#endif // WTL_WINDOW_POSITION_PROPERTY_HPP
