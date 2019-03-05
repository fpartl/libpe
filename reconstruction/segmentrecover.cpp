#include "segmentrecover.h"

SegmentRecover::SegmentRecover(int segmentSize, int recoverIterations, QObject *parent) :
    QObject(parent),
    m_fft(segmentSize)
{
    m_segmentSize = segmentSize;
    m_recoverIterations = recoverIterations;
}

QVector<float> SegmentRecover::recover(QVector<float> espd) {
    if (espd.size() != m_fft.espdSize())
        return QVector<float>();

    if (m_whiteNoise.isEmpty())
        loadWhiteNoise();

    QVector<kiss_fft_cpx> cpx_fft(m_fft.espdSize());
    QVector<polar_cpx> pol_fft(m_fft.espdSize());
    QVector<float> segment;
    segment.reserve(m_whiteNoise.size());

    std::copy(m_whiteNoise.begin(), m_whiteNoise.begin() + m_whiteNoise.size(), std::back_inserter(segment));

    for (int i = 0; i < m_recoverIterations; i++) {
        m_fft.realFFt(segment, cpx_fft);

        toPolar(cpx_fft, pol_fft);

        for (int j = 0; j < pol_fft.size(); j++)
            pol_fft[j].mag = espd[j];

        toGauss(pol_fft, cpx_fft);

        m_fft.invRealFFT(cpx_fft, segment);
    }

    return segment;
}

void SegmentRecover::loadWhiteNoise() {
    QString filename = QString("%1/noise_%2.raw").arg(QCoreApplication::applicationDirPath()).arg(m_segmentSize);
    QFile file(filename);
    sample val;

    m_whiteNoise.resize(m_segmentSize);

    if (!file.open(QIODevice::ReadOnly)) {
        emit error(QString("White noise file (%1 samples) not found!").arg(filename));
        return;
    }

    QDataStream reader(&file);
    for (int i = 0; i < m_whiteNoise.size(); i++) {
        reader >> val;
        m_whiteNoise[i] = (float)val;
    }
}

void SegmentRecover::toPolar(QVector<kiss_fft_cpx> &cpx, QVector<polar_cpx> &pol_cpx) {
    if (cpx.size() != pol_cpx.size())
        return;

    for (int i = 0; i < pol_cpx.size(); i++) {
        pol_cpx[i].mag = qSqrt((cpx[i].r * cpx[i].r) + (cpx[i].i * cpx[i].i));
        pol_cpx[i].omega = qAtan2(cpx[i].r, cpx[i].i);
    }
}

void SegmentRecover::toGauss(QVector<polar_cpx> &pol_cpx, QVector<kiss_fft_cpx> &cpx) {
    if (pol_cpx.size() != cpx.size())
        return;

    for (int i = 0; i < cpx.size(); i++) {
        cpx[i].r = pol_cpx[i].mag * qCos(pol_cpx[i].omega);
        cpx[i].i = pol_cpx[i].mag * qSin(pol_cpx[i].omega);
    }
}
