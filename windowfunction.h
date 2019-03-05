#ifndef WINDOWFUNCTION_H
#define WINDOWFUNCTION_H

#include <QObject>
#include <QVector>

#include "pe_config.h"

/*!
 * \brief Třída WindowFunction
 *
 * Třída reprezentuje obecnou váhovací funkci pro normalizaci vstupních segmentů akustického signálu.
 */
class WindowFunction : public QObject {
    Q_OBJECT

public:
    /*!
     * \brief AudioNormalizer Konstruktor třídy.
     * \param windowSize Počet vzorků normalizovaných vstupních segmentů.
     * \param parent Ukazatel na rodiče (kvůli automatické destrukci objektu).
     */
    explicit WindowFunction(int windowSize, QObject *parent = nullptr);

    /*!
     * \brief ~AudioNormalizer Destruktor třídy.
     */
    virtual ~WindowFunction() = default;

    /*!
     * \brief normalize Metoda provede normalizaci vstupního segmetu, tj. vynásobí všechny prvky daného
     *                  vektoru prvky váhovacího okna. Výsledek této operace uloží do nového vektoru, který
     *                  vrátí. V případě, že počet vzorků váhovacího okna a vstupního signálu je různý, metoda
     *                  vrátí prázdný vektor.
     * \param segment Vektor, který obsahuje vzorky segmentu pro normalizaci.
     * \return Normalizovaný segment.
     */
    QVector<float> normalize(const QVector<sample> &segment);

    /*!
     * \brief denormalize Metoda provede denormalizaci segmentu, tj. vydělí všechny prvky daného vektoru
     *                    prvky váhovacího okna. Výsledek operace uloží do nového vektoru, který vrátí.
     *                    V případě, že počet vzorků výhovacího okna a vstupního singálu je různů, metoda
     *                    vrátí prázdný vektor.
     * \param segment Vektor, který obsahuje vzorky segmentu pro denormalizaci.
     * \return Denormalizovaný vektor.
     */
    QVector<float> denormalize(const QVector<float> &segment);

protected:
    /*!
     * \brief initWindow Virtuální metoda, jejíž úlohou je vypočítat vzorky daného normalizačního okna
     *                   (Hamming, Blackman, Triangular). Účelem funkce není alokace potřebného místa,
     *                   ale pouhý výpočet prvků vektoru m_window.
     */
    virtual void initWindow() = 0;

    /*!
     * \brief window Vektor, který obsahuje vzorky váhovacího okna.
     */
    QVector<float> m_window;

signals:
    /*!
     * \brief error Signál, který je emitován při chybě.
     * \param message Popis chyby.
     */
    void error(QString message);
};

#endif
