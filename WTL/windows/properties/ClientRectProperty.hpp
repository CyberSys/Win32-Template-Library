//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\ClientRectProperty.hpp
//! \brief Encapsulates the basic client rectangle in a class-type property
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CLIENT_RECT_PROPERTY_HPP
#define WTL_CLIENT_RECT_PROPERTY_HPP

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
  //! \struct ClientRectPropertyImpl - Encapsulates the client rectangle in a read/write class-type property.
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks When the window does NOT exist, this is calculated from the size & position properties
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ClientRectPropertyImpl : WindowPropertyImpl<ENC,RectL,PropertyAccess::ReadWrite>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = ClientRectPropertyImpl;

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
    // ClientRectPropertyImpl::ClientRectPropertyImpl
    //! Create without initial value (client rectangle is initially determined from offline size/position)
    //! 
    //! \param[in,out] &wnd - Owner window
    /////////////////////////////////////////////////////////////////////////////////////////
    ClientRectPropertyImpl(WindowBase<ENC>& wnd) : base(wnd, default<value_t>())
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // ClientRectPropertyImpl::get const
    //! Get the client rectangle
    //! 
    //! \return value_t - Current rectangle if window exists, otherwise 'initial' rectangle
    //!
    //! \throw wtl::logic_error - Window is using default size or location
    //! \throw wtl::platform_error - Unable to query client rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // ClientRectPropertyImpl::set 
    //! Set the current client rectangle iff window exists, otherwise 'initial' rectangle
    //! 
    //! \param[in] rectangle - Window rectangle
    //! 
    //! \throw wtl::platform_error - Unable to set client rectangle 
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t rectangle);
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ClientRectProperty - Define client rectangle property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ClientRectProperty = Property<ClientRectPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_CLIENT_RECT_PROPERTY_HPP
