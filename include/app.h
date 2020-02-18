//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#ifndef PASTRY_CHEF_CALCULATOR_APP_H_
#define PASTRY_CHEF_CALCULATOR_APP_H_

#include <QtCore>
#include <QtQml>
#include <QtQuick>
#include <QtSql>

#include <array>
#include <memory>

#include <avn/logger/logger_txt_cout.h>
#include <avn/logger/logger_txt_file.h>
#include <avn/logger/logger_txt_group.h>

#include <database.h>

static constexpr bool _debugMode =
#ifdef DEBUG_MODE
    true
#else // DEBUG_MODE
    false
#endif // DEBUG_MODE
    ;

using namespace std::string_literals;

namespace ALogger {

    constexpr bool ALoggerThrSafe = false;

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

extern ALogger::ALoggerTxtGroup<
        ALogger::ALoggerTxtCOut<ALogger::ALoggerThrSafe, wchar_t>,
        ALogger::ALoggerTxtFile<ALogger::ALoggerThrSafe, wchar_t>
        > _log;

template< typename... T > void logInfo(    const T&... args)  { _log(ALogger::INFO,     args...); }
template< typename... T > void logDebug(   const T&... args)  { _log(ALogger::DEBUG,    args...); }
template< typename... T > void logWarning( const T&... args)  { _log(ALogger::WARNING,  args...); }
template< typename... T > void logError(   const T&... args)  { _log(ALogger::ERROR,    args...); }
template< typename... T > void logCritical(const T&... args)  { _log(ALogger::CRITICAL, args...); }
template< typename... T > void logFatal(   const T&... args)  { _log(ALogger::FATAL,    args...); }

extern std::unique_ptr<PCCDatabase> _db;

#endif // PASTRY_CHEF_CALCULATOR_APP_H_
