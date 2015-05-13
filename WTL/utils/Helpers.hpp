//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Helpers.hpp
//! \brief Collection of helper templates 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_HELPERS_HPP
#define WTL_HELPERS_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  // --------------------------------- AUTO-CAST ---------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct auto_cast - Cast from any type A to any type B  (work in progress)
  //!
  //! \tparam BASE - Base type
  //! \tparam DERIED - Deried type
  /////////////////////////////////////////////////////////////////////////////////////////
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


  // --------------------------------- CO-VARIANCE ---------------------------------

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct is_covariant - Query whether a type is co-variant
  //!
  //! \tparam BASE - Base template type
  //! \tparam DERIED - Deried template type
  //! \tparam T - Common enclosed type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <template <typename> typename BASE, 
            template <typename> typename DERIVED, 
            typename T>
  struct is_covariant : std::is_base_of<BASE<T>, DERIVED<T>>
  {};

  
  // --------------------------------- C RUNTIME LIBRARY ---------------------------------


  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::strlen_t
  //! Get length of narrow character string 
  //! 
  //! \param[in] const* str - Narrow character string
  //! \return size_t - Length in characters
  /////////////////////////////////////////////////////////////////////////////////////////
  inline size_t strlen_t(const char* str)
  {
    return strlen(str);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::strlen_t
  //! Get length of wide character string 
  //! 
  //! \param[in] const* str - Wide character string
  //! \return size_t - Length in characters
  /////////////////////////////////////////////////////////////////////////////////////////
  inline size_t strlen_t(const wchar_t* str)
  {
    return wcslen(str);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::strcmp_t
  //! Case sensitive narrow character comparison
  //! 
  //! \param[in] const* a - Narrow char string
  //! \param[in] const* b - Another narrow char string
  //! \return int32 - -1 if less, 0 if equal, 1 if greater
  /////////////////////////////////////////////////////////////////////////////////////////
  inline int32 strcmp_t(const char* a, const char* b)
  {
    return strcmp(a,b);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::strcmp_t
  //! Case sensitive wide character comparison
  //! 
  //! \param[in] const* a - Wide char string
  //! \param[in] const* b - Another wide char string
  //! \return int32 - -1 if less, 0 if equal, 1 if greater
  /////////////////////////////////////////////////////////////////////////////////////////
  inline int32 strcmp_t(const wchar_t* a, const wchar_t* b)
  {
    return wcscmp(a,b);
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::strcpy_t
  //! Copy narrow character string
  //! 
  //! \param[in] *dest - Narrow char string
  //! \param[in] const* src - Another narrow char string
  //! \return char* - Returns 'dest'
  /////////////////////////////////////////////////////////////////////////////////////////
  inline char* strcpy_t(char* dest, const char* src)
  {
    return strcpy(dest,src);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::strcpy_t
  //! Copy wide character string
  //! 
  //! \param[in] *dest - Wide char string
  //! \param[in] const* src - Another wide char string
  //! \return wchar_t* - Returns 'dest'
  /////////////////////////////////////////////////////////////////////////////////////////
  inline wchar_t* strcpy_t(wchar_t* dest, const wchar_t* src)
  {
    return wcscpy(dest,src);
  }
  


  
}


#endif // WTL_HELPERS_HPP


