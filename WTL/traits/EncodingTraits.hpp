////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\Encoding.hpp
//! \brief Defines various character encodings
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ENCODING_HPP
#define WTL_ENCODING_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \enum Encoding - Defines supported encoding types
  enum class Encoding : long32
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

  //! \alias array_ptr_t - Defines array pointer types
  template <typename ELEMENT, uint32 LENGTH>
  using array_ptr_t = ELEMENT (*)[LENGTH];
  //! Define traits: Non-Contiguous Enumeration
  template <> struct is_attribute<Encoding>  : std::false_type  {};
  template <> struct is_contiguous<Encoding> : std::false_type  {};

  //! Define limits traits
  template <> struct max_value<Encoding>     : std::integral_constant<Encoding,Encoding::UTF16>   {};
  template <> struct min_value<Encoding>     : std::integral_constant<Encoding,Encoding::ANSI>    {};
  
  //! Define names and values
  template <> struct enum_names<Encoding>  { static const char* values[];    };
  template <> struct enum_values<Encoding> { static const Encoding values[]; };



  /////////////////////////////////////////////////////////////
  //! \struct encoding_traits - Defines encoding traits
  //! 
  //! \tparam E - Type used to define specialization
  /////////////////////////////////////////////////////////////
  template <Encoding E> struct encoding_traits                   { using char_t = char;    };
  template <>           struct encoding_traits<Encoding::UTF16>  { using char_t = wchar_t; };


  /////////////////////////////////////////////////////////////
  //! \alias encoding_char_t - Encoding character type accessor
  //! 
  //! \tparam E - Type used to define specialization
  /////////////////////////////////////////////////////////////
  template <Encoding E>
  using encoding_char_t = typename encoding_traits<E>::char_t;
  
  
  /////////////////////////////////////////////////////////////
  //! \struct default_encoding - Defines default encoding for character types
  //! 
  //! \tparam E - Type used to define specialization
  /////////////////////////////////////////////////////////////
  template <typename T> struct default_encoding                                                              {};
  template <>           struct default_encoding<char>    : std::integral_constant<Encoding,Encoding::ANSI>   {};
  template <>           struct default_encoding<wchar_t> : std::integral_constant<Encoding,Encoding::UTF16>  {};

  
  // --------------------- SFINAE ----------------------

  /////////////////////////////////////////////////////////////
  //! \struct enable_if_encoding_t - Defines an SFINAE expression requiring an equal character encoding
  //! 
  //! \tparam VAR - Encoding to test
  //! \tparam ENC - Required encoding
  //! \tparam RET - [optional] Return type (Default is void)
  /////////////////////////////////////////////////////////////
  template <Encoding VAR, Encoding ENC, typename RET = void>
  using enable_if_encoding_t = std::enable_if_t<VAR == ENC, RET>;

  /////////////////////////////////////////////////////////////
  //! \struct enable_if_not_encoding_t - Defines an SFINAE expression requiring a different character encoding
  //! 
  //! \tparam VAR - Encoding to test
  //! \tparam ENC - Required encoding
  //! \tparam RET - [optional] Return type (Default is void)
  /////////////////////////////////////////////////////////////
  template <Encoding VAR, Encoding ENC, typename RET = void>
  using enable_if_not_encoding_t = std::enable_if_t<VAR != ENC, RET>;

  
  /////////////////////////////////////////////////////////////
  //! \struct enable_if_narrow_t - Defines an SFINAE expression requiring a narrow character type
  //! 
  //! \tparam CHR - Character type
  //! \tparam RET - [optional] Return type (Default is void)
  /////////////////////////////////////////////////////////////
  template <typename CHR, typename RET = void>
  using enable_if_narrow_t = std::enable_if_t<sizeof(CHR) == sizeof(char), RET>;

  /////////////////////////////////////////////////////////////
  //! \struct enable_if_wide_t - Defines an SFINAE expression requiring a wide character type
  //! 
  //! \tparam CHR - Character type
  //! \tparam RET - [optional] Return type (Default is void)
  /////////////////////////////////////////////////////////////
  template <typename CHR, typename RET = void>
  using enable_if_wide_t = std::enable_if_t<sizeof(CHR) == sizeof(wchar_t), RET>;

  
  // --------------------- TYPE SELECTORS ----------------------

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias getType - Defines a narrow/wide type based on a character type
  //!
  //! \tparam[in] CHR - Character type
  //! \tparam[in] NARROW - Narrow type
  //! \tparam[in] WIDE - Wide type
  //! \return auto - 'NARROW' if sizeof(CHR) == 1, otherwise of type 'WIDE'
  ///////////////////////////////////////////////////////////////////////////////
  template <typename CHR, typename NARROW, typename WIDE>
  using getType = std::conditional_t<sizeof(CHR)==sizeof(char),NARROW,WIDE>;
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias getType_t - Defines a narrow/wide type based on a character encoding
  //!
  //! \tparam[in] ENC - Character encoding 
  //! \tparam[in] NARROW - Narrow type
  //! \tparam[in] WIDE - Wide type
  //! \return - 'NARROW' if sizeof(CHR) == 1, otherwise of type 'WIDE'
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename NARROW, typename WIDE>
  using getType_t = getType<encoding_char_t<ENC>,NARROW,WIDE>;

  // --------------------- VALUE SELECTORS ----------------------

  ///////////////////////////////////////////////////////////////////////////////
  //! wtl::getValue constexpr
  //! Get the narrow-character implementation of any value
  //!
  //! \tparam CHR - Narrow character type
  //! \tparam NARROW - Narrow-char value type
  //! \tparam WIDE - Wide-char value type
  //! 
  //! \param[in] narrow - Narrow-char value
  //! \param[in] wide - Wide-char value
  //! \return NARROW - Returns 'narrow'
  ///////////////////////////////////////////////////////////////////////////////
  template <typename CHR, typename NARROW, typename WIDE> constexpr
  auto  getValue(NARROW narrow, WIDE wide) noexcept -> enable_if_narrow_t<CHR,NARROW>
  {
    return narrow;
  };

  ///////////////////////////////////////////////////////////////////////////////
  //! wtl::getValue constexpr
  //! Get the wide-character implementation of any value by character type
  //!
  //! \tparam CHR - Wide character type
  //! \tparam NARROW - Narrow-char value type
  //! \tparam WIDE - Wide-char value type
  //! 
  //! \param[in] narrow - Narrow-char value
  //! \param[in] wide - Wide-char value
  //! \return WIDE - Returns 'wide'
  ///////////////////////////////////////////////////////////////////////////////
  template <typename CHR, typename NARROW, typename WIDE> constexpr
  auto  getValue(NARROW narrow, WIDE wide) noexcept -> enable_if_wide_t<CHR,WIDE>
  {
    return wide;
  };
  
  ///////////////////////////////////////////////////////////////////////////////
  //! wtl::getValue constexpr
  //! Get the narrow/wide character implementation of any value by character encoding
  //!
  //! \tparam ENC - Character encoding 
  //! \tparam NARROW - Narrow-char value type
  //! \tparam WIDE - Wide-char value type
  //! 
  //! \param[in] narrow - Narrow-char value
  //! \param[in] wide - Wide-char value
  //! \return auto - 'narrow' if sizeof(CHR) == 1, otherwise 'wide'
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename NARROW, typename WIDE> constexpr
  getType_t<ENC,NARROW,WIDE>  getValue(NARROW narrow, WIDE wide) noexcept
  {
    return getValue<encoding_char_t<ENC>>(narrow, wide);
  };
  
  // --------------------- FUNCTION SELECTORS ----------------------

  ///////////////////////////////////////////////////////////////////////////////
  //! wtl::getFunc constexpr
  //! Get the narrow/wide function pointer by character type
  //!
  //! \tparam CHR - Character type
  //! \tparam NARROW - Narrow char function pointer
  //! \tparam WIDE - Wide char function pointer
  //! 
  //! \param[in] narrow - Narrow char function pointer / callable target
  //! \param[in] wide - Wide char function pointer / callable target
  //! \return auto - 'narrow' if sizeof(CHR) == 1, otherwise 'wide'
  ///////////////////////////////////////////////////////////////////////////////
  template <typename CHR, typename NARROW, typename WIDE> constexpr
  auto  getFunc(NARROW narrow, WIDE wide) noexcept
  {
    return getValue<CHR>(narrow,wide);
  };

  ///////////////////////////////////////////////////////////////////////////////
  //! wtl::getFunc constexpr
  //! Get the narrow/wide function by character encoding
  //!
  //! \tparam ENC - Character encoding 
  //! \tparam NARROW - Narrow char function pointer
  //! \tparam WIDE - Wide char function pointer
  //! 
  //! \param[in] narrow - Narrow char function pointer / callable target
  //! \param[in] wide - Wide char function pointer / callable target
  //! \return auto - 'narrow' if narrow character encoding, otherwise 'wide'
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename NARROW, typename WIDE> constexpr
  auto  getFunc(NARROW narrow, WIDE wide) noexcept
  {
    return getValue<ENC>(narrow, wide);
  };

  
} //namespace wtl
#endif // WTL_ENCODING_HPP

