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


  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias getType_t - Defines the narrow/wide character declaration of a type from an encoding
  //!
  //! \tparam[in] ENC - Encoding type
  //! \tparam[in] NARROW - Narrow type
  //! \tparam[in] WIDE - Wide type
  //! \return - 'NARROW' if sizeof(CHAR) == 1, otherwise of type 'WIDE'
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename NARROW, typename WIDE>
  using getType_t = getType<encoding_char_t<ENC>,NARROW,WIDE>;


  ///////////////////////////////////////////////////////////////////////////////
  //! wtl::getFunc
  //! Get the narrow/wide character implementation of a function 
  //!
  //! \tparam ENC - Character encoding type
  //! \tparam NARROW - Narrow char callable target type
  //! \tparam WIDE - Wide char callable target type
  //! 
  //! \param[in] narrow - Narrow char function pointer / callable target
  //! \param[in] wide - Wide char function pointer / callable target
  //! \return auto - 'narrow' if sizeof(CHAR) == 1, otherwise 'wide'
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename NARROW, typename WIDE>
  getType_t<ENC,NARROW,WIDE>  getFunc(NARROW narrow, WIDE wide) 
  {
    return getFunc<encoding_char_t<ENC>>(narrow, wide);
  };
  
  template <typename T>
  struct NonTypeArg
  {
    //NonTypeArg(4>;
  };

  ///////////////////////////////////////////////////////////////////////////////
  //! wtl::getValue
  //! Get the narrow/wide implementation of a value
  //!
  //! \tparam ENC - Character encoding type
  //! \tparam VALUE - Value type
  //! 
  //! \param[in] narrow - Narrow value
  //! \param[in] wide - Wide value
  //! \return auto - 'narrow' if sizeof(CHAR) == 1, otherwise 'wide'
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename VAL, VAL NARROW, VAL WIDE>
  VAL getValue(/*VAL narrow, VAL wide*/) 
  {
    return sizeof(encoding_char_t<ENC>) == sizeof(char) ? NARROW : WIDE;
  };
  
  ///////////////////////////////////////////////////////////////////////////////
  //! wtl::getValue
  //! Get the narrow/wide implementation of a value
  //!
  //! \tparam ENC - Character encoding type
  //! \tparam VALUE - Value type
  //! 
  //! \param[in] narrow - Narrow value
  //! \param[in] wide - Wide value
  //! \return auto - 'narrow' if sizeof(CHAR) == 1, otherwise 'wide'
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename VAL>
  constexpr VAL getValue_t(VAL narrow, VAL wide) 
  {
    return getValue<ENC,VAL,narrow,wide>();
  };
  
} //namespace wtl
#endif // WTL_ENCODING_HPP

