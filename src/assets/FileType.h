#ifndef FILETYPE_H
#define FILETYPE_H

#include <QHash>
#include "../bsa/BsaFile.h"

/**
 * @brief class describing the file types commonly encountered in an archive
 */
class FileType
{
public:
    //**************************************************************************
    // Enumeration
    //**************************************************************************
    /**
     * enum describing some known file extension encountered in bsa archive
     */
    enum Extension {CFA, IMG, CIF, INF, XFM, XMI, VOC, DFA, MIF, SET, RMD, UNKNOWN};

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
    static QString getDescriptionForExtension(FileType::Extension extension);
    /**
     * @brief return the type of a file
     *
     * @param file the file
     * @return the extension if known, UNKNOWN otherwise
     */
    static Extension getExtension(const BsaFile &file);
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
