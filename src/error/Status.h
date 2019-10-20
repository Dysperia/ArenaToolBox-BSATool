#ifndef BSATOOL_STATUS_H
#define BSATOOL_STATUS_H

#include <QString>

/**
 * @brief Describe the return of a function
 *
 * This class contains an integer indicating the status (negative int indicates
 * an error). In case the status is negative, the message should be set to
 * detail what happened. The exact meaning of a positive or zero status should
 * be explained by the operation itself
 */
class Status {
public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    explicit Status(const int &status, QString message = "");
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
     * @brief status code.
     */
    int mStatus;
    /**
     * @brief message detailling the error if any happened
     */
    QString mMessage;
};

#endif // BSATOOL_STATUS_H
