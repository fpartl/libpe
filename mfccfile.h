#ifndef MFCCFILE_H
#define MFCCFILE_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QDataStream>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include "pe_config.h"

/*!
 * \brief Třída MfccFile
 *
 * Tato třída představuje soubor typu MFCC. Umožňuje do zvoleného souboru ukládat vektory MFCC koeficientů,
 * kontrolovat validitu daného souboru, číst data z MFCC souboru, apod. Soubor MFCC je binární soubor uložený
 * pomocí malého endianu, kde první 4 B obsahují počet vektorů MFCC koeficientů a každý další vektor má 26 těchto
 * koeficientů.
 */
class MfccFile : public QObject {
    Q_OBJECT

public:
    /*!
     * \brief MfccFile Konstruktor třídy, kteřý přijímá cestu k MFCC souboru. Pro některé operace existují
     *                 statické metody, kde vytváření objektu není potřebné.
     * \param fileName Cesta k MFCC souboru.
     * \param parent Ukazatel na rodiče objektu, který v softwaru knihovny Qt lze využít pro uvolnění
     *               dynamicky alokovaného objektu.
     */
    explicit MfccFile(const QString& fileName, QObject *parent = nullptr);

    /*!
     * \brief setMfccFile Metoda nastaví objektu cestu k patřičnému MFCC souboru.
     * \param fileName Cesta k MFCC souboru.
     */
    void setMfccFile(const QString& fileName);

    /*!
     * \brief isReadable Metoda zkontroluje strukturu MFCC souboru a zjistí, jestli je soubor čitelný (zda
     *                   je počet vektorů odpovídající velikosti souboru).
     * \return True, pokud je soubor čitelný, jinak false.
     */
    bool isReadable();

    /*!
     * \brief isReadable Statická metoda, která zkontroluje strukturu MFCC souboru a zjistí, jestli je soubor
     *                   čitelný (zda je počet vektorů odpovídající velikosti souboru).
     * \param fileName Cesta ke kontrolovanému souboru.
     * \return True, pokud je soubor čitelný, jinak false.
     */
    static bool isReadable(const QString& fileName);

    /*!
     * \brief isWritable Metoda zkontroluje strukturu MFCC souboru a zjistí, je soubor čitelný a lze do něj přidat
     *                   další vektory MFCC koeficientů.
     * \return True, pokud do souboru lze zapisovat, jinak false.
     */
    bool isWritable();

    /*!
     * \brief isWritable Statická metoda, která zkontroluje strukturu MFCC souboru a zjistí, je soubor čitelný a lze
     *                   do něj přidat další vektory MFCC koeficientů.
     * \param fileName Cesta ke kontrolovanému souboru.
     * \return True, pokud do souboru lze zapisovat, jinak false.
     */
    static bool isWritable(const QString& fileName);

    /*!
     * \brief exists Metoda zjistí, zda soubor fyzicky existuje na zadané cestě, nikoli však zda je čitelný, či zapisovatelný.
     * \return True, pokud souboru existuje, jinak false.
     */
    bool exists();

    /*!
     * \brief clear Metoda odstraní soubor MFCC.
     */
    void clear();

    /*!
     * \brief clear Metoda odstraní soubor MFCC na zadaném umístění.
     * \param filename Cesta k souboru, který má být odstraněn.
     */
    static void clear(const QString &filename);

    /*!
     * \brief write Metoda, která do souboru zapíše zadané MFCC koeficienty. Pokud již soubor existuje, bude
     *              vyprázdněn.
     * \param mfccs Vektory MFCC koeficientů, které budou zapsány do souboru.
     * \return True, pokud byl zápis úspěšný, jinak false.
     */
    bool write(const QVector<QVector<float>> &mfccs);

    /*!
     * \brief append Metoda přidá zadaný vektor na konec MFCC souboru a inkrementuje jejich počet v souboru.
     * \param mfccs Vektor MFCC koeficientů, který bude přidán na konec souboru.
     * \param checkFile True, pokud je třeba před přidáním zkontrolovat validitu souboru.
     * \return True, pokud byl zápis úspěšný, jinak false.
     */
    bool append(const QVector<float> &mfccs, bool checkFile = false);

    /*!
     * \brief readAll Metoda přečte všechny vektory MFCC koeficientů z souboru. V případě potřeby dokáže
     *                soubor před čtením zkontrolovat.
     * \param checkFile True, pokud má být zkontrolována validita souboru před jeho čtením.
     * \return Vektory MFCC koeficientů nebo prázdný vektor při chybě.
     */
    QVector<QVector<float>> readAll(bool checkFile = false);


private:
    QString m_fileName;     //!< Cesta k MFCC souboru.

    /*!
     * \brief initStream Metoda provede inicializaci zadaného datového toku.
     * \param stream Ukazatel na incicializovaný datový tok.
     */
    void initStream(QDataStream *stream);

    /*!
     * \brief hasValidSize Metoda zkontroluje, zda zadaný soubor má správnou velikost, aby mohl být MFCC
     *                     souborem, tj. počet vektorů na prvních 4 B musí korespondovat s velikostí souboru.
     * \param mfccFile Ukazatel na kontrolovaný soubor.
     * \return True, pokud má soubor platnou velikost, jinak false.
     */
    bool hasValidSize(QFile *mfccFile);

    /*!
     * \brief isValidMfccVector Metoda zkontroluje, zda zadané vektory MFCC mají odpovídající velikost,
     *                          tj. všechny obsahují právě 26 koeficientů.
     * \param mfccs Vektory MFCC koeficientů.
     * \return True, pokud mají vektory správnou velikost, jinak false.
     */
    bool isValidMfccVector(const QVector<QVector<float>> &mfccs);
};

#endif
