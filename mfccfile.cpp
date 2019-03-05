#include "mfccfile.h"

MfccFile::MfccFile(const QString& fileName, QObject *parent) : QObject(parent) {
    m_fileName = fileName;
}

bool MfccFile::isReadable() {
    QFile file(m_fileName);
    if (!file.open(QFile::ReadOnly))
        return false;

    return hasValidSize(&file);
}

bool MfccFile::isReadable(const QString &fileName) {
    if (fileName.isEmpty())
        return false;

    MfccFile temp(fileName);
    return temp.isReadable();
}

bool MfccFile::isWritable() {
    if (m_fileName.isEmpty())
        return false;

    QFileInfo info(m_fileName);
    if (info.exists()) {
        QFile file(m_fileName);
        if (!file.open(QFile::ReadWrite))
            return false;

        return hasValidSize(&file);
    }
    else {
        QFileInfo dirInfo(QFileInfo(m_fileName).dir().absolutePath());
        return dirInfo.isDir() && dirInfo.isWritable();
    }
}

bool MfccFile::isWritable(const QString &fileName) {
    if (fileName.isEmpty())
        return false;

    MfccFile temp(fileName);
    return temp.isWritable();
}

bool MfccFile::exists() {
    QFileInfo fileInfo(m_fileName);

    return fileInfo.exists();
}

void MfccFile::clear() {
    QFile file (m_fileName);
    file.remove();
}

void MfccFile::clear(const QString &fileName) {
    QFile file (fileName);
    file.remove();
}

bool MfccFile::write(const QVector<QVector<float>> &mfccs) {
    if (!isValidMfccVector(mfccs))
        return false;

    QFile mfccFile(m_fileName);
    if (!mfccFile.open(QFile::WriteOnly | QFile::Truncate))
        return false;

    QDataStream output(&mfccFile);
    initStream(&output);

    output << static_cast<int32_t>(mfccs.size());

    for (QVector<float> i : mfccs) {
        for (float j : i)
            output << j;
    }

    return true;
}

bool MfccFile::append(const QVector<float> &mfccs, bool checkFile) {
    if (mfccs.size() != MFCC_COUNT || (checkFile && !isWritable()))
        return false;

    if (!exists()) {
        QVector<QVector<float>> temp;
        temp << mfccs;

        return write(temp);
    }

    QFile file(m_fileName);
    if (!file.open(QFile::ReadWrite))
        return false;

    QDataStream output(&file);
    initStream(&output);

    /* Append MFCC data at the end of the file. */
    output.device()->seek(file.size());
    for (float i : mfccs)
        output << i;

    /* Update vectors count. */
    output.device()->seek(0);
    int32_t vectors;
    output >> vectors;
    vectors++;
    output.device()->seek(0);
    output << vectors;

    return true;
}

QVector<QVector<float>> MfccFile::readAll(bool checkFile) {
    if (checkFile && !isReadable())
        return QVector<QVector<float>>();

    QFile file(m_fileName);
    if (!file.open(QFile::ReadOnly))
        return QVector<QVector<float>>();

    QDataStream stream(&file);
    initStream(&stream);

    int32_t vectors;
    stream >> vectors;

    float temp;
    QVector<QVector<float>> mfccs;
    for (int i = 0; i < vectors; i++) {
        QVector<float> segmentMfcc;

        for (int j = 0; j < MFCC_COUNT; j++) {
            stream >> temp;
            segmentMfcc.append(temp);
        }

        mfccs.append(segmentMfcc);
    }

    return mfccs;
}

void MfccFile::initStream(QDataStream *stream) {
    if (!stream) return;

    stream->setByteOrder(QDataStream::LittleEndian);
    stream->setFloatingPointPrecision(QDataStream::SinglePrecision);
    stream->setVersion(QDataStream::Qt_5_9);
}

bool MfccFile::hasValidSize(QFile *mfccFile) {
    if (!mfccFile || mfccFile->size() < static_cast<qint64>(sizeof(int32_t)))
        return false;

    QDataStream stream(mfccFile);
    initStream(&stream);

    int32_t vectorsCount;
    stream >> vectorsCount;

    qint64 vectors = ((mfccFile->size() - static_cast<qint64>(sizeof(int32_t))) / static_cast<qint64>(sizeof(float))) / MFCC_COUNT;

    if (vectors != vectorsCount) {
        qDebug() << "Invalid file size: " << vectors << " != " << vectorsCount << "!";
    }

    return vectors == vectorsCount;
}

bool MfccFile::isValidMfccVector(const QVector<QVector<float>> &mfccs) {
    if (mfccs.isEmpty())
        return false;

    for (QVector<float> i : mfccs) {
        if (i.size() != MFCC_COUNT)
            return false;
    }

    return true;
}
