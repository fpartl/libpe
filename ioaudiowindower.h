#ifndef AUDIOBUFFER_H
#define AUDIOBUFFER_H

#include <QObject>
#include <QIODevice>
#include <QDebug>
#include <QVector>

#include "pe_config.h"

/*!
 *  Multiplikátor minimálně použitelné velikosti bufferu.
 *  Minimální velikost je velikost okna nebo velikost bufferu dat ze zvukové karty. Podle toho, co je větší.
 *  Užívá se při alokaci bufferu v metodě createBuffer;
*/
#define BUFFER_SIZE_MULTIPLER 3

/*!
 * \brief Třída AudioWindower
 *
 * Třída reprezentuje vstupní zařízení, do kterého putuje akustický signál zachytávaný zvukovou kartou zařízení. Definuje metody,
 * které umožňují rozkládat vstupní signál na segmenty o dané velikosti a daném překryvu.
 */
class IOAudioWindower : public QIODevice {
    Q_OBJECT

public:
    /*!
     * \brief AudioWindower Konstruktor třídy.
     * \param segmentSize Požadovaná velikost vytvářených oken.
     * \param overlap Hodnota překryvu jednotlivých oken.
     * \param extractTimeData Udává, zda se má emitovat signál s daty přijatými ze zvukové karty. Jedná se o volitelný parametr, který
     *                        je ve výchozím stavu nastavený na hodnotu false.
     * \param bufferSize Předvolená velikost bufferu.
     * \param parent Ukazatel na rodiče objektu (kvůli dynamickému uvolňování).
     */
    explicit IOAudioWindower(int segmentSize, int overlap, bool extractTimeData = false, int bufferSize = UNDEFINED, QObject *parent = nullptr);

    /*!
     * Destruktor třídy.
     */
    ~IOAudioWindower();

    /*!
     * \brief clear Vyprázdní buffer.
     */
    void clear();

private:
    sample *m_buffer;               //!< Buffer
    int m_bufferSize;               //!< Velikost bufferu
    int m_first;                    //!< Index prvního prvku aktuálně plněného segmentu.
    int m_next;                     //!< Index prvního nezapsaného prvku, tj. index posledního prvku zapsaného do bufferu + 1.
    int m_segmentSize;              //!< Velikost vytvářených oken.
    int m_overlapSupp;              //!< Počet prvků tvořící první část pole.
    bool m_extractTimeData;         //!< Řídící proměnná, která spouští extrakci syrových dat bez vytváření oken.

    /*!
     * \brief createBuffer Vytvoří buffer podle zadané velikosti (velikost dat předaných ze zvukové karty). Velikost bufferu se odvíjí
                           od faktu, zda velikost dat předaných zvukovou kartou je větší než velikost okna. Velikost je následně vypočítána
                           jako BUFFER_SIZE_MULTIPLER * m_segmentSize, popřípadě předaný parametr * BUFFER_SIZE_MULTIPLER. POZOR! Velikost
                           bufferu se při zápisu dat nemění a zapsání většího objemu dat než je schopen buffer uložit vede k nedefinovanému
                           chování.
     * \param inputSize Udává velikost bufferu, který byl přijat od ovladače zvukové karty.
     */
    void createBuffer(qint64 inputSize);

    /*!
     * \brief finalizeWindow Předpokládá, že buffer obsahuje alespoň jedno okno. Vytvoří nový vektor a překopíruje do něj toto okno
                             z bufferu. Dále zvýší hodnotu ukazatele first o hodnotu supplementum (realizace překryvu polí) a emituje signál
                             windowCollected s nově vzniklým vektorem.
     */
    void finalizeWindow();

    /*!
     * \brief emitRawTimeData Pouze vytvoří pole na haldě, do kterého překopíruje data ze zvukové karty a emituje signál dataBuffered.
                              Uvolnění tohoto pole je v kompetenci připojeného slotu. Spouštění této metody je řízeno proměnnou
                              extractTimeData nastavené konstruktorem třídy.
     * \param data Ukazatel na syrová příchozí data ze zvukové karty.
     * \param maxSize Velikost příchozích dat v Bytech.
     */
    void emitRawTimeData(const char *data, qint64 maxSize);

    /*!
     * \brief isWindowBuffered Funkce kontroluje, zde je v bufferu alespoň m_windowSize prvků, tj. pokud je připravené okno.
     * \return True, pokud je v bufferu připravené alespoň jedno okno, jinak false.
     */
    bool isWindowBuffered();

protected:
    /*!
     * \brief readData Slot je implementací virtuální metody z QIODevice. Nevyužívá se.
     */
    qint64 readData(char *data, qint64 maxSize);

    /*!
     * \brief writeData Slot, skrze kterého se předávají data ze zvukové karty.
                        Data se zde kopírují do bufferu. Poté se pomocí metody isWindowBuffered zkontroluje, jestli buffer obsahuje
                        okno, případně se volá metoda finalizeWindow. V případě, že je atribut extractTimeData nastaven na true, volá
                        se metoda emitRawTimeData.
     * \param data Ukazatel na data přijatá ze zvukové karty.
     * \param maxSize Velikost dat ze zvukové karty v Bytech.
     * \return Funkce vrací hodnotu parametru maxSize.
     */
    qint64 writeData(const char *data, qint64 maxSize);

signals:
    /*!
     * \brief windowCollected Signál obsahující referenci na vzniklé okno. Tento signál je emitován metodou finalizeWindow().
     * \param window Ukazatel na dynamicky alokovaný segment akustického signálu.
     */
    void windowCollected(QVector<sample> window);

    /*!
     * \brief dataBuffered Signál obsahující referenci na kopii dat přejatých ze zvukové karty, tj. bez jakéhokoliv překryvu.
                           Tento signál je emitován metodou emitRawTimeData. Prvním parametrem je ukazatel na vytvořené pole,
                           druhým parametrem je počet záznamů v něm.
     * \param audio Vektor vzorků přijatých do bufferu.
     */
    void dataBuffered(QVector<sample> audio);
};

#endif
