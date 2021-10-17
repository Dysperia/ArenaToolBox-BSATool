#include <QDir>
#include <configuration/ArchiveConfigurationLoader.h>
#include <log/Logger.h>


//**************************************************************************
// Constructors
//**************************************************************************
ArchiveConfigurationLoader::ArchiveConfigurationLoader() {
    updateConfigurationList();
}

//**************************************************************************
// Methods
//**************************************************************************
Status ArchiveConfigurationLoader::loadConfiguration(const QString &name) {
    ArchiveConfiguration archive;
    archive.setName(name);
    const Status &status = archive.loadFromFile();
    Logger::getInstance().logErrorOrInfo(status,QString("Configuration loaded : %1").arg(name));
    mCurrent = archive;
    return status;
}

Status ArchiveConfigurationLoader::updateConfigurationList() {
    QString dirPath("configuration/");
    QDir confDir(dirPath);
    if (!confDir.exists() && confDir.mkdir(dirPath)) {
        return Status(-1, QString("Could not access / create the configuration directory"));
    }
    QStringList fileList = confDir.entryList({"*" + ArchiveConfiguration::CONFIGURATION_FILE_EXT}, QDir::Filter::Files, QDir::SortFlag::Name);
    mConfigurationList.clear();
    for (auto& filename : fileList) {
        int idx = filename.lastIndexOf(ArchiveConfiguration::CONFIGURATION_FILE_EXT);
        mConfigurationList.push_back(filename.left(idx));
    }
    return Status(0);
}

//**************************************************************************
// Getters/setters
//**************************************************************************
const ArchiveConfiguration &ArchiveConfigurationLoader::getCurrent() const {
    return mCurrent;
}

const QStringList &ArchiveConfigurationLoader::getConfigurationList() const {
    return mConfigurationList;
}
