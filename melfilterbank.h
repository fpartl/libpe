#ifndef FILTERBANK_H
#define FILTERBANK_H

#include <QObject>
#include <QVector>
#include <QtMath>

#include "pe_config.h"

/*!
 * \brief Třída MelFilterBank obahuje metody pro generování a uchovávání banky melovských filtrů.
 */
class MelFilterBank : public QObject {
    Q_OBJECT

public:
    /*!
     * \brief MelFilterBank Kontruktor třídy.
     * \param espdSize Počet prvků vektoru odhadu výkonové spektrální hustoty segmentu.
     * \param filterCount Požadovaný počet filtrů banky melovských filtrů.
     * \param sampleRate Frekvence vzorkování vstupního akustického signálu.
     * \param parent Ukazatel na rodiče objektu (kvůli dynamickému uvolňování).
     */
    explicit MelFilterBank(int espdSize, int filterCount, int sampleRate, QObject *parent = nullptr);

    /*!
     * \brief getFilter Metoda vrátí referenci na daný filtr. V případě chyby metoda vrací prázdný vektor.
     * \param th Index požadovaného filtru.
     * \return Vektor vzorků požadovaného filtru.
     */
    QVector<float> getFilter(int th) const;

    /*!
     * \brief getValue Metoda pro získání konkrétního vzorku konkrétního filtru. Při zadání indexů mimo
     *                 rozsah funkce vrátí hodnotu UNDEFINED.
     * \param thFilter Index filtru, ze kterého je požadována hodnota (v rozsahu 0 až m_spectrumSize - 1).
     * \param at Index konkrétního vzorku filtru.
     * \return Hodnota daného filtru v daném bodě nebo hodnota UNDEFINED při chybě.
     */
    float getValue(int thFilter, int at);

    /*!
     * \brief getFilterSum Metoda vrací hodnotu součtu všech vzorků zadaného filtru. V případě, že je zadána
     *                     hodnota mimo rozsah, metoda vrací hodnotu UNDEFINED.
     * \param thFilter Index filtru, ze kterého se požaduje součet.
     * \return Součet hodnot daného filtru.
     */
    float getFilterSum(int thFilter);

    /*!
     * \brief filterSize Metoda vrací počet prvků odhadu výkonové spektrální hustoty segmentu, tj. počet vzorků.
     * \return Počet prvků odhadu výkonové spektrální hustoty.
     */
    int filterSize() const;

    /*!
     * \brief count Vrací počet filtrů banky melovských filtrů.
     * \return Počet filtrů banky melovských filtrů.
     */
    int count() const;

    /*!
     * \brief getFrequency Vrací nastavenou hodnotu vzorkovací frekvence vstupního akustického signálu.
     * \return Hodnota vzorkovací frekvence vstupního akustického signálu.
     */
    int getSampleRate() const;

private:
    int m_espdSize;                             //!< Počet prvků vektoru odhadu výkonové spektrální hustoty.
    int m_filterCount;                          //!< Počet filtrů banky melovských filtrů.
    int m_sampleRate;                           //!< Frekvence vzorkování vstupního akustického signálu.
    QVector<float> m_sumsOfFilters;             //!< Hodnoty součtů jednotlivých filtrů. Využívá se při rekonstrukci.
    QVector<QVector<float>> m_melFilters;       //!< Filtry banky melovských filtrů.

    /*!
     * \brief initFilters Metoda provede inicializaci banky melovských filtrů podle argumentů konstruktoru.
     */
    void initFilters();

    /*!
     * \brief initFiltersSums Metoda provede inicializaci vektoru součtů jednotlivých filtrů.
     */
    void initFiltersSums();

    /*!
     * \brief distributeFilters Metoda vypočítá rovnoměrné rozložení filtrů v melspektru.
     * \return Vektor, který obsahuje body, z nichž se později vytvoří jednotlivé filtry.
     */
    QVector<float> distributeFilters();

    /*!
     * \brief hzToMels Metoda, která provádí převod jednotek Hz na jednotky Mel.
     * \param fHz Hodnota k převedení.
     * \return Hodnota fHz převedená do jednotek mel-spektra.
     */
    inline float hzToMels(float fHz);

    /*!
     * \brief melsToHz Metoda, která provádí převod jednotek Mel na jednotky Hz.
     * \param fMel Hodnota k převedení.
     * \return Hodnota fMel převedená do jednotek lineárního spektra.
     */
    inline float melsToHz(float fMel);
};

#endif
