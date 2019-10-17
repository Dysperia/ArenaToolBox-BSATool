#include "ConsoleDock.h"
#include "log/Logger.h"

//**************************************************************************
// Constructors
//**************************************************************************
ConsoleDock::ConsoleDock(QWidget *parent): QDockWidget(parent)
{
    setWindowTitle("Console");
    setAllowedAreas(Qt::BottomDockWidgetArea);
    setFeatures(QDockWidget::DockWidgetClosable);
    setWidget(Logger::getInstance().textWidget());
}
