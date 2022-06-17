/*
 * Copyright (C) 2021 ~ 2023 Deepin Technology Co., Ltd.
 *
 * Author:     caixiangrong <caixiangrong@uniontech.com>
 *
 * Maintainer: caixiangrong <caixiangrong@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "frameproxyv20.h"
#include "moduleinterface.h"

#include <QDBusMessage>
#include <QDBusConnection>
#include <QDBusPendingCall>

using namespace dccV20;

FrameProxyV20::FrameProxyV20()
{
}

FrameProxyV20::~FrameProxyV20()
{
}

void FrameProxyV20::setRootModule(DCC_NAMESPACE::ModuleObject *rootModule)
{
    m_rootModule = rootModule;
}

void FrameProxyV20::append(AdapterV20toV23Module *module)
{
    m_moduleMap.insert(module->inter(), module);
}

void FrameProxyV20::pushWidget(ModuleInterface *const inter, QWidget *const w, dccV20::FrameProxyInterface::PushType type)
{
    if (!m_moduleMap.contains(inter))
        return;

    AdapterV20toV23Module *module = m_moduleMap.value(inter);
    m_widgets.push(w);
    switch (type) {
    case Replace:
    case CoverTop:  //根据当前页面宽度去计算新增的页面放在最后面一层，或者Top页面
    case DirectTop: //不需要管页面宽度，直接将新增页面放在Top页面；为解决某些页面使用CoverTop无法全部展示的问题
        module->setChildPage(1, w);
        break;
    case Normal:
    default:
        module->setChildPage(0, w);
        break;
    }
}

void FrameProxyV20::popWidget(ModuleInterface *const inter)
{
    if (m_widgets.isEmpty())
        return;
    QWidget *w = m_widgets.pop();
    for (auto &&module : m_moduleMap)
        module->popWidget(w);
}

void FrameProxyV20::setModuleVisible(ModuleInterface *const inter, const bool visible)
{
    if (m_moduleMap.contains(inter))
        m_moduleMap.value(inter)->setHiden(!visible);
}

void FrameProxyV20::showModulePage(const QString &module, const QString &page, bool animation)
{
    // DBus 切换页面 module/page
    Q_UNUSED(animation)
    QString arg = module;
    if (!page.isEmpty())
        arg += "/" + page;
    QDBusMessage message = QDBusMessage::createMethodCall("com.deepin.dde.ControlCenter", "/com/deepin/dde/ControlCenter", "com.deepin.dde.ControlCenter", "ShowPage");
    message << arg;
    QDBusConnection::sessionBus().asyncCall(message);
}

void FrameProxyV20::setModuleSubscriptVisible(const QString &module, bool bIsDisplay)
{
    Q_UNUSED(module);
    Q_UNUSED(bIsDisplay);
}
// 该函数已废弃
void FrameProxyV20::setRemoveableDeviceStatus(QString type, bool state)
{
    Q_UNUSED(type);
    Q_UNUSED(state);
}
// 该函数已废弃
bool FrameProxyV20::getRemoveableDeviceStatus(QString type) const
{
    Q_UNUSED(type);
    return false;
}

void FrameProxyV20::setSearchPath(ModuleInterface *const inter) const
{
}

void FrameProxyV20::addChildPageTrans(const QString &menu, const QString &rran)
{
}

void FrameProxyV20::setModuleVisible(const QString &module, bool visible)
{
    auto find_it = std::find_if(m_moduleMap.cbegin(), m_moduleMap.cend(), [module](auto &it) {
        return it->displayName() == module;
    });
    if (find_it != m_moduleMap.cend())
        find_it.value()->setHiden(!visible);
}

void FrameProxyV20::setWidgetVisible(const QString &module, const QString &widget, bool visible)
{
    auto find_it = std::find_if(m_moduleMap.cbegin(), m_moduleMap.cend(), [module](auto &it) {
        return it->displayName() == module;
    });
    if (find_it == m_moduleMap.cend())
        return;

    QStringList content = find_it.value()->contentText();
    if (visible) {
        if (content.contains(widget))
            content.append(widget);
    } else {
        content.removeAll(widget);
    }
    find_it.value()->setContentText(content);
}

void FrameProxyV20::setDetailVisible(const QString &module, const QString &widget, const QString &detail, bool visible)
{
    auto find_it = std::find_if(m_moduleMap.cbegin(), m_moduleMap.cend(), [module, widget](auto &it) {
        return it->displayName() == widget || it->displayName() == module;
    });
    if (find_it == m_moduleMap.cend())
        return;

    if (visible)
        find_it.value()->addContentText(detail);
    else {
        QStringList content = find_it.value()->contentText();
        content.removeAll(detail);
        find_it.value()->setContentText(content);
    }
}

void FrameProxyV20::updateSearchData(const QString &module)
{
    Q_UNUSED(module);
}

QString FrameProxyV20::moduleDisplayName(const QString &module) const
{
    auto find_it = std::find_if(m_moduleMap.cbegin(), m_moduleMap.cend(), [module](auto &it) {
        return it->name() == module;
    });
    if (find_it == m_moduleMap.cend()) {
        qDebug() << "Not found module:" << module;
        return QString();
    }
    return find_it.key()->displayName();
}
