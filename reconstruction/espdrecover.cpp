#include "espdrecover.h"

ESPDRecover::ESPDRecover(int espdSize, int filtersCount, int sampleRate, QObject *parent)
        : QObject(parent)
        , m_filters(espdSize, filtersCount, sampleRate) {
    m_espdSize = espdSize;
    m_filtersCount = filtersCount;
    m_sampleRate = sampleRate;
}

QVector<float> ESPDRecover::recover(QVector<float> mfccs) {
    if (mfccs.isEmpty() || mfccs.size() > m_filtersCount) {
        emit error("ESPDRecover::recover: Neočekávaná délka vstupního vektoru mel-frekvenčních kepstrálních koeficientů.");
        return QVector<float>();
    }

    /* Doplnění nulami tak, aby počet prvků vektor kepstrals byl rovný počtu nastavených filtrů. */
    QVector<float> kepstrals = (mfccs.size() < m_filtersCount) ? completeMFCCs(mfccs) : mfccs;

    QVector<float> mels = recoverMelCoefs(kepstrals);

    QVector<float> espd = recESPDFromMelKoefs(mels);

    return espd;
}

QVector<float> ESPDRecover::completeMFCCs(QVector<float> mfccs) {
    QVector<float> completed(mfccs);

    while (m_filtersCount > completed.size())
        completed.append(0.0f);

    return completed;
}

QVector<float> ESPDRecover::recoverMelCoefs(QVector<float> mfccs) {
    /* Dělení normalizačním faktorem. */
    for (int i = 0; i < mfccs.size(); i++)
        mfccs[i] = mfccs[i] / getNormFactor(i);

    /* Inverzní kosinová transformace a exponenciální funkce. */
    QVector<float> mels(mfccs.size());

    for (int i = 0; i < mels.size(); i++) {
        mels[i] = idct(mfccs, i);
        mels[i] = qExp(mels[i]);
    }

    return mels;
}

float ESPDRecover::idct(QVector<float> xi, int mth) {
    float result = 0.0f;

    for (int n = 1; n < xi.size(); n++) {
        float incos = (M_PI / (float)xi.size()) * (float)n * ((float)mth + 0.5f);

        result += xi[n] * qCos(incos);
    }

    result += (0.5f * xi[0]);
    result *= (2.0f / (float)xi.size()); /* škálovací faktor */

    return result;
}

float ESPDRecover::getNormFactor(int i) {
    return (i == 0)
            ? qSqrt(1.0f / m_filtersCount)
            : qSqrt(2.0f / m_filtersCount);
}

QVector<float> ESPDRecover::recESPDFromMelKoefs(QVector<float> mels) {
    QVector<float> espd(m_espdSize, 0.0f);

    for (int i = 0; i < mels.size(); i++) {
        if (m_filters.getFilterSum(i) != 0) {
            QVector<float> filter = m_filters.getFilter(i);
            float filterSum = m_filters.getFilterSum(i);

            for (int j = 0; j < filter.size(); j++)
                espd[j] += (filter.at(j) / filterSum) * mels[i];
        }
    }

    return espd;
}
