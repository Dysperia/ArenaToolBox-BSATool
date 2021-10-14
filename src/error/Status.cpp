#include <utility>

#include "Status.h"

//**************************************************************************
// Constructors
//**************************************************************************
Status::Status(const int &status, QString message):
    mStatus(status), mMessage(move(message)), mMessageStdString(mMessage.toStdString())
{

}

//**************************************************************************
// Getters/setters
//**************************************************************************
int Status::status() const
{
    return mStatus;
}

QString Status::message() const
{
    return mMessage;
}

const char* Status::what() const noexcept
{
    return mMessageStdString.data();
}
