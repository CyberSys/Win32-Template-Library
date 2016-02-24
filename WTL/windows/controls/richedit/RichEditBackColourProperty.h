//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\richedit\RichEditBackColourProperty.hpp
//! \brief Separate class declaration for the RichEdit control 'BackColour' property
//! \date 6 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RICH_EDIT_BACK_COLOUR_PROPERTY_H
#define WTL_RICH_EDIT_BACK_COLOUR_PROPERTY_H

#include <wtl/WTL.hpp>
#include <wtl/casts/EnumCast.hpp>                     //!< enum_cast
#include <wtl/traits/EncodingTraits.hpp>              //!< Encoding
#include <wtl/platform/Colours.hpp>                   //!< Colour
#include <wtl/windows/Property.hpp>                   //!< Property
#include <wtl/windows/PropertyImpl.hpp>               //!< PropertyImpl
#include <wtl/windows/events/CreateWindowEvent.hpp>   //!< CreateWindowEventArgs

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  //! Forward declaration
  template <Encoding ENC>
  struct RichEdit;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct RichEditBackColourPropertyImpl - Provides the getters and setters for the 'Icon' window property
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct RichEditBackColourPropertyImpl : PropertyImpl<ENC,Colour,RichEdit<ENC>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = RichEditBackColourPropertyImpl;

    //! \alias base - Define base type
    using base = PropertyImpl<ENC,Colour,RichEdit<ENC>>;
      
    //! \alias value_t - Inherit value type
    using value_t = typename base::value_t;
    
    //! \alias window_t - Inherit window type
    using window_t = typename base::window_t;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEditBackColourPropertyImpl::RichEditBackColourPropertyImpl
    //! Create window property 
    //! 
    //! \param[in,out] &wnd - Owner window
    /////////////////////////////////////////////////////////////////////////////////////////
    RichEditBackColourPropertyImpl(window_t& wnd) : base(wnd, Colour::Invalid)
    {
      // Register creation handler to set initial colour
      wnd.Create += new CreateWindowEventHandler<base::encoding>(this, &type::onCreate);
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEditBackColourPropertyImpl::set 
    //! Set the current background colour iff control exists, otherwise sets the initial colour
    //! 
    //! \param[in] col - Background colour
    /////////////////////////////////////////////////////////////////////////////////////////
    void  set(value_t col);
  
  private:
    /////////////////////////////////////////////////////////////////////////////////////////
    // RichEditBackColourPropertyImpl::onCreate 
    //! Called during control creation to set the initial background colour
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Returns 0 to accept control creation
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onCreate(CreateWindowEventArgs<ENC>& args);
  };

  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias RichEditBackColourProperty - Define property type 
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using RichEditBackColourProperty = Property<RichEditBackColourPropertyImpl<ENC>>;

      
} // namespace wtl

#endif // WTL_RICH_EDIT_BACK_COLOUR_PROPERTY_H
