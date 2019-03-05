#ifndef PLOTPRINTER_H
#define PLOTPRINTER_H

#include <QTextStream>
#include <QDataStream>
#include <QFile>
#include <QVector>

#include "pe_config.h"

/*!
 * \brief Třída Printer
 *
 * Poskytuje možnost výpisu vektorů, jejichž položky jsou typu sample nebo float. Výpis je
 * možný do konzolového výstupu aplikace nebo do souboru. Souborem může být skript s příkazem
 * vytvoření grafu v prostředí Matlab/GNU Octave, binární soubor se surovými daty (možno číst
 * a přehrát například s využitím programu Audacity) nebo ve formátu csv (tabulkový formát).
 * Jednotlivé soubory jsou zapisovány do složky, která je definována v souboru pe_config.h.
 * V případě, že složka nebude existovat, metody, které zapisují do souboru, nebudou dělat nic.
 */
struct Printer {
    /*!
     * \brief printPlot Metoda provede výpis vektoru do textového souboru, a to jako příkaz
     *                  vytvoření grafu těchto hodnot. Pokud již soubor existuje, metoda přidá
     *                  položky vstupního vektoru na konec vypisovaných dat.
     * \param data Vektor, který obsahuje položky k vypsání.
     * \param fileName Název souboru, kam se budou data zapisovat.
     */
    static void printPlot(QVector<float> data, QString fileName);

    /*!
     * \brief printPlot Přetížená metoda. Metoda provede výpis vektoru do textového souboru,
     *                  a to jako příkat vytvoření grafu těchto hodnot. Pokud již soubor existuje,
     *                  metoda přidá položky vstupního vektoru na konec vypisovaných dat.
     * \param data Vektor, který obsahuje položky k vypsání.
     * \param fileName Název souboru, kam se budou data zapisovat.
     */
    static void printPlot(QVector<sample> data, QString fileName);

    /*!
     * \brief printPlotLine Metoda provede výpis vektoru do textového souboru, a to jako příkaz
     *                      vytvoření grafu těchto hodnot. Pokud soubor existuje, vytvoří v něm nový
     *                      příkaz vytvoření grafu a příkaz, který zaručí vizualizaci obou grafů
     *                      v jednom výstupním okně.
     * \param data Vektor, který obsahuje položky k vypsání.
     * \param fileName Název souboru, kam se budou data zapisovat.
     */
    static void printPlotLine(QVector<sample> data, QString fileName);

    /*!
     * \brief printPlotLine Přetížená metoda. Provede výpis vektoru do textového souboru, a to jako příkaz
     *                      vytvoření grafu těchto hodnot. Pokud soubor existuje, vytvoří v něm nový
     *                      příkaz vytvoření grafu a příkaz, který zaručí vizualizaci obou grafů
     *                      v jednom výstupním okně.
     * \param data Vektor, který obsahuje položky k vypsání.
     * \param fileName Název souboru, kam se budou data zapisovat.
     */
    static void printPlotLine(QVector<float> data, QString fileName);

    /*!
     * \brief print Provede výpis položek daného vektoru do konzolového výstupu aplikace. Jednotlivé hodnoty
     *              oddělí znakem ','.
     * \param data Vektor, který obsahuje položky k vypsání.
     */
    static void print(QVector<float> data);

    /*!
     * \brief print Přetížená metoda. Provede výpis položek daného vektoru do konzolového výstupu aplikace.
     *              Jednotlivé hodnoty oddělí znakem ','.
     * \param data Vektor, který obsahuje položky k vypsání.
     */
    static void print(QVector<sample> data);

    /*!
     * \brief saveRawData Provede výpis položek daného vektoru do binárního souboru. Používá se například pro
     *                    následné přehrání vzorků akustického signálu nástrojem Audacity.
     * \param data Vektor, který obsahuje položky k vypsání.
     * \param fileName Název souboru, kam se budou data zapisovat.
     */
    static void saveRawData(QVector<float> data, QString fileName);

    /*!
     * \brief saveRawData Přetížená metoda. Provede výpis položek daného vektoru do binárního souboru. Používá
     *                    se například pro následné přehrání vzorků akustického signálu nástrojem Audacity.
     * \param data Vektor, který obsahuje položky k vypsání.
     * \param fileName Název souboru, kam se budou data zapisovat.
     */
    static void saveRawData(QVector<sample> data, QString fileName);

    /*!
     * \brief printToCsv Provede výpis položek daného vektoru do textového souboru, a to jako jednu řádku
     *                   tabulkového souboru typu csv. V případě, že výstupní soubor existuje, je do něj
     *                   přidána další řádka.
     * \param data Vektor, který obsahuje položky k vypsání.
     * \param fileName Název souboru, kam se budou data zapisovat.
     */
    static void printToCsv(QVector<float> data, QString fileName);

    /*!
     * \brief printToCsv Přetížená metoda. Provede výpis položek daného vektoru do textového souboru, a to
     *                   jako jednu řádku tabulkového souboru typu csv. V případě, že výstupní soubor existuje,
     *                   je do něj přidána další řádka.
     * \param data Vektor, který obsahuje položky k vypsání.
     * \param fileName Název souboru, kam se budou data zapisovat.
     */
    static void printToCsv(QVector<sample> data, QString fileName);
};

#endif
