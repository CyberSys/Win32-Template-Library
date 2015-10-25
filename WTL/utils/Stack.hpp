//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Stack.hpp
//! \brief Provides a FILO stack with a simple interface 
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_STACK_HPP
#define WTL_STACK_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/Exception.hpp"          //!< Exceptions
#include <utility>                          //!< std::forward
#include <initializer_list>                 //!< std::initializer_list
#include <deque>                            //!< std::deque

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Stack - Simple FILO stack
  //! 
  //! \tparam T - Element type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct Stack 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = Stack<T>;
  
    //! \alias iterator - Inherit iterator type
    using iterator = typename std::deque<T>::iterator;

    //! \alias const_iterator - Inherit immutable iterator type
    using const_iterator = typename std::deque<T>::const_iterator;

    //! \alias value_type - Inherit value type
    using value_type = typename std::deque<T>::value_type;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    std::deque<T>  Items;      //!< Item storage

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Stack::Stack
    //! Create empty stack
    /////////////////////////////////////////////////////////////////////////////////////////
    Stack() = default;

    /////////////////////////////////////////////////////////////////////////////////////////
    // Stack::Stack
    //! Create stack and populate from initializer list
    //! 
    //! \param[in] list - List of elements
    /////////////////////////////////////////////////////////////////////////////////////////
    Stack(std::initializer_list<T> list) : Items(std::forward(list))
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    ENABLE_COPY(Stack);        //!< Can be copied
    ENABLE_MOVE(Stack);        //!< Can be moved
    ENABLE_POLY(Stack);        //!< Can be moved
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Stack::begin/end const
    //! Retrieve immutable start/end iterators
    //!
    //! \return const_iterator - Position of first/last elements
    /////////////////////////////////////////////////////////////////////////////////////////
    const_iterator begin() const  { return Items.cbegin(); }
    const_iterator end() const    { return Items.cend();   }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Stack::empty const
    //! Query whether the stack is empty
    /////////////////////////////////////////////////////////////////////////////////////////
    bool empty() const
    {
      return Items.empty();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Stack::peek const
    //! Peek the top element in the stack
    //! 
    //! \return const value_type& - Immutable reference to the top element
    //! 
    //! \throw wtl::logic_error - Stack is empty
    /////////////////////////////////////////////////////////////////////////////////////////
    const value_type&  peek() const
    {
      // Ensure not empty
      if (empty())
        throw logic_error(HERE, "Cannot peek an empty stack");

      // Return front
      return Items.front();
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Stack::begin/end 
    //! Retrieve mutable start/end iterators
    //!
    //! \return iterator - Position of first/last elements
    /////////////////////////////////////////////////////////////////////////////////////////
    iterator begin()  { return std::begin(Items); }
    iterator end()    { return std::end(Items);   }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Stack::clear
    //! Clear the stack
    /////////////////////////////////////////////////////////////////////////////////////////
    void clear()
    {
      Items.clear();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Stack::peek
    //! Peek the top element in the stack
    //! 
    //! \return value_type& - Mutable reference to top element
    //! 
    //! \throw wtl::logic_error - Stack is empty
    /////////////////////////////////////////////////////////////////////////////////////////
    value_type&  peek()
    {
      // Ensure not empty
      if (empty())
        throw logic_error(HERE, "Cannot peek an empty stack");

      // Return front
      return Items.front();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Stack::push
    //! Push element onto the stack
    //! 
    //! \param[in] &&... args - Element c-tor arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    void push(ARGS&&... args)
    {
      // Construct at back
      Items.emplace_back(std::forward<ARGS>(args)...);
    }
    
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Stack::pop
    //! Pop element from the stack
    //! 
    //! \throw wtl::logic_error - Stack is empty
    /////////////////////////////////////////////////////////////////////////////////////////
    void pop()
    {
      // Ensure not empty
      if (empty())
        throw logic_error(HERE, "Cannot pop empty stack");

      // Remove from front
      Items.pop_front();
    }
      
  };


}



#endif // WTL_STACK_HPP


