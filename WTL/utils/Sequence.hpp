//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Sequence.hpp
//! \brief Compile-time list containers
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SEQUENCE_HPP
#define WTL_SEQUENCE_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/Constant.hpp"         //!< 
#include <type_traits>

//! \namespace wtl - Windows template library
namespace wtl
{
  //
  // ------------------------------------------------------------------- INTEGRAL SEQUENCE ------------------------------------------------------------------
  //

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct integral_sequence - Provides a compile-time sequence of integral/enumeration literals
  //! 
  //! \tparam T - Element type
  //! \tparam VALUES - Sequence literals
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, T... VALUES>
  struct integral_sequence 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    // Require integral/enumeration type
    static_assert(std::is_integral<T>::value || std::is_enum<T>::value, "integral_sequence<T,T...> requires T to be an integral or enumeration type.");

    //! \alias type - Define own type
    using type = integral_sequence<T,VALUES...>;

    //! \alias value_type - Define value type
    using value_type = T;

    //! \var length - Length of sequence
    static constexpr int32 length = sizeof...(VALUES);

    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
    DEFAULT_CONSTEXPR(integral_sequence);         //!< Can be deep copied at compile-time
    DISABLE_COPY_ASSIGN(integral_sequence);       //!< Immutable
    DISABLE_MOVE_ASSIGN(integral_sequence);       //!< Immutable
    ENABLE_POLY(integral_sequence);               //!< Can be polymorphic
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias sequence_push_back_nt - Append a non-type sequence
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, T VALUE, T... VALUES>
  using sequence_push_back_nt = integral_sequence<T,VALUES...,VALUE>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias sequence_push_front_nt - Prepend a non-type sequence
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, T VALUE, T... VALUES>   
  using sequence_push_front_nt = integral_sequence<T,VALUE,VALUES...>;



  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::generate_sequence
  //! Generates a compile-time sequence of integral/enumeration literals
  //!
  //! \tparam[in] T - Element type
  //! \tparam[in] IDX - Number of elements to generate
  //! \tparam[in] VALUES - [optional] Value list, empty on first call
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, unsigned IDX, T... VALUES>
  struct generate_sequence : generate_sequence<T, IDX-1, IDX-1, VALUES...> 
  {};

  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::generate_sequence
  //! Base case
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, T... VALUES>
  struct generate_sequence<T, 0, VALUES...> 
  {
    //! \typedef type - Output
    using type = integral_sequence<T, VALUES...>;
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias generate_sequence_t - Generates a compile-time sequence of integral/enumeration literals
  //!
  //! \tparam[in] T - Element type
  //! \tparam[in] IDX - Number of elements to generate
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, unsigned IDX>
  using generate_sequence_t = typename generate_sequence<T,IDX>::type;
  

  

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct get - Integral/enum sequence element accessor
  //! 
  //! \tparam IDX - Zero-based element index
  //! \tparam T - Element type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <unsigned IDX, typename T>  
  struct get;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct get - Base-case: Return front() when IDX == 0
  //! 
  //! \tparam T - Element type
  //! \tparam VAL - First value
  //! \tparam SEQ - Remaining values
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, T VAL, T... SEQ>  
  struct get<0, integral_sequence<T, VAL, SEQ...>> : integral_constant<T, VAL> 
  {};

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct get - Recursive step: Pop front() while IDX > 0
  //! 
  //! \tparam IDX - Zero-based element index (but not zero)
  //! \tparam T - Element type
  //! \tparam VAL - First value
  //! \tparam SEQ - Remaining values
  /////////////////////////////////////////////////////////////////////////////////////////
  template <unsigned IDX, typename T, T VAL, T... SEQ>  
  struct get<IDX, integral_sequence<T, VAL, SEQ...>> : integral_constant<T,get<IDX-1, integral_sequence<T, SEQ...>>::value> 
  {};

  
  //
  // ------------------------------------------------------------------- SEQUENCE ------------------------------------------------------------------
  //

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct sequence - Provides a compile-time type-list
  //! 
  //! \tparam ...TYPES - Elements
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename... TYPES>
  struct sequence
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = sequence<TYPES...>;

    //! \var length - Define number of elements
    static constexpr int32 length = sizeof...(TYPES);

    /////////////////////////////////////////////////////////////////////////////////////////
    //! \alias index - Element accessor
    //!
    //! \tparam IDX - Zero-based element index
    //! \return auto - Element
    /////////////////////////////////////////////////////////////////////////////////////////
    template <unsigned IDX>
    using index = typename std::tuple_element<IDX, std::tuple<TYPES...>>::type;

    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
    DEFAULT_CONSTEXPR(sequence);         //!< Can be deep copied at compile-time
    DISABLE_COPY_ASSIGN(sequence);       //!< Immutable
    DISABLE_MOVE_ASSIGN(sequence);       //!< Immutable
    ENABLE_POLY(sequence);               //!< Can be polymorphic
	};

  /*template <typename... TYPES>
  using sequence_t = typename sequence<TYPES...>::type;*/

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias sequence_push_back_t - Append a type-sequence
  //! 
  //! \tparam T - Type to insert
  //! \tparam ... TYPES - [optional] Existing sequence
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename... TYPES>
  using sequence_push_back_t = sequence<TYPES..., T>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias sequence_push_front_t - Prepend a type-sequence
  //! 
  //! \tparam T - Type to insert
  //! \tparam ... TYPES - [optional] Existing sequence
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, typename... TYPES>
  using sequence_push_front_t = sequence<T, TYPES...>;


}



#endif // WTL_SEQUENCE_HPP


