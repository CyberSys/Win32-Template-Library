//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\WindowStyleExProperty.hpp
//! \brief Encapsulates the extended window style in an appropriate enumeration property
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EX_WINDOW_STYLE_PROPERTY_HPP
#define WTL_EX_WINDOW_STYLE_PROPERTY_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EncodingTraits.hpp"                  //!< Encoding
#include "wtl/platform/WindowFlags.hpp"                   //!< WindowStyleEx
#include "wtl/windows/properties/WindowProperty.hpp"      //!< WindowPropertyImpl

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct WindowStyleExPropertyImpl - Encapsulates the extended-window-style in a read/write property.
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks When the window does not exist this provides the initial value used during window creation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct WindowStyleExPropertyImpl : WindowPropertyImpl<ENC,WindowStyleEx,PropertyAccess::ReadWrite>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = WindowStyleExPropertyImpl;

    //! \alias base - Define base type
    using base = WindowPropertyImpl<ENC,WindowStyleEx,PropertyAccess::ReadWrite>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowStyleExPropertyImpl::WindowStyleExPropertyImpl
    //! Create with initial value
    //! 
    //! \param[in,out] &wnd - Owner window
    //! \param[in] init - Initial extended window style
    /////////////////////////////////////////////////////////////////////////////////////////
    WindowStyleExPropertyImpl(WindowBase<ENC>& wnd, WindowStyleEx style) : base(wnd, style)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowStyleExPropertyImpl::get const
    //! Get the extended window style
    //! 
    //! \return value_t - Current style if window exists, otherwise 'initial' style
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowStyleExPropertyImpl::set 
    //! Set the current extended window style iff window exists, otherwise 'initial' style
    //! 
    //! \param[in] style - Extended window style
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t style);
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias WindowStyleExProperty - Define extended window style property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using WindowStyleExProperty = Property<WindowStyleExPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_EX_WINDOW_STYLE_PROPERTY_HPP
