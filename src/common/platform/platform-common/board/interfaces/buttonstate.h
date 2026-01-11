#pragma once

/**
 * @brief Defines the actual state of the button.
 */
enum ButtonState
{
    ButtonReleased = 0x00,    ///< @brief The button is released (idle).
    ButtonHover = 0x01,       ///< @brief The mouse is hovering over the button.
    ButtonPressed = 0x02      ///< @brief The button is being pressed.
};
