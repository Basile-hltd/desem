#include <cassert>
#include <cstdint>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QIcon>
#include <QPoint>
#include <QFile>
#include "interfaces/ijoystickobserver.h"
#include "meshsimboard.h"

extern QString __RESOURCE_PATH[];		// in factory.cpp

/*** Mesh Simulator options. *****************************************************************************************/

extern bool 		__XF_PAUSED;		// External boolean to stop XF execution.


/*** MeshSimBoard implementation *************************************************************************************/

const int DISP_WIDTH = 150;
const int DISP_HEIGHT = 150;
const int SCREEN_WIDTH = 150;
const int SCREEN_HEIGHT = 150;
IMonochromeDisplay::Size MeshSimBoard::_DisplaySize(DISP_WIDTH , DISP_HEIGHT);

MeshSimBoard::MeshSimBoard() : QWidget( nullptr ) , _ready( false ) , _displayEnabled( true ) , _ledState( false ) ,
                               _display(DISP_WIDTH , DISP_HEIGHT, QImage::Format_Mono) , _joystickObserver( nullptr ) ,
                               _led( nullptr ) , _navigationJoystick( nullptr ) , _blackButton( nullptr ) , _blueButton( nullptr )
{
    bool iconFound = false;
    QString glueDirPath;
    const uint32_t RESOURCE_PATH_COUNT = 2; // Fixme: sizeof(__RESOURCE_PATH)/sizeof(QString) or RESOURCE_PATH_COUNT;
    const QString applicationDirPath = QApplication::applicationDirPath();

    // Search for resource directory in parent folders
    for (uint32_t r = 0; r < RESOURCE_PATH_COUNT; r++)
    {
        glueDirPath.clear();

        for (int i = 8; i > 0; i--)
        {
            _resourceDirPath = applicationDirPath + glueDirPath + __RESOURCE_PATH[r];

            if (QFile::exists(_resourceDirPath + "/resources/app-icon.png"))
            {
                iconFound = true;
                break;
            }

            glueDirPath += "/..";
        }

        if (QFile::exists(_resourceDirPath + "/resources/app-icon.png"))
        {
            break;
        }
    }

    if (!iconFound)
    {
        // Back to default
        _resourceDirPath = applicationDirPath + __RESOURCE_PATH[0];
    }

    const QIcon appIcon(_resourceDirPath + "/resources/app-icon.png");
    QApplication::setWindowIcon(appIcon);
    setWindowIcon(appIcon);

    _position.position = IJoystick::Center;

    // Set the window size to the exact size of the simulation board background image.
    setFixedSize(325, 600);

    // Use the same title for the window as for the application.
    setWindowTitle( QApplication::applicationName() );

    // Set the color table for the off-screen display image.
    _display.setColorTable( QVector<QRgb>() << qRgba( 255 , 255 , 255 , 16 ) << qRgba( 0 , 0 , 0 , 200 ) );
}

MeshSimBoard::~MeshSimBoard()
{}

bool MeshSimBoard::initialize()
{
    // Initialize only once...
    if ( !_ready )
    {
        // Load UI images.
        _background.load(_resourceDirPath + "/resources/nucleo-drawing.png" );

        // Create LED (LD2) widget (green color, positioned at 42, 396, size 20x20)
        _led = new Led(Led::Green, 170, 333, 20, 13, this);
        _led->show();

        // Create navigation joystick widget at the old joystick position (144, 391) centered on the 112x112 widget
        int joystickX = 160 - 100; // Center the 200x200 widget on the old joystick position
        int joystickY = 480 - 100;
        _navigationJoystick = new NavigationJoystick(this, joystickX, joystickY);

        // Connect navigation joystick signals
        connect(_navigationJoystick, &NavigationJoystick::directionChanged, this, [this](IJoystick::Position position) {
            _position.position = position.position;
            // Notify observer
            if ( _joystickObserver && !__XF_PAUSED )
            {
                _joystickObserver->onPositionChange( _position );
            }
            // Update UI
            update();
        });

        // Create blue button widget (positioned at 115, 88, size 20x20)
        _blueButton = new Button(Button::Blue, 115, 88, 20, 20, this);
        _blueButton->show();

        // Create black button widget (positioned at 185, 88, size 20x20)
        _blackButton = new Button(Button::Black, 185, 88, 20, 20, this);
        _blackButton->show();

        // Show widget.
        QWidget::show();

        // Initialize LCD.
        _display.fill( Qt::color0 );

        // Ok, we are ready...
        _ready = true;
    }

    return true;
}

IMonochromeDisplay::Size MeshSimBoard::resolution() const
{
    assert( _ready );
    if ( _ready )
        return _DisplaySize;
    else
        return Size( 0 , 0 );
}

IMonochromeDisplay::AcceleratedDrawingMethods MeshSimBoard::acceleratedDrawingMethods() const
{
    static AcceleratedDrawingMethods accelerationMethods = { 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 };

    assert( _ready );
    return accelerationMethods;
}

bool MeshSimBoard::enable()
{
    assert( _ready );
    _displayEnabled = true;
    update();
    return _ready;
}

bool MeshSimBoard::disable()
{
    assert( _ready );
    _displayEnabled = false;
    update();
    return _ready;
}

void MeshSimBoard::clear( IMonochromeDisplay::PixelColor background )
{
    assert( _ready );
    if ( _ready && _displayEnabled )
    {
        // Use selected background color.
        switch ( background )
        {
            case IMonochromeDisplay::White:
                _display.fill( Qt::color0 );
                break;

            case IMonochromeDisplay::Black:
                _display.fill( Qt::color1 );
                break;

            default:
                // The default is white.
                _display.fill( Qt::color0 );
                break;
        }

        // Update simulation UI.
        update();
    }
}

IMonochromeDisplay::PixelColor MeshSimBoard::pixel( const IMonochromeDisplay::Point & point )
{
    assert( _ready );
    if ( _ready && _displayEnabled && point.x < _DisplaySize.width && point.x >= 0 &&
            point.y < _DisplaySize.height && point.y >= 0 )
    {
        // Return the color of the pixel at the given position.
        if ( _display.pixelIndex( QPoint( point.x , point.y ) ) ) return IMonochromeDisplay::Black;
        else return IMonochromeDisplay::White;
    }

    return IMonochromeDisplay::Undefined;
}

void MeshSimBoard::setPixel( const IMonochromeDisplay::Point & point , IMonochromeDisplay::PixelColor color )
{
    assert( _ready );
    if ( _ready && _displayEnabled && point.x < _DisplaySize.width && point.x >= 0 &&
         point.y < _DisplaySize.height && point.y >= 0  && color != IMonochromeDisplay::Undefined )
    {
        // Change the color of the pixel.
        _display.setPixel( QPoint( point.x , point.y ) , color == White ? Qt::color0 : Qt::color1 );

        // Update simulated UI.
        update();
    }
}

void MeshSimBoard::writeDisplayMemory( size_t startAddress , const uint8_t * const sourceBitmap , size_t numberOfBits )
{
    assert( _ready );
    if ( _ready && _displayEnabled && sourceBitmap )
    {
        // We use just the setPixel method here, since the bitmap lines have to be aligned 32 bit.
        for ( size_t i = 0 ; i < numberOfBits ; ++i )
        {
            _display.setPixel( ( i + startAddress * 8 ) % DISP_WIDTH,
                               ( i + startAddress * 8 ) / DISP_WIDTH,
                               *( sourceBitmap + ( i / 8 ) ) & ( 1 << ( i % 8 ) ) ? 1 : 0 );
        }

        update();
    }
}

void MeshSimBoard::readDisplayMemory( size_t startAddress , uint8_t * const destinationBitmap , size_t numberOfBits )
{
    assert( _ready );
    if ( _ready && _displayEnabled && destinationBitmap )
    {
        // Clear target bitmap.
        memset( destinationBitmap , 0 , numberOfBits / 8 );
        for ( size_t i = 0 ; i < numberOfBits ; ++i )
        {
            // Copy pixels into the bitmap. Note that we use the pixelIndex() method, since the data is 32 bit aligned.
            if ( _display.pixelIndex( ( i + startAddress * 8 ) % DISP_WIDTH , ( i + startAddress * 8 ) / DISP_WIDTH ) == Qt::color1 )
                *( destinationBitmap + ( i / 8 ) ) |= 1 << ( i % 8 );
        }
    }
}

bool MeshSimBoard::setObserver( IJoystickObserver * observer )
{
    assert(_ready );

    // We can have only one observer.
    _joystickObserver = observer;
    return observer;
}

void MeshSimBoard::start()
{
}

IJoystick::Position MeshSimBoard::position() const
{
    assert( _ready );
    return _position;
}

bool MeshSimBoard::led() const
{
    assert( _ready );
    return _ledState;
}

void MeshSimBoard::setLed(bool enable)
{
    assert( _ready );
    // Change the led and update the UI only if really needed.
    if ( enable != _ledState )
    {
        _ledState = enable;
        if (_led) {
            _led->setState(enable);
        }
    }
}

void MeshSimBoard::paintEvent( QPaintEvent * event )
{
    Q_UNUSED(event)

    QPainter painter( this );

    // Draw opaque if the XF has been stopped.
    if ( __XF_PAUSED ) painter.setOpacity( 0.25 );

    // Draw background.
    painter.drawPixmap( 0 , 0 , _background );

    // LED is drawn as a widget, not in paintEvent

    // Navigation joystick is drawn as a widget, not in paintEvent

    // Draw off-screen display if it is enabled.
    if (_displayEnabled)
        painter.drawImage(QRectF(92, 138, SCREEN_WIDTH, SCREEN_HEIGHT), _display);

    //painter.setPen(Qt::red);
    //painter.drawRect(QRectF(92, 138, SCREEN_WIDTH, SCREEN_HEIGHT));
}

void MeshSimBoard::mousePressEvent( QMouseEvent * event )
{
    // Check for quit button (top-right area)
    // if ( QRect( 240 , 93 , 35 , 16 ).contains( event->pos() ) )
    // {
    //     QApplication::quit();
    //     return;
    // }

    // Navigation joystick handles its own mouse events
    // Let the event propagate to child widgets (including NavigationJoystick)
    event->ignore();
}

void MeshSimBoard::mouseReleaseEvent( QMouseEvent * event )
{
    // Navigation joystick handles its own release events
    // Only handle releases outside the joystick area if needed
    event->ignore();
}

void MeshSimBoard::mouseMoveEvent(QMouseEvent * event)
{
    // qDebug() << event->pos();
    emit sigLastSeenMousePosition(event->pos());
}

void MeshSimBoard::keyPressEvent( QKeyEvent * event )
{
    // Depending which key was pressed, call the hander of the observer.
    if ( event->key() == Qt::Key_Up ) _position.position = IJoystick::Up;
    else if ( event->key() == Qt::Key_Left ) _position.position = IJoystick::Left;
    else if ( event->key() == Qt::Key_Down ) _position.position = IJoystick::Down;
    else if ( event->key() == Qt::Key_Right ) _position.position = IJoystick::Right;
    else if ( event->key() == Qt::Key_Enter ||
               event->key() == Qt::Key_Return ) _position.position = IJoystick::Pressed;
    else return;

    // We need to have a registered observer for joystick events.
    if ( _joystickObserver && ! __XF_PAUSED )
    {
        _joystickObserver->onPositionChange( _position );
    }

    // Update UI.
    update();
}

void MeshSimBoard::keyReleaseEvent( QKeyEvent * event )
{
    Q_UNUSED(event)

    // React only if there was a position other than Center.
    if ( _position.position != IJoystick::Center )
    {
        _position.position = IJoystick::Center;

        // We need to have a registered observer for joystick events.
        if ( _joystickObserver && ! __XF_PAUSED )
        {
            _joystickObserver->onPositionChange( _position );
        }

        // Update UI.
        update();
    }
}
