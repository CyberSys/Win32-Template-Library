//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Encoding.hpp
//! \brief Provides conversion between character encodings
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CHAR_ENCODING_HPP
#define WTL_CHAR_ENCODING_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EncodingTraits.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct string_encoder_t - Handles conversion between character encoding types
  //! 
  //! \tparam FROM - Input character encoding
  //! \tparam TO - Output character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding FROM, Encoding TO, typename = void>
  struct string_encoder_t;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct string_encoder_t<FROM,Encoding::UTF16> - Handles narrow character -> wide character conversion
  //! 
  //! \tparam FROM - Input character encoding
  //! \tparam TO - Output character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding FROM>
  struct string_encoder_t<FROM,Encoding::UTF16,enable_if_not_encoding_t<FROM,Encoding::UTF16>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = string_encoder_t<FROM,Encoding::UTF16,enable_if_not_encoding_t<FROM,Encoding::UTF16>>;

    //! \alias input_t - Input character type
    using input_t = encoding_char_t<FROM>;

    //! \alias output_t - Output character type
    using output_t = encoding_char_t<Encoding::UTF16>;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------- CONSTRUCTION -----------------------------------
	
	  // -------------------------------- COPYING & DESTRUCTION -------------------------------

    // ----------------------------------- STATIC METHODS -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // string_encoder_t::transform
    //! Transforms a range of UTF16 characters into an output range of wide characters
    //! 
    //! \tparam INPUT1 - Input iterator type
    //! \tparam INPUT2 - Input iterator type
    //! \tparam OUTPUT1 - Output iterator type
    //! \tparam OUTPUT2 - Output iterator type
    //! 
    //! \param[in] firstIn - First character in input range
    //! \param[in] lastIn - Position immediately beyond final character in input range
    //! \param[in,out] firstOut - First character in output range
    //! \param[in] lastOut - Position immediately beyond final character of output range
    //! \return int32 - Length of output string (in characters)
    //!
    //! \throw wtl::platform_error - Unable to convert string
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename INPUT1, typename INPUT2, typename OUTPUT1, typename OUTPUT2> 
    static int32  transform(INPUT1 firstIn, INPUT2 lastIn, OUTPUT1 firstOut, OUTPUT2 lastOut)
    {
      constexpr uint32  codepage   = enum_cast(FROM);
      constexpr ulong32 flags      = enum_cast(MultiByteFlags::PreComposed);
  
      // Narrow -> wide
      if (int32 len = MultiByteToWideChar(codepage, flags, &*firstIn, lastIn-firstIn+1, &*firstOut, lastOut-firstOut))
        return len;
      
      // Succeed on empty string
      if (!*firstIn)
        return 0;

      // [ERROR] Failed
      throw platform_error(HERE, "Unable to perform narrow to wide character conversion");
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct string_encoder_t<Encoding::UTF16,TO> - Handles wide character -> narrow character conversion
  //! 
  //! \tparam FROM - Input character encoding
  //! \tparam TO - Output character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding TO>
  struct string_encoder_t<Encoding::UTF16,TO,enable_if_not_encoding_t<TO,Encoding::UTF16>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = string_encoder_t<Encoding::UTF16,TO,enable_if_not_encoding_t<TO,Encoding::UTF16>>;

    //! \alias input_t - Input character type
    using input_t = encoding_char_t<Encoding::UTF16>;
      
    //! \alias output_t - Output character type
    using output_t = encoding_char_t<TO>;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // ------------------------------------- CONSTRUCTION -----------------------------------
    
    // -------------------------------- COPYING & DESTRUCTION -------------------------------
    
    // ----------------------------------- STATIC METHODS -----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // string_encoder_t::transform
    //! Transforms a range of UTF16 characters into an output range of narrow characters
    //! 
    //! \tparam INPUT1 - Input iterator type
    //! \tparam INPUT2 - Input iterator type
    //! \tparam OUTPUT1 - Output iterator type
    //! \tparam OUTPUT2 - Output iterator type
    //! 
    //! \param[in] firstIn - First character in input range
    //! \param[in] lastIn - Position immediately beyond final character in input range
    //! \param[in,out] firstOut - First character in output range
    //! \param[in] lastOut - Position immediately beyond final character of output range
    //! \return int32 - Length of output string (in characters)
    //!
    //! \throw wtl::platform_error - Unable to convert string
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename INPUT1, typename INPUT2, typename OUTPUT1, typename OUTPUT2> 
    static int32  transform(INPUT1 firstIn, INPUT2 lastIn, OUTPUT1 firstOut, OUTPUT2 lastOut)
    {
      /*const*/ int32   useDefault = True;
      constexpr uint32  codepage   = enum_cast(TO);
      constexpr ulong32 flags      = enum_cast(WideCharFlags::CompositeCheck|WideCharFlags::NoBestFitChars);

      // Wide -> Narrow
      if (int32 len = WideCharToMultiByte(codepage, flags, &*firstIn, lastIn-firstIn+1, &*firstOut, lastOut-firstOut, "?", &useDefault))
        return len;
      
      // Succeed on empty string
      if (!*firstIn)
        return 0;

      // [ERROR] Failed
      throw platform_error(HERE, "Unable to perform wide to narrow character conversion");
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };
    
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct string_encoder_t<E,E> - Handles no conversion required
  //! 
  //! \tparam E - Character encoding of both strings
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding E>
  struct string_encoder_t<E,E,enable_always_t<>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = string_encoder_t<E,E,enable_always_t<>>;

    //! \alias input_t - Input character type
    using input_t = encoding_char_t<E>;

    //! \alias output_t - Output character type
    using output_t = input_t;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // ------------------------------------- CONSTRUCTION -----------------------------------
    
    // -------------------------------- COPYING & DESTRUCTION -------------------------------
    
    // ----------------------------------- STATIC METHODS -----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // string_encoder_t::transform
    //! Copies a range of characters into an output range without conversion
    //! 
    //! \tparam INPUT1 - Input iterator type
    //! \tparam INPUT2 - Input iterator type
    //! \tparam OUTPUT1 - Output iterator type
    //! \tparam OUTPUT2 - Output iterator type
    //! 
    //! \param[in] firstIn - First character in input range
    //! \param[in] lastIn - Position immediately beyond final character in input range
    //! \param[in,out] firstOut - First character in output range
    //! \param[in] lastOut - Position immediately beyond final character of output range
    //!
    //! \throw wtl::platform_error - Unable to convert string
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename INPUT1, typename INPUT2, typename OUTPUT1, typename OUTPUT2> 
    static int32  transform(INPUT1 firstIn, INPUT2 lastIn, OUTPUT1 firstOut, OUTPUT2 lastOut)
    {
      const int32 CAPACITY = lastOut-firstOut-1;   //!< Output buffer character capacity 

      // Clear output
      firstOut[0] = default<output_t>();

      // Prevent output buffer overrun
      if (lastIn - firstIn > CAPACITY)
        lastIn = firstIn + CAPACITY;

      // Copy characters without exceeding input/output range
      for (INPUT1 pos = firstIn; pos != lastIn && firstOut != lastOut; ++pos, ++firstOut)
        *firstOut = *pos;

      // null terminate
      *firstOut = default<output_t>();
        
      // Return number of characters copied
      return lastIn-firstIn;
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };


  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::encode_string
  //! Transforms the encoding of a range of characters into an output range
  //! 
  //! \tparam INPUT1 - Input iterator type
  //! \tparam INPUT2 - Input iterator type
  //! \tparam OUTPUT1 - Output iterator type
  //! \tparam OUTPUT2 - Output iterator type
  //! 
  //! \param[in] firstIn - First character in input range
  //! \param[in] lastIn - Position immediately beyond final character in input range
  //! \param[in,out] firstOut - First character in output range
  //! \param[in] lastOut - Position immediately beyond final character of output range
  //!
  //! \throw wtl::platform_error - Unable to convert string
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename INPUT1, typename INPUT2, typename OUTPUT1, typename OUTPUT2> 
  int32  encode_string(INPUT1 firstIn, INPUT2 lastIn, OUTPUT1 firstOut, OUTPUT2 lastOut)
  {
    constexpr Encoding FROM = default_encoding_t<decltype(*firstIn)>::value,
                         TO = default_encoding_t<decltype(*firstOut)>::value;

    return string_encoder_t<FROM,TO>::convert(firstIn,lastIn, firstOut,lastOut);
  }
}



#endif // WTL_CHAR_ENCODING_HPP


