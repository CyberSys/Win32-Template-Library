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
#include <iterator>                   //!< std::iterator

//! \namespace wtl - Windows template library
namespace wtl
{
  // ------------------ BOUNDS CHECKING -----------------


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::range
  //! Ensures a value is within a specific range
  //! 
  //! \param[in] const &min - Minimum value (inclusive)
  //! \param[in] const &val - Value to query
  //! \param[in] const &max - Maximum value (inclusive)
  //! \return T - min <= Input <= max
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  T range(const T& min, const T& val, const T& max)
  {
    // Check value is within bounds
    return val < min ? min 
         : val > max ? max
                     : val;
  }


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::range
  //! Ensures a floating point value is within a specific range
  //! 
  //! \param[in] const &min - Minimum value (inclusive)
  //! \param[in] const &val - Value to query
  //! \param[in] const &max - Maximum value (inclusive)
  //! \param[in] const &tolerance - Tolerance
  //! \return T - min <= Input <= max       
  //////////////////////////////////////////////////////////////////////////////////////////
  inline float range(const float& min, const float& val, const float& max, const float tolerance = 1.0e-5)
  {
    const float t = tolerance*0.5f;

    // Check value is within tolerance of lower/upper bounds
    return val < (min-t) ? min 
         : val > (max+t) ? max
                         : val;
  }

  
  // ------------------ DELIMITED OUTPUT -----------------


  //////////////////////////////////////////////////////////////////////////////////////////
  // \struct delimited_range_t - Associates an input range and delimiter character
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ITERATOR>
  struct delimited_range_t
  {
    /*delimited_range(char delimeter, std::intializer_list<typename ITERATOR::value_type>&& elements) 
      : First(elements.begin()), 
        Last(elements.end()), 
        Delimiter(delimeter)
    {}*/
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // wtl::delimited_range
    //! Create from containing exposing 'begin()' and 'end()' methods
    //!
    //! \tparam CONTAINER - Container type
    //!
    //! \param[in] const &container - Container
    //! \param[in] delimiter - Delimiter character
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename CONTAINER>
    delimited_range_t(const CONTAINER& container, char delimeter) 
      : First(container.begin()), 
        Last(container.end()), 
        Delimiter(delimeter)
    {}
    
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
    delimited_range_t(const ITERATOR& first, const ITERATOR& last, char delimeter) 
      : First(first), 
        Last(last), 
        Delimiter(delimeter)
    {}

    char      Delimiter;    //!< Delimiter character
    ITERATOR  First,        //!< First element in range
              Last;         //!< Position after last element in range
  };
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::delimited_range
  //! Writing elements to an output stream using a custom delimiter character
  //!
  //! \tparam ITERATOR - Iterator type
  //!
  //! \param[in] const &a - Array
  //! \param[in] delimiter - Delimiter character
  //! \return delimited_range_t<ITERATOR> : Pair containing an array and delimiter character
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename OUTPUT, typename ITERATOR>
  OUTPUT&  delimit_range(OUTPUT& output, ITERATOR first, ITERATOR last, char delimiter)
  {
    // Cannot work upon empty range
    if (first != last)
    { 
      // Write first element
      output << *first;

      // Write + delimit remaining N-1 elements
      for (auto it = first+1; it != last; ++it)
      {
        output << delimiter;
        output << *it;
      }
    }

    return output;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::delimited_range
  //! Makes an delimited range for reading/writing elements to/from an stream 
  //!
  //! \tparam ITERATOR - Iterator type
  //!
  //! \param[in] const &a - Array
  //! \param[in] delimiter - Delimiter character
  //! \return delimited_range_t<ITERATOR> : Pair containing an array and delimiter character
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename OUTPUT, typename CONTAINER>
  OUTPUT&  delimit_range(OUTPUT& output, CONTAINER& container, char delimiter)
  {
    return delimit_range(output, container.begin(), container.end(), delimiter);
  }

}


//! \namespace std - Namespace injection
namespace std
{
  // -------------- ARRAY BOUNDS --------------

  //////////////////////////////////////////////////////////////////////////////////////////
  // std::begin
  //! Get the start of a statically allocated array of unspecified length
  //! 
  //! \tparam ELEMENT - Element type
  //!
  //! \param[in] r - Statically allocated array of unspecified length
  //! \return ELEMENT* - Position of first element
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ELEMENT> 
	ELEMENT* begin(ELEMENT (&r)[]) noexcept
	{	
	  return &r[0];
	}
  

  //////////////////////////////////////////////////////////////////////////////////////////
  // std::end
  //! Get the end of a statically allocated array of unspecified length
  //! 
  //! \tparam ELEMENT - Element type
  //!
  //! \param[in] r - Statically allocated array of unspecified length
  //! \return ELEMENT* - Position beyond final element
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ELEMENT> 
	ELEMENT* end(ELEMENT (&r)[]) noexcept
	{	
    // Return position beyond last element
	  return &r[sizeof(&r) / sizeof(ELEMENT)];
	}
}


#endif // WTL_RANGE_HPP


