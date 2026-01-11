#pragma once

#include <cstdint>
#include "buttonstate.h"

class IButtonObserver;

/**
 * @ingroup interfaces
 * @brief This interface represents a button that can be in different states (released, hover, pressed).
 *
 * The state of the button can be polled or a ButtonObserver can be registered to get button events.
 * Note that just one observer is supported and if the setObserver() method is called a second time,
 * the first instance of the observer will be dropped.
 */
class IButton
{
protected:
    // It is just an interface, so constructor and destructor should be not public.
    explicit IButton() {}
    virtual ~IButton() {}

public:
    /**
     * @brief Initializes all subsystems and the object itself needed for proper button operation.
     *
     * @return True if the button could be initialized and is ready to be used, false otherwise.
     */
    virtual bool initialize() = 0;

    /**
     * @brief Sets the observer that will receive events upon button state changes.
     *
     * Note that the object pointed by observer has to implement the IButtonObserver interface.
     * If already an observer was registered before, the old one is discarded, since only one observer can
     * get button events at the same time.
     *
     * @param observer Pointer to the observer object.
     * @return True if the observer was accepted, false if it was disabled (observer=NULL) or on error.
     */
    virtual bool setObserver(IButtonObserver *observer) = 0;

    virtual void start() = 0;    ///< Starts the state machine of the button

    /**
     * @brief Returns the actual state of the button.
     *
     * @return The actual state of the button.
     */
    virtual ButtonState state() const = 0;

    /**
     * @brief Convenience method, that converts the Button object into its actual value.
     *
     * @return The actual state of the button.
     */
    inline operator ButtonState() const
    {
        return state();
    }
};
