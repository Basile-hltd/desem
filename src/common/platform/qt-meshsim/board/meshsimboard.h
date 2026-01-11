#pragma once
#include <QtWidgets/QWidget>
#include <QtGui/QPixmap>
#include <QtGui/QImage>
#include "interfaces/imonochromedisplay.h"
#include "interfaces/ijoystick.h"
#include "navigationjoystick.h"
#include "led.h"
#include "button.h"


class MeshSimBoard : public QWidget , public virtual IMonochromeDisplay ,
					 public virtual IJoystick
{
    Q_OBJECT

public:
	// Constructor and destructor.
	MeshSimBoard();
	virtual ~MeshSimBoard();

	// Interface implementation for IMonochromeDisplay and IJoystick.
	bool initialize();



	// Interface implementation for IMonochromeDisplay.
	Size resolution() const;
    AcceleratedDrawingMethods acceleratedDrawingMethods() const;
	bool enable();
	bool disable();
	void clear( PixelColor background );
	PixelColor pixel( const Point & point );
	void setPixel( const Point & point , PixelColor color );
	void writeDisplayMemory( size_t startAddress , const uint8_t * const sourceBitmap , size_t numberOfBits );
	void readDisplayMemory( size_t startAddress , uint8_t * const destinationBitmap , size_t numberOfBits );

	// Interface implementation for IJoystick.
	bool setObserver( IJoystickObserver * observer );
	void start();
	Position position() const;

	bool led() const;
	void setLed(bool enable);

signals:
    void sigLastSeenMousePosition(const QPoint & pos);

protected:
	// Qt event reception handlers.
	void paintEvent( QPaintEvent * event );
	void mousePressEvent( QMouseEvent * event );
	void mouseReleaseEvent( QMouseEvent * event );
    void mouseMoveEvent(QMouseEvent * event);
	void keyPressEvent( QKeyEvent * event );
	void keyReleaseEvent( QKeyEvent * event );

private:
	static IMonochromeDisplay::Size _DisplaySize;		// The display size (fixed).

	bool _ready;										// Are we initialized?
	bool _displayEnabled;								// Is the display enabled?
	bool _ledState;										// The actual state of the LED.
	IJoystick::Position _position;						// The actual position of the joystick.
	QImage _display;									// Display buffer.

	IJoystickObserver * _joystickObserver;				// Pointer to the joystick observer object.

    QString _resourceDirPath;                           // Path to resources (images, icons, etc.).
	QPixmap _background;								// Pixmap containing the rendered background.
	Led * _led;											// The LED widget.
	NavigationJoystick * _navigationJoystick;			// The navigation joystick widget.
	Button * _blackButton;								// The black button widget.
    Button * _blueButton;								// The blue button widget.
};
