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

#include "progressitemdelegate.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QStyleOptionProgressBar>
#include <QString>
#include <QVariant>
#include <QLocale>

ProgressItemDelegate::ProgressItemDelegate(QObject *object)
    : QStyledItemDelegate(object) {}

void
ProgressItemDelegate::paint(
    QPainter *painter,
    const QStyleOptionViewItem &option,
    const QModelIndex &index ) const
{
    if (index.column() != 2)
    {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }
    else
    {
        QStyleOptionProgressBar progressBarOption;
        progressBarOption.state = QStyle::State_Enabled;
        progressBarOption.direction = QApplication::layoutDirection();
        progressBarOption.rect = option.rect;
        progressBarOption.fontMetrics = QApplication::fontMetrics();
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.textAlignment = Qt::AlignCenter;
        progressBarOption.textVisible = true;

        int progress = index.model()
                ->data(index.model()->index(index.row(), 2)).toInt();

        progressBarOption.progress = progress < 0 ? 0 : progress;

        progressBarOption.text = QString().sprintf(
                    "%d%%",
                    progressBarOption.progress);

        QApplication::style()->drawControl(
                    QStyle::CE_ProgressBar,
                    &progressBarOption,
                    painter);
    }
}
