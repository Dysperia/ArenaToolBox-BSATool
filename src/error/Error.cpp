#include "Error.h"

//**************************************************************************
// Constructors
//**************************************************************************
Error::Error(const int &status, const QString &message):
    mStatus(status), mMessage(message)
{

}

//**************************************************************************
// Getters/setters
//**************************************************************************
int Error::status() const
{
    return mStatus;
}

QString Error::message() const
{
    return mMessage;
}
