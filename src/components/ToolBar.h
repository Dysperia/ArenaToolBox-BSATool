#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include "MenuBar.h"

class ToolBar : public QToolBar
{
public:
    // Constructors
    ToolBar(MenuBar* menuBar);
};

#endif // TOOLBAR_H
