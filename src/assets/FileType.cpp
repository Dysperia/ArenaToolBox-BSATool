#include "FileType.h"

//**************************************************************************
// Constructors
//**************************************************************************
FileType::FileType()
{

}

//**************************************************************************
// Methods
//**************************************************************************
QString FileType::getDescriptionFromExtension(const QString &ext)
{
    // fileType by extension
    static const QHash<QString, QString> fileTypes{
        {"CFA", "Animation"},
        {"IMG", "Single image"},
        {"CIF", "Image set"},
        {"INF", "Encrypted text"},
        {"XFM", "Low quality Midi audio"},
        {"XMI", "High quality Midi audio"},
        {"VOC", "Audio"},
        {"DFA", "Image partly animated"},
        {"MIF", "Interior map"},
        {"SET", "Texture set"},
        {"RMD", "Exterior map"},
    };
    return fileTypes.value(ext.toUpper().trimmed());
}
