//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\CommonApi.hpp
//! \brief Encapsulates common win32 API functions
//! \date 26 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_API_COMMON_HPP
#define WTL_API_COMMON_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/EnumCast.hpp>               //!< EnumCast
#include <wtl/utils/Handle.hpp>                 //!< Handle
#include <wtl/traits/EncodingTraits.hpp>        //!< Encoding
#include <wtl/traits/WindowTraits.hpp>          //!< HWnd
#include <wtl/traits/MessageTraits.hpp>         //!< message_traits
#include <wtl/platform/WindowMessage.hpp>       //!< WindowMessage
#include <wtl/platform/MsgResult.hpp>           //!< MsgResult, MsgRoute

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::send_message
  //! Send a message of any type to a window
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam MESSAGE - Window message type
  //! \tparam FIRST - [optional] First parameter type
  //! \tparam SECOND - [optional] Second parameter type
  //!
  //! \param[in] msg - Window message
  //! \param[in] const& wnd - Destination window
  //! \param[in] w - [optional] First parameter
  //! \param[in] l - [optional] Second parameter
  //! \return LResult - Message result and routing
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename MESSAGE, typename FIRST = ::WPARAM, typename SECOND = ::LPARAM>
  LResult send_message(MESSAGE msg, const HWnd& wnd, FIRST w = 0, SECOND l = 0)
  {
    // Send message
    ::LRESULT result = WinAPI<ENC>::sendMessage(wnd, enum_cast(msg), static_cast<::WPARAM>(w), static_cast<::LPARAM>(l));

    // Deduce and return routing from result
    return { message_traits<MESSAGE>::routing(msg, result), result };
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::post_message
  //! Post a message to a window
  //! 
  //! \tparam ENC - Window character encoding
  //! \tparam MESSAGE - Window message type
  //! \tparam FIRST - [optional] First parameter type
  //! \tparam SECOND - [optional] Second parameter type
  //!
  //! \param[in] msg - Window message
  //! \param[in] const& wnd - Destination window
  //! \param[in] w - [optional] First parameter
  //! \param[in] l - [optional] Second parameter
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename MESSAGE, typename FIRST = ::WPARAM, typename SECOND = ::LPARAM>
  void post_message(MESSAGE msg, const HWnd& wnd, FIRST w = 0, SECOND l = 0)
  {
    // Post to target window 
    WinAPI<ENC>::postMessage(wnd, enum_cast(msg), static_cast<::WPARAM>(w), static_cast<::LPARAM>(l));
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::load_function
  //! Returns the address of a function exported by another module
  //! 
  //! \tparam SIG - Function signature
  //!
  //! \param[in] h - Module handle
  //! \param[in] name - Function name
  //! \return bool - True iff unloaded successfully 
  //! 
  //! \throw wtl::invalid_argument - [Debug only] Module handle is nullptr -or- name is empty
  //! \throw wtl::domain_error - [Debug only] Function does not exist
  /////////////////////////////////////////////////////////////////////////////////////////
  //template <typename SIG>
  //std::function<SIG> load_function(Module h, std::string name)
  //{
  //  REQUIRED_PARAM(h);
  //  PARAM_INVARIANT(!name.empty());
  //  DOMAIN_INVARIANT(::GetProcAddress(h, name.c_str()) != nullptr);

  //  // Import function
  //  return reinterpret_cast<SIG*>( ::GetProcAddress(h, name.c_str()) );
  //}


}

#endif // WTL_API_COMMON_HPP
