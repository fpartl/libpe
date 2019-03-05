#include "fft.h"

FFT::FFT(int segmentSize, QObject *parent) : QObject(parent) {
    if (segmentSize <= 0) {
        emit error("FFT::FFT: Neplatná velikost vstupních segmentů.");
        return;
    }

    // tento výpočet vyplívá z definice algoritmu FFT
    for (m_maxSegmentSize = 1; m_maxSegmentSize < segmentSize; m_maxSegmentSize *= 2);
    m_espdSize = (m_maxSegmentSize / 2) + 1;

    m_fftCfg = kiss_fftr_alloc(m_maxSegmentSize, 0, nullptr, nullptr);
    m_ifftCft = kiss_fftr_alloc(m_maxSegmentSize, 1, nullptr, nullptr);
    if (!m_fftCfg || !m_ifftCft) {
        emit error("FFT::FFT: Nepodařilo se alokovat potřebné zdroje (kissFFT).");
        return;
    }
}

FFT::~FFT() {
    free(m_fftCfg);
    free(m_ifftCft);
}

int FFT::espdSize() const {
    return m_espdSize;
}

QVector<float> FFT::transformEucl(QVector<float> segment) {
    if (segment.isEmpty())
        return QVector<float>();

    if (segment.size() > m_maxSegmentSize) {
        emit error("FFT::transformEucl: Vstupní segment je větší je přednastavená velikost.");
        return QVector<float>();
    }

    if (segment.size() < m_maxSegmentSize)
        segment = prepareSegment(segment);

    QVector<kiss_fft_cpx> cpx = realFFt(segment);
    if (cpx.isEmpty()) return QVector<float>();

    return euclidNorm(cpx);
}

QVector<kiss_fft_cpx> FFT::realFFt(QVector<float> segment) {
    if (segment.size() != m_maxSegmentSize)
        return QVector<kiss_fft_cpx>();

    QVector<kiss_fft_cpx> cpx(m_espdSize);
    kiss_fftr(m_fftCfg, segment.constData(), cpx.data());

    return cpx;
}

QVector<float> FFT::invRealFFT(QVector<kiss_fft_cpx> cpx) {
    if (cpx.size() != m_espdSize)
        return QVector<float>();

    QVector<float> segment(m_maxSegmentSize);
    kiss_fftri(m_ifftCft, cpx.constData(), segment.data());

    QMutableVectorIterator<float> i(segment);
    while (i.hasNext()) {
        float i_val = i.next();

        i.setValue(i_val / static_cast<float>(m_espdSize));
    }

    return segment;
}

void FFT::realFFt(QVector<float> &segment, QVector<kiss_fft_cpx> &cpx) {
    if (segment.size() != m_maxSegmentSize || cpx.size() != m_espdSize)
        return;

    kiss_fftr(m_fftCfg, segment.constData(), cpx.data());
}

void FFT::invRealFFT(QVector<kiss_fft_cpx> &cpx, QVector<float> &segment) {
    if (cpx.size() != m_espdSize || segment.size() != m_maxSegmentSize)
        return;

    kiss_fftri(m_ifftCft, cpx.constData(), segment.data());

    QMutableVectorIterator<float> i(segment);
    while (i.hasNext()) {
        float i_val = i.next();

        i.setValue(i_val / static_cast<float>(m_espdSize));
    }
}

QVector<float> FFT::prepareSegment(QVector<float> segment) {
    if (segment.isEmpty())
        return QVector<float>();

    QVector<float> fftInput = segment;

    int toFill = m_maxSegmentSize - fftInput.size();

    for (int i = 0; i < toFill; i++)
        fftInput.append(0);

    return fftInput;
}

QVector<float> FFT::euclidNorm(QVector<kiss_fft_cpx> cpx) {
    if (cpx.isEmpty()) return QVector<float>();

    QVector<float> magnitudes;
    magnitudes.reserve(m_espdSize);

    for (kiss_fft_cpx val : cpx) {
        float mag = qSqrt((val.r * val.r) + (val.i * val.i));

        magnitudes.append(mag);
    }

    return magnitudes;
}
