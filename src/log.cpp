#include "Logger/log.h"

namespace Logger
{

Log::Log()
{
}

Log::Log(int id, LogType type, QString title, QString body, QString file, QString function, QString category, int line)
    : id(id)
    , type(type)
    , title(std::move(title))
    , body(std::move(body))
    , file(std::move(file))
    , function(std::move(function))
    , category(std::move(category))
    , line(line)
{
}

QString Logger::Log::typeString() const
{
    switch (type) {
    case DebugType:
        return "Debug";
        break;
    case InfoType:
        return "Info";
        break;
    case WarningType:
        return "Warning";
        break;
    case CriticalType:
        return "Error";
        break;
    case FatalType:
        return "Fatal";
    }

    return "";
}

}
