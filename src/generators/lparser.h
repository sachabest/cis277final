#ifndef LPARSER_H
#define LPARSER_H

#include <QString>
#include <QMap>
#include <QVector>
#include <QChar>
#include "openGL/drawable.h"
#include "scene/geometry/segment.h"
#include "scene/geometry/cylinder.h"
#include "scene/geometry/cube.h"

struct LPair_t {
    bool draw;
    glm::mat4 t;
};

class LParser
{
public:
    LParser();
    static QVector<LPair_t> makeTree();
    static QVector<LPair_t> makeBrush();
    static QString expand(const QMap<QChar, QVector<QString> > &productions, const int &iterations, QString &axiom);
    static QVector<LPair_t> createDrawables(const QString &pattern);
    static QVector<LPair_t> makeCarrieTree();

};

#endif // LPARSER_H
