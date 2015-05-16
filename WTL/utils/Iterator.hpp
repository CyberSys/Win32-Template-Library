//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Iterator.hpp
//! \brief Provides specializations for non-member begin/end iterator accessors
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ITERATOR_HPP
#define WTL_ITERATOR_HPP

#include "wtl/WTL.hpp"
#include <iterator>                   //!< std::iterator

//! \namespace std - Namespace injection
namespace std
{
  
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



#endif // WTL_ITERATOR_HPP


