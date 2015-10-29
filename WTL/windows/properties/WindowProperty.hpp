//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\WindowProperty.hpp
//! \brief Provides a common base-class for window properties
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_PROPERTY_HPP
#define WTL_WINDOW_PROPERTY_HPP

#include "wtl/WTL.hpp"
//#include "wtl/casts/EnumCast.hpp"               //!< EnumCast
#include "wtl/traits/EncodingTraits.hpp"          //!< Encoding
#include "wtl/windows/PropertyImpl.hpp"           //!< PropertyImpl
//#include "wtl/utils/Handle.hpp"                 //!< Handle
//#include "wtl/utils/Default.hpp"                //!< Default

//! \namespace wtl - Windows template library
namespace wtl 
{
  // Forward declaration
  template <Encoding ENC>
  struct WindowBase;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct WindowPropertyImpl - Encapsulates a protected reference to an owner window. 
  //!                              Acts as base for all window properties.
  //!
  //! \tparam ENC - Window encoding
  //! \tparam VALUE - Value type
  //! \tparam TYPE - Access type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename VALUE, PropertyAccess TYPE>
  struct WindowPropertyImpl : PropertyImpl<VALUE,TYPE>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = WindowPropertyImpl<ENC,VALUE,TYPE>;

    //! \alias base - Define base type
    using base = PropertyImpl<VALUE,TYPE>;
      
    //! \var encoding - Define window character encoding
    static constexpr Encoding encoding = ENC;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    WindowBase<ENC>&  Window;      //!< Owner window

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowPropertyImpl::WindowPropertyImpl
    //! Stores the owner window
    //! 
    //! \param[in,out] &wnd - Owner window
    //! \param[in] &&... args - [optional] Value constructor arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    WindowPropertyImpl(WindowBase<ENC>& wnd, ARGS&&... args) : base(std::forward<ARGS>(args)...),
                                                               Window(wnd)
    {}

    // -------------------------------- COPY, MOVE & DESTROY  -------------------------------

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

      
} // namespace wtl

#endif // WTL_WINDOW_PROPERTY_HPP
