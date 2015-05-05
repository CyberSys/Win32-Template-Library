//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Stack.hpp
//! \brief Provides a FILO stack that exposes a simple interface
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_STACK_HPP
#define WTL_STACK_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Stack - Simple FILO stack
  //! 
  //! \tparam T - Element type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct Stack : protected std::deque<T>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias base - Define base type
    using base = std::deque<T>;
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Stack::Stack
    //! Create empty stack
    /////////////////////////////////////////////////////////////////////////////////////////
    Stack()
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // Stack::Stack
    //! Create stack and populate from initializer list
    //! 
    //! \param[in] &&list - List of elements
    /////////////////////////////////////////////////////////////////////////////////////////
    Stack(std::initializer_list<T>&& list) : base(std::forward(list))
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // Stack::~Stack
    //! Virtual d-tor
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~Stack()
    {}
    
    // --------------------------------------- STATIC ---------------------------------------

    // -------------------------------------- ACCESSORS --------------------------------------			

    /////////////////////////////////////////////////////////////////////////////////////////
    // Stack::empty const
    //! Query whether the stack is empty
    /////////////////////////////////////////////////////////////////////////////////////////
    bool empty()
    {
      return base::empty();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Stack::peek const
    //! Peek the first element in the stack
    //! 
    //! \return const T& - Immutable reference to first element
    //! 
    //! \throw wtl::logic_error - Stack is empty
    /////////////////////////////////////////////////////////////////////////////////////////
    const T&  peek() const
    {
      // Ensure not empty
      if (empty())
        throw logic_error(HERE, "Stack is empty");

      // Return front
      return base::front();
    }

    // --------------------------------------- MUTATORS --------------------------------------

    using base::begin;
    using base::end;

    /////////////////////////////////////////////////////////////////////////////////////////
    // Stack::clear
    //! Clear the stack
    /////////////////////////////////////////////////////////////////////////////////////////
    void clear()
    {
      base::clear();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Stack::peek
    //! Peek the first element in the stack
    //! 
    //! \return T& - Mutable reference to first element
    //! 
    //! \throw wtl::logic_error - Stack is empty
    /////////////////////////////////////////////////////////////////////////////////////////
    T&  peek()
    {
      // Ensure not empty
      if (empty())
        throw logic_error(HERE, "Stack is empty");

      // Return front
      return base::front();
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
      base::emplace_front(std::forward<ARGS>(args)...);
    }
    
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Stack::pop
    //! Pop element from the stack
    //! 
    //! \return T - Top element in the stack
    //! 
    //! \throw wtl::logic_error - Stack is empty
    /////////////////////////////////////////////////////////////////////////////////////////
    T pop()
    {
      // Ensure not empty
      if (empty())
        throw logic_error(HERE, "Stack is empty");

      // Copy, pop, and return
      T tmp(base::front());
      base::pop_front();
      return tmp;
    }
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
  };


}



#endif // WTL_STACK_HPP


