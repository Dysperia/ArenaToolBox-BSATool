#include "Status.h"

//**************************************************************************
// Constructors
//**************************************************************************
Status::Status(const int &status, const QString &message):
    mStatus(status), mMessage(message)
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
