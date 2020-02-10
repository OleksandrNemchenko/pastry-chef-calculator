//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#ifndef PASTRY_CHEF_CALCULATOR_APP_H_
#define PASTRY_CHEF_CALCULATOR_APP_H_

#include <QtCore>
#include <QtQml>
#include <QtQuick>
#include <QtSql>

#include <avn/logger/logger_txt_cout.h>
#include <avn/logger/logger_txt_file.h>
#include <avn/logger/logger_txt_group.h>

#include <database.h>

using namespace std::string_literals;

namespace Logger {
    constexpr bool CLoggerThrSafe = false;
    enum LoggerID {
        LOGGER_COUT = 0,
        LOGGER_FILE
    };

    enum LogLevel {
        INFO = 0,
        DEBUG,
        WARNING,
        ERROR,
        CRITICAL,
        FATAL
    };
}

extern Logger::CLoggerTxtGroup<
        Logger::CLoggerTxtCOut<Logger::CLoggerThrSafe, wchar_t>,
        Logger::CLoggerTxtFile<Logger::CLoggerThrSafe, wchar_t>
        > _log;

template< typename... T > void logInfo(    const T&... args)  { _log(Logger::INFO,     args...); }
template< typename... T > void logDebug(   const T&... args)  { _log(Logger::DEBUG,    args...); }
template< typename... T > void logWarning( const T&... args)  { _log(Logger::WARNING,  args...); }
template< typename... T > void logError(   const T&... args)  { _log(Logger::ERROR,    args...); }
template< typename... T > void logCritical(const T&... args)  { _log(Logger::CRITICAL, args...); }
template< typename... T > void logFatal(   const T&... args)  { _log(Logger::FATAL,    args...); }

extern std::unique_ptr<PCCDatabaseQml> _db;

#endif // PASTRY_CHEF_CALCULATOR_APP_H_
