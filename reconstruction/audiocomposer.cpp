#include "audiocomposer.h"

AudioComposer::AudioComposer(int segmentSize, int overlap, bool avgOverlap, QObject *parent) : QObject(parent) {
    m_segmentSize = segmentSize;
    m_overlap = overlap;
    m_overlapSupp = segmentSize - overlap;
    m_avgOverlap = avgOverlap;
    m_bufferSize = m_segmentSize;
    m_buffer = new float[m_bufferSize];
    m_first = m_next = 0;
}

AudioComposer::~AudioComposer() {
    delete [] m_buffer;
}

QVector<float> AudioComposer::getAudio() {
    QVector<float> result;

    result.append(m_audio);
    result.append(extractAudio(m_overlap));

    return result;
}

void AudioComposer::clear() {
    m_audio.clear();
    m_first = m_next;
}

bool AudioComposer::isEmpty() {
    return m_first == m_next;
}

void AudioComposer::averateSegOverlap(const QVector<float> &segment) {
    int nextMod;

    for (int i = 0; i < m_overlap; i++) {
        nextMod = (m_next + i) % m_bufferSize;

        m_buffer[nextMod] = (m_buffer[nextMod] + segment[i]) / 2;
    }
}

QVector<float> AudioComposer::extractAudio(int size) {
    QVector<float> extracted;
    extracted.reserve(size);

    if (m_first + size > m_bufferSize) {
        int rest = m_bufferSize - m_first;
        int onStart = size - rest;

        std::copy(m_buffer + m_first, m_buffer + m_first + rest, std::back_inserter(extracted));
        std::copy(m_buffer, m_buffer + onStart, std::back_inserter(extracted));
    }
    else std::copy(m_buffer + m_first, m_buffer + m_first + size, std::back_inserter(extracted));

    return extracted;
}

void AudioComposer::write(const QVector<float> &floats) {
    if (m_next + floats.size() > m_bufferSize) {
        int toBufferEnd = m_bufferSize - m_next;
        int remaing = floats.size() - toBufferEnd;

        std::copy(floats.begin(), floats.begin() + toBufferEnd, m_buffer + m_next);
        std::copy(floats.begin() + toBufferEnd, floats.begin() + toBufferEnd + remaing, m_buffer);
    }
    else std::copy(floats.begin(), floats.begin() + floats.size(), m_buffer);
}

void AudioComposer::add(const QVector<float> &segment) {
    if (segment.size() != m_segmentSize)
        return;

    if (isEmpty()) {
        write(segment);
    }
    else {
        if (m_avgOverlap)
            averateSegOverlap(segment);

        write(segment.mid(m_overlap));
    }

    m_next = (m_next + m_overlapSupp) % m_bufferSize;

    QVector<float> extracted = extractAudio(m_overlapSupp);
    m_first = m_next; // same as = (m_first + m_overlapSupp) % m_bufferSize;
    m_audio.append(extracted);
}
