#pragma once
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include "interfaces/ijoystick.h"

class NavigationJoystick : public QWidget
{
    Q_OBJECT

public:
    explicit NavigationJoystick(QWidget *parent = nullptr, int x = 0, int y = 0);
    ~NavigationJoystick();

    // Set the joystick position (for external updates)
    void setPosition(IJoystick::Position position);

signals:
    // Emitted when a direction button is pressed or released
    void directionChanged(IJoystick::Position position);

private slots:
    void onUpPressed();
    void onDownPressed();
    void onLeftPressed();
    void onRightPressed();
    void onCenterPressed();
    void onButtonReleased();

protected:
    QIcon createArrowIcon(IJoystick::JoystickButton direction);

    void paintEvent(QPaintEvent *event) override;

private:
    QPushButton *_upButton;
    QPushButton *_downButton;
    QPushButton *_leftButton;
    QPushButton *_rightButton;
    QPushButton *_centerButton;

    IJoystick::Position _currentPosition;
    int _x;
    int _y;

    void setupUI();
    void setupStyling();
    void updateButtonStates();
};
