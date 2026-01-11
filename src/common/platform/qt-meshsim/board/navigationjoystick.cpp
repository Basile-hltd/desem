#include "navigationjoystick.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QApplication>
#include <QPainter>
#include <QPaintEvent>
#include <QIcon>
#include <QSvgRenderer>
#include <QPixmap>

NavigationJoystick::NavigationJoystick(QWidget *parent, int x, int y)
    : QWidget(parent)
    , _upButton(nullptr)
    , _downButton(nullptr)
    , _leftButton(nullptr)
    , _rightButton(nullptr)
    , _centerButton(nullptr)
    , _x(x)
    , _y(y)
{
    setupUI();
    setupStyling();
    updateButtonStates();
    move(_x, _y);
}

NavigationJoystick::~NavigationJoystick()
{
}

void NavigationJoystick::setupUI()
{
    setFixedSize(200, 200);
    setAutoFillBackground(true); // Enable background painting for stylesheet

    // Create buttons
    _upButton = new QPushButton(this);
    _downButton = new QPushButton(this);
    _leftButton = new QPushButton(this);
    _rightButton = new QPushButton(this);
    _centerButton = new QPushButton("OK", this);

    // Set custom SVG icons for all directional buttons
    _upButton->setIcon(createArrowIcon(IJoystick::Up));      // up arrow
    _downButton->setIcon(createArrowIcon(IJoystick::Down));  // down arrow
    _leftButton->setIcon(createArrowIcon(IJoystick::Left));  // left arrow
    _rightButton->setIcon(createArrowIcon(IJoystick::Right)); // right arrow

    // Set button sizes
    _upButton->setFixedSize(80, 48);
    _downButton->setFixedSize(80, 48);
    _leftButton->setFixedSize(48, 80);
    _rightButton->setFixedSize(48, 80);
    _centerButton->setFixedSize(80, 65);

    const uint32_t xOffset = 25;
    const uint32_t yOffset = 10;

    // Position buttons
    _upButton->move(xOffset + 36, yOffset + 8);
    _downButton->move(xOffset + 36, yOffset + 121);
    _leftButton->move(xOffset + -12, yOffset + 48);
    _rightButton->move(xOffset + 116, yOffset + 48);
    _centerButton->move(xOffset + 36, yOffset + 56);

    // Connect signals
    connect(_upButton, &QPushButton::pressed, this, &NavigationJoystick::onUpPressed);
    connect(_downButton, &QPushButton::pressed, this, &NavigationJoystick::onDownPressed);
    connect(_leftButton, &QPushButton::pressed, this, &NavigationJoystick::onLeftPressed);
    connect(_rightButton, &QPushButton::pressed, this, &NavigationJoystick::onRightPressed);
    connect(_centerButton, &QPushButton::pressed, this, &NavigationJoystick::onCenterPressed);

    connect(_upButton, &QPushButton::released, this, &NavigationJoystick::onButtonReleased);
    connect(_downButton, &QPushButton::released, this, &NavigationJoystick::onButtonReleased);
    connect(_leftButton, &QPushButton::released, this, &NavigationJoystick::onButtonReleased);
    connect(_rightButton, &QPushButton::released, this, &NavigationJoystick::onButtonReleased);
    connect(_centerButton, &QPushButton::released, this, &NavigationJoystick::onButtonReleased);
}

void NavigationJoystick::setupStyling()
{
    // Set the main widget background (outer black housing)
    setStyleSheet(R"(
        NavigationJoystick {
            background-color: black;
            border: 2px solid #374151;
            border-radius: 50%;
        }
    )");

    // Style for directional buttons
    QString buttonStyle = R"(
        QPushButton {
            background-color: #d1d5db;
            border: 1px solid #6b7280;
            color: black;
            font-weight: bold;
            font-size: 12px;
        }
        QPushButton:hover {
            background-color: #9ca3af;
        }
        QPushButton:pressed {
            background-color: #6b7280;
            border-color: #4b5563;
        }
    )";

    // Apply styles to directional buttons
    _upButton->setStyleSheet(buttonStyle);
    _downButton->setStyleSheet(buttonStyle);
    _leftButton->setStyleSheet(buttonStyle);
    _rightButton->setStyleSheet(buttonStyle);

    // Special styling for center button
    _centerButton->setStyleSheet(R"(
        QPushButton {
            background-color: #d1d5db;
            border: 2px solid #6b7280;
            color: black;
            font-weight: bold;
            font-family: monospace;
            font-size: 10px;
            border-radius: 50%;
        }
        QPushButton:hover {
            background-color: #9ca3af;
        }
        QPushButton:pressed {
            background-color: #6b7280;
            border-color: #4b5563;
        }
    )");

    // Style rounded corners for directional buttons
    _upButton->setStyleSheet(_upButton->styleSheet() + " border: 1px solid; border-top-left-radius: 8px; border-top-right-radius: 8px;");
    _downButton->setStyleSheet(_downButton->styleSheet() + " border: 1px solid; border-bottom-left-radius: 8px; border-bottom-right-radius: 8px;");
    _leftButton->setStyleSheet(_leftButton->styleSheet() + " border: 1px solid; border-top-left-radius: 8px; border-bottom-left-radius: 8px;");
    _rightButton->setStyleSheet(_rightButton->styleSheet() + " border: 1px solid; border-top-right-radius: 8px; border-bottom-right-radius: 8px;");
}

void NavigationJoystick::updateButtonStates()
{
    // This could be used to show current position state if needed
    // For now, buttons are stateless and only show press feedback
}

void NavigationJoystick::setPosition(IJoystick::Position position)
{
    _currentPosition = position;
    updateButtonStates();
}

void NavigationJoystick::onUpPressed()
{
    _currentPosition.position = IJoystick::Up;
    emit directionChanged(_currentPosition);
}

void NavigationJoystick::onDownPressed()
{
    _currentPosition.position = IJoystick::Down;
    emit directionChanged(_currentPosition);
}

void NavigationJoystick::onLeftPressed()
{
    _currentPosition.position = IJoystick::Left;
    emit directionChanged(_currentPosition);
}

void NavigationJoystick::onRightPressed()
{
    _currentPosition.position = IJoystick::Right;
    emit directionChanged(_currentPosition);
}

void NavigationJoystick::onCenterPressed()
{
    _currentPosition.position = IJoystick::Pressed;
    emit directionChanged(_currentPosition);
}

void NavigationJoystick::onButtonReleased()
{
    _currentPosition.position = IJoystick::Center;
    emit directionChanged(_currentPosition);
}

QIcon NavigationJoystick::createArrowIcon(IJoystick::JoystickButton direction)
{
    // Create minimal SVG arrow icon for different directions
    QString svgContent;
    switch (direction) {
        case IJoystick::Up: // Up arrow
            svgContent = R"(
                <svg width="16" height="16" viewBox="0 0 16 16" xmlns="http://www.w3.org/2000/svg">
                    <path d="M4 10 L8 6 L12 10" stroke="black" stroke-width="2" fill="none" stroke-linecap="round" stroke-linejoin="round"/>
                </svg>
            )";
            break;
        case IJoystick::Down: // Down arrow
            svgContent = R"(
                <svg width="16" height="16" viewBox="0 0 16 16" xmlns="http://www.w3.org/2000/svg">
                    <path d="M4 6 L8 10 L12 6" stroke="black" stroke-width="2" fill="none" stroke-linecap="round" stroke-linejoin="round"/>
                </svg>
            )";
            break;
        case IJoystick::Left: // Left arrow
            svgContent = R"(
                <svg width="16" height="16" viewBox="0 0 16 16" xmlns="http://www.w3.org/2000/svg">
                    <path d="M10 4 L6 8 L10 12" stroke="black" stroke-width="2" fill="none" stroke-linecap="round" stroke-linejoin="round"/>
                </svg>
            )";
            break;
        case IJoystick::Right: // Right arrow
            svgContent = R"(
                <svg width="16" height="16" viewBox="0 0 16 16" xmlns="http://www.w3.org/2000/svg">
                    <path d="M6 4 L10 8 L6 12" stroke="black" stroke-width="2" fill="none" stroke-linecap="round" stroke-linejoin="round"/>
                </svg>
            )";
            break;
        default:
            svgContent = "";
            break;
    }

    // Render SVG to pixmap
    QSvgRenderer renderer(svgContent.toUtf8());
    QPixmap pixmap(16, 16);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    renderer.render(&painter);

    return QIcon(pixmap);
}

void NavigationJoystick::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw the black circular background with gray border
    QRectF rect(1, 1, width() - 2, height() - 2); // Account for border
    painter.setPen(QPen(QColor("#374151"), 2)); // Gray border
    painter.setBrush(QBrush(Qt::black)); // Black fill
    painter.drawEllipse(rect);
}
