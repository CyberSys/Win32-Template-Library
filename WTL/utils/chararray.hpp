////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\CharArray.hpp
//! \brief Statically allocated string
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CHAR_ARRAY_HPP
#define WTL_CHAR_ARRAY_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  ///////////////////////////////////////////////////////////////////////////////
  //! wtl::strlen_t
  //! Get length of narrow character string 
  //! 
  //! \param[in] const* str - Narrow character string
  //! \return size_t - Length in characters
  ///////////////////////////////////////////////////////////////////////////////
  inline size_t strlen_t(const char* str)
  {
    return strlen(str);
  }

  ///////////////////////////////////////////////////////////////////////////////
  //! wtl::strlen_t
  //! Get length of wide character string 
  //! 
  //! \param[in] const* str - Wide character string
  //! \return size_t - Length in characters
  ///////////////////////////////////////////////////////////////////////////////
  inline size_t strlen_t(const wchar_t* str)
  {
    return wcslen(str);
  }

  ///////////////////////////////////////////////////////////////////////////////
  //! \struct CharArray - Fixed capacity character array with a dynamic runtime length, supporting any character type and encoding 
  //!
  //! \tparam ENCODING - Character encoding 
  //! \tparam LENGTH - Array capacity excluding null terminator
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENCODING, unsigned LENGTH>
  struct CharArray : public Array<encoding_char_t<ENCODING>, LENGTH+1, true>
  {
    friend CharArray;   // Unbound friend of itself

    // ---------------------- TYPES & CONSTANTS -------------------

    //! \alias base - Defines base type
    using base = Array<encoding_char_t<ENCODING>, LENGTH+1, true>;
    
    //! \alias char_t - Character type
    using char_t = encoding_char_t<ENCODING>;

    //! \var encoding - Character encoding
    static constexpr Encoding encoding = ENCODING;

    //! \var null_t - Null terminator
    static const char_t null_t = zero<char_t>::value;

    //! \var EMPTY - Empty character array sentinel value
    static const CharArray EMPTY;

    // --------------------- CONSTRUCTION ----------------------   
  
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::CharArray 
    //! Create an empty (null-terminated) character array
    ///////////////////////////////////////////////////////////////////////////////
    CharArray() 
    {
      // Null terminate (clears entire buffer in development mode)
      clear();
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::CharArray 
    //! Create from a fill character
    //! 
    //! \param[in] chr - Fill character
    //! \param[in] num - Number of characters
    //!
    //! \throw wtl::logic_error - [Debug only] Num exceeds capacity
    ///////////////////////////////////////////////////////////////////////////////
    CharArray(char_t chr, uint32 num) : CharArray()
    {
      // Fill character array with 'num' of character 'chr'
      fill(chr, num)
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::CharArray 
    //! Create from null-terminated decay type string (of equal type encoding)
    //! 
    //! \param[in] str - Null terminated string
    //!
    //! \throw wtl::invalid_argument - [Debug only] String is nullptr
    //! \throw wtl::logic_error - [Debug only] String exceeds capacity
    ///////////////////////////////////////////////////////////////////////////////
    explicit CharArray(const char_t* const str) : CharArray()
    {
      REQUIRED_PARAM(str);

      // Copy from input buffer, truncate if necessary, assume equal encoding
      CharArray::assign(str);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::CharArray 
    //! Create from statically allocated null-terminated string of equal type and encoding
    //! 
    //! \tparam E - Foreign character type
    //! \tparam L - Foreign array capacity 
    //! 
    //! \param[in] str - Null terminated string
    //!
    //! \throw wtl::invalid_argument - [Debug only] String is nullptr
    //! \throw wtl::logic_error - [Debug only] String exceeds capacity
    ///////////////////////////////////////////////////////////////////////////////
    template <typename E, unsigned L>
    explicit CharArray(array_ptr_t<const E,L> str) : CharArray()
    {
      REQUIRED_PARAM(str);

      // Copy from input buffer, truncate if necessary, assume equal encoding
      CharArray::assign<encoding>(str);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::CharArray 
    //! Create from statically allocated formatting string of equal type and weakly-typed arguments
    //! 
    //! \tparam LEN - Formatting string capacity 
    //! 
    //! \param[in] format - Null terminated formatting string
    //! \param[in] args - Weakly typed variadic-arguments list
    //!
    //! \throw wtl::invalid_argument - [Debug only] Formatting string is nullptr
    //! \throw wtl::logic_error - Formatted output exceeds array capacity
    ///////////////////////////////////////////////////////////////////////////////
    template <unsigned LEN>
    CharArray(array_ptr_t<const char_t,LEN> format, va_list args) : CharArray()
    {
      REQUIRED_PARAM(format);

      // Format buffer from input string, truncate if necessary
      if (!formatv(format, args))
        throw wtl::logic_error(HERE, "Insufficient capacity to format string");
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::CharArray 
    //! Copy-create from a character array of equal type
    //! 
    //! \param[in] const& r - Character array of equal type
    ///////////////////////////////////////////////////////////////////////////////
     CharArray(const CharArray& r) : CharArray()
    {
      // Copy from input buffer, truncate if necessary
      CharArray::assign(r);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::CharArray 
    //! Copy-create from another character array of dissimilar type
    //! 
    //! \tparam N - Foreign character encoding
    //! \tparam L - Foreign array capacity 
    //! 
    //! \param[in] const& r - Character array of dissimilar type
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding N, unsigned L>
     CharArray(const CharArray<N,L>& r) : CharArray()
    {
      // Copy from input buffer, truncate if necessary, convert encoding if necessary.
      CharArray::assign<N>(r);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::CharArray 
    //! Move-create from a character array of equal type 
    //! 
    //! \param[in] && r - Character array of equal type (unaffected)
    ///////////////////////////////////////////////////////////////////////////////
     CharArray(CharArray&& r) : CharArray()
    {
      // Copy from input buffer
      CharArray::assign(r);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::CharArray 
    //! Move-create from another character array of dissimilar type
    //! 
    //! \tparam N - Foreign character encoding
    //! \tparam L - Foreign array capacity 
    //! 
    //! \param[in] && r - Character array of dissimilar type (unaffected)
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding N, unsigned L>
     CharArray(CharArray<N,L>&& r) : CharArray()
    {
      // Copy from input buffer, truncate if necessary, convert encoding if necessary.
      CharArray::assign<N>(r);
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::~CharArray 
    //! Virtual destructor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~CharArray()
    {
    }

	  // ------------------------- STATIC ---------------------------

    // ------------------------ ACCESSORS -------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::c_str() const
    //! Get an immutable reference to the entire array 
    //! 
    //! \return const_array_ref - Immutable reference to the entire array
    ///////////////////////////////////////////////////////////////////////////////
    typename base::const_array_ref c_str() const
    {
      return Data;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::copy const
    //! Copy to another character array
    //! 
    //! \param[in] dest - Destination array
    //! \return char_t* - Returns 'dest'
    ///////////////////////////////////////////////////////////////////////////////
    /*char_t* copy(char_t* dest) const
    {
      return strcpy(dest, Data);
    }*/
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::copy
    //! Copy and null-terminate all characters to an output range of sufficient capacity.
    //! If there is insufficient capacity, the results are undefined
    //! 
    //! \tparam OUTPUT - Type of output iterator
    //! 
    //! \param[in] dest - Position of beginning of destination range
    //! \return iterator - Output iterator positioned one element beyond last element copied
    ///////////////////////////////////////////////////////////////////////////////
    template <typename OUTPUT>
    OUTPUT copy(OUTPUT dest) const
    {
      // Copy all characters
      std::copy(begin(), end(), dest);
      
      // Null terminate & return
      *(dest++) = null_t;
      return dest;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::operator == const
    //! Case sensitive equality operator for string literals 
    //! 
    //! \param[in] const *ptr - String literal
    //! \return bool - True iff all characters are equal
    ///////////////////////////////////////////////////////////////////////////////
    bool operator == (typename base::const_pointer ptr) const
    {
      // Check all elements are equal
      return std::equal(begin(), end(), ptr);   //! \warning C4996 - Source
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::operator != const
    //! Case sensitive inequality operator for string literals 
    //! 
    //! \param[in] const *ptr - String literal
    //! \return bool - True iff one or more characters are unequal
    ///////////////////////////////////////////////////////////////////////////////
    bool operator != (typename base::const_pointer ptr) const
    {
      // Ensure elements aren't equal
      return !std::equal(begin(), end(), ptr);   
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Array::operator == const
    //! Case sensitive equality operator for arrays of any type
    //! 
    //! \tparam V - Array element data type
    //! \tparam L - Array length
    //! \tparam D - Whether array can be resized
    //!
    //! \param[in] const &r - Array of another type
    //! \return bool - True iff all elements are equal
    ///////////////////////////////////////////////////////////////////////////////
    //using base::operator==;

    ///////////////////////////////////////////////////////////////////////////////
    // Array::operator == const
    //! Case sensitive equality operator for character arrays of equal type
    //! 
    //! \param[in] const &r - Character array of equal type
    //! \return bool - True iff all characters are equal
    ///////////////////////////////////////////////////////////////////////////////
    bool operator == (const CharArray& r) const
    {
      // Check all elements are equal
      return std::equal(begin(), end(), r.begin());
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::operator == const
    //! Case sensitive equality operator for character arrays of dissimilar type
    //! 
    //! \tparam N - Foreign character encoding
    //! \tparam L - Foreign character array length
    //!
    //! \param[in] const &r - Character array of dissimilar type
    //! \return bool - True iff all characters are equal
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding N, unsigned L>
    bool operator == (const CharArray<N,L>& r) const
    {
      // Check all elements are equal
      return std::equal(begin(), end(), r.begin());
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Array::operator != const
    //! Case sensitive inequality operator for arrays of any type
    //! 
    //! \tparam V - Array element data type
    //! \tparam L - Array length
    //! \tparam D - Whether array can be resized
    //!
    //! \param[in] const &r - Array of another type
    //! \return bool - True iff one or more elements are unequal
    ///////////////////////////////////////////////////////////////////////////////
    //using base::operator!=;
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::operator != const
    //! Case sensitive inequality operator for character arrays of equal type
    //! 
    //! \param[in] const &r - Character array of equal type
    //! \return bool - True iff one or more characters are unequal
    ///////////////////////////////////////////////////////////////////////////////
    bool operator != (const CharArray& r) const
    {
      // Check all elements aren't equal
      return !std::equal(begin(), end(), r.begin());
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::operator != const
    //! Case sensitive inequality operator for character arrays of dissimilar type
    //! 
    //! \tparam N - Foreign character encoding
    //! \tparam L - Foreign character array length
    //!
    //! \param[in] const &r - Character array of dissimilar type
    //! \return bool - True iff one or more characters are unequal
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding N, unsigned L>
    bool operator != (const CharArray<N,L>& r) const
    {
      // Check all elements are not equal
      return !std::equal(begin(), end(), r.begin());
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::operator < const
    //! Less-than relational operator for character arrays of dissimilar type
    //! 
    //! \tparam N - Foreign character encoding
    //! \tparam L - Foreign character array length
    //!
    //! \param[in] const &r - Character array of dissimilar type
    //! \return bool - True iff strcmp() of characters against 'r' returns -1
    //! \return bool - True iff characters are less-than those in 'r' by a lexiographic comparison 
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding N, unsigned L>
    bool operator < (const CharArray<N,L>& r) const
    {
      // Check string sorting order
      return std::strcmp(Data, r.Data) == -1;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::operator > const
    //! Greater-than relational operator for character arrays of dissimilar type
    //! 
    //! \tparam N - Foreign character encoding
    //! \tparam L - Foreign character array length
    //!
    //! \param[in] const &r - Character array of dissimilar type
    //! \return bool - True iff strcmp() of characters against 'r' returns +1
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding N, unsigned L>
    bool operator > (const CharArray<N,L>& r) const
    {
      // Check string sorting order
      return std::strcmp(Data, r.Data) == 1;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::operator + const
    //! Concatenate with another string
    //! 
    //! \param[in] const *ptr - String 
    //! \return CharArray - Array containing concatenation of both strings
    ///////////////////////////////////////////////////////////////////////////////
    CharArray operator+ (typename base::const_pointer ptr) const
    {
      CharArray r(*this);
      r.append(ptr);
      return r;
    }

    // ------------------------- MUTATORS -------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::append
    //! Append a null-terminated string of equal type and equivalent encoding
    //!
    //! \tparam CHAR - Foreign character element type
    //!
    //! \param[in] const *str - String (If not null terminated, the results are undefined)
    //! 
    //! \throw wtl::invalid_argument - [Debug only] String is nullptr
    //! \throw wtl::logic_error - [Debug only] String would be truncated
    ///////////////////////////////////////////////////////////////////////////////
    //template <typename CHAR>
    void append(const char_t* str)
    {
      static_assert(std::is_convertible<CHAR,char>::value, "Cannot convert to characters");

      REQUIRED_PARAM(str);
      LOGIC_INVARIANT(strlen_t(str)+Count+1 < length);
      
      // Avoid self-assignment 
      if (static_cast<const void*>(str) < static_cast<void*>(Data)
       || static_cast<const void*>(str) >= static_cast<void*>(Data+length))

        // Append string until (after) null terminator detected or capacity reached
        for (uint32 in=0, &out=Count; out < length && (Data[out] = str[in]); ++in, ++out) 
          /* no-op */;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::assign
    //! Assign from null-terminated decay type string of equal encoding
    //!
    //! \param[in] const* str - Null-terminated string of equal encoding
    //! \return int32 - New length of string
    //!
    //! \throw wtl::logic_error - [Debug only] String would be truncated
    ///////////////////////////////////////////////////////////////////////////////
    int32 assign(const char_t* const str)
    {
      REQUIRED_PARAM(str);
      LOGIC_INVARIANT(strlen_t(str) <= length);

      // Avoid self-assignment 
      if (static_cast<const void*>(str) < static_cast<void*>(Data)
       || static_cast<const void*>(str) >= static_cast<void*>(Data+length))
      {
        // Assign string until (after) null terminator detected or capacity reached
        for (uint32 in=0, &out = (Count = 0); (out < length) && (Data[out] = str[in]); ++in, ++out) 
          /* no-op */;

        // Null terminate when truncating input string
        if (Count == length)
          back() = null_t;
      }

      // Return new length
      return Count;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::assign
    //! Assign from a character array of equal type
    //!
    //! \param[in] &r - Another array of same type and length
    //! \return int32 - New length of string
    //!
    //! \throw wtl::logic_error - [Debug only] String would be truncated
    ///////////////////////////////////////////////////////////////////////////////
    int32 assign(const CharArray& r)
    {
      return CharArray::assign<encoding>(r.c_arr());
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::assign
    //! Assign from a character array of different type
    //!
    //! \tparam N - Character encoding
    //! \tparam L - Character array length
    //! 
    //! \param[in] &r - Another array
    //! \return int32 - New length of string
    //! 
    //! \throw wtl::logic_error - [Debug only] String would be truncated
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding N, unsigned L>
    int32 assign(const CharArray<N,L>& r) 
    {
      // Assign from foreign array
      return CharArray::assign<N>(r.c_arr()); 
    }

    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::assign
    //! Assign from a null-terminated string of any type with equivalent encoding.
    //! Self-assignment has no affect.
    //! 
    //! \tparam ENC - Foreign character encoding
    //! \tparam LEN - Capacity of foreign character array
    //!
    //! \param[in] const *str - Statically allocated null-terminated string. If terminator is missing the results are undefined.
    //! \return int32 - New length of string
    //! 
    //! \throw wtl::invalid_argument - [Debug only] String is nullptr
    //! \throw wtl::logic_error - [Debug only] String would be truncated
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, unsigned LEN, typename = std::enable_if_t<ENC==encoding>>
    int32 assign(array_ptr_t<const char_t,LEN> str)
    {
      REQUIRED_PARAM(str);
      LOGIC_INVARIANT(strlen_t(*str) <= length);

      // Avoid self-assignment 
      if (static_cast<const void*>(str) < static_cast<void*>(Data)
       || static_cast<const void*>(str) >= static_cast<void*>(Data+length))
      {
        // Assign string until (after) null terminator detected or capacity reached
        for (uint32 in=0, &out = (Count = 0); (out < std::min((unsigned)length,LEN)) && (Data[out] = static_cast<char_t>((*str)[in])); ++in, ++out) 
          /* no-op */;

        // Null terminate when truncating input string
        if (Count == std::min((unsigned)length,LEN))
          back() = null_t;
      }

      return Count;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::assign
    //! Assign from a null-terminated string of any type with dissimilar encoding.
    //! Self-assignment has no affect.
    //! 
    //! \tparam ENC - Foreign character encoding
    //! \tparam LEN - Capacity of foreign character array
    //!
    //! \param[in] const *str - Statically allocated null-terminated string. If terminator is missing the results are undefined.
    //! \return int32 - New length of string
    //! 
    //! \throw wtl::platform_error - Unable to perform conversion
    //! \throw wtl::invalid_argument - [Debug only] String is nullptr
    //! \throw wtl::logic_error - [Debug only] String will be truncated
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, unsigned LEN, typename = std::enable_if_t<ENC!=encoding>>
    int32 assign(array_ptr_t<const encoding_char_t<ENC>, LEN> str)
    {
      REQUIRED_PARAM(str);
      LOGIC_INVARIANT(strlen_t(*str) <= length);
      
      static_assert(sizeof(char_t) != sizeof(encoding_char_t<ENC>), "Conversion between equal sized encodings not yet supported");

      //char_t  buffer[std::max((unsigned)length,LEN)];    //!< Conversion buffer large enough to hold input
      char_t  buffer[length > LEN ? length : LEN];    //!< Conversion buffer large enough to hold input
      int32   useDefault = TRUE,
              inputLength = strlen_t(*str);

      // Convert narrow input to wide output
      if (sizeof(encoding_char_t<ENC>) < sizeof(char_t))
        Count = MultiByteToWideChar(enum_cast(ENC), enum_cast(MultiByteFlags::PreComposed), *str, inputLength, Data, length);

      // Convert wide input to narrow output
      else if (sizeof(encoding_char_t<ENC>) > sizeof(char_t))
        Count = WideCharToMultiByte(enum_cast(encoding), enum_cast(WideCharFlags::CompositeCheck|WideCharFlags::NoBestFitChars), *str, inputLength, Data, length, "?", &useDefault);

      // Ensure succeeded
      if (!Count && inputLength)
        throw platform_error(HERE, "Cannot convert to %s encoding: '%s'", toString(ENC), str);

      return Count;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::c_str() 
    //! Get a mutable reference to the entire array
    //! 
    //! \return pointer - Mutable reference to the entire array
    ///////////////////////////////////////////////////////////////////////////////
    typename base::array_ref c_str() 
    {
      return Data;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::clear
    //! Clears the array
    ///////////////////////////////////////////////////////////////////////////////
    void clear()
    {
//! \if DEVELOPMENT_MODE - Zero entire string in dev mode
#ifdef DEVELOPMENT_MODE
      base::clear();
#else
      // Null terminate
      Data[Count=0] = null_t;
#endif
    }

    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::fill
    //! Replace contents with fill character
    //! 
    //! \param[in] chr - Fill character
    //! \param[in] num - Number of characters
    //!
    //! \throw wtl::logic_error - [Debug only] Num exceeds capacity
    ///////////////////////////////////////////////////////////////////////////////
    void fill(char_t chr, uint32 num) 
    {
      LOGIC_INVARIANT(num > 0 && num <= LENGTH);

      // Prevent buffer overrun
      num = std::min(num, (uint32)LENGTH);

      // Fill 'num' characters
      for (Count = 0; Count < num; ++Count)
        Data[Count] = chr;

      // Null terminate
      Data[Count] = null_t;
    }
    

    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::format
    //! Assign from a formatting string and strongly-typed variadic arguments
    //!
    //! \param[in] const* str - Formatting string
    //! \param[in] &&... args - [optional] Formatting arguments
    //! \return uint32 - Number of characters written 
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing formatting string
    //! \throw wtl::length_error - Insufficent capacity
    //! \throw wtl::logic_error - Incorrect number of arguments
    ///////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>		
    uint32 format(string_literal_t str, ARGS&&... args) 
    {
      // Clear & format
      clear();
      format_t(str, std::forward<ARGS>(args)...);
      
      // Return new count
      return Count;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::formatv
    //! Assign from a formatting string and variadic arguments
    //!
    //! \param[in] const* format - Formatting string
    //! \param[in] args - Formatting arguments
    //! \return uint32 - Number of characters written iff successful, otherwise zero
    //! 
    //! \throw std::invalid_argument - [Debug only] Missing formatting string
    ///////////////////////////////////////////////////////////////////////////////
    uint32 formatv(string_literal_t format, va_list args) 
    {
      REQUIRED_PARAM(format);

      // Attempt to format
      int32 n = vsnprintf(Data, length, format, args);

      // Succeeded:
      if (n >= 0 && n < length)
        Count = n;

      // Failed: Erase 
      else
        clear();
      
      // Return # of chars written
      return Count;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::update
    //! Recalculates the length property (Useful after direct buffer modification)
    ///////////////////////////////////////////////////////////////////////////////
    void update() 
    {
      Count = strlen_t(Data);
    }

    ////////////////////////////////////////////////////////////////////////////////
    //! CharArray::operator=
    //! Overwrite contents with that of a null-terminated string of equivalent encoding
    //! 
    //! \param[in] str - Null-terminated string of equivalent encoding
    //! \return CharArray& - Reference to self with updated contents
    ////////////////////////////////////////////////////////////////////////////////
    CharArray& operator=(const char_t* str)
    {
      CharArray::assign(str, encoding);
      return *this;
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    //! CharArray::operator=
    //! Overwrite contents with that of a statically allocated null-terminated string of equivalent encoding 
    //! 
    //! \tparam LEN - Capacity of foreign array
    //! 
    //! \param[in] str - Statically allocated null-terminated string of equivalent encoding
    //! \return CharArray& - Reference to self with updated contents
    ////////////////////////////////////////////////////////////////////////////////
    template <unsigned LEN>
    CharArray& operator=(array_ptr_t<const char_t,LEN> str)
    {
      CharArray::assign(str, encoding);
      return *this;
    }
       
    ////////////////////////////////////////////////////////////////////////////////
    //! CharArray::operator=
    //! Overwrite contents with that of another character array (of equal type and encoding)
    //! 
    //! \param[in] const& r - Another character array (of equal type and encoding)
    //! \return CharArray& - Reference to self with updated contents
    ////////////////////////////////////////////////////////////////////////////////
    CharArray& operator=(const CharArray& r)
    {
      CharArray::assign(r, encoding);
      return *this;
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    //! CharArray::operator=
    //! Overwrite contents with that of another character array (of different type)
    //! 
    //! \tparam N - Foreign character encoding
    //! \tparam L - Foreign array capacity 
    //! 
    //! \param[in] const& r - Char array of different type
    //! \return CharArray& - Reference to self with updated contents
    ////////////////////////////////////////////////////////////////////////////////
    template <Encoding N, unsigned L>
    CharArray& operator=(const CharArray<N,L>& r)
    {
      CharArray::assign<N>(r);
      return *this;
    }
    
  protected:
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::format_t
    //! Assign from a formatting string and strongly-typed variadic arguments.
    //! 
    //! \tparam T - First pack element type
    //! \tparam ARGS - Remaining arguments parameter pack
    //!
    //! \param[in] const* str - Formatting string
    //! \param[in] value - First pack value
    //! \param[in] args - [optional] Remaining arguments
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing formatting string
    //! \throw wtl::length_error - Insufficent capacity
    //! \throw wtl::logic_error - Incorrect number of arguments
    ///////////////////////////////////////////////////////////////////////////////
	  template<typename T, typename... ARGS>		
	  void format_t(const char_t* str, T value, ARGS... args)	
	  {
      REQUIRED_PARAM(str);

      // Iterate through formatting string
      if (str) 
        do
        { // [FORMAT SPEC] Width/precision are ignored
			    if (*str == '%' && *(++str) != '%')   // Ignore %%
          {	
            // Skip Width
            if (*str == 'l')
              if (*++str == 'l')
                *++str;

            // Append value 
            int len = snprintf(Data+Count, (size_t)length-Count, format_spec_t<T>::value, value); 
          
            // Succeeded: Advance count
            if (len >= 0 && len < length)
              Count += len;
            else
              throw wtl::length_error(HERE, "Insufficient capacity to format string");
          
            // Unpack next argument
				    return format_t(++str, args...); 	
			    }

          // Write char (or null-terminator)
          Data[Count++] = *str;   
		    } 
        // Advance until null-terminator (or capacity reached)
        while (*str++ && Count < length);

      // [ERROR] Insufficent capcity / Excess arguments
		  throw wtl::logic_error(HERE, Count >= length ? "Insufficient buffer space" : "Extra arguments provided");
	  }
  
    ///////////////////////////////////////////////////////////////////////////////
    // CharArray::format_t
    //! Formats a string with no arguments
    //! 
    //! \param[in] const* str - Formatting string
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing formatting string
    //! \throw wtl::logic_error - Insufficient arguments
    ///////////////////////////////////////////////////////////////////////////////
	  void format_t(const char_t* str)	
	  {
      REQUIRED_PARAM(str);

      // Iterate through formatting string
      if (str) 
        do	
        { // [FORMAT SPEC] Invalid at this point
		 	    if (*str == '%' && *(++str) != '%')	  // Ignore %%
            // [ERROR] Insufficent arguments
		        throw wtl::logic_error(HERE, "Missing formatting argument");

          // Write char (or null-terminator)
          Data[Count++] = *str;  
		    }
        // Advance until null-terminator (or capacity reached)
        while (*str++ && Count < length);
	  }

  
    // ---------------------- REPRESENTATION ----------------------
  };
  
  ///////////////////////////////////////////////////////////////////////////////
  // wtl::c_arr
  //! Creates an appropriately encoded character array from a statically allocated string buffer
  //! 
  //! \tparam char_t - Character type
  //! \tparam encoding - [optional] Character encoding (if unspecified the default encoding for the character type is used)
  //! \tparam capacity - Array capacity (excluding null terminator)
  //!
  //! \param[in] const* str - Null terminated string
  //! \param[in] len - Length of string (in characters)
  //! \return CharArray<ENC,LEN> - Character array of capacity 'LENGTH' containing input string
  ///////////////////////////////////////////////////////////////////////////////
  template <typename char_t, Encoding encoding = default_encoding<char_t>::value, unsigned capacity>
  CharArray<encoding,capacity>  c_arr(array_ref_t<const char_t,capacity> str) 
  {
    return CharArray<encoding,capacity>(str);
  };

  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct LastErrorString - Encapsulates the string representation of ::GetLastError()
  //! 
  //! \tparam ENC - [optional] Error message character encoding (ANSI is default)
  //! \tparam LEN - [optional] Error message capacity including null terminator (1024 is default)
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC = Encoding::ANSI, unsigned LEN = 1024>
  struct LastErrorString : CharArray<ENC,LEN>
  {
    //! \alias base - Define base type
    using base = CharArray<ENC,LEN>;

    ///////////////////////////////////////////////////////////////////////////////
    // LastErrorString::LastErrorString
    //! Create string from ::GetLastError()
    ///////////////////////////////////////////////////////////////////////////////
    explicit LastErrorString()
    {
      //! \var formatMsg - Format message
      static const auto formatMsg = getFunc<encoding_char_t<ENC>>(::FormatMessageA,::FormatMessageW);

      // Lookup system error and append to user error
      formatMsg(enum_cast(FormatMessageFlags::FromSystem|FormatMessageFlags::IgnoreInserts), nullptr, ::GetLastError(), 0UL, *base::c_str(), base::length, nullptr);
    }
  };

} // GDK namespace



#endif // WTL_CHAR_ARRAY_HPP

