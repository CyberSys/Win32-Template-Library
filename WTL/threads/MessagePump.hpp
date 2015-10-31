//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\threads\MessagePump.hpp
//! \brief Provides a customizeable message pump
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MESSAGE_PUMP_HPP
#define WTL_MESSAGE_PUMP_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EncodingTraits.hpp"            //!< Encoding
#include "wtl/platform/WindowMessage.hpp"           //!< WindowMessage
#include "wtl/platform/ResourceId.hpp"              //!< ResourceId
#include "wtl/platform/WindowFlags.hpp"             //!< ShowWindowFlags
#include "wtl/windows/MessageBox.hpp"               //!< MessageBox
//#include "wtl/windows/WindowBase.hpp"               //!< WindowBase
#include "wtl/io/Console.hpp"                       //!< Console
//#include "wtl/utils/ExceptionLog.hpp"               //!< exception_log
#include <stdexcept>                                //!< std::exception

//! \namespace wtl - Windows template library
namespace wtl
{
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct MessagePump - Provides a message pump for, and encapsulates within, a top-level window
  //! 
  //! \tparam WINDOW - Window type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename WINDOW>
  struct MessagePump 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \var encoding - Define character encoding
    static constexpr Encoding encoding = WINDOW::encoding;
    
    //! \alias type - Define own type
    using type = MessagePump<WINDOW>;
  
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<encoding>;

    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<encoding>;
    
    //! \alias window_t - Define window type
    using window_t = WINDOW;

    //! \enum PumpState - Define message pump states
    enum class PumpState
    {
      Idle,           //!< Pump idle
      Running,        //!< Pumping messages normally
      ModalLoop,      //!< Pumping within modal menu/dialog/msgbox loop
      //MenuModal,      //!< Pumping within modal menu loop
      //DialogModal,    //!< Pumping within modal dialog loop
      //MsgBoxModal,    //!< Pumping within modal msgbox loop
    };
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    HINSTANCE         Instance;   //!< Module instance
    List<window_t*>   Dialogs;    //!< Currently active modeless dialogs
    window_t          Window;     //!< Main thread window
    PumpState         State;      //!< Current state
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // MessagePump::MessagePump
    //! Creates a message pump 
    //! 
    //! \param[in] instance - Instance handle
    /////////////////////////////////////////////////////////////////////////////////////////
    MessagePump(::HMODULE instance) : Instance(instance),
                                      State(PumpState::Idle),
                                      Window(instance)
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    DISABLE_COPY(MessagePump);      //!< Cannot be copied
    ENABLE_MOVE(MessagePump);       //!< Can be moved
    ENABLE_POLY(MessagePump);       //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // MessagePump::addDialog
    //! Informs the pump a dialog has been created
    //! 
    //! \return bool - True iff unique
    /////////////////////////////////////////////////////////////////////////////////////////
    bool  addDialog(window_t* dlg)
    {
      Dialogs.push_back(dlg);
      return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // MessagePump::removeDialog
    //! Informs the pump a dialog has been closed
    //! 
    //! \return bool - True iff found and removed
    /////////////////////////////////////////////////////////////////////////////////////////
    bool  removeDialog(window_t* dlg)
    {
      Dialogs.erase( std::find(Dialogs.begin(), Dialogs.end(), dlg) );
      return true;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // MessagePump::run
    //! Executes the message pump
    //! 
    //! \param[in] mode - Initial display mode
    //! \return int32_t - Return code
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual int32_t  run(ShowWindowFlags mode = ShowWindowFlags::ShowNormal)
    {
      //! \var dispatchMessage - Functor for 'DispatchMessage'
      static const auto dispatchMessage = choose<encoding>(::DispatchMessageA,::DispatchMessageW);

      //! \var getMessage - Functor for 'GetMessage'
      static const auto getMessage = choose<encoding>(::GetMessageA,::GetMessageW);

      //! \var isDialogMessage - Functor for 'isDialogMessage'
      static const auto isDialogMessage = choose<encoding>(::IsDialogMessageA,::IsDialogMessageW);

      //! \var translateAccelerator - Functor for 'TranslateAccelerator'
      static const auto translateAccelerator = choose<encoding>(::TranslateAcceleratorA,::TranslateAcceleratorW);

      MSG msg;

      try
      {
        // [EVENT] Raise 'onStart'
        onStart(mode);

        // Ensure window exists
        if (!Window.exists())
          throw platform_error(HERE, "Failed to initialise window");

        // Update state
        State = PumpState::Running;

        // Retrieve next message for any window
        while (getMessage(&msg, nullptr, 0ul, 0ul))
        {
          // [MODAL] Update state when entering/exiting modal loop
          switch (static_cast<WindowMessage>(msg.message))
          {
          case WindowMessage::ENTERMENULOOP: State = PumpState::ModalLoop;    break;
          case WindowMessage::EXITMENULOOP:  State = PumpState::Running;      break;
          }

          // [EXISTS] 
          //if (Window && Window->exists())
          //{
          //  // [ACCELERATOR] Pass all accelerators to main window
          //  if (translateAccelerator(*Window, activeAccelerators, &msg))
          //    continue;

          //  // [DIALOG] Translate accelerators or dispatch to dialog
          //  if (Dialogs.contains(msg.hwnd))
          //    if (translateAccelerator(msg.hwnd, activeAccelerators, &msg)
          //     || isDialogMessage(msg.hwnd, &msg))
          //     continue;

          //  // TODO: Property sheets
          //}

          // Translate and dispatch to target
          ::TranslateMessage(&msg);
          dispatchMessage(&msg);
        }
      
        // [EVENT] Raise 'onExit'
        onExit();
        
        // Return WM_QUIT result
        return msg.wParam;
      }
      catch (std::exception& e)
      {
        errorBox<encoding>(Window, caught_exception("Unable to dispatch message", HERE, e));
        return -1;
      }
      catch (...)
      {
        errorBox<encoding>(Window, caught_exception("An unrecognised terminal error has occurred, the program will now exit.", HERE));
        return -2;
      }
    }

  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // MessagePump::onStart
    //! Called once before message pump starts
    //!
    //! \param[in] mode - Initial display mode
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual void onStart(ShowWindowFlags mode)
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // MessagePump::onExit
    //! Called once after message pump finishes
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual void onExit()
    {}
        
  };

}

#endif  // WTL_MESSAGE_PUMP_HPP
