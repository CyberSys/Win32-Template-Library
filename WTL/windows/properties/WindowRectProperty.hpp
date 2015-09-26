//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\WindowRectProperty.hpp
//! \brief Encapsulates the basic window rectangle in a class-type property
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_RECT_PROPERTY_HPP
#define WTL_WINDOW_RECT_PROPERTY_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/Rectangle.hpp"                        //!< RectL
#include "wtl/traits/EncodingTraits.hpp"                  //!< Encoding
#include "wtl/windows/properties/WindowProperty.hpp"      //!< WindowPropertyImpl

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct WindowRectPropertyImpl - Encapsulates the window-rectangle in a read/write class-type property.
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks When the window does NOT exist, this is calculated from the size & position properties
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct WindowRectPropertyImpl : WindowPropertyImpl<ENC,RectL,PropertyAccess::ReadWrite>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = WindowRectPropertyImpl;

    //! \alias base - Define base type
    using base = WindowPropertyImpl<ENC,RectL,PropertyAccess::ReadWrite>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;
    
    //! \alias window_t - Define window type
    using window_t = WindowBase<ENC>;
    
    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowRectPropertyImpl::WindowRectPropertyImpl
    //! Create without initial value (Window rectangle is initially determined from offline size/position)
    //! 
    //! \param[in,out] &wnd - Owner window
    /////////////////////////////////////////////////////////////////////////////////////////
    WindowRectPropertyImpl(WindowBase<ENC>& wnd) : base(wnd, defvalue<value_t>())
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowRectPropertyImpl::get const
    //! Get the window rectangle
    //! 
    //! \return value_t - Current rectangle if window exists, otherwise 'initial' rectangle
    //!
    //! \throw wtl::logic_error - Window is using default size or location
    //! \throw wtl::platform_error - Unable to query window rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowRectPropertyImpl::set 
    //! Set the current window rectangle iff window exists, otherwise 'initial' rectangle
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

#endif // WTL_WINDOW_RECT_PROPERTY_HPP
