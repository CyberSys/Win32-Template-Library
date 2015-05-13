//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\io\BinaryWriter.hpp
//! \brief Provides binary encoding for output streams
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BINARY_WRITER_HPP
#define WTL_BINARY_WRITER_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //////////////////////////////////////////////////////////////////////////////////////////
	//! \struct BinaryWriter - Writes objects into an output stream in binary
  //! 
  //! \tparam STREAM - Output stream type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  struct BinaryWriter 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \typedef delegate_t - Defines signature of output overload
    typedef BinaryWriter<STREAM>& (*delegate_t)(BinaryWriter<STREAM>&);
    
    //! \typedef distance_t - Stream distance type
    typedef typename STREAM::distance_t distance_t;
    
    //! \typedef element_t - Stream element type
    typedef typename STREAM::element_t element_t;
    
    //! \typedef position_t - Stream position type
    typedef typename STREAM::position_t position_t;
    
    //! \typedef stream_t - Stream type
    typedef STREAM stream_t;
    
    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  
    /////////////////////////////////////////////////////////////////////////////////////////
    // BinaryWriter::BinaryWriter 
    //! Create a binary writer and initialise the output stream
    //! 
    //! \tparam ARGS... - Arguments parameter pack type
    //! 
    //! \param[in,out] &&... args - Stream constructor arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    explicit BinaryWriter(ARGS&&... args) : Stream(std::forward<ARGS>(args)...)
    {}
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // BinaryWriter::~BinaryWriter
    //! Can be polymorphic
    //////////////////////////////////////////////////////////////////////////////////////////
    virtual ~BinaryWriter()
    {}

    
    ENABLE_COPY(BinaryWriter);   //!< Copy semantics determined by stream type
	
	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // TextWriter::remaining() const
    //! Get the number of available elements remaining in the stream 
    //! 
    //! \return distance_t - Number of unused elements remaining in the stream buffer 
    //! 
    //! \throw wtl::logic_error - [Debug only] Stream has been closed
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    distance_t remaining() const
    {
      return Stream.remaining();
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  
    //////////////////////////////////////////////////////////////////////////////////////////
    // BinaryWriter::write
    //! Writes any object to the output stream
    //! 
    //! \tparam T - Object type
    //! 
    //! \param[in] const &obj - Immutable reference to object 
    //! 
    //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
    //! \throw wtl::logic_error - [Debug only] Stream has been closed
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    void  write(const T& obj)
    {
      CHECKED_LENGTH(sizeof(T), remaining());

      // Write directly into stream 
      Stream.write(reinterpret_cast<const byte*>(&obj), sizeof(T));
    }
    
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // BinaryWriter::write
    //! Optimized write for all elements of statically allocated array 
    //! If remaining stream capacity is exceeded, the results are undefined.
    //! 
    //! \param[in] const (&)[] str - Statically allocated array of elements
    //! 
    //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
    //! \throw wtl::logic_error - [Debug only] Stream has been closed
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    template <unsigned LENGTH>
    void write(const element_t (&arr)[LENGTH])
    {
      CHECKED_LENGTH(LENGTH * sizeof(element_t), remaining());

      // Write elements to stream
      Stream.write(arr);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // BinaryWriter::write
    //! Writes a boolean to the output stream as a single element containing 0 or 1
    //! 
    //! \param[in] const &b - Boolean flag
    //! 
    //! \throw wtl::length_error - [Debug Only] Insufficient buffer space remaining
    //! \throw wtl::logic_error - [Debug only] Stream has been closed
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    void write(bool b)
    {
      CHECKED_LENGTH(1UL, remaining()); 

      // Write as single element
      Stream.put(b ? 1 : 0);
    }

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    stream_t   Stream;     //!< Output stream
  };
  
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator<<  
  //! Writes any type to an output stream using its binary representation
  //! 
  //! \tparam STREAM - Stream type
  //! \tparam VALUE - Value type
  //!
  //! \param[in,out] &w - Binary writer
  //! \param[in] const &val - Value 
  //! \return BinaryWriter<STREAM>& - Reference to writer 'w'
  //!
  //! \throw wtl::length_error - [Debug Only] Insufficient buffer space remaining
  //! \throw wtl::logic_error - [Debug only] Stream has been closed
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  /*template <typename STREAM, typename VALUE>
  BinaryWriter<STREAM>& operator << (BinaryWriter<STREAM>& w, const VALUE& val)
  { 
    CHECKED_LENGTH(size_of<VALUE>::value, w.remaining());

    w.write(val);
    return w;
  }*/
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes any integral or floating point type to a stream in binary
  //! 
  //! \tparam STREAM - Stream type
  //! \tparam U - Integral or floating-point type
  //!
  //! \param[in,out] &w - Binary writer
  //! \param[in] val - Integral or floating point value 
  //! \return BinaryWriter<STREAM>& - Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename U>
  std::enable_if_t<std::is_integral<U>::value 
                || std::is_floating_point<U>::value, BinaryWriter<STREAM>&>
  /*BinaryWriter<STREAM>&*/ operator << (BinaryWriter<STREAM>& w, U val)
  {
    CHECKED_LENGTH(size_of<U>::value, w.remaining());

    w.write(val);
    return w;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes the numeric representation of any enumeration type to a stream in binary
  //! 
  //! \tparam STREAM - Stream type
  //! \tparam U - Enumeration type
  //!
  //! \param[in,out] &w - Binary writer
  //! \param[in] val - Enumeration value
  //! \return BinaryWriter<STREAM>& - Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename U>
  enable_if_enum_t<U,BinaryWriter<STREAM>&> operator << (BinaryWriter<STREAM>& w, U val)
  {
    // Write as underlying type
    w.write(static_cast<std::underlying_type_t<U>>(val));
    return w;
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator<< 
  //! Writes a boolean to an output stream as a byte containing 1 or 0
  //! 
  //! \tparam STREAM - Stream type
  //!
  //! \param[in,out] &w - Binary writer
  //! \param[in] val - Boolean 
  //! \return BinaryWriter<STREAM>& - Reference to writer 'w'
  //!
  //! \throw wtl::length_error - [Debug Only] Insufficient buffer space remaining
  //! \throw wtl::logic_error - [Debug only] Stream has been closed
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  BinaryWriter<STREAM>& operator<< (BinaryWriter<STREAM>& w, bool val)
  { 
    w.write(static_cast<byte>(val ? 0x01 : 0x00));
    return w;
  }  

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes dynamic-array elements to an output stream using their binary representation
  //!
  //! \tparam STREAM - Stream type
  //! \tparam E - Array element type
  //! \tparam L - Array length
  //!
  //! \param[in,out] &w - Binary writer
  //! \param[in] const &r - Immutable reference to a dynamic-array of any type
  //! \return BinaryWriter<STREAM>& : Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug Only] Insufficient buffer space remaining
  //! \throw wtl::logic_error - [Debug only] Stream has been closed
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename E, unsigned L>
  BinaryWriter<STREAM>& operator << (BinaryWriter<STREAM>& w, const Array<E,L,true>& r) 
  {
    // Write Count
    w << r.size();

    // Write 'Count' elements
    for (uint32 i = 0UL; i < r.size(); ++i) 
      w << r[i];

    return w;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes fixed-array elements to an output stream using their binary representation
  //!
  //! \tparam STREAM - Stream type
  //! \tparam E - Array element type
  //! \tparam L - Array length
  //!
  //! \param[in,out] &w - Binary writer
  //! \param[in] const &r - Immutable reference to a fixed-array of any type
  //! \return BinaryWriter<STREAM>& : Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug Only] Insufficient buffer space remaining
  //! \throw wtl::logic_error - [Debug only] Stream has been closed
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename E, unsigned L>
  BinaryWriter<STREAM>& operator << (BinaryWriter<STREAM>& w, const Array<E,L,false>& r) 
  {
    enum { LENGTH = L };

    // Write all elements
    for (uint32 i = 0UL; i < LENGTH; ++i) 
      w << r[i];

    return w;
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Write any bitset to an output stream using its binary representation
  //!
  //! \tparam STREAM - Stream type
  //! \tparam DATA - Bitset data type
  //!
  //! \param[in,out] &w - Binary writer
  //! \param[in] const &r - Immutable reference to a bitset of any type
  //! \return BinaryWriter<STREAM>& : Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug Only] Insufficient buffer space remaining
  //! \throw wtl::logic_error - [Debug only] Stream has been closed
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename DATA>
  BinaryWriter<STREAM>& operator << (BinaryWriter<STREAM>& w, const Bitset<DATA>& r) 
  {
    // Write mask
    return w << r.Mask;
  }



} //namespace wtl
#endif // WTL_BINARY_WRITER_HPP
