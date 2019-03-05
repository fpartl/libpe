#include "windowfunction.h"

WindowFunction::WindowFunction(int windowSize, QObject *parent) : QObject(parent) {
    if (windowSize <= 0) {
        emit error("WindowFunction::WindowFunction: Velikost okna nesmí být menší nebo rovna než 0.");
        return;
    }

    m_window.resize(windowSize);
}

QVector<float> WindowFunction::normalize(const QVector<sample> &segment) {
    if (segment.size() != m_window.size())
        return QVector<float>();

    QVector<float> normalized(segment.size());

    for (int i = 0; i < segment.size(); i++)
        normalized[i] = (float)segment[i] * m_window[i];

    return normalized;
}

QVector<float> WindowFunction::denormalize(const QVector<float> &segment) {
    if (segment.size() != m_window.size())
        return QVector<float>();

    QVector<float> normalized(segment.size());

    for (int i = 0; i < segment.size(); i++)
        normalized[i] = (float)segment[i] / m_window[i];

    return normalized;
}

