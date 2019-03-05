#ifndef HAMMINGWINDOW_H
#define HAMMINGWINDOW_H

#include <QObject>
#include <QtMath>

#include "windowfunction.h"

/*!
 * Konstanta se využívá při výpočtu Hammingova okna.
 */
#define ALPHA 0.53836

/*!
 * Konstanta se využívá při výpočtu Hammingova okna.
 */
#define BETA 0.46164

/*!
 * \brief Třída HammingWindow
 *
 * Třída je konkrétní implementací virtuální třídy WindowFunction. Třída reprezentuje
 * Hammingovu váhovou funkci.
 */
class HammingWindow : public WindowFunction {
    Q_OBJECT

public:
    /*!
     * \brief HammingWindow Konstruktor třídy.
     * \param windowSize Velikost vstupního segmentu.
     * \param parent Ukazatel na rodiče objektu (kvůli dynamickému uvolňování).
     */
    explicit HammingWindow(int windowSize, QObject *parent = nullptr);

private:
    /*!
     * \brief initWindow Konkrétní implementace virtuální metody initWindow. Pomocí
     *                   koeficientů ALPHA a BETA počítá průběh Hammingovy funkce.
     */
    void initWindow();
};

#endif
