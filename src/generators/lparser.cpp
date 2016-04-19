#include "lparser.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>
#include <iostream>

LParser::LParser() {}

static float drawSize = 10.0f;
static const float delta = 22.5f;
static float cylinderRadius = 10.0f;
static const float shrinkFactor = 1.5f;
static float storedRadius = 5.0f;
static float leafRadius = .50f;

/**
 * @brief LParser::makeTree a convenience method for drawing trees using the static methods expand() and
 * createDrawables().
 * @return a QVector containing pairs of transformations and booleans in LPair_t structs.
 */
QVector<LPair_t> LParser::makeBrush() {
    QString axiom = "A";
    QVector<QString> a, b, c, d;
    a.push_back("[&FLA]/////[&FLA]///////`[&FLA]");
    b.push_back("S ///// F");
    c.push_back("F");
    d.push_back("[Q^^Q][Q\\\\Q]");
    QMap<QChar, QVector<QString> > productions {
        {'A', a},
        {'F', b},
        {'S', c},
        {'L', d}};
    return createDrawables(expand(productions, 7, axiom));
}

/**
 * @brief LParser::makeTree a convenience method for drawing trees using the static methods expand() and
 * createDrawables().
 * @return a QVector containing pairs of transformations and booleans in LPair_t structs.
 */
QVector<LPair_t> LParser::makeTree() {
    QString axiom = "A";
    QVector<QString> a, b, c, d;
    a.push_back("[&FL!A]/////`[&FL!A]///////`[&FL!A]");
    b.push_back("S ///// F");
    c.push_back("F L");
    d.push_back("[```^^{-f+f+f-|-f+f+f}]");
    QMap<QChar, QVector<QString> > productions {
        {'A', a},
        {'F', b},
        {'S', c},
        {'L', d}};
    return createDrawables(expand(productions, 7, axiom));
}

/**
 * @brief LParser::makeCarrieTree
 * @return
 */
QVector<LPair_t> LParser::makeCarrieTree() {
    QString axiom = "A";
    QVector<QString> a, b, c, d;
    a.push_back("----ffffB");
    b.push_back("q //// qAB");
    QMap<QChar, QVector<QString> > productions {
        {'A', a},
        {'B', b},
        {'S', c},
        {'L', d}};
    return createDrawables(expand(productions, 7, axiom));
}

/**
 * @brief LParser::createDrawables - creates a vector of LPair_t structs that contain local-space transforms
 * as well as a boolean value that describes whether or not a block should be drawn at that specific transform
 * component. This is useful for chaining together transformations efficiently.
 * @param pattern - the input grammar to create shapes from
 * @return a QVector containing pairs of transformations and booleans in LPair_t structs.
 */
QVector<LPair_t> LParser::createDrawables(const QString &pattern) {
    srand(time(NULL));
    QVector<LPair_t> v;
    for (int i = 0; i < pattern.length(); i++) {
        QChar letter = pattern.at(i);
        LPair_t p;
        p.draw = false;
        if (letter.toLower() == 'f') {
            p.draw = true;
            p.t = glm::translate(glm::mat4(), glm::vec3(0, 0, drawSize));
        } else if (letter == '+') {
            p.t = glm::rotate(glm::mat4(), delta, glm::vec3(0, 1, 0));
        } else if (letter == '-') {
            p.t = glm::rotate(glm::mat4(), -1 * delta, glm::vec3(0, 1, 0));
        } else if (letter == '&') {
            p.t = glm::rotate(glm::mat4(), delta, glm::vec3(1, 0, 0));
        } else if (letter == '^') {
            p.t = glm::rotate(glm::mat4(), -1 * delta, glm::vec3(1, 0, 0));
        } else if (letter == '\\') {
            p.t = glm::rotate(glm::mat4(), delta, glm::vec3(1, 0, 0));
        } else if (letter == '/') {
            p.t = glm::rotate(glm::mat4(), -1 * delta, glm::vec3(1, 0, 0));
        } else if (letter == '|') {
            p.t = glm::rotate(glm::mat4(), 180.0f, glm::vec3(0, 1, 0));
        } else if (letter.toLower() == 'q') {
            // print leaf material
            p.t = glm::translate(glm::mat4(), glm::vec3(0, 0, 1.0f + cylinderRadius));
            float random = ((float) rand()) / (float) RAND_MAX;
            float rot = random * 80.0f;
            random = ((float) rand()) / (float) RAND_MAX;
            float rx = random * 0.5;
            random = ((float) rand()) / (float) RAND_MAX;
            float ry = random * 0.5;
            random = ((float) rand()) / (float) RAND_MAX;
            float rz = random * 0.5;
            p.t = glm::rotate(p.t, rot, glm::vec3(rx, ry, rz));
            // Make Disk here
            p.draw = true;
            v.push_back(p);
            p.t = glm::rotate(glm::mat4(), rot, glm::vec3( -1 * rx, -1 * ry, -1 *rz));
            p.t = glm::rotate(p.t, 180.0f, glm::vec3(0, 1, 0));
            p.t = glm::translate(p.t, glm::vec3(0, 0, 1.0f + cylinderRadius));
            p.draw = false;
            // print bark material
            v.push_back(p);
        } else if (letter == '[') {
            cylinderRadius /= shrinkFactor;
            p.t = glm::translate(glm::mat4(), glm::vec3(0, 0, -1 * cylinderRadius));
        } else if (letter == ']') {
            cylinderRadius *= shrinkFactor;
        } else if (letter == '{' || letter == '<') {
            storedRadius = cylinderRadius;
            cylinderRadius  = leafRadius;
            if (letter == '{') {
                drawSize = 0.7f;
                // print leaf material
            } else {
                // print flower material
            }
        } else if (letter == '}' || letter == '>') {
            cylinderRadius = storedRadius;
            drawSize = 1.0f;
            // print bark material
        } else {
            // NOOP
        }
        v.push_back(p);
    }
    return v;
}

/**
 * @brief LParser::expand turns a set of productions and an axiom into a derivative string using CFG rules
 * @param productions - a mapping between chars and a vector of strings that they expand to (nonterminals)
 * @param iterations - the number of iterations (max) to expand
 * @param axiom - the initial string to expand
 * @return a fully expanded version of the grammar in string form
 */
QString LParser::expand(const QMap<QChar, QVector<QString> > &productions, const int &iterations, QString &axiom) {
    srand(time(NULL));
    QString &current = axiom;
    QString next;
    for (int i = 0; i < iterations; i++) {
        for (int letter = 0; letter < axiom.length(); letter++) {
            bool found;
            for (QChar key : productions.keys()) {
                QVector<QString> toReplace = productions[key];
                QString replacement;
                if (current.at(letter) == key) {
                    // found a replacement
                    if (toReplace.size() > 1) {
                        float random = ((float) rand()) / (float) RAND_MAX;
                        int randomIndex = (int) random * toReplace.size();
                        replacement = toReplace[randomIndex];
                    } else {
                        replacement = toReplace[0];
                    }
                    next.append(replacement);
                    found = true;
                    break;
                }
            }
            if (!found) {
                next.append(QString(current.at(letter)));
            }
        }
        current = next;
        next = "";
    }
    return current;
}
