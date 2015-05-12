//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\commands\CutClipboardCommand.hpp
//! \brief Encapsulates the 'cut text' command
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CUT_CLIPBOARD_HPP
#define WTL_CUT_CLIPBOARD_HPP

#include "wtl/WTL.hpp"
#include "wtl/windows/Action.hpp"             //!< Action
#include "wtl/windows/WindowBase.hpp"         //!< WindowBase

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct CutClipboardCommand - Encapsulates 'Cutting' text from the control with input focus
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct CutClipboardCommand : Action<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias base - Define base type
    using base = Action<ENC>;

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias window_t - Define window base type
    using window_t = WindowBase<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding encoding = ENC;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    window_t*  TargetWnd;       //!< Destination window

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // CutClipboardCommand::CutClipboardCommand
    //! Create command
    /////////////////////////////////////////////////////////////////////////////////////////
    CutClipboardCommand()  
      : base(CommandId::Edit_Cut, [this] () { if (TargetWnd = window_t::getFocus())
                                                TargetWnd->setText(c_arr(L"TODO: Cut text to clipboard")); },
                                  [this] () { if (TargetWnd)
                                                TargetWnd->setText(c_arr(L"TODO: Restore previous text")); })
    {}
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CutClipboardCommand::clone const
    //! Create a new instance of the command
    //! 
    //! \return type* - New instance of command
    /////////////////////////////////////////////////////////////////////////////////////////
    typename base::type*  clone() const 
    {
      return new CutClipboardCommand(*this);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // CutClipboardCommand::state const
    //! Query the current state of the command 
    //! 
    //! \return ActionState - Current state of command
    /////////////////////////////////////////////////////////////////////////////////////////
    ActionState state() const override
    {
      // TODO: Query window text selection
      return ActionState::Enabled;
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
  };
  

}

#endif // WTL_CUT_CLIPBOARD_HPP
