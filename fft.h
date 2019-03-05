#ifndef FFT_H
#define FFT_H

#include <QObject>
#include <QVector>
#include <QtMath>
#include <QMutableVectorIterator>
#include <QDebug>

#include "pe_config.h"

#include "kiss_fft//kiss_fft.h"
#include "kiss_fft/kiss_fftr.h"

/*!
 * \brief Třída FFT
 *
 * Tato třída reprezentuje dopřednou diskrétní Fourierovu transformaci. Kromě samotné transformace
 * metoda nad výsledkem provádí výpočet magnitud jednotlivých členů komplexních váhových koeficientů,
 * a to použitím Euclidovské normy (l2). Využívá algoritmu FFT, jehož podmínkou je velikost vstupních
 * segmentů v mocninách čísla 2. Velikost výstupního vektoru odhadu výkonové spektrální hustoty je
 * vypočítán již při konstrukci objeku. Pokud je pak velikost dalších segmentů menší než zadaná, jsou
 * tyto segmenty doplněny nulami. V případě většího segmentu třídá hlásí chybu pomocí signálu error.
 */
class FFT : public QObject {
    Q_OBJECT

public:
    /*!
     * \brief FFT Konstruktor třídy. Provede výpočet maximální velikosti vstupních segmentů a podle
     *            toho i výpočet velikosti výsledných vektorů odhadu výkonové spektrální hustoty.
     * \param segmentSize Předpokládaná velikost vstupních segmentů normalizovaného signálu.
     * \param parent Ukazatel na rodičovský objekt (kvůli dynamickému uvolnění).
     */
    explicit FFT(int segmentSize, QObject *parent = nullptr);

    /*!
     * Destruktor třídy.
     */
    ~FFT();

    /*!
     * \brief spectrumSize Metoda vrátí velikost výstupních vektorů odhadu výkonové spektrální hustoty.
     * \return Hodnota velikosti výstupních vektorů odhadu výkonové spektrální hustoty.
     */
    int espdSize() const;

    /*!
     * \brief transformEucl Metoda provede samotnou diskréní Fourierovu transformaci a následný výpočet
     *                      výsledných komplexních koeficientů pomocí metody euclidNorm. V případě, že
     *                      vstupní segment je větší než m_maxSegmentSize, metoda hlásí chybu skrze
     *                      signál error a vrací prázdný QVector.
     * \param segment Vektor, který obsahuje vzorky segmentu, z něhož je počítána disktréní F. transformace.
     * \return Vektor, který reprezentuje odhad výkonové spektrální hustoty daného segmentu nebo prázdný
     *         vektor při chybě.
     */
    QVector<float> transformEucl(QVector<float> segment);

    /*!
     * \brief realFft Metoda volá samotné funkce knihovny kissFFT a provádí samotný výpočet komplexních
     *                váhových koeficientů, které vrací jako vektor těchto hodnot.
     * \param segment Vstupní segment akustického signálu.
     * \return Vektor komplexních váhových koeficientů.
     */
    QVector<kiss_fft_cpx> realFFt(QVector<float> segment);

    /*!
     * \brief invRealFFT Metoda volá samotné funkce knihovny kissFFT a provádí inverzi diskrétní Fourierovy
     *                   transformace pomocí daného vektoru váhových koeficientů. Prvky vypočtené inverze jsou
     *                   dále děleny hodnotou 1/N (viz inverse FFT scaling factor).
     * \param cpx Vstupní vektor váhových koeficientů.
     * \return Vektor segmentu akustického signálu.
     */
    QVector<float> invRealFFT(QVector<kiss_fft_cpx> cpx);

    /*!
     * \brief realFFt Přetížená metoda. Provádí samotné volání funkcí knihovny kissFFT, kdy výsledné váhové koeficienty
     *                předává do předem daného vektoru. Tento vektor se předpokládá již naalokovaný na velikost
     *                vektoru váhových koeficientů (m_espdSize). V případě, že tento vektor není dostatečně velký, metoda
     *                neudělá nic.
     * \param segment Reference na vektor, který obsahuje prvky vektoru vstupního segmentu.
     * \param cpx Reference na vektor, do kterého budou vloženy výsledné váhové koeficienty.
     */
    void realFFt(QVector<float> &segment, QVector<kiss_fft_cpx> &cpx);

    /*!
     * \brief invRealFFT Přetížena metoda. Provádí inverzi diskrétní Fourierovy transformace voláním funkcí z knihovny
     *                   kissFFT. Výsledný sektor ukládá do vektoru daném referencí. Tento vektor se předpokládá naalokovaný
     *                   na velikost vektoru segmentu (m_maxSegmentSize). Pokud tento vektor není dostatečně velký, metoda
     *                   neudělá nic.
     * \param cpx Reference na vektor, který obsahuje váhové koeficienty.
     * \param segment Reference na vektor, do kterého budou vloženy výsledné vzorky segmentu.
     */
    void invRealFFT(QVector<kiss_fft_cpx> &cpx, QVector<float> &segment);

private:
    int m_maxSegmentSize;       //!< Maximální velikost vstupních segmentů (mocnina čísla 2).
    int m_espdSize;             //!< Velikost výstupních vektorů odhadu výkonové spektrální hustoty.
    kiss_fftr_cfg m_fftCfg;     //!< Struktura knihovny kissFFT potřebná pro výpočet FFT.
    kiss_fftr_cfg m_ifftCft;    //!< Struktura knihovny kissFFT potřebná pro výpočet inverze FFT.

    /*!
     * \brief prepareSegment Tato metoda je volána v případě, že velikost vstupního segmentu je menší
     *                       než hodnota m_maxSegmentSize (mocnina 2). V takovém případě metoda vytvoří nový
     *                       vektor, do kterého zkopíruje prvky vstupního vektoru. Takto vytvořený vektor
     *                       doplní čísly nula do počtu m_maxSegmentSize.
     * \param segment Vstupní segment akustického signálu.
     * \return Zkopírovaný vektor, který je doplněný nulami na délku m_maxSegmentSize.
     */
    QVector<float> prepareSegment(QVector<float> segment);

    /*!
     * \brief euclidNorm Podle zadaného pole komplexních výkonových koeficientů provede výpočet jejich magnitud
     *                   pomocí Euclidovy normy (l2). Tyto magnitudy vloží do vektoru, který vrátí.
     * \param cpx Vektor komplexních váhových koeficientů, u kterých jsou počítány jejich magnitudy.
     * \return Vektor magnitud vstupních komplexních váhových koeficientů, tj. odhad výkonové spektrální hustoty.
     */
    QVector<float> euclidNorm(QVector<kiss_fft_cpx> cpx);

signals:
    /*!
     * \brief error Signál, který je emitován při chybě.
     * \param message Popis chyby.
     */
    void error(QString message);
};

#endif
