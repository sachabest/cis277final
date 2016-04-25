#include "soundmanager.h"
#include <QFileInfo>
#include <QDir>

const QString SoundManager::dir = "://sounds/";
const QString SoundManager::on = "beep_on.wav";
const QString SoundManager::off = "beep_off.wav";
const QString SoundManager::miss = "beep_miss.wav";

QString SoundManager::makeAbsolutePath(const QString &in) {
    QFileInfo fileInfo(in);

    if (fileInfo.isRelative()) {
        //  we assume one of our own sound files in a relative path
        fileInfo.setFile(SoundManager::dir + in);
        fileInfo.makeAbsolute();
    }

    if (!fileInfo.exists()) {
        throw;
    }
    return QDir::toNativeSeparators(fileInfo.filePath());
}

SoundManager::SoundManager()
{

}

void SoundManager::playOn() {
    QSound::play(makeAbsolutePath(on));
}

void SoundManager::playOff() {
    QSound::play(makeAbsolutePath(off));
}

void SoundManager::playMiss() {
    QSound::play(makeAbsolutePath(miss));
}
