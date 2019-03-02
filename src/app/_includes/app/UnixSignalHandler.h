#pragma once

#include "gsl/pointers"

#include <QObject>
#include <QPointer>

#include <array>

class QCoreApplication;
class QSocketNotifier;

namespace appkit
{

class UnixSignalHandler : public QObject
{
    Q_OBJECT
public:
    /**
     * Construct signal hander for a specific @a signal
     */
    explicit UnixSignalHandler(int signal, QObject* parent = nullptr);

signals:
    void raised();

private slots:
    void read(int socket);

private:
    int m_sockets[2] = { 0, 0 };
    QSocketNotifier* m_notifier = nullptr;

    static const int max_signal = 32;
    static std::array<QPointer<UnixSignalHandler>, max_signal> m_handlers;
    static void handle(int signal);
};

void installSignalHandler(gsl::not_null<QCoreApplication*> app);

} // namespace appkit
