#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include "MenuBar.h"

/**
 * @brief The ToolBar class
 */
class ToolBar : public QToolBar
{
public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * @brief ToolBar constructor
     *
     * needs the menu bar to get the action to associate with the tool bar's items
     * @param menuBar the menu bar
     */
    ToolBar(MenuBar *menuBar);
};

#endif // TOOLBAR_H
