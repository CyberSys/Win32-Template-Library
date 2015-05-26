//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\io\StreamIterator.hpp
//! \brief Stream output iterator
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_STREAM_ITERATOR_HPP
#define WTL_STREAM_ITERATOR_HPP

#include "wtl/WTL.hpp"
#include <iterator>           //!< Iterator traits

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct StreamIterator - Output stream iterator which overloads both assignment & function operators
  //! 
  //! \tparam STREAM - Any type that overloads the output stream operator
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  struct StreamIterator : std::_Outit
  {	
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = StreamIterator<STREAM>;

    //! \alias base - Define base type
    using base = std::_Outit;

    //! \alias stream_t - Define output stream type
	  using stream_t = STREAM;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
	  stream_t&  Stream;	// Output stream

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // StreamIterator::StreamIterator
    //! Create from output stream
    //!
    //! \param[in,out] &stream - Output stream
    //////////////////////////////////////////////////////////////////////////////////////////
	  StreamIterator(stream_t& stream) : Stream(stream)
	  {	      
	  }
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    //////////////////////////////////////////////////////////////////////////////////////////
    // StreamIterator::operator=
    //! Write any type to the stream
    //! 
    //! \param[in] const& value - Value
    //! \return StreamIterator& - Reference to self
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    StreamIterator& operator=(const T& value)
	  {	
		  Stream << value;
		  return *this;
	  }

    //////////////////////////////////////////////////////////////////////////////////////////
    // StreamIterator::operator()
    //! Write any type to the stream
    //! 
    //! \param[in] const& value - Value
    //! \return StreamIterator& - Reference to self
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    StreamIterator& operator() (const T& value)
	  {	
      Stream << (value);
		  return *this;
	  }

    //////////////////////////////////////////////////////////////////////////////////////////
    // StreamIterator::operator*
    //! Mock dereference operator - No-op
    //! 
    //! \return StreamIterator& - Reference to self
    //////////////////////////////////////////////////////////////////////////////////////////
	  StreamIterator& operator*()
	  {	
		  return *this;
	  }

    //////////////////////////////////////////////////////////////////////////////////////////
    // StreamIterator::operator++
    //! Mock pre-increment operator - No-op
    //! 
    //! \return StreamIterator& - Reference to self
    //////////////////////////////////////////////////////////////////////////////////////////
	  StreamIterator& operator++()
	  {	
		  return *this;
	  }

    //////////////////////////////////////////////////////////////////////////////////////////
    // StreamIterator::operator++
    //! Mock post-increment operator - No-op
    //! 
    //! \param[in] - Ignored
    //! \return StreamIterator& - Reference to self
    //////////////////////////////////////////////////////////////////////////////////////////
	  StreamIterator& operator++(int)
	  {	
		  return *this;
	  }
  };
  
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::stream_iterator
  //! Creates a stream iterator
  //! 
  //! \tparam T- Stream type
  //! \param[in,out] &s - Input stream
  //! \return StreamIterator<T> - Stream iterator
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  StreamIterator<T> stream_iterator(T& s)
  {
    return StreamIterator<T>(s);
  }
}



#endif // WTL_STREAM_ITERATOR_HPP


