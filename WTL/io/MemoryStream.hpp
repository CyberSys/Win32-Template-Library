/////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\io\MemoryStream.hpp
//! \brief Provides a custom stream buffer and custom stream I/O for memory buffer backed stream I/O
//! \date 28 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MEMORY_STREAM_HPP
#define WTL_MEMORY_STREAM_HPP

#include <string>                 //!< std::char_traits
#include <ios>                    //!< std::ios_base
#include <iostream>               //!< std::iostream

//////////////////////////////////////////////////////////////////////////////////////////
//! \struct memory_streambuf - Provides a memory stream buffer for standard I/O streams
//! 
//! \tparam CHAR - Character type
//! \tparam TRAITS - [optional] Type providing character traits
//////////////////////////////////////////////////////////////////////////////////////////
template <typename CHAR, typename TRAITS = std::char_traits<CHAR>>
struct memory_streambuf : std::basic_streambuf<CHAR,TRAITS>
{
  // ---------------------------------- TYPES & CONSTANTS ---------------------------------

  //! \alias type - Define own type
  using type = memory_streambuf<CHAR,TRAITS>;

  //! \alias base - Define base type
  using base = std::basic_streambuf<CHAR,TRAITS>;

  //! \alias char_type - Inherit character type
  using char_type = typename base::char_type;

  //! \alias int_type - Inherit integer representation type
  using int_type = typename base::int_type;
  
  //! \alias traits_type - Inherit traits type
  using traits_type = typename base::traits_type;

  // ----------------------------------- REPRESENTATION -----------------------------------
private:
  char_type  *Begin,      //!< Start of buffer
             *End;        //!< End of buffer

  // ------------------------------------ CONSTRUCTION ------------------------------------
public:
  //////////////////////////////////////////////////////////////////////////////////////////
  // memory_streambuf::memory_streambuf
  //! Construct from memory buffer
  //! 
  //! \param[in] *start - Position of first element
  //! \param[in] *end - Position immediately following last element
  //////////////////////////////////////////////////////////////////////////////////////////
  memory_streambuf(char_type* start, char_type* end) : Begin(start), End(end)
  {
    // Define 'put' area
    this->setp(Begin, End);   
    this->setg(Begin, Begin, End);
  }

  // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  
  memory_streambuf(const memory_streambuf&) = default;    //!< Copying produces a shallow copy  
  ~memory_streambuf() override = default;                 //!< Can be polymorphic

  // ----------------------------------- STATIC METHODS -----------------------------------

  // ---------------------------------- ACCESSOR METHODS ----------------------------------
private:  
  //////////////////////////////////////////////////////////////////////////////////////////
  // memory_streambuf::used const
  //! Query the number of characters written to the 'put' area
  //! 
  //! \return uint32_t - Number of characters in the 'put' area
  //////////////////////////////////////////////////////////////////////////////////////////
  uint32_t used() const
  {
    return static_cast<uint32_t>( this->pptr() - this->pbase() );
  }
    
  // ----------------------------------- MUTATOR METHODS ----------------------------------
protected:
  //////////////////////////////////////////////////////////////////////////////////////////
  // memory_streambuf::overflow
  //! Ordinarily would flush the buffer
  //! 
  //! \param[in] ch - Ignored
  //! \return int_type - Always returns EOF 
  //////////////////////////////////////////////////////////////////////////////////////////
  int_type overflow(int_type ch = traits_type::eof()) override
  {
    //! Destroy put area
    this->setp(nullptr, nullptr);

    // [FULL] Buffer is full
    return traits_type::eof();
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // memory_streambuf::underflow
  //! Ordinarily would repopulate the buffer
  //! 
  //! \return int_type - Always returns EOF 
  //////////////////////////////////////////////////////////////////////////////////////////
  int_type underflow() override
  {
    //! Destroy 'get' area
    this->setg(nullptr, nullptr, nullptr);

    // [EMPTY] Buffer is empty
    return traits_type::eof();
  }
};


//////////////////////////////////////////////////////////////////////////////////////////
//! \struct memory_stream - Provides an I/O stream whose backing store is a memory buffer
//! 
//! \tparam CHAR - Character type
//! \tparam TRAITS - [optional] Character traits provider type
//////////////////////////////////////////////////////////////////////////////////////////
template <typename CHAR, typename TRAITS = std::char_traits<CHAR>>
struct memory_stream : std::basic_iostream<CHAR,TRAITS>
{
  // ---------------------------------- TYPES & CONSTANTS ---------------------------------

  //! \alias type - Define own type
  using type = memory_stream<CHAR,TRAITS>;

  //! \alias base - Define base type
  using base = std::basic_iostream<CHAR,TRAITS>;
  
  //! \alias char_t - Define character type
  using char_t = CHAR;

  // ----------------------------------- REPRESENTATION -----------------------------------
private:
  memory_streambuf<CHAR,TRAITS>  Buffer;   //!< Stream buffer

  // ------------------------------------ CONSTRUCTION ------------------------------------
public:
  //////////////////////////////////////////////////////////////////////////////////////////
  // memory_stream::memory_stream
  //! Construct from memory buffer
  //! 
  //! \param[in] *start - Position of first element
  //! \param[in] *end - Position immediately following last element
  //////////////////////////////////////////////////////////////////////////////////////////
  memory_stream(char_t* begin, char_t* end) : base(&Buffer), Buffer(begin,end)
  { 
  }

  // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
	memory_stream(const memory_stream&) = default;			//!< Can be copied
	memory_stream(memory_stream&&) = default;			      //!< Can be moved
  ~memory_stream() override = default;    			      //!< Can be polymorphic
		
  // ----------------------------------- STATIC METHODS -----------------------------------
    
  // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
  // ----------------------------------- MUTATOR METHODS ----------------------------------
  
};

#endif // WTL_MEMORY_STREAM_HPP

