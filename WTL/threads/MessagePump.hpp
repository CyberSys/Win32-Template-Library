////////////////////////////////////////////////////////////////////////////////
//! \file wtl\threads\MessagePump.hpp
//! \brief Provides a customizeable message pump
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MESSAGE_PUMP_HPP
#define WTL_MESSAGE_PUMP_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  ////////////////////////////////////////////////////////////////////////////////
  //! \struct MessagePump - Message pump
  //! 
  //! \tparam ENC - Character encoding type 
  //! \tparam WINDOW - Window type
  ////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename WINDOW>
  struct MessagePump 
  {
    static_assert(ENC == WINDOW::encoding, "Message thread and window must have the same character encoding");

    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;
    
    //! \alias window_t - Define window type
    using window_t = WINDOW;

    //! \alias DialogCollection - Define dialog windows collection type
    using DialogCollection = typename window_t::WindowCollection;

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
    
    //! \var encoding - Define character encoding
    static constexpr Encoding encoding = ENC;

    // --------------------- CONSTRUCTION ----------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // MessagePump::MessagePump
    //! Creates a message pump 
    //! 
    //! \param[in] instance - Instance handle
    ///////////////////////////////////////////////////////////////////////////////
    MessagePump(::HMODULE instance) : Instance(instance),
                                      Window(instance)
    {}
    
    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // MessagePump::addDialog
    //! Informs the pump a dialog has been created
    //! 
    //! \return bool - True iff unique
    ///////////////////////////////////////////////////////////////////////////////
    bool  addDialog(window_t* dlg)
    {
      Dialogs.push_back(dlg);
      return true;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // MessagePump::removeDialog
    //! Informs the pump a dialog has been closed
    //! 
    //! \return bool - True iff found and removed
    ///////////////////////////////////////////////////////////////////////////////
    bool  removeDialog(window_t* dlg)
    {
      Dialogs.erase( std::find(Dialogs.begin(), Dialogs.end(), dlg) );
      return true;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // MessagePump::run
    //! Executes the message pump
    //! 
    //! \param[in] mode - Initial display mode
    //! \return int32 - Return code
    ///////////////////////////////////////////////////////////////////////////////
    int32 run(ShowWindowFlags mode = ShowWindowFlags::ShowNormal)
    {
      //! \var dispatchMessage - Functor for 'DispatchMessage'
      static const auto dispatchMessage = getFunc<ENC>(::DispatchMessageA,::DispatchMessageW);

      //! \var getMessage - Functor for 'GetMessage'
      static const auto getMessage = getFunc<ENC>(::GetMessageA,::GetMessageW);

      //! \var isDialogMessage - Functor for 'isDialogMessage'
      static const auto isDialogMessage = getFunc<ENC>(::IsDialogMessageA,::IsDialogMessageW);

      //! \var translateAccelerator - Functor for 'TranslateAccelerator'
      static const auto translateAccelerator = getFunc<ENC>(::TranslateAcceleratorA,::TranslateAcceleratorW);

      MSG msg;

      try
      {
        // [EVENT] Raise 'onStart'
        onStart(mode);

        // Ensure window exists
        if (!Window.exists())
          throw platform_error(HERE, "Failed to initialise window");

        // Retrieve next message for any window
        while (getMessage(&msg, nullptr, 0ul, 0ul))
        {
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
        cdebug.log(HERE, e);
        errorBox(Window, c_arr("Program Error"), e);
        return -1;
      }
      catch (...)
      {
        cdebug << wtl::Cons::Error << "Unknown exception" << wtl::endl;
        errorBox(Window, c_arr("Program Error"), c_arr("An unrecognised terminal error has occurred, the program will now exit."));
        return -2;
      }
    }

  protected:
    ///////////////////////////////////////////////////////////////////////////////
    // MessagePump::onStart
    //! Called once before message pump starts
    //!
    //! \param[in] mode - Initial display mode
    ///////////////////////////////////////////////////////////////////////////////
    virtual void onStart(ShowWindowFlags mode)
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // MessagePump::onExit
    //! Called once after message pump finishes
    ///////////////////////////////////////////////////////////////////////////////
    virtual void onExit()
    {}
        
    // -------------------- REPRESENTATION ---------------------
  protected:
    HINSTANCE         Instance;   //!< Module instance
    DialogCollection  Dialogs;    //!< Currently active modeless dialogs
    window_t          Window;     //!< Main thread window
  };

}

#endif  // WTL_MESSAGE_PUMP_HPP
