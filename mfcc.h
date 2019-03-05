#ifndef MFCC_H
#define MFCC_H

#include <QObject>
#include <QVector>
#include <QMutableVectorIterator>

#include "pe_config.h"
#include "melfilterbank.h"

/*!
 * \brief Třída MFCC
 *
 * Třída obsahuje metody pro parametrizaci vstupních vektorů odhadů výkonových spekter. Využívá
 * třídy MelFilterBank, která obsahuje patřičné trojúhelníkové filtry. Kvůli konstrukci těchto filtrů
 * je třeba velikosti vstupních vektorů definovat okamžitě během konstrukce objektu. Při přijetí
 * vektorů jiné velikosti se pak emituje signál error.
 */
class MFCC : public QObject {
    Q_OBJECT

public:
    /*!
     * \brief MFCC Konstruktor třídy.
     * \param sampleRate Frekvence vzorkování parametrizovaného akustického signálu.
     * \param filtersCount Počet filtrů využitých při parametrizaci. Tato hodnota odpovídá počtu
     *                     počítaných MFC koeficientů.
     * \param espdSize Počet prvků vstupních vektorů odhadů výkonových spekter.
     * \param parent Ukazatel na rodiče objektu (kvůli dynamickému uvolnění).
     */
    explicit MFCC(int sampleRate, int filtersCount, int espdSize, QObject *parent = nullptr);

    /*!
     * \brief calculate Metoda vypočítá samotné MFC koeficienty podle zadaného vektoru odhadu výkonového
     *                  spektra. Pokud se počet přijatých prvků nerovná hodnotě m_espdSize, je emitován
     *                  signál error a je vrácen prázdný vektor. V případě neplatného počtu kepstrálních
     *                  koeficientů je vypočítáno všech m_filtersCount koeficientů.
     * \param espd Vstupní vektor odhadu výkonového spektra.
     * \param count Počet požadovaných kepstrálních koeficientů.
     * \return Vektor vypočítaných MFC koeficientů. Velikost tohoto vektoru odpovídá hodnotě m_filtersCount.
     */
    QVector<float> calculate(QVector<float> espd, int count);

private:
    int m_sampleRate;           //!< Frekvence vzorkování parametrizovaného signálu.
    int m_espdSize;             //!< Očekávaná velikost vektorů odhadu výkonové spektrální hustoty.
    int m_filtersCount;         //!< Počet použitých filtrů melovské banky.
    MelFilterBank m_filters;    //!< Objekt, který obsahuje samotné trojúhelníkové filtry.

    /*!
     * \brief getMelCoefs Tato metoda vypočítá melovské koeficienty vstupního odhadu výkonové spektrální
     *                    hustoty, tj. skalární součiny vektorů jednotlivých melovských filtrů a vstupního
     *                    vektoru.
     * \param espd Vstupní vektor odhadu výkonové spektrální hustoty.
     * \return Vektor melovských koeficientů.
     */
    QVector<float> calcMelCoefs(QVector<float> espd);

    /*!
     * \brief dct Metoda, která reprezentuje dopřednou disktrétní kosinovou transformaci, která se v této
     *            třídě využívá pro převod melovských koeficientů na kepstrální.
     * \param xi Vektor melovských koeficientů.
     * \param mth Index požadovaného koeficientu diskrétní kosinové transformace.
     * \return Hodnota koeficientu diskrétní kosinové transformace.
     */
    inline float dct(QVector<float> xi, int mth);

    /*!
     * \brief getNormFactor Získá normalizační faktor dle jeho definice.
     * \param i Parametr pro získání normalizačního faktoru.
     * \return Normalizační faktor.
     */
    inline float getNormFactor(int i);

signals:
    /*!
     * \brief error Signál, který je emitován při chybě.
     * \param message Popis chyby.
     */
    void error(QString message);
};

#endif
