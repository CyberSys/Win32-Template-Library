//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\CharArray.hpp
//! \brief Statically allocated string
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CHAR_ARRAY_HPP
#define WTL_CHAR_ARRAY_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/FixedArray.hpp"
#include "wtl/utils/Helpers.hpp"
#include "wtl/traits/EncodingTraits.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct CharArray - Fixed capacity character array with a dynamic runtime length, supporting any character type and encoding 
  //!
  //! \tparam ENCODING - Character encoding 
  //! \tparam LENGTH - Array capacity excluding null terminator
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENCODING, unsigned LENGTH>
  struct CharArray : public DynamicArray<encoding_char_t<ENCODING>, LENGTH+1>
  {
    friend CharArray;   // Unbound friend of itself

    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias base - Defines base type
    using base = DynamicArray<encoding_char_t<ENCODING>, LENGTH+1>;
    
    //! \alias type - Defines own type
    using type = CharArray<ENCODING,LENGTH>;
    
    //! \alias char_t - Character type
    using char_t = encoding_char_t<ENCODING>;

    //! \var encoding - Character encoding
    static constexpr Encoding encoding = ENCODING;

    //! \var null_t - Null terminator
    static constexpr char_t null_t = default<char_t>();

    //! \var EMPTY - Empty character array sentinel value
    static const CharArray<ENCODING,LENGTH> EMPTY;
    
  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct conversion_proxy - Handles conversion between character encoding types
    //! 
    //! \tparam FROM - Input character encoding
    //! \tparam TO - Output character encoding
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding FROM, Encoding TO, typename>
    struct conversion_proxy;

    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct conversion_proxy<FROM,Encoding::UTF16> - Handles narrow character -> wide character conversion
    //! 
    //! \tparam FROM - Input character encoding
    //! \tparam TO - Output character encoding
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding FROM>
    struct conversion_proxy<FROM,Encoding::UTF16,enable_if_not_encoding_t<FROM,Encoding::UTF16>>
    {
      //! \alias input_t - Input character type
      using input_t = encoding_char_t<FROM>;

      //! \alias output_t - Output character type
      using output_t = encoding_char_t<Encoding::UTF16>;

      /////////////////////////////////////////////////////////////////////////////////////////
      // conversion_proxy::convert
      //! Copy a narrow character string into a wide character buffer
      //! 
      //! \tparam INPUT - Input iterator type
      //! 
      //! \param[in] const* first - First character
      //! \param[in] const* last - Position beyond final character
      //! \param[in,out] *output - First character in output range
      //! \param[in] const* lastOut - Position immediately beyond final character of output range
      //!
      //! \throw wtl::invalid_argument - [Debug only] Any position is nullptr
      /////////////////////////////////////////////////////////////////////////////////////////
      template <typename INPUT = const input_t*>
      static int32  convert(INPUT first, INPUT last, output_t* output, const output_t* lastOut)
      {
        REQUIRED_PARAM(first);  REQUIRED_PARAM(output);
        REQUIRED_PARAM(last);   REQUIRED_PARAM(lastOut);
        
        // narrow -> wide
        return MultiByteToWideChar(enum_cast(FROM), enum_cast(MultiByteFlags::PreComposed), std::addressof(*first), last-first, output, lastOut-output);
      }
    };

    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct conversion_proxy<Encoding::UTF16,TO> - Handles wide character -> narrow character conversion
    //! 
    //! \tparam FROM - Input character encoding
    //! \tparam TO - Output character encoding
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding TO>
    struct conversion_proxy<Encoding::UTF16,TO,enable_if_not_encoding_t<TO,Encoding::UTF16>>
    {
      //! \alias input_t - Input character type
      using input_t = encoding_char_t<Encoding::UTF16>;
      
      //! \alias output_t - Output character type
      using output_t = encoding_char_t<TO>;

      /////////////////////////////////////////////////////////////////////////////////////////
      // conversion_proxy::convert
      //! Copy an wide character string into a narrow character buffer
      //! 
      //! \tparam INPUT - Input iterator type
      //! 
      //! \param[in] const* first - First character
      //! \param[in] const* last - Position beyond final character
      //! \param[in,out] *output - First character in output range
      //! \param[in] const* lastOut - Position immediately beyond final character of output range
      //!
      //! \throw wtl::invalid_argument - [Debug only] Any position is nullptr
      /////////////////////////////////////////////////////////////////////////////////////////
      template <typename INPUT = const input_t*>
      static int32  convert(INPUT first, INPUT last, output_t* output, const output_t* lastOut)
      {
        REQUIRED_PARAM(first);  REQUIRED_PARAM(output);
        REQUIRED_PARAM(last);   REQUIRED_PARAM(lastOut);

        // wide -> narrow
        int32  useDefault = TRUE;
        return WideCharToMultiByte(enum_cast(TO), enum_cast(WideCharFlags::CompositeCheck|WideCharFlags::NoBestFitChars), std::addressof(*first), last-first, output, lastOut-output, "?", &useDefault);
      }
    };
    
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct conversion_proxy<E,E> - Handles no conversion required
    //! 
    //! \tparam E - Character encoding of both strings
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding E>
    struct conversion_proxy<E,E,void>
    {
      //! \alias input_t - Input character type
      using input_t = encoding_char_t<E>;

      //! \alias output_t - Output character type
      using output_t = input_t;

      /////////////////////////////////////////////////////////////////////////////////////////
      // conversion_proxy::convert
      //! Copy an input string of equal encoding
      //! 
      //! \tparam INPUT - Input iterator type
      //! 
      //! \param[in] const* first - First character
      //! \param[in] const* last - Position beyond final character
      //! \param[in,out] *output - First character in output range
      //! \param[in] const* lastOut - Position immediately beyond final character of output range
      //!
      //! \throw wtl::invalid_argument - [Debug only] Any position is nullptr
      /////////////////////////////////////////////////////////////////////////////////////////
      template <typename INPUT = const input_t*>
      static int32  convert(INPUT first, INPUT last, output_t* output, const output_t* lastOut)
      {
        /*REQUIRED_PARAM(first);*/  REQUIRED_PARAM(output);
        /*REQUIRED_PARAM(last);*/   REQUIRED_PARAM(lastOut);

        const int32 CAPACITY = lastOut-output-1;   //!< Output buffer character capacity 

        // Clear output
        output[0] = default<output_t>();

        // Prevent output buffer overrun
        if (last - first > CAPACITY)
          last = first + CAPACITY;

        // Copy characters from input range
        for (INPUT pos = first; pos != last; ++pos, ++output)
          *output = *pos;

        // null terminate
        *output = default<output_t>();
        
        // Return number of characters copied
        return last-first;
      }
    };

    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct buffer_proxy - Encapsulates updating the 'Count' property after external modification
    /////////////////////////////////////////////////////////////////////////////////////////
    struct buffer_proxy final
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------

      //! \using const_array_ref_t - Immutable array reference type
      using const_array_ref_t = typename base::const_array_ref_t;

      //! \using array_ref_t - Array reference type
      using array_ref_t = typename base::array_ref_t;
      
      // ----------------------------------- REPRESENTATION -----------------------------------
    public:
      array_ref_t  Text;        //!< Character array text buffer

    protected:
      type&        Owner;       //!< Character array

      // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------  
    public:
      /////////////////////////////////////////////////////////////////////////////////////////
      // buffer_proxy::buffer_proxy 
      //! Create proxy for character array
      //! 
      //! \param[in,out] &r - Character array
      /////////////////////////////////////////////////////////////////////////////////////////
      buffer_proxy(type& r) : Text(r.Data), Owner(r)
      {}

      /////////////////////////////////////////////////////////////////////////////////////////
      // buffer_proxy::~buffer_proxy 
      //! Updates the character array length
      /////////////////////////////////////////////////////////////////////////////////////////
      ~buffer_proxy() 
      {
        Owner.Count = strlen_t(Text);
      }
      
	    // ----------------------------------- STATIC METHODS -----------------------------------

      // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
      /////////////////////////////////////////////////////////////////////////////////////////
      // buffer_proxy::operator const_array_ref_t const
      //! Implicit user conversion to immutable buffer
      /////////////////////////////////////////////////////////////////////////////////////////
      operator const_array_ref_t () const
      {
        return Text;
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      // buffer_proxy::operator array_ref_t 
      //! Implicit user conversion to mutable buffer
      /////////////////////////////////////////////////////////////////////////////////////////
      operator array_ref_t () 
      {
        return Text;
      }
      
      // ----------------------------------- MUTATOR METHODS ----------------------------------
    };

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::CharArray 
    //! Create an empty (null-terminated) character array
    /////////////////////////////////////////////////////////////////////////////////////////
    CharArray() 
    {
      // Null terminate (clears entire buffer in development mode)
      clear();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::CharArray 
    //! Create from a fill character
    //! 
    //! \param[in] chr - Fill character
    //! \param[in] num - Number of characters
    //!
    //! \throw wtl::logic_error - [Debug only] Num exceeds capacity
    /////////////////////////////////////////////////////////////////////////////////////////
    CharArray(char_t chr, uint32 num) : CharArray()
    {
      // Fill character array with 'num' of character 'chr'
      fill(chr, num)
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::CharArray 
    //! Create from null-terminated decay type string (of equal type encoding)
    //! 
    //! \param[in] str - Null terminated string
    //!
    //! \throw wtl::invalid_argument - [Debug only] String is nullptr
    //! \throw wtl::logic_error - [Debug only] String exceeds capacity
    /////////////////////////////////////////////////////////////////////////////////////////
    explicit CharArray(const char_t* str) : CharArray()
    {
      REQUIRED_PARAM(str);

      // Copy from input buffer, truncate if necessary, assume equal encoding
      CharArray::assign<encoding>(str, str+strlen_t(str));
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::CharArray 
    //! Create from range of characters
    //! 
    //! \tparam ITERATOR - Input iterator type 
    //! 
    //! \param[in] first - First character in range
    //! \param[in] last - Position beyond final character in range
    //!
    //! \throw wtl::invalid_argument - [Debug only] Either position is nullptr
    //! \throw wtl::logic_error - String exceeds array capacity
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename ITERATOR = const char_t*>
    CharArray(ITERATOR first, ITERATOR last) : CharArray()
    {
      /*REQUIRED_PARAM(first);
      REQUIRED_PARAM(last);*/

      // Copy from input buffer, truncate if necessary, assume equal encoding
      CharArray::assign<encoding>(first, last);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::CharArray 
    //! Copy-create from another character array of dissimilar type
    //! 
    //! \tparam N - Foreign character encoding
    //! \tparam L - Foreign array capacity 
    //! 
    //! \param[in] const& r - Character array of dissimilar type
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding N, unsigned L>
    CharArray(const CharArray<N,L>& r) : CharArray()
    {
      // Copy from input buffer, truncate if necessary, convert encoding if necessary.
      CharArray::assign<N>(r.begin(), r.end());
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::CharArray 
    //! Move-create from another character array of dissimilar type
    //! 
    //! \tparam N - Foreign character encoding
    //! \tparam L - Foreign array capacity 
    //! 
    //! \param[in] && r - Character array of dissimilar type (unaffected)
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding N, unsigned L>
    CharArray(CharArray<N,L>&& r) : CharArray()
    {
      // Copy from input buffer, truncate if necessary, convert encoding if necessary.
      CharArray::assign<N>(r.begin(), r.end());
    }
    
    // -------------------------------- COPYING & DESTRUCTION -------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::CharArray 
    //! Copy-create from a character array of equal type
    //! 
    //! \param[in] const& r - Character array of equal type
    /////////////////////////////////////////////////////////////////////////////////////////
    CharArray(const type& r) : CharArray(r.begin(), r.end())    // Copy from input buffer, truncate if necessary
    {}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::CharArray 
    //! Move-create from a character array of equal type 
    //! 
    //! \param[in] && r - Character array of equal type (unaffected)
    /////////////////////////////////////////////////////////////////////////////////////////
    CharArray(type&& r) : CharArray(r.begin(), r.end())    // Copy from input buffer, truncate if necessary
    {}
    
    //////////////////////////////////////////////////////////////////////////////////////////
    //! CharArray::operator = 
    //! Overwrite contents with that of a null-terminated string of equivalent encoding
    //! 
    //! \param[in] str - Null-terminated string of equivalent encoding
    //! \return CharArray& - Reference to self with updated contents
    //////////////////////////////////////////////////////////////////////////////////////////
    type& operator = (const char_t* str)
    {
      CharArray::assign(str, encoding);
      return *this;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    //! CharArray::operator = 
    //! Overwrite contents with that of a statically allocated null-terminated string of equivalent encoding 
    //! 
    //! \tparam LEN - Capacity of foreign array
    //! 
    //! \param[in] str - Statically allocated null-terminated string of equivalent encoding
    //! \return CharArray& - Reference to self with updated contents
    //////////////////////////////////////////////////////////////////////////////////////////
    template <unsigned LEN>
    type& operator = (array_ptr_t<const char_t,LEN> str)
    {
      CharArray::assign(str, encoding);
      return *this;
    }
       
    //////////////////////////////////////////////////////////////////////////////////////////
    //! CharArray::operator = 
    //! Overwrite contents with that of another character array (of equal type and encoding)
    //! 
    //! \param[in] const& r - Another character array (of equal type and encoding)
    //! \return CharArray& - Reference to self with updated contents
    //////////////////////////////////////////////////////////////////////////////////////////
    type& operator = (const type& r)
    {
      CharArray::assign(r, encoding);
      return *this;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    //! CharArray::operator = 
    //! Overwrite contents with that of another character array (of different type)
    //! 
    //! \tparam N - Foreign character encoding
    //! \tparam L - Foreign array capacity 
    //! 
    //! \param[in] const& r - Char array of different type
    //! \return CharArray& - Reference to self with updated contents
    //////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding N, unsigned L>
    type& operator = (const CharArray<N,L>& r)
    {
      CharArray::assign<N>(r);
      return *this;
    }
    
    ENABLE_POLY(CharArray);         //!< Can be polymorphic
    
	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::buffer const
    //! Get direct access to the character array buffer
    //! 
    //! \return buffer_proxy - Buffer proxy
    /////////////////////////////////////////////////////////////////////////////////////////
    buffer_proxy buffer() const
    {
      return {*this};
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::c_str() const
    //! Get an immutable reference to the entire array 
    //! 
    //! \return const_array_ref - Immutable reference to the entire array
    /////////////////////////////////////////////////////////////////////////////////////////
    typename base::const_array_ref c_str() const
    {
      return this->Data;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::copy const
    //! Copy to another character array
    //! 
    //! \param[in] dest - Destination buffer
    //! \return char_t* - Returns 'dest'
    /////////////////////////////////////////////////////////////////////////////////////////
    char_t* copy(char_t* dest) const
    {
      return strcpy_t(dest, &this->Data[0]);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::operator == const
    //! Case sensitive equality operator for null terminated strings
    //! 
    //! \param[in] const* str - Null terminated string
    //! \return bool - True iff all characters are equal
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator == (const char_t* str) const
    {
      // Check all characters are equal
      return strcmp_t(this->Data, str) == 0;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::operator != const
    //! Case sensitive inequality operator for null terminated strings
    //! 
    //! \param[in] const* str - Null terminated string
    //! \return bool - True iff any characters are unequal
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator != (const char_t* str) const
    {
      // Check strings are not equal
      return strcmp_t(this->Data, str) != 0;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::operator == const
    //! Case sensitive equality operator for arrays of any type
    //! 
    //! \tparam V - Array element data type
    //! \tparam L - Array length
    //! \tparam D - Whether array can be resized
    //!
    //! \param[in] const &r - Array of another type
    //! \return bool - True iff all elements are equal
    /////////////////////////////////////////////////////////////////////////////////////////
    //using base::operator==;

    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::operator == const
    //! Case sensitive equality operator for character arrays of equal type
    //! 
    //! \param[in] const &r - Character array of equal type
    //! \return bool - True iff all characters are equal
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator == (const type& r) const
    {
      // Check all characters are equal
      return strcmp_t(this->Data, r.Data) == 0;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::operator == const
    //! Case sensitive equality operator for character arrays of dissimilar type
    //! 
    //! \tparam N - Foreign character encoding
    //! \tparam L - Foreign character array length
    //!
    //! \param[in] const &r - Character array of dissimilar type
    //! \return bool - True iff all characters are equal
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding N, unsigned L>
    bool operator == (const CharArray<N,L>& r) const
    {
      // Check all elements are equal
      return std::equal(this->begin(), this->end(), r.begin());
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Array::operator != const
    //! Case sensitive inequality operator for arrays of any type
    //! 
    //! \tparam V - Array element data type
    //! \tparam L - Array length
    //! \tparam D - Whether array can be resized
    //!
    //! \param[in] const &r - Array of another type
    //! \return bool - True iff one or more elements are unequal
    /////////////////////////////////////////////////////////////////////////////////////////
    //using base::operator!=;
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::operator != const
    //! Case sensitive inequality operator for character arrays of equal type
    //! 
    //! \param[in] const &r - Character array of equal type
    //! \return bool - True iff one or more characters are unequal
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator != (const type& r) const
    {
      // Check characters are unequal
      return strcmp_t(this->Data, r.Data) != 0;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::operator != const
    //! Case sensitive inequality operator for character arrays of dissimilar type
    //! 
    //! \tparam N - Foreign character encoding
    //! \tparam L - Foreign character array length
    //!
    //! \param[in] const &r - Character array of dissimilar type
    //! \return bool - True iff one or more characters are unequal
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding N, unsigned L>
    bool operator != (const CharArray<N,L>& r) const
    {
      // Check all elements are not equal
      return !std::equal(this->begin(), this->end(), r.begin());
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::operator < const
    //! Less-than relational operator for character arrays of dissimilar type
    //! 
    //! \tparam N - Foreign character encoding
    //! \tparam L - Foreign character array length
    //!
    //! \param[in] const &r - Character array of dissimilar type
    //! \return bool - True iff strcmp() of characters against 'r' returns -1
    //! \return bool - True iff characters are less-than those in 'r' by a lexiographic comparison 
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding N, unsigned L>
    bool operator < (const CharArray<N,L>& r) const
    {
      // Check string sorting order
      return strcmp_t(this->Data, r.Data) < 0;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::operator > const
    //! Greater-than relational operator for character arrays of dissimilar type
    //! 
    //! \tparam N - Foreign character encoding
    //! \tparam L - Foreign character array length
    //!
    //! \param[in] const &r - Character array of dissimilar type
    //! \return bool - True iff strcmp() of characters against 'r' returns +1
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding N, unsigned L>
    bool operator > (const CharArray<N,L>& r) const
    {
      // Check string sorting order
      return strcmp_t(this->Data, r.Data) > 0;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::operator + const
    //! Create concatenatation with another string
    //! 
    //! \param[in] const* str - String 
    //! \return type - Array containing concatenation of both strings
    /////////////////////////////////////////////////////////////////////////////////////////
    type operator+ (const char_t* str) const
    {
      CharArray r(*this);
      r.append(str);
      return r;
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::append
    //! Append a null-terminated string of any encoding
    //! 
    //! \tparam E - Foreign character encoding, if unspecified then equal encoding is assumed
    //! \tparam CHR - Foreign character type
    //!
    //! \param[in] const* first - First character
    //! \param[in] const* last - Position immediately beyond last character
    //! \return int32 - New length of string, in characters
    //! 
    //! \throw wtl::platform_error - Unable to perform conversion
    //! \throw wtl::invalid_argument - [Debug only] String is nullptr
    //! \throw wtl::logic_error - [Debug only] String will be truncated
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding E = encoding, typename CHR = encoding_char_t<E>>
    int32 append(const CHR* first, const CHR* last)
    {
      REQUIRED_PARAM(first);
      REQUIRED_PARAM(last);
      LOGIC_INVARIANT(last-first < this->Count+length-1);
      
      // Convert/Append input string
      int32 num = conversion_proxy<E,encoding,void>::convert(first, last, &this->Data[this->Count], &this->Data[length]);
      this->Count += num;

      // Ensure succeeded
      if (last-first && !num)
        throw platform_error(HERE, "Unable to convert character encoding");  //throw platform_error(HERE, "Cannot convert to %s encoding: '%s'", toString(E), str);

      // Return new length
      return this->Count;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::assign
    //! Assign from a character array of equal type
    //!
    //! \param[in] &r - Another array of same type and length
    //! \return int32 - New length of string
    //!
    //! \throw wtl::logic_error - [Debug only] String would be truncated
    /////////////////////////////////////////////////////////////////////////////////////////
    int32 assign(const type& r)
    {
      // Assign from foreign array
      return CharArray::assign<encoding>(r.begin(), r.end());
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::assign
    //! Assign from a character array of different type
    //!
    //! \tparam E - Foreign character encoding
    //! \tparam L - Foreign character array length
    //! 
    //! \param[in] &r - Another array
    //! \return int32 - New length of string
    //! 
    //! \throw wtl::logic_error - [Debug only] String would be truncated
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding E, unsigned L>
    int32 assign(const CharArray<E,L>& r) 
    {
      // Assign from foreign array
      return CharArray::assign<E>(r.begin(), r.end()); 
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::assign
    //! Assign from a null-terminated string of any encoding
    //! 
    //! \tparam E - Foreign character encoding, if unspecified then equal encoding is assumed
    //! \tparam CHR - Foreign character type
    //!
    //! \param[in] const* first - First character
    //! \param[in] const* last - Position immediately beyond last character
    //! \return int32 - New length of string, in characters
    //! 
    //! \throw wtl::platform_error - Unable to perform conversion
    //! \throw wtl::invalid_argument - [Debug only] String is nullptr
    //! \throw wtl::logic_error - [Debug only] String will be truncated
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding E = encoding, typename CHR = const encoding_char_t<E>*>
    int32 assign(CHR first, CHR last)
    {
      //REQUIRED_PARAM(first);
      //REQUIRED_PARAM(last);
      LOGIC_INVARIANT(last-first < length-1);
      
      // Convert/Assign input string
      this->Count = conversion_proxy<E,encoding,void>::convert(first, last, &this->Data[0], &this->Data[length]);

      // Ensure succeeded
      if (last-first && !this->Count)
        throw platform_error(HERE, "Unable to convert character encoding");  

      // Return new length
      return this->Count;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::c_str() 
    //! Get a mutable reference to the entire array
    //! 
    //! \return pointer - Mutable reference to the entire array
    /////////////////////////////////////////////////////////////////////////////////////////
    typename base::array_ref c_str() 
    {
      return this->Data;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::clear
    //! Clears the array
    /////////////////////////////////////////////////////////////////////////////////////////
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

    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::fill
    //! Replace contents with fill character
    //! 
    //! \param[in] chr - Fill character
    //! \param[in] num - Number of characters
    //!
    //! \throw wtl::logic_error - [Debug only] Num exceeds capacity
    /////////////////////////////////////////////////////////////////////////////////////////
    void fill(char_t chr, uint32 num) 
    {
      LOGIC_INVARIANT(num > 0 && num <= LENGTH);

      // Prevent buffer overrun
      num = std::min(num, (uint32)LENGTH);

      // Fill 'num' characters
      for (this->Count = 0; this->Count < num; ++this->Count)
        this->Data[this->Count] = chr;

      // Null terminate
      this->Data[this->Count] = null_t;
    }
    

    /////////////////////////////////////////////////////////////////////////////////////////
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
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>		
    uint32 format(const char_t* str, ARGS&&... args) 
    {
      // Clear & format
      clear();
      format_t(str, std::forward<ARGS>(args)...);
      
      // Return new count
      return this->Count;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::formatv
    //! Assign from a formatting string and variadic arguments
    //!
    //! \param[in] const* format - Formatting string
    //! \param[in] args - Formatting arguments
    //! \return uint32 - Number of characters written iff successful, otherwise zero
    //! 
    //! \throw std::invalid_argument - [Debug only] Missing formatting string
    /////////////////////////////////////////////////////////////////////////////////////////
    uint32 formatv(const char_t* format, va_list args) 
    {
      REQUIRED_PARAM(format);

      // Attempt to format
      int32 n = vsnprintf(this->Data, this->length, format, args);

      // Succeeded:
      if (n >= 0 && n < this->length)
        this->Count = n;

      // Failed: Erase 
      else
        clear();
      
      // Return # of chars written
      return this->Count;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::update
    //! Recalculates the length property (Useful after direct buffer modification)
    /////////////////////////////////////////////////////////////////////////////////////////
    void update() 
    {
      this->Count = strlen_t(this->Data);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::operator +=
    //! Concatenate with another string
    //! 
    //! \param[in] const *str - String 
    //! \return CharArray& - Reference to self containing concatenation of both strings
    /////////////////////////////////////////////////////////////////////////////////////////
    type& operator += (const char_t* str) 
    {
      r.append(str);
      return r;
    }

  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
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
    /////////////////////////////////////////////////////////////////////////////////////////
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
            int len = snprintf(this->Data+this->Count, (size_t)length-this->Count, format_spec_t<T>::value, value); 
          
            // Succeeded: Advance count
            if (len >= 0 && len < length)
              Count += len;
            else
              throw wtl::length_error(HERE, "Insufficient capacity to format string");
          
            // Unpack next argument
				    return format_t(++str, args...); 	
			    }

          // Write char (or null-terminator)
          this->Data[this->Count++] = *str;   
		    } 
        // Advance until null-terminator (or capacity reached)
        while (*str++ && this->Count < length);

      // [ERROR] Insufficent capcity / Excess arguments
		  throw wtl::logic_error(HERE, this->Count >= length ? "Insufficient buffer space" : "Extra arguments provided");
	  }
  
    /////////////////////////////////////////////////////////////////////////////////////////
    // CharArray::format_t
    //! Formats a string with no arguments
    //! 
    //! \param[in] const* str - Formatting string
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing formatting string
    //! \throw wtl::logic_error - Insufficient arguments
    /////////////////////////////////////////////////////////////////////////////////////////
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
          this->Data[this->Count++] = *str;  
		    }
        // Advance until null-terminator (or capacity reached)
        while (*str++ && this->Count < length);
	  }
  };

  //! \var CharArray<ENC,LEN>::EMPTY - Empty character array
  template <Encoding ENC, unsigned LEN>
  const CharArray<ENC,LEN>  CharArray<ENC,LEN>::EMPTY;
  
  /////////////////////////////////////////////////////////////////////////////////////////
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
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename char_t, Encoding encoding = default_encoding<char_t>::value, unsigned capacity>
  CharArray<encoding,capacity>  c_arr(array_ref_t<const char_t,capacity> str) 
  {
    return CharArray<encoding,capacity>(str);
  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct LastErrorString - Encapsulates the string representation of ::GetLastError()
  //! 
  //! \tparam ENC - [optional] Error message character encoding (ANSI is default)
  //! \tparam LEN - [optional] Error message capacity including null terminator (1024 is default)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC = Encoding::ANSI, unsigned LEN = 1024>
  struct LastErrorString : CharArray<ENC,LEN>
  {
    //! \alias base - Define base type
    using base = CharArray<ENC,LEN>;

    /////////////////////////////////////////////////////////////////////////////////////////
    // LastErrorString::LastErrorString
    //! Create from last system error
    /////////////////////////////////////////////////////////////////////////////////////////
    LastErrorString()
    {
      //! \var formatMsg - Format message
      static const auto formatMsg = getFunc<encoding_char_t<ENC>>(::FormatMessageA,::FormatMessageW);

      // Lookup system error and append to user error
      formatMsg(enum_cast(FormatMessageFlags::FromSystem|FormatMessageFlags::IgnoreInserts), nullptr, ::GetLastError(), 0UL, *base::c_str(), base::length, nullptr);
    }
  };

} // WTL namespace



#endif // WTL_CHAR_ARRAY_HPP

