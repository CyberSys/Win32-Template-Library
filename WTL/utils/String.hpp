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
#include "wtl/io/Console.hpp"                   //!< Console
#include <string>                               //!< std::basic_string

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::strlen_t
  //! Get length of narrow character string 
  //! 
  //! \param[in] const* str - Narrow character string
  //! \return size_t - Length in characters
  /////////////////////////////////////////////////////////////////////////////////////////
  inline size_t strlen_t(const char* str)
  {
    return strlen(str);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::strlen_t
  //! Get length of wide character string 
  //! 
  //! \param[in] const* str - Wide character string
  //! \return size_t - Length in characters
  /////////////////////////////////////////////////////////////////////////////////////////
  inline size_t strlen_t(const wchar_t* str)
  {
    return wcslen(str);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::strcmp_t
  //! Case sensitive narrow character comparison
  //! 
  //! \param[in] const* a - Narrow char string
  //! \param[in] const* b - Another narrow char string
  //! \return int32_t - -1 if less, 0 if equal, 1 if greater
  /////////////////////////////////////////////////////////////////////////////////////////
  inline int32_t strcmp_t(const char* a, const char* b)
  {
    return strcmp(a,b);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::strcmp_t
  //! Case sensitive wide character comparison
  //! 
  //! \param[in] const* a - Wide char string
  //! \param[in] const* b - Another wide char string
  //! \return int32_t - -1 if less, 0 if equal, 1 if greater
  /////////////////////////////////////////////////////////////////////////////////////////
  inline int32_t strcmp_t(const wchar_t* a, const wchar_t* b)
  {
    return wcscmp(a,b);
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::strcpy_t
  //! Copy narrow character string
  //! 
  //! \param[in] *dest - Narrow char string
  //! \param[in] const* src - Another narrow char string
  //! \return char* - Returns 'dest'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <unsigned LEN>
  inline char* strcpy_t(array_ref_t<char,LEN> dest, const char* src)
  {
    return strcpy_s(dest,src);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::strcpy_t
  //! Copy wide character string
  //! 
  //! \param[in] *dest - Wide char string
  //! \param[in] const* src - Another wide char string
  //! \return wchar_t* - Returns 'dest'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <unsigned LEN>
  inline wchar_t* strcpy_t(array_ref_t<wchar_t,LEN> dest, const wchar_t* src)
  {
    return wcscpy_s(dest,src);
  }
  





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
  //    static int32_t  convert(INPUT first, INPUT last, output_t* output, const output_t* lastOut)
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
  //    static int32_t  convert(INPUT first, INPUT last, output_t* output, const output_t* lastOut)
  //    {
  //      REQUIRED_PARAM(first);  REQUIRED_PARAM(output);
  //      REQUIRED_PARAM(last);   REQUIRED_PARAM(lastOut);

  //      // wide -> narrow
  //      int32_t  useDefault = True;
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
  //    static int32_t  convert(INPUT first, INPUT last, output_t* output, const output_t* lastOut)
  //    {
  //      /*REQUIRED_PARAM(first);*/  REQUIRED_PARAM(output);
  //      /*REQUIRED_PARAM(last);*/   REQUIRED_PARAM(lastOut);

  //      const int32_t CAPACITY = lastOut-output-1;   //!< Output buffer character capacity 

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


  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::c_str
  //! Creates a dynamic string from a character array or string literal
  //! 
  //! \tparam char_t - Character type
  //! \tparam encoding - [optional] Character encoding (if unspecified the default encoding for the character type is used)
  //!
  //! \param[in] const* str - Null terminated string
  //! \return String<ENC> - String containing 'str'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename char_t, Encoding encoding = default_encoding<char_t>::value>
  String<encoding>  c_str(const char_t* str) 
  {
    return String<encoding>(str);
  };


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes a STL string to the debugging console
  //! 
  //! \param[in,out] &c - Debug console
  //! \param[in] const& s - String
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  inline Console& operator << (Console& c, const std::string& s)
  { 
    return c << s.c_str();
  }
  


} // WTL namespace



#endif // WTL_STRING_HPP

