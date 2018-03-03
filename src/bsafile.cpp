#include <algorithm>
#include "bsafile.hpp"

//================================================================================
BSAFile *BSAFile::getInstance()
{
    static BSAFile instance;
    return &instance;
}

//================================================================================
BSAFile::BSAFile() : modifiedSize(0), isOpened(false), isModified(false), isSaved(false)
{
    this->fileTypeMap["CFA"] = "Animation";
    this->fileTypeMap["IMG"] = "Single image";
    this->fileTypeMap["CIF"] = "Animation";
    this->fileTypeMap["INF"] = "Encrypted text";
    this->fileTypeMap["XFM"] = "Low quality Midi audio";
    this->fileTypeMap["XMI"] = "High quality Midi audio";
    this->fileTypeMap["VOC"] = "Audio";
    this->fileTypeMap["DFA"] = "Image partly animated";
    this->fileTypeMap["MIF"] = "Interior map";
    this->fileTypeMap["SET"] = "Texture set";
    this->fileTypeMap["RMD"] = "Exterior map";
}

//================================================================================
// Return 0 all good, 1 bad bsa format or corrupted bsa, 2 error while opening
int BSAFile::openArchive(const std::string &filePath)
{
    // Open the archive for reading
    bsaIStream.open(filePath.c_str(), std::ios_base::in | std::ios_base::binary);

    if (bsaIStream.is_open())
    {
        // Store the file name and path
        archiveFilePath = filePath;
        size_t fileNamePosition(archiveFilePath.find_last_of("/\\"));
        archiveFileName = archiveFilePath.substr(fileNamePosition+1);

        // Get the archive's total size and the number of file (from the header)
        bsaIStream.seekg(0, std::ios_base::end);
        bsaSize = bsaIStream.tellg();
        bsaIStream.seekg(0);
        bsaIStream.read(reinterpret_cast<char*>(&fileNumber), 2);

        // Construct the index table
        bsaIStream.seekg(bsaSize - 18*fileNumber);
        fileNameBuffer.resize(14*fileNumber);
        fileTable.resize(fileNumber);

        for (size_t i(0); i < fileNumber; i++)
        {
            bsaIStream.read(&fileNameBuffer[14*i], 14);

            FileStruct &fs = fileTable[i];
            fs.newFilePath = "";
            fs.newFileName = "";
            fs.updateFilePath = "";
            fs.isDeleted = false;
            fs.isNew = false;
            fs.isUpdated = false;
            fs.fileName = &fileNameBuffer[14*i];
            fileMap[fs.fileName] = i;
            bsaIStream.read(reinterpret_cast<char*>(&fs.fileSize), 4);
            if (i == 0)
            {
                fs.fileOffset = 2;
            }
            else
            {
                fs.fileOffset = fileTable[i-1].fileOffset + fileTable[i-1].fileSize;
            }
        }
        if ((18*fileNumber + fileTable[fileNumber-1].fileSize + fileTable[fileNumber-1].fileOffset) != bsaSize)
        {
            // BSA Archive is either corrupted or not a Arena BSA Archive
            isOpened = false;
            clear();
            return 1;
        }
        else
        {
            // All good
            isOpened = true;
            return 0;
        }
    }
    else
    {
        // Error while opening BSA Archive for reading
        isOpened = false;
        clear();
        return 2;
    }
}

//================================================================================
std::string BSAFile::getArchiveFileName() const
{
    return archiveFileName;
}

//================================================================================
std::ifstream *BSAFile::getStream()
{
    return &bsaIStream;
}

//================================================================================
std::string BSAFile::getArchiveFilePath() const
{
    return archiveFilePath;
}

//================================================================================
bool BSAFile::getIsOpened() const
{
    return isOpened;
}

//================================================================================
bool BSAFile::getIsModified() const
{
    return isModified;
}

//================================================================================
bool BSAFile::getIsSaved() const
{
    return isSaved;
}

//================================================================================
uint16_t BSAFile::getFileNumber() const
{
    if (isOpened)
    {
        return fileNumber;
    }
    else
    {
        return 0;
    }
}

//================================================================================
size_t BSAFile::getArchiveSize() const
{
    return bsaSize;
}

//================================================================================
std::string BSAFile::getFileName(int index) const
{
    return fileTable[index].fileName;
}

//================================================================================
size_t BSAFile::getFileSize(int index) const
{
    return fileTable[index].fileSize;
}

//================================================================================
uint32_t BSAFile::getFileOffset(int index) const
{
    return fileTable[index].fileOffset;
}

//================================================================================
// Return an upper file extension
std::string BSAFile::getFileExtension(int index)
{
    const std::string filename = fileTable[index].fileName;
    size_t lastPointPosition = filename.find_last_of(".");
    if (lastPointPosition == std::string::npos)
    {
        return "";
    }
    else
    {
        std::string ext = filename.substr(lastPointPosition+1);
        std::string upperExt(ext);
        std::transform(ext.begin(), ext.end(), upperExt.begin(), toupper);
        return upperExt;
    }
}

//================================================================================
// Get file type associated with a given file extension
std::string BSAFile::getFileType(const std::string &ext)
{
    for (std::map<std::string, std::string>::iterator it=fileTypeMap.begin(); it!=fileTypeMap.end(); ++it)
    {
        std::string upperExt(ext);
        std::transform(ext.begin(), ext.end(), upperExt.begin(), toupper);
        if (upperExt == it->first)
        {
            return it->second;
        }
    }
    std::string ret = "Unknown file type";
    return ret;
}

//================================================================================
// Get the index number of a given file name
int BSAFile::getIndex(const std::string &fileName)
{
    return fileMap[fileName];
}

//================================================================================
// Clear bsa data to prepare a new bsa load or a new creation
void BSAFile::clear()
{
    bsaIStream.close();
    archiveFileName.clear();
    archiveFilePath.clear();
    fileNumber = 0;
    bsaSize = 0;
    modifiedSize = 0;
    fileNameBuffer.clear();
    fileTable.clear();
    fileMap.clear();
    isOpened = false;
    isModified = false;
    isSaved = false;
}

//================================================================================
// Extract raw file from archive
// return 0 all good, 1 error while opening ofstream or cannot extract new file
int BSAFile::extractFile(const std::string &filePath, int index)
{
    if (getIsFileNew(index) == false)
    {
        std::ofstream OFStream;
        OFStream.open(filePath.c_str(), std::ios_base::out | std::ios_base::binary);
        size_t fileSize = fileTable[index].fileSize;
        char *data = new char[fileSize];
        bsaIStream.seekg(fileTable[index].fileOffset, std::ios_base::beg);
        bsaIStream.read(data, fileSize);
        if (OFStream.is_open())
        {
            OFStream.write(data, fileSize);
            delete[] data;
            return 0;
        }
        else
        {
            delete[] data;
            return 1;
        }
    }
    else
    {
        return 1;
    }
}

//================================================================================
// Update a file in archive data with an other
void BSAFile::updateFile(int index, const std::string &filePath)
{
    FileStruct &fs = fileTable[index];
    fs.isUpdated = true;
    fs.updateFilePath = filePath;
    isModified = true;
    std::ifstream ifstream;
    ifstream.open(filePath, std::ios_base::in | std::ios_base::binary);
    if (ifstream.is_open())
    {
        int fileSize = fs.fileSize;
        ifstream.seekg(0, std::ios_base::end);
        fs.updatedSize = ifstream.tellg();
        modifiedSize += fs.updatedSize - fileSize;
    }
}

//================================================================================
// Delete a file in archive data
void BSAFile::deleteFile(int index)
{
    FileStruct &fs = fileTable[index];
    fs.isDeleted = true;
    isModified = true;
    modifiedSize -= fs.fileSize;
}

//================================================================================
// Add a file to archive data
void BSAFile::addFile(const std::string &filePath)
{
    isModified = true;
    fileNumber += 1;
    fileTable.resize(fileNumber);
    FileStruct &fs = fileTable[fileNumber - 1];
    int fileNamePos = filePath.find_last_of("/\\");
    std::string fileName = filePath.substr(fileNamePos + 1);
    std::ifstream ifstream;
    ifstream.open(filePath, std::ios_base::in | std::ios_base::binary);
    if (ifstream.is_open())
    {
        ifstream.seekg(0, std::ios_base::end);
        fs.fileSize = ifstream.tellg();
    }
    fs.newFileName = fileName;
    fs.fileName = fs.newFileName.c_str();
    fs.fileOffset = 0;
    fs.isDeleted = false;
    fs.isNew = true;
    fs.isUpdated = false;
    fs.newFilePath = filePath;
    fs.updateFilePath = "";
    fileMap[fileName] = fileNumber - 1;
    modifiedSize += fs.fileSize;
}

//================================================================================
// Cancel delete file to archive data
void BSAFile::cancelDeleteFile(int index)
{
    FileStruct &fs = fileTable[index];
    fs.isDeleted = false;
    modifiedSize += fs.fileSize;
}

//================================================================================
// Cancel update file to archive data
void BSAFile::cancelUpdateFile(int index)
{
    FileStruct &fs = fileTable[index];
    fs.isUpdated = false;
    modifiedSize += fs.fileSize - fs.updatedSize;
    fs.updateFilePath = "";
    fs.updatedSize = 0;
}

//================================================================================
int BSAFile::getArchiveModifiedSize()
{
    return modifiedSize;
}

//================================================================================
bool BSAFile::getIsFileDelete(int index)
{
    return fileTable[index].isDeleted;
}

//================================================================================
bool BSAFile::getIsFileNew(int index)
{
    return fileTable[index].isNew;
}

//================================================================================
bool BSAFile::getIsFileUpdated(int index)
{
    return fileTable[index].isUpdated;
}

//================================================================================
// Return the updated file size or current file size if not new
int BSAFile::getUpdatedFileSize(int index)
{
    return fileTable[index].updatedSize;
}

//================================================================================
// create a new empty bsa archive
void BSAFile::createArchive()
{
    clear();
    archiveFileName = "New BSA";
    isOpened = true;
}

//================================================================================
// Return the file path of a new file, or empty string
std::string BSAFile::getNewFilePath(int index)
{
    return fileTable[index].newFilePath;
}

//================================================================================
// Return the file path of an updated file, or empty string
std::string BSAFile::getUpdatedFilePath(int index)
{
    return fileTable[index].updateFilePath;
}

//================================================================================
/* Save the current data to a BSA archive. Don't forget to reload the saved archive
 * in order to update all the data and have a cleaned window
 * Return 0: ok, 1: stop because file updated or new could not be opened for reading,
 * 2: stop because save path could not be opened for writing,
 * 3: no write because nbr of delete file = total nbr of file
 */
int BSAFile::saveArchiveToBSA(const std::string &filePath)
{
    uint16_t nbrSavedFile = fileNumber;
    for (size_t i(0); i<fileNumber; i++)
    {
        if (getIsFileDelete(i))
        {
            nbrSavedFile --;
        }
    }
    if (nbrSavedFile == 0)
    {
        return 3;
    }
    else
    {
        std::ofstream saveStream;
        saveStream.open(filePath, std::ios_base::out | std::ios_base::binary);
        if (saveStream.is_open())
        {
            // write header
            saveStream.write(reinterpret_cast<char*>(&nbrSavedFile), 2);
            // write file(s)
            for (size_t i(0); i<fileNumber; i++)
            {
                if (getIsFileDelete(i) == false)
                {
                    if (getIsFileUpdated(i))
                    {
                        std::string sourceFilePath = getUpdatedFilePath(i);
                        std::ifstream ifstream;
                        ifstream.open(sourceFilePath, std::ios_base::in | std::ios_base::binary);
                        if (ifstream.is_open())
                        {
                            size_t updateFileSize = getUpdatedFileSize(i);
                            char *data = new char[updateFileSize];
                            ifstream.read(data, updateFileSize);
                            saveStream.write(data, updateFileSize);
                            delete[] data;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else if (getIsFileNew(i))
                    {
                        std::string sourceFilePath = getNewFilePath(i);
                        std::ifstream ifstream;
                        ifstream.open(sourceFilePath, std::ios_base::in | std::ios_base::binary);
                        if (ifstream.is_open())
                        {
                            size_t newFileSize = getFileSize(i);
                            char *data = new char[newFileSize];
                            ifstream.read(data, newFileSize);
                            saveStream.write(data, newFileSize);
                            delete[] data;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        size_t fileSize = getFileSize(i);
                        char *data = new char[fileSize];
                        bsaIStream.seekg(getFileOffset(i), std::ios_base::beg);
                        bsaIStream.read(data, fileSize);
                        saveStream.write(data, fileSize);
                        delete[] data;
                    }
                }
            }
            // write file table
            for (size_t i(0); i<fileNumber; i++)
            {
                if (getIsFileDelete(i) == false)
                {
                    std::string fileName = getFileName(i);
                    saveStream.write(fileName.c_str(), fileName.size());
                    for (size_t j(0); j < 14-fileName.size(); j++)
                    {
                        saveStream.write("\0", 1);
                    }
                    uint32_t fileSize;
                    if (getIsFileUpdated(i))
                    {
                        fileSize = getUpdatedFileSize(i);
                    }
                    else
                    {
                        fileSize = getFileSize(i);
                    }
                    saveStream.write(reinterpret_cast<char*>(&fileSize), 4);
                }
            }
            return 0;
        }
        else
        {
            return 2;
        }
    }
}
