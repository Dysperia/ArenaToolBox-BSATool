#include <utility>

#include "Status.h"

//**************************************************************************
// Constructors
//**************************************************************************
Status::Status(const int &status, QString message):
    mStatus(status), mMessage(std::move(message))
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
