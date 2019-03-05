#include "melfilterbank.h"

MelFilterBank::MelFilterBank(int espdSize, int filterCount, int sampleRate, QObject *parent) : QObject(parent) {
    m_espdSize = espdSize;
    m_filterCount = filterCount;
    m_sampleRate = sampleRate;

    initFilters();
    initFiltersSums();
}

QVector<float> MelFilterBank::getFilter(int th) const {
    if (th < 0 || th > m_melFilters.count())
        return QVector<float>();

    return m_melFilters.at(th);
}

float MelFilterBank::getValue(int thFilter, int at) {
    if (thFilter < 0 || thFilter > m_melFilters.count() || at < 0 || at > m_melFilters[thFilter].count())
        return UNDEFINED;

    return m_melFilters.at(thFilter).at(at);
}

float MelFilterBank::getFilterSum(int thFilter) {
    if (thFilter < 0 || thFilter > m_sumsOfFilters.count())
        return UNDEFINED;

    return m_sumsOfFilters[thFilter];
}

int MelFilterBank::filterSize() const {
    return m_espdSize;
}

int MelFilterBank::count() const {
    return m_filterCount;
}

int MelFilterBank::getSampleRate() const {
    return m_sampleRate;
}

void MelFilterBank::initFilters() {
    QVector<float> filterPoints = distributeFilters();

    for (int i = 0; i < filterPoints.count(); i++) {
        filterPoints[i] = this->melsToHz(filterPoints[i]);

        float scaledFPoint = qFloor(((float)m_espdSize / (float)m_sampleRate) * filterPoints[i]);

        if (scaledFPoint > (float)(m_espdSize - 1))
            filterPoints[i] = (float)(m_espdSize - 1);
        else
            filterPoints[i] = scaledFPoint;
    }

    m_melFilters.reserve(m_filterCount);

    for (int i = 1; i < filterPoints.count() - 1; i++) {
        QVector<float> filter;
        filter.reserve(m_espdSize);

        for (int k = 0; k < m_espdSize; k++) {
            if (k < filterPoints[i - 1]) {
                filter.append(0); // nuluju všechno před trojúhelníkem
            }
            else if (filterPoints[i - 1] <= k && k <= filterPoints[i]) { // hrabu se do kopce
                if ((filterPoints[i] - filterPoints[i - 1]) != 0)
                    filter.append((k - filterPoints[i - 1]) / (filterPoints[i] - filterPoints[i - 1]));
                else
                    filter.append(1);
            }
            else if (filterPoints[i] <= k && k <= filterPoints[i + 1]) { // jedu z kopce
                if ((filterPoints[i + 1] - filterPoints[i]) != 0)
                    filter.append((filterPoints[i + 1] - k) / (filterPoints[i + 1] - filterPoints[i]));
                else
                    filter.append(1);
            }
            else filter.append(0); // nuluju všechno za trojúhelníkem
        }

        m_melFilters.append(filter);
    }
}

void MelFilterBank::initFiltersSums() {
    m_sumsOfFilters.reserve(m_filterCount);

    for (int i = 0; i < m_filterCount; i++) {
        float filterSum = 0;

        for (int j = 0; j < m_espdSize; j++)
            filterSum += m_melFilters[i][j];

        m_sumsOfFilters.append(filterSum);
    }
}

QVector<float> MelFilterBank::distributeFilters() {
    int countOfPoints = m_filterCount + 2; // + 2 protože mám ještě nejnižší frekvenci a nejvyšší frekvenci
    QVector<float> filterPoints(countOfPoints);

    filterPoints[0] = this->hzToMels(0.0f);
    filterPoints[countOfPoints - 1] = this->hzToMels(m_sampleRate);

    float step = (filterPoints[countOfPoints - 1] - filterPoints[0]) / (m_filterCount + 1);

    for (int i = 1; i < countOfPoints - 1; i++)
        filterPoints[i] = filterPoints[i - 1] + step;

    return filterPoints;
}

inline float MelFilterBank::hzToMels(float fHz) {
    return 1125 * qLn(1 + (fHz / 700));
}

inline float MelFilterBank::melsToHz(float fMel) {
    return 700 * (qExp(fMel / 1125) - 1);
}
