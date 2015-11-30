//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Random.hpp
//! \brief Utility templates for random number generation
//! \date 26 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RANDOM_HPP
#define WTL_RANDOM_HPP

#include <wtl/WTL.hpp>
#include <ctime>            //!< ::time

//! \namespace wtl - Windows template library
namespace wtl
{
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Random - Provides random numbers
  //////////////////////////////////////////////////////////////////////////////////////////
  struct Random final
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------
  
    DISABLE_CTOR(Random);     //!< Cannot instantiate

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  
    DISABLE_COPY(Random);     //!< Cannot instantiate
    DISABLE_MOVE(Random);     //!< Cannot instantiate

    // ----------------------------------- STATIC METHODS -----------------------------------
  
    //////////////////////////////////////////////////////////////////////////////////////////
    // Random::number
    //! Returns an random number from within a half-open range
    //! 
    //! \param[in] begin - Inclusive lower bound
    //! \param[in] end - Exclusive upper bound
    //! \return int32_t - Random number between begin and end
    //////////////////////////////////////////////////////////////////////////////////////////
    static int32_t number(int32_t begin, int32_t end)
    {
      seed();

      return begin + (::rand() % (end-begin));
    }

  private:
    //////////////////////////////////////////////////////////////////////////////////////////
    // Random::seed
    //! Ensures RNG has been seeded
    //////////////////////////////////////////////////////////////////////////////////////////
    static void seed()
    {
      static bool initialized = false;

      // Seed RNG using current time unless already initialized
      if (!initialized)
      {
        ::srand(static_cast<int32_t>(::time(nullptr)));
        initialized = true;
      }
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };
  

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::random_element
  //! Returns an element from an array at random
  //! 
  //! \param[in] array - Reference to array of immutable elements
  //! \return E - Random element
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename E, unsigned L>
  const E  random_element(const E (&array)[L])
  {
    return array[Random::number(0, L)];
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::random_element
  //! Returns an element from an array at random
  //! 
  //! \param[in] array - Reference to array of mutable elements
  //! \return E - Random element
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename E, unsigned L>
  E  random_element(E (&array)[L])
  {
    return array[Random::number(0, L)];
  }


}



#endif // WTL_RANDOM_HPP


