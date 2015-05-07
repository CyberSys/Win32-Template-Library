//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Bitset.hpp
//! \brief Simple implementation of a bitset
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BITSET_HPP
#define WTL_BITSET_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  /*template <typename STREAM>
  struct BinaryReader;*/

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Bitset - Simple implementation of a bitset
  //! 
  //! \tparam MASK - Underlying mask type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename MASK>
  struct Bitset 
  {
    //! \friend wtl::operator >> - Allow 
    /*template <typename STREAM, typename DATA> friend
    BinaryReader<STREAM>& operator >> (BinaryReader<STREAM>& r, Bitset<DATA>& bs);*/

    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  public:
    //! \typedef mask_t - Mask type
    typedef MASK mask_t;

    //! \enum - Defines bitset properties
    enum 
    {
      BYTES = sizeof(mask_t),   //!< Defines the number of bytes in the set
      BITS  = 8 * BYTES,        //!< Defines the number of bits in the set
    };

    //! \typedef BitArray - Array large enough to hold all bit indicies
    typedef Array<uint32,BITS,true> BitArray;

    //! \var ZERO - Define how we represent zero
    static const mask_t ZERO = default<mask_t>();
      
    //////////////////////////////////////////////////////////////////////////////////////////
    //! \struct loop - Unfurls the high bits of the mask into a variable length array
    //!
    //! \tparam IDX - Zero-based iteration index
    //////////////////////////////////////////////////////////////////////////////////////////
    template <int32 IDX>
    struct loop
    {
      static_assert((IDX >= 0) && (IDX < BITS), "Invalid loop index");

      //////////////////////////////////////////////////////////////////////////////////////////
      // Bitset::flatten
      //! Populates an array with the zero-based indicies of all high bits 
      //!
      //! \param[in] const &mask - Bitset mask to be flattened
      //! \param[in,out] &out - Array to populate. For efficiency this is not cleared before use.
      //////////////////////////////////////////////////////////////////////////////////////////
      static void flatten(const mask_t& mask, BitArray& out)
      {
        // Query mask for relevant bit: 2^(n-1)
        if (mask & static_cast<mask_t>(power_of<2,IDX>::value))
          out.emplace_back(IDX);

        // Recursive step
        loop<IDX+1>::flatten(mask,out);
      }
    };

    //////////////////////////////////////////////////////////////////////////////////////////
    //! \struct loop<BITS> - Base case 
    //////////////////////////////////////////////////////////////////////////////////////////
    template <>
    struct loop<BITS>
    {
      static void flatten(const mask_t& mask, BitArray& out)
      { /*no-op*/ }
    };

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::Bitset
    //! Initialize an empty/clear bitset 
    //////////////////////////////////////////////////////////////////////////////////////////
    Bitset() : Mask(ZERO)
    {}

    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::Bitset
    //! Initialize a bitset with an initial value
    //! 
    //! \param[in] m - Initial value for mask
    //////////////////////////////////////////////////////////////////////////////////////////
    explicit Bitset(mask_t m) : Mask(m)
    {}

    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::~Bitset
    //! Nothing
    //////////////////////////////////////////////////////////////////////////////////////////
    virtual ~Bitset()
    {}

    // ----------------------------------- STATIC METHODS -----------------------------------
  public:

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::get
    //! Query the state of a bit
    //!
    //! \param[in] index - Zero-based index of bit to query
    //! \return bool - State of desired bit
    //! 
    //! \throw wtl::out_of_range - [Debug only] Index out of range
    //////////////////////////////////////////////////////////////////////////////////////////
    bool get(uint32 index) const
    {
      CHECKED_INDEX(index, 0, BITS);

      // Query state of bit
      return (Mask & static_cast<mask_t>(1 << index)) != ZERO;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::empty
    //! Query whether mask is empty
    //!
    //! \return bool - True if empty, otherwise False
    //////////////////////////////////////////////////////////////////////////////////////////
    bool empty() const
    {
      return Mask == ZERO;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::flatten const
    //! Generates an array containing the zero-based indicies of any high bits 
    //!
    //! \param[in,out] &b - Array to populate with high-bit indicies
    //! \return BitArray : Reference to 'b'
    //! 
    //! \remarks The array is not cleared before it is populated
    //////////////////////////////////////////////////////////////////////////////////////////
    BitArray& flatten(BitArray& bits) const
    {
      // Populate array with indicies of high bits
      return loop<0>::flatten(Mask, bits), bits;
    };


    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::operator mask_t()
    //! Implicitly cast to desired underlying type
    //////////////////////////////////////////////////////////////////////////////////////////
    operator mask_t() const
    {
      return Mask;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::operator []
    //! Query the state of a bit
    //!
    //! \param[in] index - zero-based index of bit to query
    //! \return bool - State of desired bit
    //////////////////////////////////////////////////////////////////////////////////////////
    bool operator[](uint32 index) const
    {
      return get(index);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::operator &
    //! Bitwise AND operator upon a value
    //!
    //! \param[in] v - Operand
    //! \return mask_t - Returns result of 'mask bitwise-and value'
    //////////////////////////////////////////////////////////////////////////////////////////
    mask_t operator& (const mask_t& v) const
    {
      return Mask & v;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::operator &
    //! Bitwise AND operator upon another bitset
    //!
    //! \param[in] r - Another bitset
    //! \return mask_t - Returns result of 'mask bitwise-and value'
    //////////////////////////////////////////////////////////////////////////////////////////
    mask_t operator& (const Bitset& r) const
    {
      return Mask & r.Mask;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::operator |
    //! Bitwise OR operator upon a value
    //!
    //! \param[in] v - Operand
    //! \return mask_t - Returns result of 'mask bitwise-or value'
    //////////////////////////////////////////////////////////////////////////////////////////
    mask_t operator| (const mask_t& v) const
    {
      return Mask | v;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::operator |
    //! Bitwise OR operator upon another bitset
    //!
    //! \param[in] r - Another bitset
    //! \return mask_t - Returns result of 'mask bitwise-or value'
    //////////////////////////////////////////////////////////////////////////////////////////
    mask_t operator| (const Bitset& r) const
    {
      return Mask | r.Mask;
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::clear
    //! Clears the mask
    //////////////////////////////////////////////////////////////////////////////////////////
    void  clear()
    {
      Mask = ZERO;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::clear
    //! Clears the state of a bit
    //!
    //! \param[in] index - Zero-based bit index 
    //! 
    //! \throw wtl::out_of_range - [Debug only] Index out of range
    //////////////////////////////////////////////////////////////////////////////////////////
    void  clear(uint32 index)
    {
      CHECKED_INDEX(index, 0, BITS);

      // Clear bit
      Mask &= ~static_cast<mask_t>(1 << index);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::set
    //! Sets the state of a bit
    //!
    //! \param[in] index - Zero-based bit index 
    //! 
    //! \throw wtl::out_of_range - [Debug only] Index out of range
    //////////////////////////////////////////////////////////////////////////////////////////
    void  set(uint32 index)
    {
      CHECKED_INDEX(index, 0, BITS);
      
      // Set bit
      Mask |= static_cast<mask_t>(1 << index);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::operator =
    //! Overwrite the bitset mask 
    //!
    //! \param[in] m - Mask
    //! \return Bitset& - Reference to self containing new mask
    //////////////////////////////////////////////////////////////////////////////////////////
    Bitset& operator= (mask_t m) 
    {
      Mask = m;
      return *this;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::operator =
    //! Copies the mask from another Bitset of equal type
    //!
    //! \param[in] const& r - Another bitset (of equal type)
    //! \return Bitset& - Reference to self containing new mask
    //////////////////////////////////////////////////////////////////////////////////////////
    Bitset& operator= (const Bitset& r) 
    {
      if (this != &r)
        Mask = r.Mask;

      return *this;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::operator =
    //! Copies the mask from another Bitset of different type
    //!
    //! \tparam M - Foreign bitset mask type
    //! 
    //! \param[in] const& r - Another bitset (of different type)
    //! \return Bitset& - Reference to self containing new mask
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename M>
    Bitset& operator= (const Bitset<M>& r) 
    {
      // Perform implicit conversion upon foreign mask
      if (static_cast<void*>(this) != static_cast<const void*>(&r))
        Mask = static_cast<mask_t>(r.Mask);

      return *this;
    }

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    mask_t  Mask;   //!< Underlying representation
  };
  
  

} //namespace wtl


#endif // WTL_BITSET_HPP


