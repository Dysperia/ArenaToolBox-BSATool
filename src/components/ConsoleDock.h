#ifndef CONSOLEDOCK_H
#define CONSOLEDOCK_H

#include <QDockWidget>

/**
 * @brief DockWidget containing the console logging the application events
 * @see Logger
 */
class ConsoleDock : public QDockWidget
{
public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * @brief Constructor
     * @param parent parent of the component
     */
    explicit ConsoleDock(QWidget *parent = Q_NULLPTR);
};

#endif // CONSOLEDOCK_H
