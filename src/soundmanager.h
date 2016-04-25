#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QString>
#include <QSound>

class SoundManager
{
public:
    SoundManager();
    static void playOn();
    static void playOff();
    static void playMiss();
private:
    static const QString dir;
    static const QString on;
    static const QString off;
    static const QString miss;
    static QString makeAbsolutePath(const QString &in);

};

#endif // SOUNDMANAGER_H
