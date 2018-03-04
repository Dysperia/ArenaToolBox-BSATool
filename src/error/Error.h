#ifndef ERROR_H
#define ERROR_H

#include <QString>

/**
 * @brief Describe the return of a function
 *
 * This class contains an integer indicating the status (zero indicating that no
 * error occured, any other int indicates an error). In case the status is not
 * zero, the message should be set to detail what happened)
 */
class Error {
public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    Error(const int &status, const QString &message = "");
    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    int status() const;
    QString message() const;

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * @brief status code. Any code different of zero indicates an error
     */
    int mStatus;
    /**
     * @brief message detailling the error if any
     */
    QString mMessage;
};

#endif // ERROR_H
