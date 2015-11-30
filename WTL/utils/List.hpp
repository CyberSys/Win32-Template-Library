//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\List.hpp
//! \brief Provides a linked list exposing a simple interface
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_LIST_HPP
#define WTL_LIST_HPP

#include <wtl/WTL.hpp>
#include <utility>                          //!< std::forward
#include <initializer_list>                 //!< std::initializer_list
#include <list>                             //!< std::list
#include <algorithm>                        //!< std::find_if
#include <functional>                       //!< std::function
#include <wtl/utils/exception.hpp>          //!< out_of_range

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct List - Bi-directional linked list supporting indexed element access
  //!
  //! \tparam T - Element type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct List 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = List<T>;

    //! \alias iterator - Inherit iterator type
    using iterator = typename std::list<T>::iterator;

    //! \alias const_iterator - Inherit immutable iterator type
    using const_iterator = typename std::list<T>::const_iterator;

    //! \alias value_type - Inherit value type
    using value_type = typename std::list<T>::value_type;

    //! \alias predicate_t - Unary element predicate type
    using predicate_t = bool (const value_type&);

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    std::list<T>   Items;       //!< List implementation

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // List::List
    //! Create empty list
    /////////////////////////////////////////////////////////////////////////////////////////
    List() = default;

    /////////////////////////////////////////////////////////////////////////////////////////
    // List::List
    //! Create list and populate from initializer list
    //!
    //! \param[in] &&list - List of elements
    /////////////////////////////////////////////////////////////////////////////////////////
    List(std::initializer_list<T>&& list) : Items(std::forward(list))
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    ENABLE_COPY(List);        //!< Can be copied
    ENABLE_MOVE(List);        //!< Can be moved
    ENABLE_POLY(List);        //!< Can be polymorphic
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // List::at const
    //! Finds an element by index
    //!
    //! \param[in] idx - Zero-based element index
    //! \return const_iterator - Immutable element position
    //!
    //! \throw wtl::out_of_range - [Debug only] Invalid index
    /////////////////////////////////////////////////////////////////////////////////////////
    const_iterator  at(uint32_t idx) const
    {
      CHECKED_INDEX(idx,0,Items.size()+1);    //!< Permit query of position beyond last element

      // Return 'end' if querying beyond last element
      if (idx == Items.size())
        return Items.cend();

      // Forward linear search
      auto pos = Items.cbegin();
      std::advance(pos, idx);
      return pos;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // List::begin/end const
    //! Retrieve start/end iterators
    //!
    //! \return const_iterator - Position of first/last elements
    /////////////////////////////////////////////////////////////////////////////////////////
    const_iterator begin() const  { return Items.cbegin(); }
    const_iterator end() const    { return Items.cend();   }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // List::find_if const
    //! Search entire containiner for an element matching predicate
    //!
    //! \param[in] pred - Element predicate
    //! \return const_iterator - Immutable element position if found, otherwise 'end'
    /////////////////////////////////////////////////////////////////////////////////////////
    const_iterator  find_if(std::function<predicate_t> pred) const
    {
      return std::find_if(Items.cbegin(), Items.cend(), pred);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // List::operator [] const
    //! Access element by index
    //!
    //! \param[in] idx - Zero-based element index
    //! \return const value_type& - Immutable element reference
    //!
    //! \throw wtl::out_of_range - [Debug only] Invalid index
    /////////////////////////////////////////////////////////////////////////////////////////
    const value_type&  operator [] (uint32_t idx) const
    {
      CHECKED_INDEX(idx,0,Items.size());

      // Lookup element
      return *at(idx);
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // List::at
    //! Finds an element by index
    //!
    //! \param[in] idx - Zero-based element index
    //! \return iterator - Mutable element position
    //!
    //! \throw wtl::out_of_range - [Debug only] Invalid index
    /////////////////////////////////////////////////////////////////////////////////////////
    iterator  at(uint32_t idx)
    {
      CHECKED_INDEX(idx,0,Items.size()+1);    //!< Permit query of position beyond last element

      // Return 'end' if querying beyond last element
      if (idx == Items.size())
        return Items.end();

      // Forward linear search
      auto pos = Items.begin();
      std::advance(pos, idx);
      return pos;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // List::begin/end 
    //! Retrieve start/end iterators
    //!
    //! \return iterator - Position of first/last elements
    /////////////////////////////////////////////////////////////////////////////////////////
    iterator begin()  { return std::begin(Items); }
    iterator end()    { return std::end(Items);   }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // List::emplace
    //! Contructs an element immediately preceding 'pos'
    //!
    //! \param[in] pos - Insert position
    //! \param[in] &&... args - Constructor arguments
    //! \return iterator - Position of emplaced element
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    iterator emplace(const_iterator pos, ARGS&&... args)
    {
      return Items.emplace(pos, std::forward<ARGS>(args)...);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // List::emplace_back
    //! Contructs an element at the end of the list
    //!
    //! \param[in] &&... args - Constructor arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    void emplace_back(ARGS&&... args)
    {
      return Items.emplace_back(std::forward<ARGS>(args)...);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // List::find_if
    //! Search entire containiner for an element matching predicate
    //!
    //! \param[in] pred - Element predicate
    //! \return const_iterator - Mutable element position if found, otherwise 'end'
    /////////////////////////////////////////////////////////////////////////////////////////
    iterator  find_if(std::function<predicate_t> pred)
    {
      return std::find_if(Items.begin(), Items.end(), pred);
    }
    
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // List::insert
    //! Inserts an element immediately preceding a position
    //!
    //! \param[in] pos - Position to insert
    //! \param[in] const& val - Element to insert
    /////////////////////////////////////////////////////////////////////////////////////////
    iterator insert(const_iterator pos, const value_type& val)   { return Items.insert(pos, val);               }
		iterator insert(const_iterator pos, value_type&& val)        { return Items.insert(pos, std::forward(val)); }

    /////////////////////////////////////////////////////////////////////////////////////////
    // List::insert
    //! Inserts copies of an element immediately preceding a position
    //!
    //! \param[in] pos - Position to insert
    //! \param[in] count - Number of insert
    //! \param[in] const& val - Element to insert
    /////////////////////////////////////////////////////////////////////////////////////////
	  iterator insert(const_iterator pos, uint32_t count, const value_type& val)
		{	
      return Items.insert(pos, count, val);
		}

    /////////////////////////////////////////////////////////////////////////////////////////
    // List::insert
    //! Insert a range of elements immediately preceding a position
    //!
    //! \param[in] pos - Position to insert
    //! \param[in] first - Position of first element
    //! \param[in] last - Position immediately following last element
    /////////////////////////////////////////////////////////////////////////////////////////
	  template <class ITERATOR>
		iterator insert(const_iterator pos, ITERATOR first, ITERATOR last)
		{	
      return Items.insert(pos, first, last);
		}

    /////////////////////////////////////////////////////////////////////////////////////////
    // List::insert
    //! Insert a list of elements immediately preceding a position
    //!
    //! \param[in] pos - Position to insert
    //! \param[in] values - Elements to insert
    /////////////////////////////////////////////////////////////////////////////////////////
	  iterator insert(const_iterator pos, std::initializer_list<value_type> values)
		{	
		  return Items.insert(pos, values);
		}

    /////////////////////////////////////////////////////////////////////////////////////////
    // List::push_back
    //! Appends an element at the end of the list
    //!
    //! \param[in] const& e - Element to append
    /////////////////////////////////////////////////////////////////////////////////////////
    void push_back(value_type&& e)        { return Items.push_back(e); }
    void push_back(const value_type& e)   { return Items.push_back(e); }

    /////////////////////////////////////////////////////////////////////////////////////////
    // List::remove_if
    //! Removes elements mated by a predicate
    //!
    //! \param[in] pred - Element predicate
    /////////////////////////////////////////////////////////////////////////////////////////
    void remove_if(std::function<predicate_t> pred)
    {
      return Items.remove_if(pred);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // List::operator []
    //! Access element by index
    //!
    //! \param[in] idx - Zero-based element index
    //! \return value_type& - Mutable element reference
    //!
    //! \throw wtl::out_of_range - [Debug only] Invalid index
    /////////////////////////////////////////////////////////////////////////////////////////
    value_type&  operator [] (uint32_t idx)
    {
      CHECKED_INDEX(idx,0,Items.size());

      // Lookup element
      return *at(idx);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // List::operator +=
    //! Append an element to the list
    //!
    //! \param[in] const& value - Item
    /////////////////////////////////////////////////////////////////////////////////////////
    List& operator += (const value_type& value)  { push_back(value);               return *this; }
    List& operator += (value_type&& value)       { push_back(std::forward(value)); return *this; }
  };


}



#endif // WTL_LIST_HPP


