# Qt Logger

Useful logging library for Qt5.x

 * Save log to file
 * Show log in QTableView (when widget module enabled)
 * Show log in Table mode in terminal (when widget module not enabled)
 * Can use on exists code (handle qDebug, qInfo, qWarning and qFatal)

Logger inits with one method call
```cpp
Logger::instance()->init();
```

