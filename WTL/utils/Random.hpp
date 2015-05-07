//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Random.hpp
//! \brief Utility templates for random number generation
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RANDOM_HPP
#define WTL_RANDOM_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Random - Provides random numbers
  //////////////////////////////////////////////////////////////////////////////////////////
  struct Random
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  protected:
    //////////////////////////////////////////////////////////////////////////////////////////
    // Random::Random
    //! Create random number generator
    //////////////////////////////////////////////////////////////////////////////////////////
    Random() 
    {
      srand((int32)time(0));
    }

    // ----------------------------------- STATIC METHODS -----------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // Random::number
    //! Returns an random number from within a half-open range
    //! 
    //! \param[in] begin - Inclusive lower bound
    //! \param[in] end - Exclusive upper bound
    //! \return int32 - Random number between begin and end
    //////////////////////////////////////////////////////////////////////////////////////////
    static int32 number(int32 begin, int32 end)
    {
      return begin + (rand() % (end-begin));
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
  };
  

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::random_element
  //! Returns an element from an array at random
  //! 
  //! \param[in] array - Reference to statically allocated array
  //! \return E - Random element
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename E, unsigned L>
  const E  random_element(array_ref_t<const E,L> array)
  {
    return array[Random::number(0, L)];
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::random_element
  //! Returns an element from an array at random
  //! 
  //! \param[in] array - Reference to statically allocated array
  //! \return E - Random element
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename E, unsigned L>
  E  random_element(array_ref_t<E,L> array)
  {
    return array[Random::number(0, L)];
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::random_enum
  //! Returns a random enumeration literal
  //! 
  //! \return const ENUM - Random enumeration literal
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ENUM>
  ENUM random_enum()
  {
    //return enum_values<ENUM>::values[ Random::number(0,size_of(enum_values<ENUM>::values)) ];
    return random_element(enum_values<ENUM>::values);
    //return min_value<ENUM>::value;
  }

}



#endif // WTL_RANDOM_HPP


