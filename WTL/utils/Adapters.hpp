//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Adapters.hpp
//! \brief Provides a variety of useful function objects and function object adapters
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ADAPTERS_HPP
#define WTL_ADAPTERS_HPP

#include "wtl/WTL.hpp"
#include <functional>               //!< std::unary_function

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct accumulate_t - Function object that sums each element
  //! \tparam ELEM - Type of the elements / Type of resultant sum
  //!                    *TYPE MUST IMPLEMENT += OPERATOR*
  //!
  //! \remarks Clients must pass this by reference since it maintains state
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename ELEM>
  struct accumulate_t : public std::unary_function<ELEM,ELEM>
  {
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    mutable ELEM Sum;     //!< Resultant sum

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // accumulate_t::accumulate_t
    //! Initialize the result
    //! 
    //! \param[in] seed - Accumulator seed value
    /////////////////////////////////////////////////////////////////////////////////////////
    accumulate_t(ELEM seed) : Sum(seed)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // accumulate_t::sum const
    //! Get the sum of all elements
    //! 
    //! \return ELEM - Resulant sum
    /////////////////////////////////////////////////////////////////////////////////////////
    ELEM sum() const
    {
      return Sum;
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // accumulate_t::operator()
    //! Sums the value of each element
    //! 
    //! \param[in,out] &d - Element 
    //! \return ELEM - Sum accumulated so far
    /////////////////////////////////////////////////////////////////////////////////////////
    ELEM operator()(const ELEM& d) const
    {
      return Sum += d;   
    }
  };

  //////////////////////////////////////////////////////////////////////////////////////////
  // ::accumulate
  //! Convenience function for creating accumulate function object
  //! 
  //! \tparam ELEM - Type of elements / Type of resultant sum
  //! \param[in] &seed - Accumulator seed value
  //! \return accumulate_t<ELEM> - Desired adapter
  //////////////////////////////////////////////////////////////////////////////////////////
  template <class ELEM>
  inline accumulate_t<ELEM> accumulate(const ELEM& seed) 
  {
    return accumulate_t<ELEM>(seed);
  }
  


  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct compose_f_gx_t - Composes one function object within another
  //! 
  //! \tparam FIRST - First operation applied
  //! \tparam SECOND - Second operation applied
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename FIRST, typename SECOND>
  struct compose_f_gx_t : public std::unary_function<typename FIRST::argument_type,
                                                     typename SECOND::result_type>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    FIRST  g;    //!< First operation function object representing g(x)
    SECOND f;    //!< Second operation function object representing f(x)

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // compose_f_gx_t::compose_f_gx_t
    //! Create composition adapter
    //! 
    //! \param[in] &gx - First operation applied
    //! \param[in] &fx - Second operation applied
    /////////////////////////////////////////////////////////////////////////////////////////
    compose_f_gx_t(const FIRST& gx, const SECOND& fx) : g(gx), f(fx)
    {
    }
	
	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // compose_f_gx_t::fx() const
    //! Get the f(x) function
    //! 
    //! \return SECOND - Second operation f(x)
    /////////////////////////////////////////////////////////////////////////////////////////
    SECOND fx() const
    {
      return f;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // compose_f_gx_t::gx() const
    //! Get the g(x) function
    //! 
    //! \return FIRST - First operation g(x)
    /////////////////////////////////////////////////////////////////////////////////////////
    FIRST gx() const
    {
      return g;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // compose_f_gx_t::operator() const
    //! Execute composition adapter
    //! 
    //! \param[in] &x - Input to first function
    //! \return SECOND::result_type - Result of second operation f(g(x))
    /////////////////////////////////////////////////////////////////////////////////////////
    typename SECOND::result_type operator() (const typename FIRST::argument_type& x) const 
    {
      return f( g(x) );
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };

  //////////////////////////////////////////////////////////////////////////////////////////
  // ::compose_f_gx
  //! Convenience function for creating compose_f_gx adapter
  //! 
  //! \tparam FIRST - Type of function g(x)
  //! \tparam SECOND - Type of function f(x)
  //! \param[in] &g - Function g(x)
  //! \param[in] &f - Function f(x)
  //! \return compose_f_gx_t<FIRST,SECOND> - Desired adapter
  //////////////////////////////////////////////////////////////////////////////////////////
  template <class FIRST, class SECOND>
  inline compose_f_gx_t<FIRST,SECOND> compose_f_gx(const FIRST& g, const SECOND& f) 
  {
    return compose_f_gx_t<FIRST,SECOND>(g,f);
  }






  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct compose_f_gx_hx_t - Composes two unary functions within a binary function : f(g(x), h(x))
  //! 
  //! \tparam OPERATION - Binary function to apply
  //! \tparam LEFT - Unary function, used as input for left-hand-side
  //! \tparam RIGHT - Unary function, used as input for right-hand-side
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename LEFT, typename OPERATION, typename RIGHT>
  struct compose_f_gx_hx_t : public std::unary_function<typename LEFT::argument_type,
                                                        typename OPERATION::result_type>
  {
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    OPERATION f;     //!< Function object representing f(g(x), h(x))
    LEFT      g;     //!< Function object representing g(x)
    RIGHT     h;     //!< Function object representing h(x)

    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // compose_f_gx_hx_t::compose_f_gx_hx_t
    //! Create composition adapter
    //! 
    //! \param[in] &gx - Left unary operation
    //! \param[in] &fx - Binary operation
    //! \param[in] &hx - Right unary operation
    /////////////////////////////////////////////////////////////////////////////////////////
    compose_f_gx_hx_t(const LEFT& gx, const OPERATION& fx, const RIGHT& hx) : f(fx), g(gx), h(hx)
    {
    }
	
	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // compose_f_gx_hx_t::fx() const
    //! Get the binary f(x,y) function
    //! 
    //! \return OPERATION - Binary Operation f(x,y)
    /////////////////////////////////////////////////////////////////////////////////////////
    OPERATION fx() const
    {
      return f;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // compose_f_gx_hx_t::gx() const
    //! Get the unary g(x) function
    //! 
    //! \return LEFT - Unary Operation g(x)
    /////////////////////////////////////////////////////////////////////////////////////////
    LEFT gx() const
    {
      return g;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // compose_f_gx_hx_t::hx() const
    //! Get the unary h(x) function
    //! 
    //! \return RIGHT - Unary Operation h(x)
    /////////////////////////////////////////////////////////////////////////////////////////
    RIGHT hx() const
    {
      return h;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // compose_f_gx_hx_t::operator() const
    //! Execute composition adapter
    //! 
    //! \param[in] &x - Operand
    //! \return OPERATION::result_type -  Result of operation f(g(x))
    /////////////////////////////////////////////////////////////////////////////////////////
    typename OPERATION::result_type operator() (const typename LEFT::argument_type& x) const 
    {
      return f( g(x), h(x) );
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

  //////////////////////////////////////////////////////////////////////////////////////////
  // ::compose_f_gx_hx
  //! Convenience function for creating compose_f_gx_hx adapter
  //! 
  //! \tparam OPERATION - Binary function 
  //! \tparam LEFT - Unary function, used as input for left-hand-side
  //! \tparam RIGHT - Unary function, used as input for right-hand-side
  //! \param[in] &g - Unary g(x)
  //! \param[in] &f - Binary f(x,y)
  //! \param[in] &h - Unary h(x)
  //! \return compose_f_gx_hx_t<OUTER,INNER> - Desired adapter
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename LEFT, typename OPERATION, typename RIGHT>
  inline compose_f_gx_hx_t<LEFT,OPERATION,RIGHT> compose_f_gx_hx(const LEFT& g, 
                                                                 const OPERATION& f, 
                                                                 const RIGHT& h) 
  {
    return compose_f_gx_hx_t<LEFT,OPERATION,RIGHT>(g,f,h);
  };





  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct execute_method_t - Executes a single parameter method upon an object
  //! 
  //! \tparam RESULT - Method result type
  //! \tparam TYPE - Object type
  //! \tparam PARAM - Parameter type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename RESULT, typename TYPE, typename PARAM>
  struct execute_method_t : public std::unary_function<TYPE, RESULT>
  {	
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  public:
    // \alias MethodPtr - Friendly name for a method pointer
    using MethodPtr = RESULT (TYPE::*)(PARAM) const; 
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    MethodPtr Method;         //!< Method pointer to execute
    PARAM     Argument;       //!< Argument to use as input

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // execute_method_t::execute_method_t
    //! Create method execution adapter
    //! 
    //! \param[in] m - Method pointer to execute
    //! \param[in] &p - Parameter to use as input to method
    /////////////////////////////////////////////////////////////////////////////////////////
    execute_method_t(MethodPtr m, const PARAM& p) : Method(m), Argument(p)
    {
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // execute_method_t::operator() const
    //! Create method execution adapter
    //! 
    //! \param[in] &obj - Method pointer to execute
    /////////////////////////////////////////////////////////////////////////////////////////
    RESULT operator()(const TYPE& obj) const
    {	
      return (obj.*Method)(Argument);
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  
  };

  //////////////////////////////////////////////////////////////////////////////////////////
  // ::execute_method
  //! Creates a function object adapter that invokes a method of an element
  //! 
  //! \tparam RESULT - Method result type
  //! \tparam TYPE - Object type
  //! \tparam PARAM - Parameter type
  //! \param[in] *m - Method pointer
  //! \param[in] &p - Parameter used as input to method
  //! \return execute_method_t<RESULT,TYPE,PARAM> - Adapter that invokes a method of an element
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename RESULT, typename TYPE, typename PARAM>
  inline execute_method_t<RESULT,TYPE,PARAM> execute_method(RESULT (TYPE::*m)(PARAM) const, PARAM& p)
  {
    return execute_method_t<RESULT,TYPE,PARAM>(m,p);
  };







  /////////////////////////////////////////////////////////////////////////////////////////
  // \struct execute_upon_t - Passes an element to a method of an object
  //! 
  //! \tparam RESULT - Method result type
  //! \tparam TYPE - Object type
  //! \tparam PARAM - Argument type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename RESULT, typename TYPE, typename PARAM>
  struct execute_upon_t : public std::unary_function<PARAM, RESULT>
  {
    //! \alias base - Define base type
    using base = std::unary_function<PARAM, RESULT>;

    //! \alias argument_type - Define argument type
    using argument_type = typename base::argument_type;

    //! \alias result_type - Define result type
    using result_type = typename base::result_type;

    //! \alias function_type - Define method type
    using function_type = result_type (TYPE::*)(argument_type);
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    function_type Method;     //!< method pointer
    TYPE&         Object;     //!< Object instance

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  public: 
    /////////////////////////////////////////////////////////////////////////////////////////
    // execute_upon_t::execute_upon_t
    //! Create adapter that passes an element to an object method
    //! 
    //! \param[in,out] &obj - Object instance
    //! \param[in] &m - Pointer to class method
    /////////////////////////////////////////////////////////////////////////////////////////
    execute_upon_t(TYPE& obj, function_type& m) : Method(m), 
                                                  Object(obj)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // execute_upon_t::operator()
    //! Invokes class method using element as parameter
    //! 
    //! \param[in] const &e - element to use as parameter
    /////////////////////////////////////////////////////////////////////////////////////////
    result_type operator()(const argument_type& e) const
    {
      return (Object.*Method)(e);   // Invoke method
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  
  };

  //////////////////////////////////////////////////////////////////////////////////////////
  // ::execute_upon
  //! Convenience function for creating execute_upon adapter
  //! 
  //! \tparam RESULT - Method result type
  //! \tparam TYPE - Object type
  //! \tparam PARAM - Argument type
  //! \param[in,out] &obj - Object instance
  //! \param[in] m - Pointer to class method
  //! \return execute_upon_t<RESULT,TYPE,PARAM> - Desired adapter
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename RESULT, typename TYPE, typename PARAM>
  inline execute_upon_t<RESULT,TYPE,PARAM> execute_upon(TYPE& obj, RESULT (TYPE::*m)(PARAM)) 
  {
    return execute_upon_t<RESULT,TYPE,PARAM>(obj, m);
  }




  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct if_then_t - Unary operation that executes a function upon operands
  //!                     that satisfy a predicate.
  //! 
  //! \tparam PRED - Unary predicate
  //! \tparam FUNC - Unary function
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename PRED, typename FUNC>
  struct if_then_t : public std::unary_function<typename PRED::argument_type, bool>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    PRED p;     //!< Predicate
    FUNC f;     //!< Operation

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // if_then_t::if_then_t
    //! Create composition adapter
    //! 
    //! \param[in] &px - Unary predicate
    //! \param[in] &fx - Unary function
    /////////////////////////////////////////////////////////////////////////////////////////
    if_then_t(const PRED& px, const FUNC& fx) : p(px), f(fx)
    {
    }
	
	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // if_then_t::px() const
    //! Get the predicate
    //! 
    //! \return PRED - Unary predicate p(x)
    /////////////////////////////////////////////////////////////////////////////////////////
    PRED px() const
    {
      return p;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // if_then_t::fx() const
    //! Get the operation
    //! 
    //! \return FUNC - Unary function f(x)
    /////////////////////////////////////////////////////////////////////////////////////////
    FUNC fx() const
    {
      return f;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // if_then_t::operator() const
    //! Execute predicate/function adapter
    //! 
    //! \param[in] &x - Parameter used as input to predicate and function
    //! \return bool - Whether function was applied to operand [ie. Whether predicate was satisfied]
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator() (const typename PRED::argument_type& x) const 
    {
      // Check predicate 
      if (!p(x))    
        return false;
      
      // Apply function
      f(x);   
      return true;
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // ::if_then
  //! Convenience function for creating if_then adapter
  //! 
  //! \tparam PRED - Type of predicate p(x)
  //! \tparam FUNC - Type of function f(x)
  //! \param[in] &p - Predicate p(x)
  //! \param[in] &f - Function f(x)
  //! \return if_then_t<PRED,FUNC> - Desired adapter
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename PRED, typename FUNC>
  inline if_then_t<PRED,FUNC> if_then(const PRED& p, const FUNC& f) 
  {
    return if_then_t<PRED,FUNC>(p,f);
  }


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct implicit_cast - Function object that casts an element to another type
  //! 
  //! \tparam INPUT - Type of the elements
  //! \tparam OUTPUT - Desired type of the elements
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename INPUT, typename OUTPUT>
  struct implicit_cast : public std::unary_function<INPUT,OUTPUT>
  {
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // implicit_cast::operator() const
    //! Returns the result of an implicit cast upon the element
    //! 
    //! \param[in,out] &d - Element 
    //! \return OUTPUT& - Result of implicit cast
    /////////////////////////////////////////////////////////////////////////////////////////
    OUTPUT operator()(const INPUT& d) const
    {
      return static_cast<OUTPUT>(d);
    }
  };
  

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct select_field_t 
  //! Unary function object adapater that returns the value of an accessible field of its input
  //! 
  //! \tparam STRUCT - Class/structure type
  //! \tparam FIELD - Field type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename STRUCT, typename FIELD>
  struct select_field_t : public std::unary_function<STRUCT, FIELD>
  {
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    const FIELD STRUCT::* FieldPtr;     //!< Class field pointer

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    //! select_field_t::select_field_t
    //! Create transformation selector that extracts a class/structure field
    //! 
    //! \param[in] *field - Pointer to class/structure field
    /////////////////////////////////////////////////////////////////////////////////////////
    select_field_t(const FIELD STRUCT::* field) 
      : FieldPtr(field)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  
    /////////////////////////////////////////////////////////////////////////////////////////
    //! select_field_t::operator() const
    //! Return field value
    //! 
    //! \param[in] const &s - Element 
    //! \return FIELD& - Immutable reference to field
    /////////////////////////////////////////////////////////////////////////////////////////
    const FIELD& operator()(const STRUCT& s) const 
    {
      return s.*FieldPtr;   // Dereference field pointer 
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    //! select_field_t::operator()
    //! Return field value
    //! 
    //! \param[in,out] &s - Element 
    //! \return FIELD& - Mutable reference to field
    /////////////////////////////////////////////////////////////////////////////////////////
    FIELD& operator()(STRUCT& s) 
    {
      return s.*FieldPtr;   // Dereference field pointer 
    }
  };

  //////////////////////////////////////////////////////////////////////////////////////////
  // ::select_field
  //! Convenience function for creating select_field adapter
  //! 
  //! \tparam STRUCT - Class/structure type
  //! \tparam FIELD - Field type
  //! \param[in] &field - Field pointer
  //! \return select_field_t<STRUCT,FIELD> - Desired adapter
  //////////////////////////////////////////////////////////////////////////////////////////
  template <class STRUCT, class FIELD>
  inline select_field_t<STRUCT,FIELD> select_field(const FIELD STRUCT::* field) 
  {
    return select_field_t<STRUCT,FIELD>(field);
  }




} //namespace wtl
#endif // WTL_ADAPTERS_HPP
