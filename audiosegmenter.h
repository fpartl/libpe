#ifndef AUDIOWINDOWER_H
#define AUDIOWINDOWER_H

#include <QObject>
#include <QVector>

#include "pe_config.h"

/*!
 * \brief Třída AudioSegmenter
 *
 * Třída obsahuje kruhový buffer a nad ním definované metody, které umožňují zadaný akustický signál rozkládat na segmenty
 * o dané velikosti a překryvu. Jediným varovaním je, že velikost bufferu je dána již při konstrukci objektu třídy a při
 * vložení většího množství dat může aplikace skončit chybou SIGSEGV. S velikostí bufferu tedy nebuďte skromní.
 */
class AudioSegmenter : public QObject {
    Q_OBJECT

public:
    /*!
     * \brief AudioSegmenter Konstruktor třídy.
     * \param segmentSize Velikost segmentů.
     * \param overlap Překryv segmentů.
     * \param bufferSize Velikost pracovního bufferu. Pozor! Při zvolení malé velikosti může dojít k přetečení => SIGSEGV.
     * \param parent Ukazatel na rodiče objektu, který lze dle notace Qt využít k automatické destrukci objektu.
     */
    explicit AudioSegmenter(int segmentSize, int overlap, int bufferSize, QObject *parent = nullptr);

    /*!
     * Destruktor třídy.
     */
    ~AudioSegmenter();

    /*!
     * \brief writeAudio Metoda, která zapíše vstupní data do bufferu. Při zadání většího množství dat než se do bufferu vejde
     *                   metoda emituje chybový signál a neprovede žádnou další akci.
     * \param data Reference na vektor vstupních dat.
     */
    void writeAudio(const QVector<sample>& data);

    /*!
     * \brief hasNextSegment Metoda, která zjistí, zda je v bufferu připraven alespoň jeden CELÝ segment. Zda je v bufferu alespoň
     *                       část segmentu řekne metoda AudioSegmenter::isEmpty().
     * \return True, pokud je v bufferu připraven CELÝ segment, jinak false.
     */
    bool hasNextSegment();

    /*!
     * \brief nextSegment Metoda, která z bufferu extrahuje jeden segment. Pokud v bufferu není celý segment, extrahuje dostupná data
     *                    a doloží je nulami na požadovanou velikost segmentu. V případě prázdného bufferu vrátí prázdný vektor.
     * \return Vektor obsahují vzorky jednoho extrahovaného segmentu.
     */
    QVector<sample> nextSegment();

    /*!
     * \brief isEmpty Metoda zjistí, zda jdou v bufferu dostupná nějaká data. Nemusí to být celý segment.
     * \return True, pokud buffer obsahuje nějaká data (i třeba neúplný segment), jinak false.
     */
    bool isEmpty();

private:
    sample *m_buffer;               //!< Buffer obsahující data.
    int m_bufferSize;               //!< Velikost bufferu.
    int m_first;                    //!< Index prvního prvku aktuálně plněného segmentu.
    int m_next;                     //!< Index prvního nezapsaného prvku, tj. index posledního prvku zapsaného do bufferu + 1.
    int m_segmentSize;              //!< Velikost vytvářených oken.
    int m_overlapSupp;              //!< Počet prvků tvořící první část výsledného segmentu (m_segmentSize - overlap).

    /*!
     * \brief flush Metoda z bufferu extrahuje zbývající data, která doloží nulami na požadovanou velikost segmentu a vyprázdní
     *              buffer. Tato metoda je vnitřně volána metodou AudioSegmenter::nextSegment.
     * \return Vektor obsahující zvývající vzorky bufferu doložené nulami.
     */
    QVector<sample> flush();

signals:
    /*!
     * \brief error Signál, který informuje o chybovém stavu. Tento signál je spíše informativní povahy.
     * \param message Obsah chybového hlášení.
     */
    void error(const QString& message);
};

#endif
