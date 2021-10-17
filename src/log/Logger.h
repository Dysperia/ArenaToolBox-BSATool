#ifndef BSATOOL_LOGGER_H
#define BSATOOL_LOGGER_H

#include <QPlainTextEdit>
#include <designpatterns/Singleton.h>
#include <error/Status.h>

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
    explicit Logger(token t);
    /**
     * @brief Destrutor
     */
    ~Logger() override;

    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    /**
     * @brief get the widget allowing to display the logs in the application
     * @return the logging widget
     */
    [[nodiscard]]QPlainTextEdit *textWidget() const;

    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * @brief log an info message
     * @param type message type
     * @param message message to log
     */
    void log(const MessageType &type,
              const QString &message);

    /**
     * @brief log the error message if the status is negative. The message
     * logged is the one detailing the error in the Status object
     * @param status Status object to log if error
     */
    void logError(const Status &status);

    /**
     * @brief log the error message if the status is negative. The message
     * logged is the one detailing the error in the Status object. In case of
     * null or positive status, the info message is logged
     * @param status Status object to log if error
     * @param infoMessage message to log if not error
     */
    void logErrorOrInfo(const Status &status, const QString &infoMessage);

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * @brief Widget used to display the logs in the interface
     */
    QPlainTextEdit *mTextWidget;
};

#endif // BSATOOL_LOGGER_H
