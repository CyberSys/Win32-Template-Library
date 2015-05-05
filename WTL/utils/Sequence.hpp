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

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct integral_sequence 
  //! Holds an constant expression integral sequence for use in meta-programming
  //! 
  //! \tparam T - Element type
  //! \tparam VALUES - Type list
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, T... VALUES>
  struct integral_sequence 
  {
    constexpr integral_sequence() : values{VALUES...}
    {}

    //! \typedef type - Expose type
    typedef integral_sequence<T,VALUES...> type;
    
    //! \var values - Sequence values
    const T values[];// { VALUES... };
  };

  // Push a type to the back of the type list
  template <typename T, T VALUE, T... VALUES>
  using push_back = integral_sequence<T,VALUES...,VALUE>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias push_front
  // Push a type to the front of the list
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T, T VALUE, T... VALUES>   // usage: probably push_front<..>::type
  using push_front = integral_sequence<T,VALUE,VALUES...>;

  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::generate_sequence
  //! Sequence generating meta-function
  //!
  //! \tparam[in] T -Element type
  //! \tparam[in] N - Number of elements to generate
  //! \tparam[in] VALUES - Type-list, initially empty
  //! \treturn type - Sequence type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <int IDX, int... VALUES>
  struct generate_sequence : generate_sequence<IDX-1, IDX-1, VALUES...> 
  {};

  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::generate_sequence
  //! Base case
  /////////////////////////////////////////////////////////////////////////////////////////
  template <int... VALUES>
  struct generate_sequence<0, VALUES...> 
  {
    //! \typedef type - Output
    typedef integral_sequence<int, VALUES...>  type;
  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct sequence - Simple type-list 
  //! 
  //! \tparam ...ELEM - Element types
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename... ELEM>
  struct sequence
  {
    //! \var count - Define number of elements
    static constexpr int32 count = sizeof...(ELEM);

    //! \var type - Define types
    using type = sequence<ELEM...>;
  };

  template <typename... ELEM>
  using sequence_t = typename sequence<ELEM...>::type;

}



#endif // WTL_SEQUENCE_HPP


