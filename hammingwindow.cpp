#include "hammingwindow.h"

HammingWindow::HammingWindow(int windowSize, QObject *parent) : WindowFunction(windowSize, parent) {
    initWindow();
}

void HammingWindow::initWindow() {
    if (m_window.size() <= 1) { // kvůli dělení nulou
        emit error("HammingWindow::initWindow: Velikost okna nesmí být menší nebo rovna než 1.");
        return;
    }

    for (int n = 0; n < m_window.size(); n++)
        m_window[n] = ALPHA - BETA * qCos((2 * M_PI * n) / (m_window.size() - 1));
}
