////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Zero.hpp
//! \brief Defines zero for any type
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ZERO_HPP
#define WTL_ZERO_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  // ------------------ ZERO -----------------

  /////////////////////////////////////////////////////////////
  //! \struct zero - Defines zero for any type
  //! 
  //! \tparam T - Any type
  /////////////////////////////////////////////////////////////
  //template <typename T> struct zero          {         /* Undefined */                  };
  template <typename T> struct zero          { static constexpr T value = static_cast<T>(0); };
  template <>           struct zero<uint64>  { static constexpr uint64 value = 0ULL;    };
  template <>           struct zero<int64>   { static constexpr int64 value = 0LL;      };
  template <>           struct zero<ulong32> { static constexpr ulong32 value = 0UL;    };
  template <>           struct zero<long32>  { static constexpr long32 value = 0L;      };
  template <>           struct zero<uint32>  { static constexpr uint32 value = 0UL;     };
  template <>           struct zero<int32>   { static constexpr int32 value = 0L;       };
  template <>           struct zero<uint16>  { static constexpr uint16 value = 0x0000U; };
  template <>           struct zero<int16>   { static constexpr int16 value = 0x0000;   };
  template <>           struct zero<uint8>   { static constexpr uint8 value = 0x00U;    };
  template <>           struct zero<int8>    { static constexpr int8 value = 0x00;      };
  template <>           struct zero<char>    { static constexpr char value = 0x00;      };

//! \if CONSTEXPR_CAP - Define here if compiler supports static storage
#ifdef CONSTEXPR_CAP
  template <>           struct zero<double>  { static constexpr double value = 0.0; };
  template <>           struct zero<float>   { static constexpr float value = 0.0f; };
#else
  template <>           struct zero<double>  { static const double value;  };
  template <>           struct zero<float>   { static const float value;   };
#endif

  /////////////////////////////////////////////////////////////
  //! \typedef zero_t - Zero type accessor 
  //! 
  //! \tparam T - Any type
  /////////////////////////////////////////////////////////////
  template <typename T>
  using zero_t = typename zero<std::remove_const<T>>;

}



#endif // WTL_ZERO_HPP


