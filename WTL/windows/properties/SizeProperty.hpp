//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\SizeProperty.hpp
//! \brief Encapsulates the basic window size in a class-type property
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_SIZE_PROPERTY_HPP
#define WTL_WINDOW_SIZE_PROPERTY_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/Size.hpp"                             //!< SizeL
#include "wtl/traits/EncodingTraits.hpp"                  //!< Encoding
#include "wtl/windows/properties/WindowProperty.hpp"      //!< WindowPropertyImpl

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct SizePropertyImpl - Encapsulates the window-size in a read/write class-type property.
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks When the window does NOT exist, this provides the initial size used during window creation.
  //! \remarks When the window DOES exist, the size is determined from the WindowRect, and vice versa when it does not exist.
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct SizePropertyImpl : WindowPropertyImpl<ENC,SizeL,PropertyAccess::ReadWrite>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = SizePropertyImpl;

    //! \alias base - Define base type
    using base = WindowPropertyImpl<ENC,SizeL,PropertyAccess::ReadWrite>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;
    
    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // SizePropertyImpl::SizePropertyImpl
    //! Create with initial value
    //! 
    //! \param[in,out] &wnd - Owner window
    //! \param[in] init - Initial window size
    /////////////////////////////////////////////////////////////////////////////////////////
    SizePropertyImpl(WindowBase<ENC>& wnd, value_t size) : base(wnd, size)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // SizePropertyImpl::get const
    //! Get the window size
    //! 
    //! \return value_t - Current size if window exists, otherwise 'initial' size
    //! 
    //! \throw wtl::platform_error - [Exists] Unable to query window rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // SizePropertyImpl::set 
    //! Set the current window size iff window exists, otherwise 'initial' size
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

#endif // WTL_WINDOW_SIZE_PROPERTY_HPP
