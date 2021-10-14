#include "FileConfiguration.h"

//**************************************************************************
// Methods
//**************************************************************************
void FileConfiguration::read(const QJsonObject &json)
{
    if (json.contains("filename") && json["filename"].isString()) {
        mFilename = json["filename"].toString();
    }
    if (json.contains("paletteFile") && json["paletteFile"].isString()) {
        mPaletteFile = json["paletteFile"].toString();
    }
    if (json.contains("customPalette") && json["customPalette"].isBool()) {
        mCustomPalette = json["customPalette"].toBool();
    }
    if (json.contains("width") && json["width"].isDouble()) {
        mWidth = json["width"].toInt();
    }
    if (json.contains("height") && json["height"].isDouble()) {
        mHeight = json["height"].toInt();
    }
    if (json.contains("customSize") && json["customSize"].isBool()) {
        mCustomSize = json["customSize"].toBool();
    }
    if (json.contains("noHeader") && json["noHeader"].isBool()) {
        mNoHeader = json["noHeader"].toBool();
    }
}

void FileConfiguration::write(QJsonObject &json) const
{
    json["filename"] = mFilename;
    json["paletteFile"] = mPaletteFile;
    json["customPalette"] = mCustomPalette;
    json["width"] = mWidth;
    json["height"] = mHeight;
    json["customSize"] = mCustomSize;
    json["noHeader"] = mNoHeader;
}

//**************************************************************************
// Getters/setters
//**************************************************************************
const QString &FileConfiguration::getFilename() const {
    return mFilename;
}

void FileConfiguration::setFilename(const QString &filename) {
    FileConfiguration::mFilename = filename;
}

const QString &FileConfiguration::getPaletteFile() const {
    return mPaletteFile;
}

void FileConfiguration::setPaletteFile(const QString &paletteFile) {
    FileConfiguration::mPaletteFile = paletteFile;
}

bool FileConfiguration::isCustomPalette() const {
    return mCustomPalette;
}

void FileConfiguration::setCustomPalette(bool customPalette) {
    FileConfiguration::mCustomPalette = customPalette;
}

quint16 FileConfiguration::getWidth() const {
    return mWidth;
}

void FileConfiguration::setWidth(quint16 width) {
    FileConfiguration::mWidth = width;
}

quint16 FileConfiguration::getHeight() const {
    return mHeight;
}

void FileConfiguration::setHeight(quint16 height) {
    FileConfiguration::mHeight = height;
}

bool FileConfiguration::isCustomSize() const {
    return mCustomSize;
}

void FileConfiguration::setCustomSize(bool customSize) {
    FileConfiguration::mCustomSize = customSize;
}

bool FileConfiguration::isNoHeader() const {
    return mNoHeader;
}

void FileConfiguration::setNoHeader(bool noHeader) {
    FileConfiguration::mNoHeader = noHeader;
}
