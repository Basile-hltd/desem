#pragma once
#include "buttonstate.h"

/**
 * @ingroup interfaces
 * @brief This interface has to be implemented by all objects which want to register themselves as observer at an object
 *        implementing the IButton interface.
 *
 * Any time the state of the button changes, the onButtonStateChange() method will be called. Attention, the context
 * in which the observers method onButtonStateChange() will be called is not defined and can be another thread or even a
 * interrupt service routine. Take care of that.
 */
class IButtonObserver
{
protected:
    // It is just an interface, so constructor and destructor should be not public.
    explicit IButtonObserver() {}
    virtual ~IButtonObserver() {}

public:
    /**
     * @brief This method is called by the object implementing the IButton interface whenever the state
     * of the hardware button changed.
     *
     * Note that the context in which this method will be called is not defined
     * and can be another thread or an interrupt service routine. To avoid problems, do not use the new()
     * method or the malloc() method inside this method and do not call methods that may use these. Write
     * just as much code as needed inside the method.
     *
     * @param state The actual state of the button.
     */
    virtual void onButtonStateChange(ButtonState state) = 0;
};
