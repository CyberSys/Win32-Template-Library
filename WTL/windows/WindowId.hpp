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
//#include <wtl/platform/WindowFlags.hpp>                           //!< WindowId
#include <map>                                                    //!< std::map
#include <type_traits>                                            //!< std::underlying_type_t

//! \namespace wtl - Windows template library
namespace wtl 
{
  //! \enum WindowId - Represents a Window Id
  enum class WindowId : uint16_t
  {
    Ok = 1,			        //!< 'Ok' button
    Cancel = 2,			    //!< 'Cancel' button
    Abort = 3,			    //!< 'Abort' button
    Retry = 4,			    //!< 'Retry' button
    Ignore = 5,			    //!< 'Ignore' button
    Yes = 6,			      //!< 'Yes' button
    No = 7,			        //!< 'No' button
    Close = 8,			    //!< 'Close' button
    Help = 9,			      //!< 'Help' button
    TryAgain = 10,			//!< [Windows 5.00] 'Try Again' button
    Continue = 11,			//!< [Windows 5.00] 'Continue' button
    User = 0x0100,      //!< User defined
    Timeout = 32000,		//!< [Windows 5.01] Message box timed out
  };
  
  //! Define traits: Non-Contiguous enumeration
  template <> struct is_attribute<WindowId>  : std::false_type  {};
  template <> struct is_contiguous<WindowId> : std::false_type  {};
  template <> struct default_t<WindowId>     : std::integral_constant<WindowId,WindowId::Ok>   {};
  
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

