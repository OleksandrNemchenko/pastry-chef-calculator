#include <app.h>
#include <filesystem>
#include <codecvt>

void initLogger() {
    QSettings settings;
    std::error_code errorCode;

    std::wstring defaultLogFilePath = std::filesystem::temp_directory_path(errorCode).wstring();

    if( errorCode )
        defaultLogFilePath = L""s;

    std::wstring defaultLogFileName = defaultLogFilePath +
                                      QCoreApplication::organizationName().toStdWString() +
                                      L"_"s + QCoreApplication::applicationName().toStdWString() +
                                      L".log"s;

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
    addLevel(Logger::INFO,     L"INFO"s,     true,  true);
    addLevel(Logger::DEBUG,    L"DEBUG"s,    true,  true);
    addLevel(Logger::WARNING,  L"WARNING"s,  true,  true);
    addLevel(Logger::ERROR,    L"ERROR"s,    true,  true);
    addLevel(Logger::CRITICAL, L"CRITICAL"s, true,  true);
    addLevel(Logger::FATAL,    L"FATAL"s,    true,  true);

    loggerCOut(Logger::INFO, L"Log file: "s, logFileName);

    logInfo(L"Git hash: "s, COMPILE_GIT_HASH);
}

void initQML(PCCDatabaseQml *dbQmlInterface, QQmlApplicationEngine &engine)
{
    QQmlContext *context = engine.rootContext();    // Создаём корневой контекст
    context->setContextProperty("db", dbQmlInterface);

    QObject::connect(_db.get(), &PCCDatabase::databaseInitialization, dbQmlInterface, &PCCDatabaseQml::databaseInitialization);
    QObject::connect(_db.get(), &PCCDatabase::databaseInitializedSuccessfully, dbQmlInterface, &PCCDatabaseQml::databaseInitializedSuccessfully);
    QObject::connect(_db.get(), &PCCDatabase::databaseInitializationError, dbQmlInterface, &PCCDatabaseQml::databaseInitializationError);
    QObject::connect(dbQmlInterface, &PCCDatabaseQml::Initialize, _db.get(), &PCCDatabase::Initialize);

    const QUrl url(QStringLiteral("qrc:/qml/Main.qml"));
    engine.load(url);
}

void initApplication(PCCDatabaseQml *dbQmlInterface, QQmlApplicationEngine &engine)
{
    QCoreApplication::setOrganizationName("AVN");
    QCoreApplication::setOrganizationDomain("avn.com");
    QCoreApplication::setApplicationName("pastry_chef_calc");

    initLogger();

    _db = std::make_unique<PCCDatabase>();

    initQML(dbQmlInterface, engine);
}

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication a(argc, argv);

    try {
        QQmlApplicationEngine engine;
        PCCDatabaseQml dbQmlInterface;

        initApplication(&dbQmlInterface, engine);

        int retCode = QGuiApplication::instance()->exec();

        _db.release();
    }
    catch(std::runtime_error &error) {
        std::wcout << "Runtime error: " << error.what() << std::endl;
    }
    catch(...) {
        std::wcout << "Any runtime error has been happened" << std::endl;
    }
}
