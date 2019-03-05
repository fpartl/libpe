#ifndef SEGMENTRECOVER_H
#define SEGMENTRECOVER_H

#include <QCoreApplication>
#include <QObject>
#include <QVector>
#include <QMutableVectorIterator>
#include <QDataStream>
#include <QFile>
#include <QtMath>

#include "../pe_config.h"
#include "../fft.h"

/*!
 * \brief Struktura polar_cpx
 *
 * Představuje vyjádření polohy bodu v rovině užitím polárních souřadnic. Tato struktura se využívá
 * při vyjádření polohy konkrétního váhového koeficientu Fourierovy transformace v komplexní rovině.
 */
struct polar_cpx {
    float mag;      //!< Vzdálenost od počátku soustavy souřadné.
    float omega;    //!< Úhel spojnice počátku a bodu od osy kartézských souřadnic.
};

/*!
 * \brief Třída SegmentRecover
 *
 * Třída pomocí algoritmu Iterative Inverse Short-time Fourier Transform Magnitude algorithm dokáže obnovit
 * časový průbět segmentu z jeho odhadu výkonové spektrální hustoty. K jejímu správnému fungování je nutné,
 * aby ve složce, kde se vyskytuje finální spustitelný soubor, byl i soubor s bílým šumem o velikosti
 * m_segmentSize vzorků (např.: whitenoise_1024.raw).
 */
class SegmentRecover : public QObject {
    Q_OBJECT

public:
    /*!
     * \brief SegmentRecover Konstruktor třídy.
     * \param segmentSize Velikost reskonstruovaných segmentů.
     * \param recoverIterations Počet iterací algoritmu Iterative Inverse Short-time Fourier Transform
     *                          Magnitude algorithm.
     * \param parent Ukazatel na rodiče objektu (kvůli uvolňování).
     */
    explicit SegmentRecover(int segmentSize, int recoverIterations, QObject *parent = nullptr);

    /*!
     * \brief recover Metoda, která provádí samotou rekonstrukci časového průběhu segmentu z daného odhadu
     *                výkonové spektrální hustoty. Využívá algoritmu Iterative Inverse Short-time Fourier
     *                Transform Magnitude algorithm.
     *
     * \param espd Odhad výkonové spektrální hustoty, ze kterého bude rekonstruován segment signálu.
     * \return Vektor, který obsahuje prvky rekonstruovaného časového průběhu segmentu akustikého signálu.
     */
    QVector<float> recover(QVector<float> espd);

private:
    int m_segmentSize;              //!< Počet vzorků rekonstruovaných segmentů akustického signálu.
    int m_recoverIterations;        //!< Počet iterací algrogitmu Iterative Inverse Short-time Fourier Transform Magnitude algorithm
    FFT m_fft;                      //!< Objekt, který zabezpečuje výpočet Fourierovy transformace.
    QVector<float> m_whiteNoise;    //!< Vektro, který obsahuje načtený bílý šum.

    /*!
     * \brief loadWhiteNoise Metoda provádí načtení bílého šumu z externího souboru do vektoru m_whiteNoise,
     *                       který je hledán v adresáři se spustitelným souborem. Pokud tento soubor není nalezen,
     *                       emituje se chybový signál error. Metoda nijak nekontroluje vnitřní sturkturu zdrojového
     *                       souboru.
     */
    void loadWhiteNoise();

    /*!
     * \brief toPolar Metoda provede konverzi kartézských souřadnic na polární.
     * \param cpx Vektor kartézských souřadnic, určených ke konverzi.
     * \param pol_cpx Vektor polárních souřadnic, kam budou zapsány výsledky konverze.
     */
    void toPolar(QVector<kiss_fft_cpx> &cpx, QVector<polar_cpx> &pol_cpx);

    /*!
     * \brief toGauss Metoda provádí konverzi polárních souřadnic na kartézské.
     * \param pol_cpx Vektor polárních souřadnic, určených ke konverzi.
     * \param cpx Vektor kartézských souřadnic, kam budou zapsány výsledky konverze.
     */
    void toGauss(QVector<polar_cpx> &pol_cpx, QVector<kiss_fft_cpx> &cpx);

signals:
    /*!
     * \brief error Signál, který je emitován při chybě.
     * \param message Popis chyby.
     */
    void error(QString message);
};

#endif
