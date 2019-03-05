#ifndef AUDIOSCALER_H
#define AUDIOSCALER_H

#include <QObject>
#include <QVector>
#include <limits>
#include <cmath>

#include "../pe_config.h"

/*!
 * \brief Třída AudioScaler
 *
 * Třída poskytuje mechanismus pro přeškálování akustických signálů, jejihž vzorky jsou uloženy
 * jako float do signálu s datovým typem sample. Implicitní konverze by mohla způsobit přetečení
 * vzorků typu sample, proto je nutné v případě potřeby původní vzorky přeškálovat.
 */
class AudioScaler : public QObject {

public:
    /*!
     * \brief AudioScaler Konstruktor třídy.
     * \param parent Ukazatel na rodičovský objekt (kvůli dynamickému uvolnění).
     */
    explicit AudioScaler(QObject *parent = nullptr);

    /*!
     * \brief scale Metoda provádí samotné škálování daného segmentu se vzorky v datovém typu
     *              float do nově vytvořeného segmentu s prvky typu sample.
     * \param segment Segment, jehož vzorky jsou určeny ke škálování.
     * \return Přeškálovaný segment, jehož vzorky jsou typu sample.
     */
    QVector<sample> scale(const QVector<float> &segment);

private:
    /*!
     * \brief findScaleFactor Metoda projde daný segment, nejde v něm vzorek s největší aboslutní hodnotou
     *                        a pokud hodnota tohoto vzorku přesahuje rozsah typu sample, je stanoven výsledný
     *                        škálovací faktor.
     * \param segment Segment, pro který se bude hledat škálovací faktor.
     * \return Hodnota škálovacího faktoru.
     */
    float findScaleFactor(const QVector<float> &segment);
};

#endif
