////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Helpers.hpp
//! \brief Collection of helper templates 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_HELPERS_HPP
#define WTL_HELPERS_HPP

#include "wtl/WTL.hpp"


//! \namespace wtl - Windows template library
namespace wtl
{
  // -------------- ARRAY REFERENCE --------------

  //! \alias array_ref_t - Defines array reference types
  template <typename ELEMENT, uint32 LENGTH>
  using array_ref_t = ELEMENT (&)[LENGTH];
  

  // ------------------ COPY N -----------------


  ////////////////////////////////////////////////////////////////////////////////
  // wtl::copy_n
  //! Copies up to N values from one range to another
  //! 
  //! \tparam INPUT - Input iterator type
  //! \tparam OUTPUT - Output iterator type
  //! \param[in] input - First element in input range
  //! \param[in] n - Maximum number of elements to copy
  //! \param[in] output - First element in output range
  //! \return OUTPUT - Position in output range immediately following the last element copied
  ////////////////////////////////////////////////////////////////////////////////
  //template<typename INPUT, typename OUTPUT>
  //OUTPUT copy_n(INPUT input, int32 n, OUTPUT output)
  //{
  //  // Stop when zero
  //  if (n <= 0)
  //    return output;

  //  // Copy/decrement step
  //  *output = static_cast<OUTPUT::value_type>(*input);
  //  return copy_n(++input, n-1, ++output);
  //}
  
  // ------------------ LEAST EXPENSIVE CAST -----------------

  /////////////////////////////////////////////////////////////
  //! \struct auto_cast - Cast from any type A to any type B  (work in progress)
  /////////////////////////////////////////////////////////////
  template <typename BASE, typename DERIVED>
  DERIVED auto_cast(BASE b);


  template <typename DERIVED, typename BASE>
  std::enable_if_t<std::is_base_of<BASE,DERIVED>::value, DERIVED*>
  auto_cast(BASE* b)
  {
    return dynamic_cast<DERIVED*>(b);
  }

  template <typename TO, typename FROM>
  std::enable_if_t<std::is_convertible<FROM,TO>::value, TO&> 
  auto_cast(FROM& f)
  {
    return static_cast<TO&>(f);
  }

  template <typename BASE, typename DERIVED>
  std::enable_if_t<!std::is_base_of<BASE,DERIVED>::value
                && !std::is_convertible<BASE,DERIVED>::value,
                    DERIVED> 
  auto_cast(BASE& x)
  {
    return reinterpret_cast<DERIVED>(x);
  }


  // ------------------ RELATIONAL -----------------

  template <template <typename> class BASE, 
            template <typename> class DERIVED, 
            typename T>
  struct is_covariant : std::is_base_of<BASE<T>, DERIVED<T>>
  {};
  
}



#endif // WTL_HELPERS_HPP


