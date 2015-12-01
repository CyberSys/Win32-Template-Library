//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\FocusProperty.hpp
//! \brief Separate class declaration for the 'Focus' window property
//! \date 1 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_FOCUS_PROPERTY_H
#define WTL_WINDOW_FOCUS_PROPERTY_H

#include <wtl/WTL.hpp>
#include <wtl/traits/EncodingTraits.hpp>              //!< Encoding
#include <wtl/windows/PropertyImpl.hpp>               //!< PropertyImpl
#include <wtl/windows/events/CreateWindowEvent.hpp>   //!< CreateWindowEventArgs

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct FocusPropertyImpl - Provides a read-only property indicating whether a window has keyboard focus
  //! 
  //! \tparam ENC - Window encoding
  //!
  //! \remarks When the window does not exist, this property defines the initial keyboard focus state
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct FocusPropertyImpl : PropertyImpl<ENC,bool>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = FocusPropertyImpl;

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
    // FocusPropertyImpl::FocusPropertyImpl
    //! Create keyboard focus window property
    //! 
    //! \param[in,out] &wnd - Owner window
    /////////////////////////////////////////////////////////////////////////////////////////
    FocusPropertyImpl(window_t& wnd) : base(wnd, false)
    {
      // Register creation handler to set initial focus state
      wnd.Create += new CreateWindowEventHandler<base::encoding>(this, &type::onCreate);
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // FocusPropertyImpl::get const
    //! Query whether window has keyboard focus
    //! 
    //! \return value_t - True iff window has keyboard focus
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t  get() const;

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  private:
    /////////////////////////////////////////////////////////////////////////////////////////
    // FocusPropertyImpl::onCreate
    //! Called during window creation to set the initial keyboard focus
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Returns 0 to accept window creation
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onCreate(CreateWindowEventArgs<ENC>& args);
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias FocusProperty - Define window-focus property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using FocusProperty = Property<FocusPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_WINDOW_FOCUS_PROPERTY_H
