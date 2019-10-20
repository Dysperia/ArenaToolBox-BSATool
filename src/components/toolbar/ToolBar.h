#ifndef BSATOOL_TOOLBAR_H
#define BSATOOL_TOOLBAR_H

#include <QToolBar>
#include "components/menu/MenuBar.h"

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
    explicit ToolBar(MenuBar *menuBar);
};

#endif // BSATOOL_TOOLBAR_H
