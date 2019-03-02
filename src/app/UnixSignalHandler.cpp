#include "app/UnixSignalHandler.h"

#include <QCoreApplication>
#include <QSocketNotifier>

#include <csignal>
#include <sys/socket.h>
#include <unistd.h>

namespace appkit
{

UnixSignalHandler::UnixSignalHandler(int signal, QObject* parent)
    : QObject(parent)
{
    Q_ASSERT(m_handlers[signal] == nullptr);

    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, m_sockets))
    {
        return;
    }

    m_notifier = new QSocketNotifier(m_sockets[1], QSocketNotifier::Read, this);
    connect(
        m_notifier,
        &QSocketNotifier::activated,
        this,
        &UnixSignalHandler::read);

    std::signal(signal, &UnixSignalHandler::handle);

    m_handlers[signal] = this;
}

void UnixSignalHandler::read(int socket)
{
    m_notifier->setEnabled(false);
    char tmp;
    ::read(socket, &tmp, sizeof(tmp));
    emit raised();
    m_notifier->setEnabled(true);
}

void UnixSignalHandler::handle(int signal)
{
    auto const signalHandler = m_handlers[signal];
    if (signalHandler)
    {
        char c = 1;
        ::write(signalHandler->m_sockets[0], &c, sizeof(c));
    }
}

std::array<QPointer<UnixSignalHandler>, UnixSignalHandler::max_signal>
    UnixSignalHandler::m_handlers;

void installSignalHandler(gsl::not_null<QCoreApplication*> app)
{
#ifdef SIGINT
    QObject::connect(
        new UnixSignalHandler(SIGINT, app),
        &UnixSignalHandler::raised,
        app,
        &QCoreApplication::quit,
        Qt::QueuedConnection);
#endif
#ifdef SIGTERM
    QObject::connect(
        new UnixSignalHandler(SIGTERM, app),
        &UnixSignalHandler::raised,
        app,
        &QCoreApplication::quit,
        Qt::QueuedConnection);
#endif
}

} // namespace appkit
