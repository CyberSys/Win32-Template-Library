//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\FormatSpec.hpp
//! \brief Utility templates for formatting strings
//! \date 26 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_FORMAT_SPEC_HPP
#define WTL_FORMAT_SPEC_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EncodingTraits.hpp"
#include <type_traits>
#include <exception>
#include <cwchar>          // Wide char

//! \namespace wtl - Windows template library
namespace wtl
{

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct format_spec - Defines printf type specification for any type
  //!
  //! \tparam CHR - Character type
  //! \tparam T - Output type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename CHR, typename T> 
  struct format_spec; 

  //! \struct format_spec<...> - Define specializations for each type
  template <typename CHR> struct format_spec<CHR,ulong64_t>        { static constexpr const CHR* value = getValue<CHR>("%llu", L"%llu");  };
  template <typename CHR> struct format_spec<CHR,long64_t>         { static constexpr const CHR* value = getValue<CHR>("%lld", L"%lld");  };
  template <typename CHR> struct format_spec<CHR,float64_t>        { static constexpr const CHR* value = getValue<CHR>("%llf", L"%llf");  };
  template <typename CHR> struct format_spec<CHR,float32_t>        { static constexpr const CHR* value = getValue<CHR>("%lf", L"%lf");    };
  template <typename CHR> struct format_spec<CHR,ulong32_t>        { static constexpr const CHR* value = getValue<CHR>("%lu", L"%lu");    };
  template <typename CHR> struct format_spec<CHR,long32_t>         { static constexpr const CHR* value = getValue<CHR>("%ld", L"%ld");    };
  template <typename CHR> struct format_spec<CHR,uint32_t>         { static constexpr const CHR* value = getValue<CHR>("%lu", L"%lu");    };
  template <typename CHR> struct format_spec<CHR,int32_t>          { static constexpr const CHR* value = getValue<CHR>("%ld", L"%ld");    };
  template <typename CHR> struct format_spec<CHR,uint16_t>         { static constexpr const CHR* value = getValue<CHR>("%hu", L"%hu");    };
  template <typename CHR> struct format_spec<CHR,int16_t>          { static constexpr const CHR* value = getValue<CHR>("%hd", L"%hd");    };
  template <typename CHR> struct format_spec<CHR,CHR>              { static constexpr const CHR* value = getValue<CHR>("%c", L"%c");      };
  template <typename CHR> struct format_spec<CHR,uint8_t>          { static constexpr const CHR* value = getValue<CHR>("%cu", L"%cu");    };
  template <typename CHR> struct format_spec<CHR,int8_t>           { static constexpr const CHR* value = getValue<CHR>("%c", L"%c");      };
  template <typename CHR> struct format_spec<CHR,CHR*>             { static constexpr const CHR* value = getValue<CHR>("%s", L"%s");      };
  template <typename CHR> struct format_spec<CHR,uint8_t*>         { static constexpr const CHR* value = getValue<CHR>("%s", L"%s");      };
  template <typename CHR> struct format_spec<CHR,const CHR*>       { static constexpr const CHR* value = getValue<CHR>("%s", L"%s");      };
  template <typename CHR> struct format_spec<CHR,const uint8_t*>   { static constexpr const CHR* value = getValue<CHR>("%s", L"%s");      };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias format_spec_t - Type accessor for format_spec<>
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename CHR, typename T>
  using format_spec_t = format_spec<CHR, std::conditional_t<std::is_enum<T>::value, int32_t, std::remove_const_t<T>> >;




  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::snprintf_t
  //! Formats a string with no arguments
  //!
  //! \param[in] *output - Output buffer
  //! \param[in] capacity - Output buffer capacity
  //! \param[in] const* str - Formatting string
  //! \return uint32_t - Number of characters written
  //!
  //! \throw std::logic_error - Insufficient arguments
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename char_t>
  inline uint32_t  snprintf_t(char_t* output, uint32_t capacity, const char_t* str)
	{
    char *position = output,         //!< Output iterator
         *eof = output+capacity;

    // Iterate through formatting string
    if (str && output)
      do
      {
        // [FORMAT SPEC] Invalid at this point
		 	  if (*str == '%' && *(++str) != '%')	  // Ignore %%
          // [ERROR] Insufficent arguments
		      throw std::logic_error("Missing formatting argument");

        // Write char (or null-terminator)
        *(position++) = *str;
		  }
      // Advance until null-terminator (or capacity reached)
      while (*str++ && position < eof);

    // Return # of chars written
    return position - output;
	}

  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::snprintf_t
  //! Assign from a formatting string and strongly-typed variadic arguments.
  //!
  //! \tparam T - First pack element type
  //! \tparam ARGS - Remaining arguments parameter pack
  //!
  //! \param[in] *output - Output buffer
  //! \param[in] capacity - Output buffer capacity
  //! \param[in] const* str - Formatting string
  //! \param[in] value - First pack value
  //! \param[in] args - [optional] Remaining arguments
  //! \return uint32_t - Number of characters written
  //!
  //! \throw std::length_error - Insufficent capacity to format string
  //! \throw std::logic_error - Incorrect number of formatting arguments
  /////////////////////////////////////////////////////////////////////////////////////////
	template <typename char_t, typename T, typename... ARGS>
  uint32_t  snprintf_t(char_t* output, uint32_t capacity, const char_t* str, T value, ARGS... args)
	{
    char *position = output,         //!< Output iterator
         *eof = output+capacity;     //!< End of buffer position

    // Iterate through formatting string
    if (str && output)
      do
      { // [FORMAT SPEC] Width/precision are ignored
			  if (*str == '%' && *(++str) != '%')   // Ignore %%
        {
          // Skip Width
          if (*str == 'l')
            if (*++str == 'l')
              *++str;

          // Append value
          int len = getFunc<char_t>(::_snprintf,::_snwprintf)(position, eof-position, format_spec_t<char_t,T>::value, value);

          // Succeeded: Advance
          if (len >= 0 && len < eof-position)
            position += len;
          else
            throw std::length_error("Insufficient capacity to format string");

          // Return # chars written and unpack next argument
				  return (position-output) + snprintf_t<char_t>(position, eof-position, ++str, args...); 	  // Consume type field
			  }

        // Write char (or null-terminator)
        *(position++) = *str;
		  }
      // Advance until null-terminator (or capacity reached)
      while (*str++ && position < eof);

    // [ERROR] Insufficent capcity / Excess arguments
		throw std::logic_error(position >= eof ? "Insufficient buffer space" : "Extra arguments provided");
	}


}



#endif // WTL_FORMAT_SPEC_HPP


