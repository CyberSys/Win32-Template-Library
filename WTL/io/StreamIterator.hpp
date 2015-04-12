////////////////////////////////////////////////////////////////////////////////
//! \file wtl\io\StreamIterator.hpp
//! \brief Stream output iterator
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_STREAM_ITERATOR_HPP
#define WTL_STREAM_ITERATOR_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ////////////////////////////////////////////////////////////////////////////////
  //! \struct StreamIterator - Output stream iterator which overloads both assignment & function operators
  //! 
  //! \tparam STREAM - Stream type
  ////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  struct StreamIterator : std::_Outit
  {	
    // ------------------- TYPES & CONSTANTS -------------------
    
    //! \typedef stream_t - Stream type
	  typedef STREAM stream_t;

    // --------------------- CONSTRUCTION ----------------------
  
    ////////////////////////////////////////////////////////////////////////////////
    // StreamIterator::StreamIterator
    //! Create from output stream
    //!
    //! \param[in,out] &stream - Output stream
    ////////////////////////////////////////////////////////////////////////////////
	  StreamIterator(stream_t& stream) : Stream(stream)
	  {	      
	  }
    
    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------

    ////////////////////////////////////////////////////////////////////////////////
    // StreamIterator::operator=
    //! Write any type to the stream
    //! 
    //! \param[in] const& value - Value
    //! \return StreamIterator& - Reference to self
    ////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    StreamIterator& operator=(const T& value)
	  {	
		  Stream << value;
		  return *this;
	  }

    ////////////////////////////////////////////////////////////////////////////////
    // StreamIterator::operator()
    //! Write any type to the stream
    //! 
    //! \param[in] const& value - Value
    //! \return StreamIterator& - Reference to self
    ////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    StreamIterator& operator() (const T& value)
	  {	
      Stream << (value);
		  return *this;
	  }

    ////////////////////////////////////////////////////////////////////////////////
    // StreamIterator::operator*
    //! Mock dereference operator - No-op
    //! 
    //! \return StreamIterator& - Reference to self
    ////////////////////////////////////////////////////////////////////////////////
	  StreamIterator& operator*()
	  {	
		  return *this;
	  }

    ////////////////////////////////////////////////////////////////////////////////
    // StreamIterator::operator++
    //! Mock pre-increment operator - No-op
    //! 
    //! \return StreamIterator& - Reference to self
    ////////////////////////////////////////////////////////////////////////////////
	  StreamIterator& operator++()
	  {	
		  return *this;
	  }

    ////////////////////////////////////////////////////////////////////////////////
    // StreamIterator::operator++
    //! Mock post-increment operator - No-op
    //! 
    //! \param[in] - Ignored
    //! \return StreamIterator& - Reference to self
    ////////////////////////////////////////////////////////////////////////////////
	  StreamIterator& operator++(int)
	  {	
		  return *this;
	  }

    // -------------------- REPRESENTATION ---------------------
  protected:
	  stream_t&  Stream;	// Output stream
  };
  

}



#endif // WTL_STREAM_ITERATOR_HPP


