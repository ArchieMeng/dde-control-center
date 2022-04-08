/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
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

#ifndef KEYBOARDMODEL_H
#define KEYBOARDMODEL_H

#include "interface/namespace.h"
#include "metadata.h"

#include <QObject>
#include <QStringList>
#include <QMap>

namespace DCC_NAMESPACE {

class KeyboardModel : public QObject
{
    Q_OBJECT
public:
    explicit KeyboardModel(QObject *parent = nullptr);
    enum KBLayoutScope {
        system = 0,
        application = 1
    };
#ifndef DCC_DISABLE_KBLAYOUT
    void setLayoutLists(QMap<QString, QString> lists);
#endif
    QString langByKey(const QString &key) const;
    QString langFromText(const QString &text) const;

    QString curLayout() const;
    QString curLang() const;
    QMap<QString, QString> userLayout() const;
    QMap<QString, QString> kbLayout() const;
    QStringList localLang() const;
    QList<MetaData> langLists() const;
    bool capsLock() const;
    QMap<QStringList, int> allShortcut() const;

    uint repeatInterval() const;
    void setRepeatInterval(const uint &repeatInterval);

    uint repeatDelay() const;
    void setRepeatDelay(const uint &repeatDelay);

    bool numLock() const;
    void setNumLock(bool numLock);

    void cleanUserLayout();

    inline int getLangChangedState() const { return m_status; }
    void setLangChangedState(const int state);
    inline QStringList &getUserLayoutList() { return m_userLaylist; }

Q_SIGNALS:
#ifndef DCC_DISABLE_KBLAYOUT
    void curLayoutChanged(const QString &layout);
#endif
    void curLangChanged(const QString &lang);
    void capsLockChanged(bool value);
    void numLockChanged(bool value);
    void repeatDelayChanged(const uint value);
    void repeatIntervalChanged(const uint value);
    void userLayoutChanged(const QString &id, const QString &value);
    void langChanged(const QList<MetaData> &data);

    void curLocalLangChanged(const QStringList &localLangList);    
    void onSetCurLangFinish(const int value);

public Q_SLOTS:
#ifndef DCC_DISABLE_KBLAYOUT
    void setLayout(const QString &key);
#endif
    void setLang(const QString &value);
    void setLocaleLang(const QStringList &localLangList);
    void addUserLayout(const QString &id, const QString &value);
    void setLocaleList(const QList<MetaData> &langList);
    void setCapsLock(bool value);
    void setAllShortcut(const QMap<QStringList, int> &map);
private:
    QStringList convertLang(const QStringList &langList);
private:
    bool m_capsLock;
    bool m_numLock;
    uint m_repeatInterval;
    uint m_repeatDelay;
    QString m_layout;
    QString m_currentLangKey;
    QStringList m_localLangList;
    QStringList m_userLaylist;
    QMap<QString, QString> m_userLayout;
    QMap<QString, QString> m_layouts;
    QList<MetaData> m_langList;
    QMap<QStringList, int> m_shortcutMap;
    int m_status{0};
};
}
#endif // KEYBOARDMODEL_H
