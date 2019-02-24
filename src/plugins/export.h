/** @file
 * @brief    Export plugin helper macro
 *
 * @copyright  (C) 2015 PKB RIO Design Department
 *
 * $Id: $
 */

#include <QtPlugin>

#ifndef RIO_EXPORT_PLUGIN
#   ifndef QT5_BUILD
#       define RIO_EXPORT_PLUGIN(name) Q_EXPORT_PLUGIN2(PLUGIN_TARGET, name)
#   else
#       define RIO_EXPORT_PLUGIN(name)
#   endif // QT5_BUILD
#endif // RIO_EXPORT_PLUGIN
