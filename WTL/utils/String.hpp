//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\String.hpp
//! \brief Dynamically allocated string
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_STRING_HPP
#define WTL_STRING_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EncodingTraits.hpp"        //!< Character encoding
#include "wtl/utils/Exception.hpp"              //!< Exceptions
#include <string>                               //!< std::basic_string

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct String - Character string with an unlimited capacity and variable runtime length
  //!
  //! \tparam ENC - Character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using String = std::basic_string<encoding_char_t<ENC>>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct String - Character string with an unlimited capacity and variable runtime length
  //!
  //! \tparam ENC - Character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  //template <Encoding ENC>
  //struct String : std::basic_string<encoding_char_t<ENC>>
  //{
  //  friend String;   // Unbound friend of itself

  //  // ---------------------------------- TYPES & CONSTANTS ---------------------------------

  //  //! \alias base - Defines base type
  //  using base = std::basic_string<encoding_char_t<ENC>>;
  //  
  //  //! \alias type - Defines own type
  //  using type = String<ENC>;
  //  
  //  //! \alias char_t - Character type
  //  using char_t = encoding_char_t<ENC>;

  //  //! \var encoding - Character encoding
  //  static constexpr Encoding encoding = ENC;

  //  //! \var null_t - Null terminator
  //  static constexpr char_t null_t = default<char_t>();
  //  
  //protected:
  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  //! \struct conversion_proxy - Handles conversion between character encoding types
  //  //! 
  //  //! \tparam FROM - Input character encoding
  //  //! \tparam TO - Output character encoding
  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  template <Encoding FROM, Encoding TO, typename>
  //  struct conversion_proxy;

  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  //! \struct conversion_proxy<FROM,Encoding::UTF16> - Handles narrow character -> wide character conversion
  //  //! 
  //  //! \tparam FROM - Input character encoding
  //  //! \tparam TO - Output character encoding
  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  template <Encoding FROM>
  //  struct conversion_proxy<FROM,Encoding::UTF16,enable_if_not_encoding_t<FROM,Encoding::UTF16>>
  //  {
  //    //! \alias input_t - Input character type
  //    using input_t = encoding_char_t<FROM>;

  //    //! \alias output_t - Output character type
  //    using output_t = encoding_char_t<Encoding::UTF16>;

  //    /////////////////////////////////////////////////////////////////////////////////////////
  //    // conversion_proxy::convert
  //    //! Copy a narrow character string into a wide character buffer
  //    //! 
  //    //! \tparam INPUT - Input iterator type
  //    //! 
  //    //! \param[in] const* first - First character
  //    //! \param[in] const* last - Position beyond final character
  //    //! \param[in,out] *output - First character in output range
  //    //! \param[in] const* lastOut - Position immediately beyond final character of output range
  //    //!
  //    //! \throw wtl::invalid_argument - [Debug only] Any position is nullptr
  //    /////////////////////////////////////////////////////////////////////////////////////////
  //    template <typename INPUT = const input_t*>
  //    static int32  convert(INPUT first, INPUT last, output_t* output, const output_t* lastOut)
  //    {
  //      REQUIRED_PARAM(first);  REQUIRED_PARAM(output);
  //      REQUIRED_PARAM(last);   REQUIRED_PARAM(lastOut);
  //      
  //      // narrow -> wide
  //      return MultiByteToWideChar(enum_cast(FROM), enum_cast(MultiByteFlags::PreComposed), std::addressof(*first), last-first, output, lastOut-output);
  //    }
  //  };

  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  //! \struct conversion_proxy<Encoding::UTF16,TO> - Handles wide character -> narrow character conversion
  //  //! 
  //  //! \tparam FROM - Input character encoding
  //  //! \tparam TO - Output character encoding
  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  template <Encoding TO>
  //  struct conversion_proxy<Encoding::UTF16,TO,enable_if_not_encoding_t<TO,Encoding::UTF16>>
  //  {
  //    //! \alias input_t - Input character type
  //    using input_t = encoding_char_t<Encoding::UTF16>;
  //    
  //    //! \alias output_t - Output character type
  //    using output_t = encoding_char_t<TO>;

  //    /////////////////////////////////////////////////////////////////////////////////////////
  //    // conversion_proxy::convert
  //    //! Copy an wide character string into a narrow character buffer
  //    //! 
  //    //! \tparam INPUT - Input iterator type
  //    //! 
  //    //! \param[in] const* first - First character
  //    //! \param[in] const* last - Position beyond final character
  //    //! \param[in,out] *output - First character in output range
  //    //! \param[in] const* lastOut - Position immediately beyond final character of output range
  //    //!
  //    //! \throw wtl::invalid_argument - [Debug only] Any position is nullptr
  //    /////////////////////////////////////////////////////////////////////////////////////////
  //    template <typename INPUT = const input_t*>
  //    static int32  convert(INPUT first, INPUT last, output_t* output, const output_t* lastOut)
  //    {
  //      REQUIRED_PARAM(first);  REQUIRED_PARAM(output);
  //      REQUIRED_PARAM(last);   REQUIRED_PARAM(lastOut);

  //      // wide -> narrow
  //      int32  useDefault = TRUE;
  //      return WideCharToMultiByte(enum_cast(TO), enum_cast(WideCharFlags::CompositeCheck|WideCharFlags::NoBestFitChars), std::addressof(*first), last-first, output, lastOut-output, "?", &useDefault);
  //    }
  //  };
  //  
  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  //! \struct conversion_proxy<E,E> - Handles no conversion required
  //  //! 
  //  //! \tparam E - Character encoding of both strings
  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  template <Encoding E>
  //  struct conversion_proxy<E,E,void>
  //  {
  //    //! \alias input_t - Input character type
  //    using input_t = encoding_char_t<E>;

  //    //! \alias output_t - Output character type
  //    using output_t = input_t;

  //    /////////////////////////////////////////////////////////////////////////////////////////
  //    // conversion_proxy::convert
  //    //! Copy an input string of equal encoding
  //    //! 
  //    //! \tparam INPUT - Input iterator type
  //    //! 
  //    //! \param[in] const* first - First character
  //    //! \param[in] const* last - Position beyond final character
  //    //! \param[in,out] *output - First character in output range
  //    //! \param[in] const* lastOut - Position immediately beyond final character of output range
  //    //!
  //    //! \throw wtl::invalid_argument - [Debug only] Any position is nullptr
  //    /////////////////////////////////////////////////////////////////////////////////////////
  //    template <typename INPUT = const input_t*>
  //    static int32  convert(INPUT first, INPUT last, output_t* output, const output_t* lastOut)
  //    {
  //      /*REQUIRED_PARAM(first);*/  REQUIRED_PARAM(output);
  //      /*REQUIRED_PARAM(last);*/   REQUIRED_PARAM(lastOut);

  //      const int32 CAPACITY = lastOut-output-1;   //!< Output buffer character capacity 

  //      // Clear output
  //      output[0] = default<output_t>();

  //      // Prevent output buffer overrun
  //      if (last - first > CAPACITY)
  //        last = first + CAPACITY;

  //      // Copy characters from input range
  //      for (INPUT pos = first; pos != last; ++pos, ++output)
  //        *output = *pos;

  //      // null terminate
  //      *output = default<output_t>();
  //      
  //      // Return number of characters copied
  //      return last-first;
  //    }
  //  };

  //};

} // WTL namespace



#endif // WTL_STRING_HPP

