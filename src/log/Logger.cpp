#include <log/Logger.h>
#include <QDateTime>

//******************************************************************************
// Constructors/Destructor
//******************************************************************************
Logger::Logger(token t) {
    mTextWidget = new QPlainTextEdit;
    this->mTextWidget->setReadOnly(true);
}

Logger::~Logger() = default;

//******************************************************************************
// Getters/setters
//******************************************************************************
QPlainTextEdit *Logger::textWidget() const {
    return mTextWidget;
}

//**************************************************************************
// Methods
//**************************************************************************
/**
 * @brief build the line of log. The line ends with a new line
 * @param type log type
 * @param message log message
 * @return the log line
 */
QString buildLogLine(const Logger::MessageType &type,
                     const QString &message) {
    QString line = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss ");
    switch (type) {
        case Logger::MessageType::INFO:
            line.append("[INFO] ");
            break;
        case Logger::MessageType::WARNING:
            line.append("[WARNING] ");
            break;
        case Logger::MessageType::ERROR:
            line.append("[ERROR] ");
            break;
        default :
            break;
    }
    line.append(message);
    return line;
}

void Logger::log(const Logger::MessageType &type,
                 const QString &message) {
    QString line = buildLogLine(type, message);
    mTextWidget->appendPlainText(line);
}

void Logger::logError(const QString &message) {
    if (!message.isEmpty()) {
        this->log(MessageType::ERROR, message);
    }
}

void Logger::logWarning(const QString &message) {
    if (!message.isEmpty()) {
        this->log(MessageType::WARNING, message);
    }
}

void Logger::logInfo(const QString &message) {
    if (!message.isEmpty()) {
        this->log(MessageType::INFO, message);
    }
}

void Logger::logErrorOrInfo(const Status &status, const QString &infoMessage) {
    if (status.code() < 0) {
        logError(status.message());
    } else {
        logInfo(infoMessage);
    }
}
