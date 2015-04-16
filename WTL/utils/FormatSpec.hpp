////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\FormatSpec.hpp
//! \brief Utility templates for formatting strings 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_FORMAT_SPEC_HPP
#define WTL_FORMAT_SPEC_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  // ------------------ FORMAT SPEC -----------------

//! \if CONSTEXPR_CAP - Define here if compiler supports static storage
#ifdef CONSTEXPR_CAP
  /////////////////////////////////////////////////////////////
  //! \struct format_spec - Defines printf type specification for fundemental types
  /////////////////////////////////////////////////////////////
  template <typename char_t, typename T> struct format_spec     { static const char value[]; /* Undefined */ };

  template <> struct format_spec<char,double>      { static constexpr const char value[] = "%llf"; };
  template <> struct format_spec<char,uint64>      { static constexpr const char value[] = "%llu"; };
  template <> struct format_spec<char,int64>       { static constexpr const char value[] = "%lld"; };
  template <> struct format_spec<char,float>       { static constexpr const char value[] = "%lf";  };
  template <> struct format_spec<char,ulong32>     { static constexpr const char value[] = "%lu";  };
  template <> struct format_spec<char,long32>      { static constexpr const char value[] = "%ld";  };
  template <> struct format_spec<char,uint32>      { static constexpr const char value[] = "%lu";  };
  template <> struct format_spec<char,int32>       { static constexpr const char value[] = "%ld";  };
  template <> struct format_spec<char,uint16>      { static constexpr const char value[] = "%hu";  };
  template <> struct format_spec<char,int16>       { static constexpr const char value[] = "%hd";  };
  template <> struct format_spec<char,uint8>       { static constexpr const char value[] = "%c";  };
  template <> struct format_spec<char,int8>        { static constexpr const char value[] = "%c";  };
  template <> struct format_spec<char,char>        { static constexpr const char value[] = "%c";  };
  template <> struct format_spec<char,char*>       { static constexpr const char value[] = "%s";  };
  template <> struct format_spec<char,const char*> { static constexpr const char value[] = "%s";  };
#else
  /////////////////////////////////////////////////////////////
  //! \struct format_spec - Defines printf type specification for fundemental types
  /////////////////////////////////////////////////////////////
  template <typename char_t, typename T> struct format_spec     { static const char value[]; /* Undefined */ };

  template <> struct format_spec<char,double>             { static const char value[]; };
  template <> struct format_spec<char,uint64>             { static const char value[]; };
  template <> struct format_spec<char,int64>              { static const char value[]; };
  template <> struct format_spec<char,float>              { static const char value[]; };
  template <> struct format_spec<char,ulong32>            { static const char value[]; };
  template <> struct format_spec<char,long32>             { static const char value[]; };
  template <> struct format_spec<char,uint32>             { static const char value[]; };
  template <> struct format_spec<char,int32>              { static const char value[]; };
  template <> struct format_spec<char,uint16>             { static const char value[]; };
  template <> struct format_spec<char,int16>              { static const char value[]; };
  template <> struct format_spec<char,uint8>              { static const char value[]; };
  template <> struct format_spec<char,int8>               { static const char value[]; };
  template <> struct format_spec<char,char>               { static const char value[]; };
  template <> struct format_spec<char,char*>              { static const char value[]; };
  template <> struct format_spec<char,uint8*>             { static const char value[]; };
  template <> struct format_spec<char,const char*>        { static const char value[]; };
  template <> struct format_spec<char,const uint8*>       { static const char value[]; };

  template <> struct format_spec<wchar_t,double>          { static const wchar_t value[]; };
  template <> struct format_spec<wchar_t,uint64>          { static const wchar_t value[]; };
  template <> struct format_spec<wchar_t,int64>           { static const wchar_t value[]; };
  template <> struct format_spec<wchar_t,float>           { static const wchar_t value[]; };
  template <> struct format_spec<wchar_t,ulong32>         { static const wchar_t value[]; };
  template <> struct format_spec<wchar_t,long32>          { static const wchar_t value[]; };
  template <> struct format_spec<wchar_t,uint32>          { static const wchar_t value[]; };
  template <> struct format_spec<wchar_t,int32>           { static const wchar_t value[]; };
  template <> struct format_spec<wchar_t,uint16>          { static const wchar_t value[]; };
  template <> struct format_spec<wchar_t,int16>           { static const wchar_t value[]; };
  template <> struct format_spec<wchar_t,uint8>           { static const wchar_t value[]; };
  template <> struct format_spec<wchar_t,int8>            { static const wchar_t value[]; };
  template <> struct format_spec<wchar_t,wchar_t>         { static const wchar_t value[]; };
  template <> struct format_spec<wchar_t,wchar_t*>        { static const wchar_t value[]; };
  template <> struct format_spec<wchar_t,uint16*>         { static const wchar_t value[]; };
  template <> struct format_spec<wchar_t,const wchar_t*>  { static const wchar_t value[]; };
  template <> struct format_spec<wchar_t,const uint16*>   { static const wchar_t value[]; };
#endif
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \typedef format_spec_t - Query printf type specification for any type
  ///////////////////////////////////////////////////////////////////////////////
  template <typename char_t, typename T> 
  using format_spec_t = format_spec<char_t, std::conditional_t<std::is_enum<T>::value, int, std::remove_const_t<T>> >;

  ///////////////////////////////////////////////////////////////////////////////
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
  //! \return uint32 - Number of characters written
  //! 
  //! \throw std::length_error - Insufficent capacity to format string
  //! \throw std::logic_error - Incorrect number of formatting arguments
  ///////////////////////////////////////////////////////////////////////////////
	template <typename char_t, typename T, typename... ARGS>		
  uint32 snprintf_t(char_t* output, uint32 capacity, const char_t* str, T value, ARGS... args)	
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
          int len = _snprintf(position, eof-position, format_spec_t<char,T>::value, value); 
          
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

  
  ///////////////////////////////////////////////////////////////////////////////
  // wtl::snprintf_t
  //! Formats a string with no arguments
  //! 
  //! \param[in] *output - Output buffer
  //! \param[in] capacity - Output buffer capacity
  //! \param[in] const* str - Formatting string
  //! \return uint32 - Number of characters written
  //! 
  //! \throw std::logic_error - Insufficient arguments
  ///////////////////////////////////////////////////////////////////////////////
  template <typename char_t>
  inline uint32 snprintf_t(char_t* output, uint32 capacity, const char_t* str)	
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

}



#endif // WTL_FORMAT_SPEC_HPP


