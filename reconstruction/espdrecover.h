#ifndef ESPDRECOVER_H
#define ESPDRECOVER_H

#include <QObject>
#include <QVector>
#include <QMutableVectorIterator>
#include <QtMath>

#include "../melfilterbank.h"
#include "../pe_config.h"

/*!
 * \brief Třída ESPDRecover
 *
 * Třída obsahuje metody pro obnovu odhadu výkonové spektrální hustoty z daných mel-frekvenčních kesptrálních
 * koeficientů. Vzorkovací frekvence rekonstruovaného akustického signálu, počet vzorků odhahu výkonové spektrální
 * hustoty a počet použitých filtrů musí být určen již při konstrukci objektu.
 */
class ESPDRecover : public QObject {
    Q_OBJECT

public:
    /*!
     * \brief ESPDRecover Konstruktor třídy.
     * \param espdSize Počet vzorků odhadu výkonové spektrální hustoty.
     * \param filtersCount Počet filtrů použitých při parametrizaci.
     * \param sampleRate Vzorkovací frekvence obnovovaného signálu.
     * \param parent Ukazatel na rodičovský objekt (kvůli automatickému obnovování).
     */
    explicit ESPDRecover(int espdSize, int filtersCount, int sampleRate, QObject *parent = nullptr);

    /*!
     * \brief recover Metoda, která provádí somotnou rekonstrukci odhadu výkonové spektrální hustoty. Počet vstupních
     *                mel-frekvenčních koeficientů musí být v rozmezí od 1 do počtu filtrů použitých pro parametrizaci.
     *                Pokud je počet vstupních koeficientů menší než počet filtrů, zbývající pozice jsou při inverzní
     *                diskrétní kosinové transformaci rovny nulové hodnotě. Při chybém počtu vstupních koeficientů metoda
     *                vrací prázdný vektor a emituje signál error.
     * \param mfccs Vektor vstupních mel-frekvenčních koeficientů.
     * \return Vektor, který obsahuje vzorky rekonstrukovaného odhadu výkonové spektrální hustoty. V případě chyby je
     *         výsledný vektor prázdný.
     */
    QVector<float> recover(QVector<float> mfccs);

private:
    int m_espdSize;             //!< Počet vzorků odhadu výkonové spektrální hustoty.
    int m_filtersCount;         //!< Počet trojúhelníkových filtrů, použitých při parametrizaci.
    int m_sampleRate;           //!< Frekvence vzorkování rekonstruovaného signálu.
    MelFilterBank m_filters;    //!< Objekt, který obsahuje samotné trojúhelníkové filtry.

    /*!
     * \brief completeMFCCs Tato metoda je volána v případě, že počet vstupních kepstrálních koeficientů je menší než počet
     *                      trojůhelníkových filtrů. V takovém případě metoda zkopíruje daný vektor, který doplní nulami, aby
     *                      počet prvků vektoru byl rovný počtu filtrů.
     * \param mfccs Vektor kepstrálních koeficientů, jehož počet prvků je menší než počet filtrů.
     * \return Vektro MFC koeficientů doplněný nulami, tak aby velikost tototo vektoru byla rovna počtu filtrů.
     */
    QVector<float> completeMFCCs(QVector<float> mfccs);

    /*!
     * \brief recoverMelCoefs Metoda provede rekonstrukci melovských koeficientů z kepstrálních. Koeficienty jsou nejdříve
     *                        vyděleny normalizačním faktorem, následně je nad nimi provedena inverzní diskrétní kosinová
     *                        transformace a nakonec jsou tyto hodnoty dány jako argumenty exponenciální funkci o základu
     *                        Eulerovy hodnoty, která je inverzní funkcí k přirozenému logaritmu.
     * \param mfccs Mel-frekvenční kepstrální koficienty.
     * \return Rekonstruované melovské koeficietny, tj. váhy jednotlivých filtrů.
     */
    QVector<float> recoverMelCoefs(QVector<float> mfccs);

    /*!
     * \brief idct Metoda, která reprezentuje inverzní kosinovou transformaci, která se využivá k převodu kepstrálních
     *             koeficientů na melovské.
     * \param xi Vektor kepstrálních koeficientů.
     * \param mth Index požadovaného koeficientu inverzní disktrétní kosinové transformace.
     * \return Hodnota koeficientu inverzní diskrétní kosinové transformace.
     */
    inline float idct(QVector<float> xi, int mth);

    /*!
     * \brief getNormFactor Získá normalizační faktor dle jeho definice.
     * \param i Parametr pro získání normlizačního faktoru.
     * \return Normalizační faktor.
     */
    inline float getNormFactor(int i);

    /*!
     * \brief recESPDFromMelKoefs
     * \param mels
     * \return
     */
    QVector<float> recESPDFromMelKoefs(QVector<float> mels);

signals:
    /*!
     * \brief error Signál, který je emitován při chybě.
     * \param message Popis chyby.
     */
    void error(QString message);
};

#endif
