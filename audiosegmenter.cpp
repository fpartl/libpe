#include "audiosegmenter.h"

AudioSegmenter::AudioSegmenter(int segmentSize, int overlap, int bufferSize, QObject *parent) : QObject(parent) {
    m_segmentSize = segmentSize;
    m_overlapSupp = segmentSize - overlap;

    if (m_segmentSize + m_overlapSupp > bufferSize) {
        m_bufferSize = m_segmentSize + m_overlapSupp + 1; // +1 kvůli modulo operaci, aby bylo možné dynamicky nastavit přesnou velikost bufferu
    }
    else m_bufferSize = bufferSize + 1;

    m_buffer = new sample[m_bufferSize];
    m_first = m_next = 0;
}

AudioSegmenter::~AudioSegmenter() {
    delete [] m_buffer;
}

void AudioSegmenter::writeAudio(const QVector<sample> &data) {
    if (data.size() > m_bufferSize) {
        emit error("AudioSegmenter: Přijatá data jsou větší než buffer. V lepším případě to skončí špatně, v horším to padne na SIGSEGV.");
        return;
    }

    int newPosition = m_next + data.size();
    if (newPosition > m_bufferSize) {
        int restCount = m_bufferSize - m_next;
        int onStart = data.size() - restCount;

        memcpy(m_buffer + m_next, data.constData(), static_cast<size_t>(restCount) * sizeof(sample));
        memcpy(m_buffer, data.constData() + restCount, static_cast<size_t>(onStart) * sizeof(sample));
    }
    else memcpy(m_buffer + m_next, data.constData(), static_cast<size_t>(data.size()) * sizeof(sample));

    m_next = newPosition % m_bufferSize;
}

bool AudioSegmenter::hasNextSegment() {
    int sizeNotOver = m_next - m_first;
    int sizeOverflow = m_bufferSize - m_first + m_next;

    if (m_first <= m_next)
        return sizeNotOver > m_segmentSize;
    else
        return sizeOverflow > m_segmentSize;
}

QVector<sample> AudioSegmenter::nextSegment() {
    if (isEmpty())
        return QVector<sample>();
    else if (!hasNextSegment())
        return flush();

    QVector<sample> segment;
    segment.reserve(m_segmentSize);

    if (m_first + m_segmentSize > m_bufferSize) {
        int rest = m_bufferSize - m_first;
        int onStart = m_segmentSize - rest;

        std::copy(m_buffer + m_first, m_buffer + m_first + rest, std::back_inserter(segment));
        std::copy(m_buffer, m_buffer + onStart, std::back_inserter(segment));
    }
    else std::copy(m_buffer + m_first, m_buffer + m_first + m_segmentSize, std::back_inserter(segment));

    m_first = (m_first + m_overlapSupp) % m_bufferSize;

    return segment;
}

QVector<sample> AudioSegmenter::flush() {
    QVector<sample> segment;
    segment.reserve(m_segmentSize);

    if (m_first > m_next) {
        int rest = m_bufferSize - m_first;
        int onStart = m_next;

        std::copy(m_buffer + m_first, m_buffer + m_first + rest, std::back_inserter(segment));
        std::copy(m_buffer, m_buffer + onStart, std::back_inserter(segment));
    }
    else std::copy(m_buffer + m_first, m_buffer + m_next, std::back_inserter(segment));

    segment.append(QVector<sample>(m_segmentSize - segment.size(), 0));

    m_first = m_next;
    return segment;
}

bool AudioSegmenter::isEmpty() {
    return m_first == m_next;
}
