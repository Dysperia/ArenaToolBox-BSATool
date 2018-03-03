#include "Logger.h"
#include <fstream>
#include <QDateTime>

//******************************************************************************
// Constructors/Destructor
//******************************************************************************
Logger::Logger(token t) {
    mTextWidget = new QPlainTextEdit;
    this->mTextWidget->setReadOnly(true);
}

Logger::~Logger() {}

//******************************************************************************
// Getters/setters
//******************************************************************************
QPlainTextEdit *Logger::textWidget() const
{
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
QString buildLogLine(const Logger::MessageType type,
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
    line.append(message).append("\n");
    return line;
}

void Logger::log(const Logger::MessageType type,
                 const QString &message)
{
    QString line = buildLogLine(type, message);
    mTextWidget->appendPlainText(line);
}
