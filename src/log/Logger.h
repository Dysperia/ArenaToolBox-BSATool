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
class Logger : public Singleton<Logger> {
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
     * @brief log the message as error if not empty
     * @param message message to log
     */
    void logError(const QString &message);

    /**
     * @brief log the message as warning if not empty
     * @param message message to log
     */
    void logWarning(const QString &message);

    /**
     * @brief log the message as info if not empty
     * @param message message to log
     */
    void logInfo(const QString &message);

    /**
     * @brief log an error message if the status is negative. The message
     * logged is the one detailing the error in the Status object. In case of
     * null or positive status, the info message is logged.
     * If the message to log (error or info) is empty, nothing is logged
     * @param status Status object to log if error
     * @param infoMessage message to log if no error
     */
    void logErrorOrInfo(const Status &status, const QString &infoMessage);

    /**
     * Log the exception message if an error is thrown during the execution of the function func. If no error,
     * log the message noErrorMsg. Use only with void returning function unless you agree to ignore the result
     * @param func function to execute
     * @param noErrorMsgProvider function of type () -> QString, providing the message to log if no error
     * @return the result of func if needed
     */
    template<typename Callable1, typename Callable2>
    auto exception_handle_void(Callable1 &&func, Callable2 &&noErrorMsgProvider) -> decltype(func());

    /**
     * Log the exception message if an error is thrown during the execution of the function func. If no error,
     * log the message noErrorMsg. Use only with non void returning function
     * @param func function to execute
     * @param noErrorMsgProvider function of type () -> QString, providing the message to log if no error
     * @return the result of func if needed
     */
    template<typename Callable1, typename Callable2>
    auto exception_handle_nonVoid(Callable1 &&func, Callable2 &&noErrorMsgProvider) -> decltype(func());

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * @brief Widget used to display the logs in the interface
     */
    QPlainTextEdit *mTextWidget;
};

//**************************************************************************
// Definitions
//**************************************************************************

template<typename Callable1, typename Callable2>
auto Logger::exception_handle_void(Callable1 &&func, Callable2 &&noErrorMsgProvider) -> decltype(func()) {
    try {
        func();
        logInfo(noErrorMsgProvider());
    } catch (Status &e) {
        logError(e.message());
    } catch (exception &e) {
        logError(e.what());
    }
}

template<typename Callable1, typename Callable2>
auto Logger::exception_handle_nonVoid(Callable1 &&func, Callable2 &&noErrorMsgProvider) -> decltype(func()) {
    try {
        decltype(func()) result = func();
        logInfo(noErrorMsgProvider());
        return result;
    } catch (Status &e) {
        logError(e.message());
    } catch (exception &e) {
        logError(e.what());
    }
}

#endif // BSATOOL_LOGGER_H
