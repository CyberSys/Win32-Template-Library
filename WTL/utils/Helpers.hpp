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
  
  //! \alias array_ptr_t - Defines array pointer types
  template <typename ELEMENT, uint32 LENGTH>
  using array_ptr_t = ELEMENT (*)[LENGTH];

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
  
  // ------------------ CHARACTER SELECTORS -----------------

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias getType - Defines the narrow/wide character declaration of a type
  //!
  //! \tparam[in] CHAR - Character type
  //! \tparam[in] NARROW - Narrow type
  //! \tparam[in] WIDE - Wide type
  //! \return - 'NARROW' if sizeof(CHAR) == 1, otherwise of type 'WIDE'
  ///////////////////////////////////////////////////////////////////////////////
  template <typename CHAR, typename NARROW, typename WIDE>
  using getType = std::conditional_t<sizeof(CHAR)==sizeof(char),NARROW,WIDE>;
  

  ///////////////////////////////////////////////////////////////////////////////
  //! wtl::getFunc
  //! Get the narrow character implementation of a function 
  //!
  //! \tparam CHAR - Narrow character type
  //! \tparam NARROW - Narrow char callable target type
  //! \tparam WIDE - Wide char callable target type
  //! 
  //! \param[in] narrow - Narrow char function pointer / callable target
  //! \param[in] wide - Wide char function pointer / callable target
  //! \return NARROW - Returns 'narrow'
  ///////////////////////////////////////////////////////////////////////////////
  template <typename CHAR, typename NARROW, typename WIDE>
  NARROW getFunc(NARROW narrow, WIDE wide, std::enable_if_t<sizeof(CHAR)==sizeof(char)>* = nullptr) 
  {
    return narrow;
  };

  
  ///////////////////////////////////////////////////////////////////////////////
  //! wtl::getFunc
  //! Get the wide character implementation of a function 
  //!
  //! \tparam CHAR - Wide character type
  //! \tparam NARROW - Narrow char callable target type
  //! \tparam WIDE - Wide char callable target type
  //! 
  //! \param[in] narrow - Narrow char function pointer / callable target
  //! \param[in] wide - Wide char function pointer / callable target
  //! \return WIDE - Returns 'wide'
  ///////////////////////////////////////////////////////////////////////////////
  template <typename CHAR, typename NARROW, typename WIDE>
  WIDE getFunc(NARROW narrow, WIDE wide, std::enable_if_t<sizeof(CHAR)==sizeof(int16)>* = nullptr) 
  {
    return wide;
  };

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

  // ------------------ NAME/VALUE PAIR -----------------

  ////////////////////////////////////////////////////////////////////////////////
  //! \struct NameValuePair - Provides an immutable name/value pair
  //! 
  //! \tparam VALUE - Value type
  ////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE>
  struct NameValuePair
  {
    /////////////////////////////////////////////////////////////
    // NameValuePair::NameValuePair 
    //! Create a name-value pair
    //! 
    //! \param[in] const* name - Name
    //! \param[in] const& value - Value
    /////////////////////////////////////////////////////////////
    NameValuePair(const char* name, const VALUE& value) : Name(name), Value(value)
    {}

    const char*   Name;     //!< Name
    const VALUE&  Value;    //!< Value reference
  };


  ////////////////////////////////////////////////////////////////////////////////
  // wtl::name_value_pair 
  //! Creates a name/value pair
  //! 
  //! \tparam VALUE - Value type
  //! 
  //! \param[in] const* name - Name
  //! \param[in] const& value - Value
  ////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE>
  NameValuePair<VALUE>  name_value_pair(const char* name, const VALUE& value)
  {
    return NameValuePair<VALUE>(name, value);
  }
  
  // ------------------ SFINAE -----------------

  /////////////////////////////////////////////////////////////
  //! \struct enable_if_enum_t - Provides a convenient SFINAE expression for specifying enumeration types
  /////////////////////////////////////////////////////////////
  template <typename E, typename... ARGS>
  struct enable_if_enum_t; // Undefined

  /////////////////////////////////////////////////////////////
  //! \struct enable_if_enum_t - Provides an SFINAE expression for a single enumeration type
  //! 
  //! \tparam E - Enumeration type
  /////////////////////////////////////////////////////////////
  template <typename E>
  struct enable_if_enum_t<E> : std::enable_if_t<std::is_enum<E>::value>
  {};

  /////////////////////////////////////////////////////////////
  //! \struct enable_if_enum_t - Provides an SFINAE expression for two enumeration types
  //! 
  //! \tparam E1 - Enumeration type
  //! \tparam E2 - Another enumeration type
  /////////////////////////////////////////////////////////////
  template <typename E1, typename E2>
  struct enable_if_enum_t<E1,E2> : std::enable_if_t<std::is_enum<E1>::value && std::is_enum<E2>::value>
  {};

  // ------------------ POWER OF -----------------

  /////////////////////////////////////////////////////////////
  //! \struct power_of - Compile-time power-of-N calculator
  //! 
  //! \tparam N - Base 
  //! \tparam EXP - Exponent
  /////////////////////////////////////////////////////////////
  template <int32 N, int32 EXP>
  struct power_of
  {
    static_assert(N > 0L, "Invalid base");
    static_assert(EXP > 0L, "Invalid exponent");

    // Sum between 0 and N: N(N-1)
    enum { value = N * power_of<N,EXP-1>::value };
  };

  /////////////////////////////////////////////////////////////
  //! \struct power_of<N,0> - Handle the base case
  /////////////////////////////////////////////////////////////
  template <int32 N>
  struct power_of<N, 0>
  {
    enum { value = 1L };
  };

  // ------------------ RELATIONAL -----------------

  template <template <typename> class BASE, 
            template <typename> class DERIVED, 
            typename T>
  struct is_covariant : std::is_base_of<BASE<T>, DERIVED<T>>
  {};
  
  // ------------------ SIZE OF -----------------
  
  /////////////////////////////////////////////////////////////
  //! \struct size_of - Defines separate size for any type
  //! 
  //! \tparam T - Any type
  /////////////////////////////////////////////////////////////
  
  /*template <typename T> struct size_of { enum { value = sizeof(T) }; };
  template <>           struct size_of< { enum { value = sizeof(T) }; };*/

  /////////////////////////////////////////////////////////////
  //! \typedef size_of_t - Accessor for separate size 
  /////////////////////////////////////////////////////////////
  /*template <typename T>
  using size_of_t = typename size_of<T>::value;*/
  
  /////////////////////////////////////////////////////////////
  // wtd::size_of 
  //! Get separate size for any time
  //! 
  //! \tparam T - Any type
  //! \return int32 - Size of type, in bytes
  /////////////////////////////////////////////////////////////
  template <typename T> 
  constexpr int32 size_of(const T&)
  {
    return sizeof(T);
  };
  
  /////////////////////////////////////////////////////////////
  // wtd::size_of 
  //! Get number of elements in a 1-dimensional statically allocated array
  //! 
  //! \tparam T - Element type
  //! \tparam L - Length of array
  //! \return int32 - Number of elements
  /////////////////////////////////////////////////////////////
  template <typename T, unsigned L> 
  constexpr int32 size_of(const T (&array)[L])
  {
    return L;
  };
  
  /////////////////////////////////////////////////////////////
  // wtd::size_of 
  //! Get number of elements in a 2-dimensional statically allocated array
  //! 
  //! \tparam T - Element type
  //! \tparam X - Rank of first dimension
  //! \tparam Y - Rank of second dimension
  //! \return int32 - Total number of elements in array
  /////////////////////////////////////////////////////////////
  template <typename T, unsigned X, unsigned Y> 
  constexpr int32 size_of(const T (&array)[X][Y])
  {
    return X * Y;
  };

  // ------------------ SEQUENCE -----------------
  
  /////////////////////////////////////////////////////////////
  //! \struct integral_sequence 
  //! Holds an constant expression integral sequence for use in meta-programming
  //! 
  //! \tparam T - Element type
  //! \tparam VALUES - Type list
  /////////////////////////////////////////////////////////////
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
  
  /////////////////////////////////////////////////////////////
  //! \alias push_front
  // Push a type to the front of the list
  /////////////////////////////////////////////////////////////
  template <typename T, T VALUE, T... VALUES>   // usage: probably push_front<..>::type
  using push_front = integral_sequence<T,VALUE,VALUES...>;

  /////////////////////////////////////////////////////////////
  // wtl::generate_sequence
  //! Sequence generating meta-function
  //!
  //! \tparam[in] T -Element type
  //! \tparam[in] N - Number of elements to generate
  //! \tparam[in] VALUES - Type-list, initially empty
  //! \treturn type - Sequence type
  /////////////////////////////////////////////////////////////
  template <int IDX, int... VALUES>
  struct generate_sequence : generate_sequence<IDX-1, IDX-1, VALUES...> 
  {};

  /////////////////////////////////////////////////////////////
  // wtl::generate_sequence
  //! Base case
  /////////////////////////////////////////////////////////////
  template <int... VALUES>
  struct generate_sequence<0, VALUES...> 
  {
    //! \typedef type - Output
    typedef integral_sequence<int, VALUES...>  type;
  };

  // ------------------ TUPLE ITERATION -----------------
  
  ////////////////////////////////////////////////////////////////////////////////
  // wtl::for_each<0> - Base case
  ////////////////////////////////////////////////////////////////////////////////
  template <unsigned IDX = 0U, typename FUNC, typename... TYPES>
  std::enable_if_t<IDX == sizeof...(TYPES)>   //std::enable_if_t<IDX == std::tuple_size<TYPES...>::value>
  for_each(const std::tuple<TYPES...>& t, FUNC fn) 
  { 
    /*no-op*/ 
  }


  ////////////////////////////////////////////////////////////////////////////////
  // wtl::for_each
  //! Executes a unary function upon each tuple element 
  //! 
  //! \tparam IDX - Zero-based iteration index
  //! \tparam FUNC - Function type
  //! \tparam TYPES - Tuple types
  //!
  //! \param[in] t& - Tuple to iterate over
  //! \param[in] fn - Function to execute
  ////////////////////////////////////////////////////////////////////////////////
  template <unsigned IDX = 0U, typename FUNC, typename... TYPES>
  std::enable_if_t<(IDX < sizeof...(TYPES))>
  for_each(const std::tuple<TYPES...>& t, FUNC fn)
  {
    // Execute functor
    fn(std::get<IDX>(t));
    
    // Unfurl next
    for_each<IDX+1U>(t, fn);
  };
  
}



#endif // WTL_HELPERS_HPP


