#pragma once

#include <stdint.h>
#include <QWidget>

namespace board {

/**
 * @brief Class interfacing the real accelerometer hardware.
 */
class Accelerometer : public QObject
{
    Q_OBJECT
public:
	Accelerometer();

	bool initialize();
	static void getAccelerationValues(int16_t & x, int16_t & y, int16_t & z);

public slots:
    void onLastSeenMousePosition(const QPoint & pos);

protected:
	inline static bool isInitialized() { return true; }

protected:
    static QWidget * _pMainWindow;
    static QPoint lastSeenMousePosition;    // Hack to get a 'window position' to acceleromter in Linux and macOS.
};

} // namespace board
