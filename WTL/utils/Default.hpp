//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Default.hpp
//! \brief Defines the default value for any type
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_DEFAULT_HPP
#define WTL_DEFAULT_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/Constant.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct default_t - Defines the default value for any type
  //! 
  //! \tparam T - Any type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  struct default_t : integral_constant<T, static_cast<T>(0)>  { CONSTEXPR_CTOR(default_t); };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct default_t<bool> - Defines default bool as false
  /////////////////////////////////////////////////////////////////////////////////////////
  template <> 
  struct default_t<bool> : integral_constant<bool, false>     { CONSTEXPR_CTOR(default_t); };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct default_t<T*> - Defines default pointer as nullptr
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  struct default_t<T*> : integral_constant<T*, nullptr>       { CONSTEXPR_CTOR(default_t); };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct default_t<T&> - Disallows references 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  struct default_t<T&>;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct default_t<double> - Defines default floating points as zero
  /////////////////////////////////////////////////////////////////////////////////////////
  template <> struct default_t<double> { static constexpr double value = 0.0; CONSTEXPR_CTOR(default_t); };
  template <> struct default_t<float>  { static constexpr float value = 0.0f; CONSTEXPR_CTOR(default_t); };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::default constexpr
  //! Get the default-value for any type
  //! 
  //! \tparam T - Any type
  //!
  //! \return default_t<T> - Default value
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> constexpr
  T  default() noexcept
  {
    return default_t<T>()();
  }

  

  /////////////////////////////////////////////////////////////////////////////////////////
  //! wtl::default constexpr
  //! Get the default-value for any type
  //! 
  //! \tparam T - Any type
  //!
  //! \param[in] const& - Ignored
  //! \return default_t<T> - Default value
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> constexpr
  T  default(const T&) noexcept
  {
    return default_t<T>()();
  }
}



#endif // WTL_DEFAULT_HPP


