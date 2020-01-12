#include <app.h>
#include <filesystem>
#include <codecvt>

void initLogger() {
    QSettings settings;
    std::error_code errorCode;

    std::wstring defaultLogFilePath = std::filesystem::temp_directory_path(errorCode).wstring();

    if( errorCode )
        defaultLogFilePath = L"";

    std::wstring defaultLogFileName = defaultLogFilePath +
                                      QCoreApplication::organizationName().toStdWString() +
                                      L"_" + QCoreApplication::applicationName().toStdWString() +
                                      L".log";

    std::wstring logFileName = settings.value( "loggerFilePath", QString::fromStdWString(defaultLogFileName)).toString().toStdWString();

    const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());

    auto &loggerCOut = _log.Logger<Logger::LOGGER_COUT>();
    auto &loggerFile = _log.Logger<Logger::LOGGER_FILE>();

    loggerFile.OpenFile(logFileName);
    loggerFile.Imbue( utf8_locale );

    auto addLevel = [&loggerCOut, &loggerFile]( Logger::LogLevel logLevel, const std::wstring &levelName, bool enableCOut, bool enableFile) {
        loggerCOut.AddLevelDescr(logLevel, levelName);
        loggerFile.AddLevelDescr(logLevel, levelName);
        if(enableCOut)  loggerCOut.EnableLevel(logLevel);
        if(enableFile)  loggerFile.EnableLevel(logLevel);
    };
    addLevel(Logger::INFO,     L"INFO",     true,  true);
    addLevel(Logger::DEBUG,    L"DEBUG",    false, true);
    addLevel(Logger::WARNING,  L"WARNING",  false, true);
    addLevel(Logger::ERROR,    L"ERROR",    true,  true);
    addLevel(Logger::CRITICAL, L"CRITICAL", true,  true);
    addLevel(Logger::FATAL,    L"FATAL",    true,  true);

    loggerCOut(Logger::INFO, L"Log file: ", logFileName);

    logInfo(L"Git hash: ", COMPILE_GIT_HASH);
}

void initApplication()
{
    QCoreApplication::setOrganizationName("AVN");
    QCoreApplication::setOrganizationDomain("avn.com");
    QCoreApplication::setApplicationName("pastry_chef_calc");

    initLogger();
}

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication a(argc, argv);
    QQmlApplicationEngine engine;

    initApplication();

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    engine.load(url);

    return QGuiApplication::instance()->exec();
}
