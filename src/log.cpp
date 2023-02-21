#include "Logger/log.h"

namespace Logger
{

Log::Log()
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
