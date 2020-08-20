#include "FileType.h"

//**************************************************************************
// Constructors
//**************************************************************************
FileType::FileType() = default;

//**************************************************************************
// Methods
//**************************************************************************
QString FileType::getDescriptionForExtension(const FileType::Extension extension) {
    switch (extension) {
        case CFA:
            return "Animation";
        case IMG:
            return "Single image";
        case CIF:
            return "Image set";
        case INF:
            return "Encrypted text";
        case XFM:
            return "Low quality Midi audio";
        case XMI:
            return "High quality Midi audio";
        case VOC:
            return "Audio";
        case DFA:
            return "Image partly animated";
        case MIF:
            return "Interior map";
        case SET:
            return "Texture set";
        case TXT:
            return "Text file";
        case RMD:
            return "Exterior map";
        case UNKNOWN:
        default:
            return "Unknown file type";
    }
}

FileType::Extension FileType::getExtension(const BsaFile &file)
{
    if (file.getExtension().compare("CFA", Qt::CaseInsensitive) == 0) {
        return Extension::CFA;
    }
    else if (file.getExtension().compare("IMG", Qt::CaseInsensitive) == 0) {
        return Extension::IMG;
    }
    else if (file.getExtension().compare("CIF", Qt::CaseInsensitive) == 0) {
        return Extension::CIF;
    }
    else if (file.getExtension().compare("INF", Qt::CaseInsensitive) == 0) {
        return Extension::INF;
    }
    else if (file.getExtension().compare("XFM", Qt::CaseInsensitive) == 0) {
        return Extension::XFM;
    }
    else if (file.getExtension().compare("XMI", Qt::CaseInsensitive) == 0) {
        return Extension::XMI;
    }
    else if (file.getExtension().compare("VOC", Qt::CaseInsensitive) == 0) {
        return Extension::VOC;
    }
    else if (file.getExtension().compare("DFA", Qt::CaseInsensitive) == 0) {
        return Extension::DFA;
    }
    else if (file.getExtension().compare("MIF", Qt::CaseInsensitive) == 0) {
        return Extension::MIF;
    }
    else if (file.getExtension().compare("SET", Qt::CaseInsensitive) == 0) {
        return Extension::SET;
    }
    else if (file.getExtension().compare("TXT", Qt::CaseInsensitive) == 0) {
        return Extension::TXT;
    }
    else if (file.getExtension().compare("RMD", Qt::CaseInsensitive) == 0) {
        return Extension::RMD;
    }
    else {return Extension::UNKNOWN;}
}
