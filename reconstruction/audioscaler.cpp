#include "audioscaler.h"

AudioScaler::AudioScaler(QObject *parent) : QObject(parent) {

}

QVector<sample> AudioScaler::scale(const QVector<float> &segment) {
    float scaleFactor = findScaleFactor(segment);
    QVector<sample> scaled(segment.size());

    for (int i = 0; i < segment.size(); i++)
        scaled[i] = (sample)rint(segment[i] / scaleFactor);

    return scaled;
}

float AudioScaler::findScaleFactor(const QVector<float> &segment) {
    float maxValue = 0;

    for (int i = 0; i < segment.size(); i++) {
        float absValue = qAbs(segment[i]);

        if (absValue > maxValue)
            maxValue = absValue;
    }

    return (maxValue > std::numeric_limits<sample>::max()) ? maxValue / std::numeric_limits<sample>::max() : 1;
}
