#ifndef BSATOOL_STATUS_H
#define BSATOOL_STATUS_H

#include <QString>

using namespace std;

/**
 * @brief Describe the return of a function
 *
 * This class contains an integer indicating the status (negative int indicates
 * an error). In case the status is negative, the message should be set to
 * detail what happened. The exact meaning of a positive or zero status should
 * be explained by the operation itself
 */
class Status : public exception {
public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    explicit Status(const int &status, QString message = "");
    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    [[nodiscard]]int status() const;
    [[nodiscard]]QString message() const;
    //**************************************************************************
    // Methods
    //**************************************************************************
    [[nodiscard]]const char* what() const noexcept override;

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * @brief status code.
     */
    int mStatus;
    /**
     * @brief message detailing the error if any happened
     */
    QString mMessage;
    /**
     * @brief std::string version of the message detailing the error if any happened
     */
    std::string mMessageStdString;
};

#endif // BSATOOL_STATUS_H
