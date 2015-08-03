/*******************************************************************************
 * Copyright 2015 Panagiotis Anastasiadis
 * This file is part of SYLoader.
 *
 * SYLoader is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * SYLoader is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SYLoader. If not, see http://www.gnu.org/licenses.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 *
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL. If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so. If you
 * do not wish to do so, delete this exception statement from your
 * version. If you delete this exception statement from all source
 * files in the program, then also delete it here.
 ******************************************************************************/

#include "updater.h"
#include "global.h"
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QStringList>





Updater::Updater()
{

}





bool
Updater::checkForUpdates(bool &update)
{
    QEventLoop loop;
    //QNetworkAccessManager gateway;
    QNetworkReply *updateReply = Gateway->get(
        QNetworkRequest(QUrl(UPDATE_CHECK_URL)));
    QObject::connect(updateReply, SIGNAL(finished()), &loop, SLOT(quit()));

    loop.exec();

    int major, minor, patch;
    bool newVersion = false, error = true;
    bool majorOk, minorOk, patchOk;

    QByteArray replyBytes = updateReply->readAll();
    QString replyString (replyBytes);
    updateReply->deleteLater();

    QStringList versionDigits = replyString.split(".", QString::SkipEmptyParts);

    if (versionDigits.length() == 3)
    {
        major = versionDigits.at(0).toInt(&majorOk, 10);
        minor = versionDigits.at(1).toInt(&minorOk, 10);
        patch = versionDigits.at(2).toInt(&patchOk, 10);

        if (majorOk == true && minorOk == true && patchOk == true)
        {
            if (major > SOFTWARE_VERSION_MAJOR)
                newVersion = true;
            else if (major == SOFTWARE_VERSION_MAJOR)
            {
                if (minor > SOFTWARE_VERSION_MINOR)
                    newVersion = true;
                else if (minor == SOFTWARE_VERSION_MINOR)
                {
                    if (patch > SOFTWARE_VERSION_PATCH)
                        newVersion = true;
                }
            }

            error = false;
        }
    }

    delete updateReply;

    update = newVersion;
    return error;
}
