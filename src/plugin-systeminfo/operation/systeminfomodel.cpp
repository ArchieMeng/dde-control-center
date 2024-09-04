//SPDX-FileCopyrightText: 2018 - 2023 UnionTech Software Technology Co., Ltd.
//
//SPDX-License-Identifier: GPL-3.0-or-later
#include "systeminfomodel.h"
#include "QtQml/qqml.h"
#include "dccfactory.h"
#include "math.h"

#include <qdebug.h>

namespace DCC_NAMESPACE{

static QString formatCap(qulonglong cap, const int size = 1024, quint8 precision = 1)
{
    static QStringList type = {" B", " KB", " MB", " GB", " TB"};
    qulonglong lc = cap;
    double dc = cap;
    double ds = size;

    for(int p = 0; p < type.count(); ++p) {
        if (cap < pow(size, p + 1) || p == type.count() - 1) {
            if (!precision) {
#ifdef __sw_64__
                return QString::number(ceil(lc / pow(size, p))) + type[p];
#else
                return QString::number(round(lc / pow(size, p))) + type[p];
#endif
            }

            return QString::number(dc / pow(ds, p), 'f', precision) + type[p];
        }
    }

    return "";
}

SystemInfoModel::SystemInfoModel(QObject *parent)
    : QObject(parent)
    , m_type("64")
    , m_licenseState(ActiveState::Unauthorized)
    , m_privacyPolicy("")
    , m_gnuLinceseTitle("")
    , m_gnuLinceseContent("")
    , m_userLicense("")
    , m_joinUeProgram(false)
{

}

void SystemInfoModel::setProductName(const QString& name)
{
    if(m_productName == name)
        return;

    m_productName = name;
    Q_EMIT productNameChanged(m_productName);
}
void SystemInfoModel::setVersionNumber(const QString& number)
{
    if(m_versionNumber == number)
        return;

    m_versionNumber = number;
    Q_EMIT versionNumberChanged(m_versionNumber);
}

void SystemInfoModel::setVersion(const QString &version)
{
    if(m_version == version)
        return;

    m_version = version;
    Q_EMIT versionChanged(m_version);
}

void SystemInfoModel::setType(qlonglong type)
{
    if(m_type == QString("%1").arg(type))
        return ;

    m_type = QString("%1").arg(type);
    typeChanged(m_type);
}

void SystemInfoModel::setProcessor(const QString &processor)
{
    if(m_processor == processor)
        return;

    m_processor = processor;
    processorChanged(processor);
}

void SystemInfoModel::setHostName(const QString &hostName)
{
    m_hostName = hostName;
    hostNameChanged(hostName);
}

void SystemInfoModel::setEndUserAgreementPath(const QString &path)
{
    m_endUserAgreementTextPath = path;
}

bool SystemInfoModel::showDetail() const
{
    return m_showDetail;
}

void SystemInfoModel::setShowDetail(bool newShowDetail)
{
    if (m_showDetail == newShowDetail)
        return;
    m_showDetail = newShowDetail;
    emit showDetailChanged();
}

void SystemInfoModel::onLicenseStateChanged(const ActiveState &state)
{
    if (state == Authorized) {
        setLicenseStatusText(QObject::tr("Activated"));
        setLicenseStatusColor(QColor(21, 187, 24));
        setLicenseActionText(QObject::tr("View"));
    } else if (state == Unauthorized) {
        setLicenseStatusText(QObject::tr("To be activated"));
        setLicenseStatusColor(QColor(255, 87, 54));
        setLicenseActionText(QObject::tr("Activate"));
    } else if (state == AuthorizedLapse) {
        setLicenseStatusText(QObject::tr("Expired"));
        setLicenseStatusColor(QColor(255, 87, 54));
        setLicenseActionText(QObject::tr("View"));
    } else if (state == TrialAuthorized) {
        setLicenseStatusText(QObject::tr("In trial period"));
        setLicenseStatusColor(QColor(255, 170, 0));
        setLicenseActionText(QObject::tr("Activate"));
    } else if (state == TrialExpired) {
        setLicenseStatusText(QObject::tr("Trial expired"));
        setLicenseStatusColor(QColor(255, 87, 54));
        setLicenseActionText(QObject::tr("Activate"));
    }
}

QColor SystemInfoModel::licenseStatusColor() const
{
    return m_licenseStatusColor;
}

void SystemInfoModel::setLicenseStatusColor(const QColor &newLicenseStatusColor)
{
    if (m_licenseStatusColor == newLicenseStatusColor)
        return;
    m_licenseStatusColor = newLicenseStatusColor;
    emit licenseStatusColorChanged();
}

QString SystemInfoModel::licenseActionText() const
{
    return m_licenseActionText;
}

void SystemInfoModel::setLicenseActionText(const QString &newLicenseActionText)
{
    if (m_licenseActionText == newLicenseActionText)
        return;
    m_licenseActionText = newLicenseActionText;
    emit licenseActionTextChanged();
}

QString SystemInfoModel::licenseStatusText() const
{
    return m_licenseStatusText;
}

void SystemInfoModel::setLicenseStatusText(const QString &newLicenseStatusText)
{
    if (m_licenseStatusText == newLicenseStatusText)
        return;
    m_licenseStatusText = newLicenseStatusText;
    emit licenseStatusTextChanged();
}

QString SystemInfoModel::systemCopyright() const
{
    return m_systemCopyright;
}

void SystemInfoModel::setSystemCopyright(const QString &newSystemCopyright)
{
    if (m_systemCopyright == newSystemCopyright)
        return;
    m_systemCopyright = newSystemCopyright;
    emit systemCopyrightChanged();
}

bool SystemInfoModel::joinUeProgram() const
{
    return m_joinUeProgram;
}

void SystemInfoModel::setJoinUeProgram(bool newJoinUeProgram)
{
    if (m_joinUeProgram == newJoinUeProgram)
        return;
    m_joinUeProgram = newJoinUeProgram;
    emit joinUeProgramChanged();
}

QString SystemInfoModel::userExperienceProgramText() const
{
    return m_userExperienceProgramText;
}

void SystemInfoModel::setUserExperienceProgramText(const QString &newUserExperienceProgramText)
{
    if (m_userExperienceProgramText == newUserExperienceProgramText)
        return;
    m_userExperienceProgramText = newUserExperienceProgramText;
    emit userExperienceProgramTextChanged();
}

QString SystemInfoModel::userLicense() const
{
    return m_userLicense;
}

void SystemInfoModel::setUserLicense(const QString &newUserLicense)
{
    if (m_userLicense == newUserLicense)
        return;
    m_userLicense = newUserLicense;
    emit userLicenseChanged();
}

QString SystemInfoModel::gnuLinceseContent() const
{
    return m_gnuLinceseContent;
}

void SystemInfoModel::setGnuLinceseContent(const QString &newGnuLinceseContent)
{
    if (m_gnuLinceseContent == newGnuLinceseContent)
        return;
    m_gnuLinceseContent = newGnuLinceseContent;
    emit gnuLinceseContentChanged();
}

QString SystemInfoModel::gnuLinceseTitle() const
{
    return m_gnuLinceseTitle;
}

void SystemInfoModel::setGnuLinceseTitle(const QString &newGnuLinceseTitle)
{
    if (m_gnuLinceseTitle == newGnuLinceseTitle)
        return;
    m_gnuLinceseTitle = newGnuLinceseTitle;
    emit gnuLinceseTitleChanged();
}

QString SystemInfoModel::privacyPolicy() const
{
    return m_privacyPolicy;
}

void SystemInfoModel::setPrivacyPolicy(const QString &newPrivacyPolicy)
{
    if (m_privacyPolicy == newPrivacyPolicy)
        return;
    m_privacyPolicy = newPrivacyPolicy;
    emit privacyPolicyChanged();
}

void SystemInfoModel::setMemory(qulonglong totalMemory, qulonglong installedMemory)
{
    QString mem_device_size = formatCap(installedMemory, 1024, 0);
    QString mem = formatCap(totalMemory);
    if(m_memory == mem)
        return ;

    m_memory = mem;
    m_memory = QString("%1 (%2)").arg(mem, tr("available"));
    memoryChanged(m_memory);
}


void SystemInfoModel::setKernel(const QString &kernel)
{
    if (m_kernel == kernel)
        return;

    m_kernel = kernel;
    kernelChanged(kernel);
}

void SystemInfoModel::setLicenseState(DCC_NAMESPACE::ActiveState state)
{
    if (m_licenseState != state) {
        m_licenseState = state;
        Q_EMIT licenseStateChanged(state);
    }

    onLicenseStateChanged(state);
}

}
