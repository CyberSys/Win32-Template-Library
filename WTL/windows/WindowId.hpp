//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\WindowId.hpp
//! \brief 
//! \date 25 February 2016
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_ID_HPP
#define WTL_WINDOW_ID_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/EnumCast.hpp>                                 //!< enum_cast
#include <wtl/traits/EncodingTraits.hpp>                          //!< Encoding
#include <wtl/utils/List.hpp>                                     //!< List
#include <wtl/utils/SFINAE.hpp>                                   //!< enable_if_numeric_t
#include <wtl/platform/WindowFlags.hpp>                           //!< WindowId
#include <map>                                                    //!< std::map
#include <type_traits>                                            //!< std::underlying_type_t

//! \namespace wtl - Windows template library
namespace wtl 
{
  // Forward declaration
  template <Encoding ENC>
  struct Window;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias WindowCollection - Window collection type
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using WindowCollection = List<Window<ENC>*>;
    
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias WindowHandleCollection - Provides an association between native window handles and Window objects
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using WindowHandleCollection = std::map<::HWND,Window<ENC>*>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias WindowIdCollection - Provides an association between window Ids and Window objects
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using WindowIdCollection = std::map<WindowId,Window<ENC>*>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::window_id
  //! Creates a strongly typed window id from any integral or enumeration type
  //!
  //! \tparam TYPE - Integral or enumeration type
  //! 
  //! \param[in] id - Value representing window id
  //! \return WindowId - WindowId representation of 'id'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE, typename = enable_if_numeric_t<VALUE>>
  WindowId window_id(VALUE id)
  {
    // Convert into underlying type then cast to enumeration
    return enum_cast<WindowId>( static_cast<std::underlying_type_t<WindowId>>(id) );
  }

} // namespace wtl


#endif // WTL_WINDOW_ID_HPP

