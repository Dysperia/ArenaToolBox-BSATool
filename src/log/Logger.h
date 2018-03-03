#ifndef LOGGER_H
#define LOGGER_H

#include <QPlainTextEdit>
#include "../designpatterns/Singleton.h"

/**
 * @brief Implementation of a simple logger
 *
 * The logger write logs in a QPlainTextEdit.
 */
class Logger: public Singleton<Logger>
{
public:
    //**************************************************************************
    // Enumerations
    //**************************************************************************
    /**
     * @brief Enumeration of the different message type
     */
    enum MessageType {
        /** Info log */
        INFO,
        /** Info log */
        WARNING,
        /** Error log */
        ERROR
    };

    //**************************************************************************
    // Constructors/Destructor
    //**************************************************************************
    /**
     * @brief Logger constructor for the singleton
     * @param t token used by the singleton
     */
    Logger(token t);
    /**
     * @brief Destrutor
     */
    ~Logger();

    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    /**
     * @brief get the widget allowing to display the logs in the application
     * @return the logging widget
     */
    QPlainTextEdit *textWidget() const;

    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * @brief log an info message
     * @param type message type
     * @param message message to log
     */
    void log(const MessageType type,
              const QString &message);

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * @brief Widget used to display the logs in the interface
     */
    QPlainTextEdit *mTextWidget;
};

#endif // LOGGER_H
