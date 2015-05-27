//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\io\BinaryReader.hpp
//! \brief Provides binary decoding for input streams
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BINARY_READER_HPP
#define WTL_BINARY_READER_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{ 
  
  //////////////////////////////////////////////////////////////////////////////////////////
	//! \struct BinaryReader - Decodes objects from an input stream stored as binary 
  //! 
  //! \tparam STREAM - Input stream type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  struct BinaryReader 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \typedef delegate_t - Defines signature of output overload
    typedef BinaryReader<STREAM>& (*delegate_t)(BinaryReader<STREAM>&);
    
    //! \typedef distance_t - Stream distance type
    typedef typename STREAM::distance_t distance_t;
    
    //! \typedef element_t - Stream element type
    typedef typename STREAM::element_t element_t;
    
    //! \typedef position_t - Stream position type
    typedef typename STREAM::position_t position_t;
    
    //! \typedef stream_t - Stream type
    typedef STREAM stream_t;
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
  
    /////////////////////////////////////////////////////////////////////////////////////////
    // BinaryReader::BinaryReader 
    //! Create a binary reader and initialise the input stream
    //! 
    //! \tparam ARGS... - Arguments parameter pack type
    //! 
    //! \param[in,out] &&... args - Stream constructor arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    explicit BinaryReader(ARGS&&... args) : Stream(std::forward<ARGS>(args)...)
    {}
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // BinaryReader::~BinaryReader
    //! Can be polymorphic
    //////////////////////////////////////////////////////////////////////////////////////////
    virtual ~BinaryReader()
    {}

    // Copy semantics determined by stream type
    ENABLE_COPY(BinaryReader);
	
	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  
    //////////////////////////////////////////////////////////////////////////////////////////
    // BinaryReader::read
    //! Reads an object from the input stream
    //! 
    //! \tparam T - Type of the object to read
    //! 
    //! \param[in,out] &obj - Object to read
    //! 
    //! \throw wtl::length_error - [Debug Mode] Insufficient buffer space remaining
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    void read(T& obj)
    {
      CHECKED_LENGTH(sizeof(T), Stream.remaining());

      // Read from stream directly into object
      Stream.read(reinterpret_cast<byte*>(&obj), sizeof(T));

      // Copy-reconstruct object and advance position
      /*static_alloc<T>::reconstruct(&obj, Stream.buffer());
      Position += size_of<T>::value;*/
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // BinaryReader::read
    //! Reads a boolean using a single byte from the input stream
    //! 
    //! \param[in,out] &b - Boolean to read
    //! 
    //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    template <>
    void read<bool>(bool& b)
    {
      CHECKED_LENGTH(1, Stream.remaining());

      // Decode from 1 or 0
      b = Stream.get() == static_cast<element_t>(0x01);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // BinaryReader::read
    //! Optimized read for all elements of statically allocated array 
    //! If remaining stream capacity is exceeded, the results are undefined.
    //! 
    //! \param[in] (&)[] str - Mutable reference to statically allocated array of elements
    //! 
    //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
    //! \throw wtl::logic_error - [Debug only] Stream has been closed
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    template <unsigned LENGTH>
    void read(element_t (&arr)[LENGTH])
    {
      CHECKED_LENGTH(LENGTH * sizeof(element_t), Stream.remaining());

      // Read directly into elements from stream
      Stream.read(arr);
    }
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    stream_t  Stream;     //!< Input stream
  };
  

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::deserialize_construct
  //! Copy-constructs an object from an input stream
  //! 
  //! \tparam VALUE - Value type
  //! \tparam STORAGE - Storage type
  //! \tparam STREAM - Input stream type
  //!
  //! \param[in,out] &r - Binary reader
  //! \return VALUE - Copy-constructed value
  //////////////////////////////////////////////////////////////////////////////////////////
  //template <typename VALUE, typename STORAGE = VALUE, typename STREAM>
  //VALUE  deserialize_construct(BinaryReader<STREAM>& r)
  //{
  //  //Array<uint8_t,size_of<STORAGE>::value,false> storage;
  //  uint8_t   storage[size_of<STORAGE>::value];                             //!< Byte storage buffer
  //  STORAGE (&element)[1] = reinterpret_cast<STORAGE (&)[1]>(storage);    //!< Strongly typed, single element, value array

  //  // Read into storage buffer
  //  memset(storage, 0, sizeof(storage));
  //  r.read(element[0]);

  //  // Copy-construct value
  //  return VALUE(element[0]);
  //}
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Reads any integral or floating-point type from a stream in binary
  //! 
  //! \tparam STREAM - Stream type
  //! \tparam U - Integral or floating-point type
  //!
  //! \param[in,out] &r - Text reader
  //! \param[in] val - Integral or floating point value 
  //! \return BinaryReader<STREAM>& - Reference to 'r'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename U>
  std::enable_if_t<std::is_integral<U>::value || std::is_floating_point<U>::value, BinaryReader<STREAM>&>
  /*BinaryReader<STREAM>&*/ operator >> (BinaryReader<STREAM>& r, U& val)
  {
    r.read(val);
    return r;
  }
  
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Reads any enumeration type from a stream in binary
  //! 
  //! \tparam STREAM - Stream type
  //! \tparam U - Enumeration type
  //!
  //! \param[in,out] &r - Text reader
  //! \param[in] val - Enumeration value 
  //! \return BinaryReader<STREAM>& - Reference to 'r'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename U>
  enable_if_enum_t<U,BinaryReader<STREAM>&> operator >> (BinaryReader<STREAM>& r, U& val)
  {
    r.read( reinterpret_cast<std::underlying_type_t<U>&>(val) );  // sizeof(underlying(U)) <= sizeof(U) 
    return r;
  }
  

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator >> 
  //! Appends elements encoded in binary from an input stream into a dynamic array.
  //! The array is not cleared before use.
  //!
  //! \tparam STREAM - Stream type
  //! \tparam E - Array element type
  //! \tparam L - Array length
  //!
  //! \param[in,out] &r - Binary reader
  //! \param[in,out] &arr - Mutable reference to a dynamic-array of any type
  //! \return BinaryReader<STREAM>& : Reference to reader 'r'
  //!
  //! \throw wtl::length_error - [Debug Only] Insufficient buffer space remaining
  //! \throw wtl::logic_error - [Debug only] Stream has been closed
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename E, unsigned L/*, typename SF = std::enable_if_t<std::is_pod<E>::value>*/>
  BinaryReader<STREAM>& operator >> (BinaryReader<STREAM>& r, Array<E,L,true>& arr) 
  {
    E value;
    
    // Read count
    uint32_t count;
    r >> count;

    // Copy-construct elements from an element constructed from the stream
    for (uint32_t i = 0UL; i < count; ++i) 
    {
      r >> value;
      arr.emplace_back(value);
    }

    return r;
  }
  
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator >> 
  //! Overwrites all elements in a fixed-array with elements decoded from a binary input stream
  //!
  //! \tparam STREAM - Stream type
  //! \tparam E - Array element type
  //! \tparam L - Array length
  //!
  //! \param[in,out] &r - Binary reader
  //! \param[in,out] &arr - Mutable reference to a fixed-array of any type
  //! \return BinaryReader<STREAM>& : Reference to reader 'r'
  //!
  //! \throw wtl::length_error - [Debug Only] Insufficient buffer space remaining
  //! \throw wtl::logic_error - [Debug only] Stream has been closed
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename E, unsigned L>
  BinaryReader<STREAM>& operator >> (BinaryReader<STREAM>& r, Array<E,L,false>& arr) 
  {
    enum { LENGTH = L };

    // Read/overwrite each element
    for (uint32_t i = 0UL; i < LENGTH; ++i) 
      r >> arr[i];

    return r;
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator >>
  //! Read a bitset from an input stream using its binary representation
  //!
  //! \tparam STREAM - Stream type
  //! \tparam DATA - Bitset data type
  //!
  //! \param[in,out] &r - Binary reader
  //! \param[in,out] &r - Immutable reference to a bitset of any type
  //! \return BinaryReader<STREAM>& : Reference to 'r'
  //!
  //! \throw wtl::length_error - [Debug Only] Insufficient buffer space remaining
  //! \throw wtl::logic_error - [Debug only] Stream has been closed
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename DATA>
  BinaryReader<STREAM>& operator >> (BinaryReader<STREAM>& r, Bitset<DATA>& bs) 
  {
    typename Bitset<DATA>::mask_t  mask;      //!< Bitset mask 

    // Read and set mask
    r >> mask;
    bs = mask;
    
    // Return
    return r;
  }



} //namespace wtl
#endif // WTL_BINARY_READER_HPP
