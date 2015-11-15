//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Encoding.hpp
//! \brief Encapsulates converting between character arrays of different encodings
//! \date 31 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CHAR_ENCODING_HPP
#define WTL_CHAR_ENCODING_HPP

#include <wtl/WTL.hpp>
#include <wtl/traits/EncodingTraits.hpp>

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct string_encoder - Handles conversion between character encoding types
  //! 
  //! \tparam FROM - Input character encoding 
  //! \tparam TO - Output character encoding
  //! 
  //! \remarks Due to the problems of mapping several narrow characters to a single wide character, both
  //! \remarks Win32 API functions and the C standard library functions for character conversion operate
  //! \remarks upon character arrays rather than iterator ranges. This limitation applies to this class also.
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding FROM, Encoding TO, typename = void>
  struct string_encoder;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct string_encoder<FROM,Encoding::UTF16> - Handles narrow character -> wide character conversion
  //! 
  //! \tparam FROM - Input character encoding
  //! \tparam TO - Output character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding FROM>
  struct string_encoder<FROM,Encoding::UTF16,enable_if_not_encoding_t<FROM,Encoding::UTF16>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = string_encoder<FROM,Encoding::UTF16,enable_if_not_encoding_t<FROM,Encoding::UTF16>>;

    //! \alias input_t - Input character type
    using input_t = encoding_char_t<FROM>;

    //! \alias output_t - Output character type
    using output_t = encoding_char_t<Encoding::UTF16>;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------- CONSTRUCTION -----------------------------------
	
	  // -------------------------------- COPY, MOVE & DESTROY  -------------------------------

    // ----------------------------------- STATIC METHODS -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // string_encoder::convert
    //! Transforms an array of UTF16 characters into an output array of wide characters
    //! 
    //! \tparam INPUT - Input character type
    //! \tparam OUTPUT - Output character type
    //! 
    //! \param[in] const* srcBegin - First character in input array
    //! \param[in] const* srcEnd - Position immediately beyond final character in input array
    //! \param[in,out] *destBegin - First character in output array
    //! \param[in,out] *destEnd - Position immediately beyond final character of output array
    //! \return int32_t - Length of output string (in characters)
    //!
    //! \throw wtl::platform_error - Unable to convert string
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename INPUT, typename OUTPUT> 
    static int32_t  convert(const INPUT* srcBegin, const INPUT* srcEnd, OUTPUT* destBegin, OUTPUT* destEnd)
    {
      constexpr uint32_t  codepage   = enum_cast(FROM);
      constexpr ulong32_t flags      = enum_cast(MultiByteFlags::PreComposed);
  
      // Succeed on empty string
      if (srcBegin[0] == '\0')
        return 0;

      // Narrow -> wide
      if (int32_t len = ::MultiByteToWideChar(codepage, flags, srcBegin, srcEnd-srcBegin+1, destBegin, destEnd-destBegin))
        return len;
      
      // [ERROR] Failed
      throw platform_error(HERE, "Unable to perform narrow to wide character conversion");
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct string_encoder<Encoding::UTF16,TO> - Handles wide character -> narrow character conversion
  //! 
  //! \tparam FROM - Input character encoding
  //! \tparam TO - Output character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding TO>
  struct string_encoder<Encoding::UTF16,TO,enable_if_not_encoding_t<TO,Encoding::UTF16>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = string_encoder<Encoding::UTF16,TO,enable_if_not_encoding_t<TO,Encoding::UTF16>>;

    //! \alias input_t - Input character type
    using input_t = encoding_char_t<Encoding::UTF16>;
      
    //! \alias output_t - Output character type
    using output_t = encoding_char_t<TO>;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // ------------------------------------- CONSTRUCTION -----------------------------------
    
    // -------------------------------- COPY, MOVE & DESTROY  -------------------------------
    
    // ----------------------------------- STATIC METHODS -----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // string_encoder::convert
    //! Transforms an array of UTF16 characters into an array of narrow characters
    //! 
    //! \tparam INPUT - Input character type
    //! \tparam OUTPUT - Output character type
    //! 
    //! \param[in] const* srcBegin - First character in input array
    //! \param[in] const* srcEnd - Position immediately beyond final character in input array
    //! \param[in,out] *destBegin - First character in output array
    //! \param[in,out] *destEnd - Position immediately beyond final character of output array
    //! \return int32_t - Length of output string (in characters)
    //!
    //! \throw wtl::platform_error - Unable to convert string
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename INPUT, typename OUTPUT> 
    static int32_t  convert(const INPUT* srcBegin, const INPUT* srcEnd, OUTPUT* destBegin, OUTPUT* destEnd)
    {
      /*const*/ int32_t   useDefault = True;
      constexpr uint32_t  codepage   = enum_cast(TO);
      constexpr ulong32_t flags      = enum_cast(WideCharFlags::CompositeCheck|WideCharFlags::NoBestFitChars);
      
      // Succeed on empty string
      if (srcBegin[0] == '\0')
        return 0;

      // Wide -> Narrow
      if (int32_t len = ::WideCharToMultiByte(codepage, flags, srcBegin, srcEnd-srcBegin+1, destBegin, destEnd-destBegin, "?", &useDefault))
        return len;
      
      // [ERROR] Failed
      throw platform_error(HERE, "Unable to perform wide to narrow character conversion");
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };
    
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct string_encoder<E,E> - Handles no conversion required
  //! 
  //! \tparam E - Character encoding of both strings
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding E>
  struct string_encoder<E,E,enable_always_t<>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = string_encoder<E,E,enable_always_t<>>;

    //! \alias input_t - Input character type
    using input_t = encoding_char_t<E>;

    //! \alias output_t - Output character type
    using output_t = input_t;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // ------------------------------------- CONSTRUCTION -----------------------------------
    
    // -------------------------------- COPY, MOVE & DESTROY  -------------------------------
    
    // ----------------------------------- STATIC METHODS -----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // string_encoder::convert
    //! Copies an array of characters into an output array without conversion
    //! 
    //! \tparam INPUT - Input iterator type
    //! \tparam OUTPUT - Output iterator type
    //! 
    //! \param[in] const* srcBegin - First character in input array
    //! \param[in] const* srcEnd - Position immediately beyond final character in input array
    //! \param[in,out] *destBegin - First character in output array
    //! \param[in,out] *destEnd - Position immediately beyond final character of output array
    //! \return int32_t - Length of output string (in characters)
    //!
    //! \throw wtl::platform_error - Unable to convert string
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename INPUT, typename OUTPUT> 
    static int32_t  convert(const INPUT* srcBegin, const INPUT* srcEnd, OUTPUT* destBegin, OUTPUT* destEnd)
    {
      const int32_t CAPACITY = destEnd-destBegin-1;   //!< Output buffer character capacity 

      // Clear output
      destBegin[0] = defvalue<output_t>();

      // Prevent output buffer overrun
      if (srcEnd - srcBegin > CAPACITY)
        srcEnd = srcBegin + CAPACITY;

      // Copy characters without exceeding input/output array
      for (const INPUT* pos = srcBegin; pos != srcEnd && destBegin != destEnd; ++pos, ++destBegin)
        *destBegin = static_cast<INPUT>( *pos );

      // null terminate
      *destBegin = defvalue<output_t>();
        
      // Return number of characters copied
      return srcEnd-srcBegin;
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };


  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::encode_string
  //! Transforms the encoding of an array of characters into an output array
  //! 
  //! \tparam INPUT - Input iterator type
  //! \tparam OUTPUT - Output iterator type
  //! 
  //! \param[in] const* srcBegin - First character in input array
  //! \param[in] const* srcEnd - Position immediately beyond final character in input array
  //! \param[in,out] *destBegin - First character in output array
  //! \param[in,out] *destEnd - Position immediately beyond final character of output array
  //!
  //! \throw wtl::platform_error - Unable to convert string
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename INPUT, typename OUTPUT> 
  int32_t  encode_string(const INPUT* srcBegin, const INPUT* srcEnd, OUTPUT* destBegin, OUTPUT* destEnd)
  {
    constexpr Encoding FROM = default_encoding_t<decltype(*srcBegin)>::value,
                         TO = default_encoding_t<decltype(*destBegin)>::value;

    return string_encoder<FROM,TO>::convert(srcBegin,srcEnd, destBegin,destEnd);
  }
}



#endif // WTL_CHAR_ENCODING_HPP


