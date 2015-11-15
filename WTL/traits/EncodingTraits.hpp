//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\Encoding.hpp
//! \brief Defines various character encodings
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ENCODING_HPP
#define WTL_ENCODING_HPP

#include <wtl/WTL.hpp>
#include <wtl/traits/EnumTraits.hpp>      //!< is_attribute, is_contiguous
#include <wtl/utils/Default.hpp>          //!< default_t
#include <wtl/utils/SFINAE.hpp>           //!< enable_if_equal_t, enable_if_greater_t
#include <type_traits>                    //!< std::enable_if_t, std::integral_constant

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \enum Encoding - Defines supported encoding types
  enum class Encoding : long32_t
  {
    ANSI = CP_ACP,                //!< Windows ANSI
    ASCII = 20127,                //!< ASCII
    OEM = CP_OEMCP,               //!< Windows OEM
    MAC = CP_MACCP,               //!< MAC 
    THREAD_ANSI = CP_THREAD_ACP,  //!< Thread Windows ANSI
    SYMBOL = CP_SYMBOL,           //!< Symbol
    UTF7 = CP_UTF7,               //!< UTF-7
    UTF8 = CP_UTF8,               //!< UTF-8
    UTF16 = 1200,                 //!< UTF-16
  };

  //! Define traits: Non-Contiguous Enumeration
  template <> struct is_attribute<Encoding>  : std::false_type  {};
  template <> struct is_contiguous<Encoding> : std::false_type  {};
  template <> struct default_t<Encoding>     : std::integral_constant<Encoding,Encoding::ANSI>   {};
  

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct encoding_traits - Defines encoding traits
  //! 
  //! \tparam E - Encoding type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding E> struct encoding_traits                   { using char_t = char;    };
  template <>           struct encoding_traits<Encoding::UTF16>  { using char_t = wchar_t; };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias encoding_char_t - Encoding character type accessor
  //! 
  //! \tparam E - Encoding type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding E>
  using encoding_char_t = typename encoding_traits<E>::char_t;
  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct default_encoding - Defines default encoding for character types
  //! 
  //! \tparam T - Character type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> struct default_encoding                                                              {};
  template <>           struct default_encoding<char>    : std::integral_constant<Encoding,Encoding::ANSI>   {};
  template <>           struct default_encoding<wchar_t> : std::integral_constant<Encoding,Encoding::UTF16>  {};
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct default_encoding_t - Type accessor for default encoding
  //! 
  //! \tparam T - Type used to define specialization
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  using default_encoding_t = default_encoding< std::remove_const_t<std::remove_reference_t<T>> >;

  
  // --------------------------------- SFINAE ---------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct enable_if_encoding_t - Defines an SFINAE expression requiring an equal character encoding
  //! 
  //! \tparam E1 - Encoding to test
  //! \tparam E2 - Required encoding
  //! \tparam RET - [optional] Return type (Default is void)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding E1, Encoding E2, typename RET = void>
  using enable_if_encoding_t = std::enable_if_t<E1 == E2, RET>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct enable_if_not_encoding_t - Defines an SFINAE expression requiring a different character encoding
  //! 
  //! \tparam E1 - Encoding to test
  //! \tparam E2 - Required encoding
  //! \tparam RET - [optional] Return type (Default is void)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding E1, Encoding E2, typename RET = void>
  using enable_if_not_encoding_t = std::enable_if_t<E1 != E2, RET>;

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct enable_if_narrow_t - Defines an SFINAE expression requiring a narrow character encoding
  //! 
  //! \tparam ENC - Character encoding 
  //! \tparam RET - [optional] Return type (Default is void)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename RET = void>
  using enable_if_narrow_t = enable_if_sizeof_t<char, encoding_char_t<ENC>, RET>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct enable_if_wide_t - Defines an SFINAE expression requiring a wide character encoding
  //! 
  //! \tparam ENC - Character encoding 
  //! \tparam RET - [optional] Return type (Default is void)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename RET = void>
  using enable_if_wide_t = enable_if_smaller_t<char, encoding_char_t<ENC>, RET>;

  
  // --------------------------------- TYPE SELECTORS ---------------------------------
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias choose_t - Chooses between two types based on character encoding
  //!
  //! \tparam ENC - Character encoding 
  //! \tparam NARROW - Narrow type
  //! \tparam WIDE - Wide type
  //! \return auto - 'NARROW' if sizeof(CHR) == 1, otherwise 'WIDE'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename NARROW, typename WIDE>
  using choose_t = std::conditional_t<sizeof(encoding_char_t<ENC>) == sizeof(char), NARROW, WIDE>;
  
  // --------------------------------- VALUE SELECTORS ---------------------------------
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::choose constexpr
  //! Chooses between two values based on character encoding
  //!
  //! \tparam ENC - Character encoding 
  //! \tparam NARROW - Narrow-char value type
  //! \tparam WIDE - Wide-char value type
  //! 
  //! \param[in] narrow - Narrow-char value
  //! \param[in] wide - Wide-char value
  //! \return auto - 'narrow' if sizeof(CHR) == 1, otherwise 'wide'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename NARROW, typename WIDE> constexpr
  auto choose(NARROW narrow, WIDE wide) noexcept -> enable_if_narrow_t<ENC, NARROW>
  {
    return narrow;
  }
  
  template <Encoding ENC, typename NARROW, typename WIDE> constexpr
  auto choose(NARROW narrow, WIDE wide) noexcept -> enable_if_wide_t<ENC, WIDE>
  {
    return wide;
  }
  
} //namespace wtl
#endif // WTL_ENCODING_HPP

