#include "ioaudiowindower.h"
#include "printer.h"

IOAudioWindower::IOAudioWindower(int segmentSize, int overlap, bool extractTimeData, int bufferSize, QObject *parent) : QIODevice(parent) {
    m_segmentSize = segmentSize;
    m_overlapSupp = segmentSize - overlap;
    m_extractTimeData = extractTimeData;
    m_buffer = nullptr;

    if (bufferSize > 0)
        createBuffer(bufferSize);
}

IOAudioWindower::~IOAudioWindower() {
   if (m_buffer)
       delete [] m_buffer;
}

void IOAudioWindower::clear() {
    m_first = m_next;
}

void IOAudioWindower::finalizeWindow() {
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

    emit windowCollected(segment);
}

bool IOAudioWindower::isWindowBuffered() {
    int sizeNotOver = m_next - m_first;
    int sizeOverflow = m_bufferSize - m_first + m_next;

    if (m_first <= m_next)
        return sizeNotOver > m_segmentSize;
    else
        return sizeOverflow > m_segmentSize;
}

void IOAudioWindower::createBuffer(qint64 inputSize) {
    m_bufferSize = ((m_segmentSize > inputSize) ? m_segmentSize : inputSize) * BUFFER_SIZE_MULTIPLER;

    m_buffer = new sample[m_bufferSize];

    m_first = m_next = 0;
}

void IOAudioWindower::emitRawTimeData(const char *data, qint64 maxSize) {
    int dataCount = maxSize / sizeof(sample);

    QVector<sample> copy;
    copy.reserve(dataCount);
    std::copy((sample *)data, ((sample *)data) + dataCount, std::back_inserter(copy));

    emit dataBuffered(copy);
}

qint64 IOAudioWindower::readData(char *data, qint64 maxSize) {
    Q_UNUSED(data);
    Q_UNUSED(maxSize);
    return -1;
}

qint64 IOAudioWindower::writeData(const char *data, qint64 maxSize) {
    if (!m_buffer)
        createBuffer(maxSize / sizeof(sample));

    if (m_extractTimeData)
        emitRawTimeData(data, maxSize);

    int newPosition = m_next + (maxSize / sizeof(sample));
    if (newPosition > m_bufferSize) {
        size_t restCountBytes = (m_bufferSize - m_next) * sizeof(sample);
        size_t onStartBytes = maxSize - restCountBytes;

        memcpy(m_buffer + m_next, data, restCountBytes);
        memcpy(m_buffer, data + restCountBytes, onStartBytes);
    }
    else memcpy(m_buffer + m_next, data, static_cast<size_t>(maxSize));

    m_next = newPosition % m_bufferSize;

    while (isWindowBuffered()) finalizeWindow();

    return maxSize;
}
