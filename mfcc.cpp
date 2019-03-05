#include "mfcc.h"

MFCC::MFCC(int sampleRate, int filtersCount, int espdSize, QObject *parent)
        : QObject(parent)
        , m_filters(espdSize, filtersCount, sampleRate) {
    m_sampleRate = sampleRate;
    m_filtersCount = filtersCount;
    m_espdSize = espdSize;
}

QVector<float> MFCC::calculate(QVector<float> espd, int count) {
    if (espd.size() != m_espdSize) {
        emit error("MFCC::calculate: Neočekávaná délka vstupního vektoru odhadu výkonové spektrální hustoty.");
        return QVector<float>();
    }

    QVector<float> mels = calcMelCoefs(espd);

    QMutableVectorIterator<float> melsIt(mels);
    while (melsIt.hasNext()) {
        float mel = melsIt.next();

        if (mel > 0.0f)
            melsIt.setValue(qLn(mel));
    }

    if (count <= 0 || count > m_filtersCount)
        count = m_filtersCount;

    QVector<float> keps(count);

    for (int i = 0; i < keps.size(); i++) {
        keps[i] = dct(mels, i);
        keps[i] *= getNormFactor(i);
    }

    return keps;
}

QVector<float> MFCC::calcMelCoefs(QVector<float> espd) {
    QVector<float> mels(m_filtersCount);

    for (int i = 0; i < m_filtersCount; i++) {
        mels[i] = 0;

        QVector<float> filter = m_filters.getFilter(i);

        for (int j = 0; j < filter.size(); j++)
            mels[i] += fabs(espd[j] * filter.at(j));
    }

    return mels;
}

float MFCC::dct(QVector<float> xi, int mth) {
    float result = 0.0f;

    for (int n = 0; n < xi.size(); n++) {
        float incos = (M_PI / (float)xi.size()) * (float)mth * ((float)n + 0.5f);

        result += xi[n] * qCos(incos);
    }

    return result;
}

float MFCC::getNormFactor(int i) {
    return (i == 0)
            ? qSqrt(1.0f / m_filtersCount)
            : qSqrt(2.0f / m_filtersCount);
}
