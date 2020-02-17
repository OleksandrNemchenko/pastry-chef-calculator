#include <app.h>

#include <filesystem>
#include <codecvt>

#include <tables/units.h>

namespace {

    void initLogger() {
        QSettings settings;
        QString defaultLogFilePath = QDir::tempPath();
        QString defaultLogFileName = QString("%1%2_%3.log"). arg(defaultLogFilePath). arg(QCoreApplication::organizationName()). arg(QCoreApplication::applicationName());
        QString logFileName = settings.value("loggerFilePath", defaultLogFileName).toString();
        const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
    
        auto &loggerCOut = _log.logger<ALogger::LOGGER_COUT>();
        auto &loggerFile = _log.logger<ALogger::LOGGER_FILE>();
    
        loggerFile.openFile(logFileName);
    
        _log.imbue(utf8_locale);
    
        auto addLevel = [&loggerCOut, &loggerFile]( ALogger::LogLevel logLevel, const std::wstring &levelName, bool enableCOut, bool enableFile) {
            loggerCOut.addLevelDescr(logLevel, levelName);
            loggerFile.addLevelDescr(logLevel, levelName);
            if(enableCOut)  loggerCOut.enableLevel(logLevel);
            if(enableFile)  loggerFile.enableLevel(logLevel);
        };
    
        addLevel(ALogger::INFO,     L"INFO"s,     true,       true      );
        addLevel(ALogger::DEBUG,    L"DEBUG"s,    _debugMode, _debugMode);
        addLevel(ALogger::WARNING,  L"WARNING"s,  _debugMode, true      );
        addLevel(ALogger::ERROR,    L"ERROR"s,    _debugMode, true      );
        addLevel(ALogger::CRITICAL, L"CRITICAL"s, true,       true      );
        addLevel(ALogger::FATAL,    L"FATAL"s,    true,       true      );
    
        if (_debugMode){
            loggerCOut.disableTasks();
            logDebug("Debug mode");
        }
    
        loggerCOut(ALogger::INFO, L"Log file: "s, logFileName);
    
        logInfo(L"Git hash: "s, COMPILE_GIT_HASH);
    }
    
    void initQML(QQmlApplicationEngine &engine)
    {
        QQmlContext *context = engine.rootContext();    // Создаём корневой контекст
        context->setContextProperty("hasDbError", _db->hasDbError());
        context->setContextProperty("lastDbError", _db->lastDbError());
        context->setContextProperty("PCCUnits", &_db->units());

        const QUrl url(QStringLiteral("qrc:/qml/Main.qml"));
        engine.load(url);
    
    #if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS) && !defined(Q_OS_WATCHOS)
        {
            QObject *rootObject(engine.rootObjects().first());
            rootObject->setProperty("width", 640);
            rootObject->setProperty("height", 500);
        }
    #endif
    
    }
    
    void initApplication(QQmlApplicationEngine &engine)
    {
        QCoreApplication::setOrganizationName("AVN");
        QCoreApplication::setOrganizationDomain("avn.com");
        QCoreApplication::setApplicationName("pastry_chef_calc");
    
        initLogger();
    
        _db = std::make_unique<PCCDatabase>();
    
        initQML(engine);
    }

} // namespace

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication a(argc, argv);

    try {
        QQmlApplicationEngine engine;

        initApplication(engine);

        int retCode = QGuiApplication::instance()->exec();

        _db.release();

        return retCode;
    }
    catch(std::runtime_error &error) {
        std::wcout << "Runtime error: " << error.what() << std::endl;

        return -1;
    }
    catch(...) {
        std::wcout << "Any runtime error has been happened" << std::endl;

        return -2;
    }
}
