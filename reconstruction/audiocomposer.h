#ifndef AUDIOCOMPOSER_H
#define AUDIOCOMPOSER_H

#include <QObject>
#include <QVector>
#include "../pe_config.h"

/*!
 * \brief Třída AudioComposer
 *
 * Třída poskytuje možnost zpětného spojování vytvořených segmentů akustického signálu. Pomocí metody write jednotlivé segmenty
 * zapisovány do cyklického bufferu, ze kterého je pak zkompletovaný signál odebrán a předán signálem audioComposed. Mezi další
 * funkcionality třídy patří notifikace při zkompletování určitého množství segmentů nebo průměrování vzorků v překrávajících se
 * částech akustického signálu.
 */
class AudioComposer : public QObject {
    Q_OBJECT

public:
    /*!
     * \brief AudioComposer Konstruktor třídy.
     * \param segmentSize Velikost vstupních segmentů akustického signálu.
     * \param overlap Počet vzorků, ve kterých se sousední segmenty překrývají.
     * \param avgOverlap TRUE, pokud chcete průměrovat hodnoty v překryvech sousedních segmentů. V případě, že bude nastavena hodnota
     *                   FALSE, budou při překryvu využity vzorky předchozího segmentu.
     * \param parent Ukazatel na rodičovský objekt (kvůli uvoňování).
     */
    explicit AudioComposer(int segmentSize, int overlap, bool avgOverlap = true, QObject *parent = nullptr);

    /*!
     * Destruktor třídy.
     */
    ~AudioComposer();

    /*!
     * \brief getComposedAudio Metoda vratí aktuálně sestavený akustický signál.
     * \return Vektor vzorků sestaveného akusticého signálu.
     */
    QVector<float> getAudio();

    /*!
     * \brief add Metoda, pomocí které probíhá samotné zpracování vstupního segmentu. Vzorky daného segmentu
     *            zkopírovány do cyklického bufferu a popřípadě využity při průměrování vzorků v překrývajících
     *            se částech sousedních polích.
     * \param segment Segment akustického signálu, který je určen ke zkompletování.
     */
    void add(const QVector<float> &segment);

    /*!
     * \brief clear Metoda odstraní dosud sestavený akustický signál a vyprázdní buffer.
     */
    void clear();

    /*!
     * \brief isEmpty Metoda zjistí, zda je buffer prázdný.
     * \return True, pokudje cyklický buffer prázdný, jinak false.
     */
    bool isEmpty();

private:
    int m_segmentSize;      //!< Velikost vstupních segmentů akustického signálu.
    int m_overlap;          //!< Počet vzorků, ve kterých se překrývají sousední segmenty.
    int m_overlapSupp;      //!< Počet vzorků, které nejsou překryty s dalším segmentem (m_segmentSize - m_overlap).
    bool m_avgOverlap;      //!< Logická hodnota, která určuje, zda se mají překrávající se hodnoty průměrovat.

    float *m_buffer;        //!< Dynamicky alokovaný cyklický buffer.
    int m_bufferSize;       //!< Počet vzorků cyklického bufferu m_buffer.
    int m_first;            //!< Index prvního prvky aktuálně kompletovaného segmentu.
    int m_next;             //!< Index prvku, na který se bude zapisovat při dalším volání metody add.
    QVector<float> m_audio; //!< Vektor, který obsahuje sestavený zvuk.

    /*!
     * \brief averateSegOverlap Metoda provede průměrování překrávajících se vzorků sousedních segmentů.
     * \details V daném segmentu se iteruje pouze nad prvními m_overlap hodnotami. Tuto hodnoty se průměrují
     *          s posledními m_overlap vzorky, které jsou zapsány v cyklickém bufferu. Posledních m_overlap
     *          hodnot v cyklickém bufferu jsou poté nahrazeny výsledky průměrování.
     * \param window
     */
    void averateSegOverlap(const QVector<float> &segment);

    /*!
     * \brief extractAudio Metoda extrahuje size vzorků z cykleckého bufferu počínaje pozicí m_first. Metoda nijak neovlivňuje
     *                     hodnoty řídících proměnných m_first nebo m_next.
     * \param size Počet vzorků, které metdoa extrahuje.
     * \return Vektor extrahovaných vzorků.
     */
    QVector<float> extractAudio(int size);

    /*!
     * \brief write Metoda provádí samotný zápis daného vektoru do cyklického bufferu. Voláním této metody se
     *              implicitně nezmění řídící proměnné m_first, m_next a m_written.
     * \param segment Segment akustického signálu určený k zapsání do cyklického bufferu.
     */
    void write(const QVector<float> &segment);

signals:
    /*!
     * \brief error Signál, který je emitován při chybě.
     * \param message Popis chyby.
     */
    void error(QString message);
};

#endif
