//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Range.hpp
//! \brief Utility templates for various ranges 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RANGE_HPP
#define WTL_RANGE_HPP

#include "wtl/WTL.hpp"
#include "wtl/io/Console.hpp"           //!< Console
#include "wtl/io/StreamIterator.hpp"    //!< StreamIterator

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::range
  //! Bounds a value to within a half open range
  //! 
  //! \tparam MIN - Minimum value type
  //! \tparam VAL - Value type
  //! \tparam MAX - Maximum value type
  //!
  //! \param[in] const& min - Minimum value (inclusive)
  //! \param[in] const& val - Value 
  //! \param[in] const& max - Maximum value (exclusive)
  //! \return VAL - min <= Input < max
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename MIN, typename VAL, typename MAX>
  VAL range(const MIN& min, const VAL& val, const MAX& max)
  {
    // Check value is within bounds
    return val <  min ? min 
         : val >= max ? max
                      : val;
  }



  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct delimited_range_t - Associates a delimiter character with an input range
  //!
  //! \tparam ITERATOR - Input iterator type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ITERATOR>
  struct delimited_range_t
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = delimited_range_t<ITERATOR>;
  
    //! \alias iterator_t - Define iterator type
    using iterator_t = ITERATOR;

    // ----------------------------------- REPRESENTATION -----------------------------------
  
    char        Delimiter;    //!< Delimiter character
    iterator_t  First,        //!< First element in range
                Last;         //!< Position after last element in range

    // ------------------------------------- CONSTRUCTION -----------------------------------
	
    //////////////////////////////////////////////////////////////////////////////////////////
    // delimited_range_t::delimited_range_t
    //! Create from start and finish position
    //!
    //! \tparam ITERATOR - Iterator type
    //!
    //! \param[in] const &first - First position in input range
    //! \param[in] const &last - Position immediately beyond input range
    //! \param[in] delimiter - Delimiter character
    //////////////////////////////////////////////////////////////////////////////////////////
    delimited_range_t(const iterator_t& first, const iterator_t& last, char delimeter) : First(first), 
                                                                                         Last(last), 
                                                                                         Delimiter(delimeter)
    {}
    
    /*delimited_range(char delimeter, std::intializer_list<typename ITERATOR::value_type>&& elements) 
      : First(elements.begin()), 
        Last(elements.end()), 
        Delimiter(delimeter)
    {}*/
    
	  // -------------------------------- COPY, MOVE & DESTROY  -------------------------------

    ENABLE_COPY(delimited_range_t);     //!< Can be copied
    ENABLE_MOVE(delimited_range_t);     //!< Can be moved
    ENABLE_POLY(delimited_range_t);     //!< Can be polymorphic
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };
  

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::delimited_range
  //! Create a delimited range 
  //!
  //! \tparam INPUT - Iterator type
  //!
  //! \param[in] first - First position in input range
  //! \param[in] last - Position immediately beyond last position in output range
  //! \param[in] delimiter - Delimiter character
  //! \return delimited_range_t<ITERATOR> - Delimited range
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ITERATOR>
  delimited_range_t<ITERATOR>  delimited_range(ITERATOR first, ITERATOR last, char delimiter)
  {
    return delimited_range_t<ITERATOR>(first, last, delimiter);
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::delimited_range
  //! Create a delimited range for an entire container
  //!
  //! \tparam CONTAINER - Container type
  //!
  //! \param[in] const& container - Container
  //! \param[in] delimiter - Delimiter character
  //! \return delimited_range_t<const_iterator> - Delimited range
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename CONTAINER>
  delimited_range_t<typename CONTAINER::const_iterator>  delimited_range(const CONTAINER& container, char delimiter) 
  {
    return delimited_range_t<typename CONTAINER::const_iterator>( container.begin(), container.end(), delimiter );
  }
  

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::delimit
  //! Writes elements to an output stream using a custom delimiter character
  //!
  //! \tparam INPUT - Input iterator type
  //! \tparam OUTPUT - Output iterator type
  //!
  //! \param[in] first - First position in input range
  //! \param[in] last - Position immediately beyond last position in output range
  //! \param[in] output - First position in output range
  //! \param[in] delimiter - Delimiter character
  //! \return OUTPUT - Position immediately beyond the last element written
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename OUTPUT, typename INPUT>
  OUTPUT  delimit(INPUT first, INPUT last, OUTPUT output, char delimiter)
  {
    // Cannot work upon empty range
    if (first != last)
    { 
      // Write first element
      *output = *first;
      ++output;

      // Write + delimit remaining N-1 elements
      for (auto it = first+1; it != last; ++it)
      {
        // Write delimiter
        *output = delimiter;
        ++output;

        // Write element
        *output = *it;
        ++output;
      }
    }

    return output;
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::delimit
  //! Write a delimited range to an output stream
  //!
  //! \tparam OUTPUT - Output iterator type
  //!
  //! \param[in,out] output - First position in output range
  //! \param[in] const& range - Delimited range
  //! \return OUTPUT - Position immediately beyond the last element written
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename OUTPUT>
  OUTPUT  delimit(OUTPUT output, const delimited_range_t<INPUT>& range)
  {
    return delimit(range.First, range.Last, output, range.Delimiter);
  }
  

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes a delimited range to the console
  //!
  //! \tparam ITERATOR - Iterator type
  //!
  //! \param[in,out] &c - Debug console
  //! \param[in] const& range - Delimited input range
  //! \return Console& : Reference to console 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ITERATOR>
  Console& operator << (Console& c, const delimited_range_t<ITERATOR>& range)
  {
    delimit(StreamIterator<Console>(c), range);
    return c;
  }


}

#endif // WTL_RANGE_HPP


