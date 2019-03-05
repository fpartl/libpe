/*
 * Varování!
 *
 * Konstanty definované v tomto konfiguračním souboru nejsou implicitně nijak propojeny
 * s jednotlivými třídami knihovny libpe. Z důvodu lepší udržitelnosti kódu se doporučuje
 * využívání těchto konstant v případech vytváření jednotlivých instancí tříd knihovny.
 * Pokud tyto konstanty nevyužijete, jejich následná změna v tomto souboru nebude mít vliv
 * na Vaši aplikaci.
 */

#ifndef PE_CONFIG_H
#define PE_CONFIG_H

/* Definice datového typu sample. */
#include <QMetaType>
#include <QVector>
typedef short sample;
Q_DECLARE_METATYPE(QVector<sample>);

/* Definice celočíselné hodnoty, která označuje nestandardní stav. Nesahat! */
#define UNDEFINED -1

/* Počet kanálů zaznamenávaného zvuku. */
#define CHANNEL_COUNT 1

/* Metoda kódování zvukových vzorků. */
#define AUDIO_CODEC "audio/pcm"

/* Počet vzorků segmentu akustického signálu. */
#define SEGMENT_SIZE 1024

/* Počet vzorků, které se překrývají s dalším polem. */
#define OVERLAP 512

/* Vzorkovací frekvence zvuku. */
#define SAMPLE_RATE 44100

/* Počet prvků souvislého vektoru vektorů MFC koeficientů. */
#define WINDOW_VECTOR_COUNT 256 // přibližně odpovídá 3 sekundám audio záznamu

/* Počet filtrů užitých při výpočtu MFCC. */
#define NUM_FILTERS 48

/* Počet získávaných MFC koeficientů. */
#define MFCC_COUNT 26

/* Počet iterací algoritmu Iterative Inverse Short-time Fourier Transform Magnitude algorithm. */
#define ESPD_RECOVERY_ITERS 1000

/* Adresář, kam se budou ukládat výsledné soubory třídy Printer. */
#define PLOT_DRIVE_LOCATION "/home/frantisek/Plocha/azs_test/"

#endif
