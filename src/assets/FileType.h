#ifndef FILETYPE_H
#define FILETYPE_H

#include <QHash>

/**
 * @brief class describing the file types commonly encountered in an archive
 */
class FileType
{
public:
    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * @brief get the description from a file extension.
     *
     * The search is not case-sensitive. If the extension is unknown, an empty
     * string is return
     * @param ext extension
     * @return the description
     */
    static QString getDescriptionFromExtension(const QString &ext);
private:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * @brief Constructor
     */
    FileType();
};

#endif // FILETYPE_H
