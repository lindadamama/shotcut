/*
 * Copyright (c) 2018-2023 Meltytech, LLC
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KEYFRAMESMODEL_H
#define KEYFRAMESMODEL_H

#include <MltAnimation.h>
#include <MltProperties.h>
#include <QAbstractItemModel>
#include <QString>

class QmlMetadata;
class QmlFilter;

class KeyframesModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum InterpolationType {
        DiscreteInterpolation,
        LinearInterpolation,
        SmoothLooseInterpolation,
        SmoothNaturalInterpolation,
        SmoothTightInterpolation,
        EaseInSinusoidal,
        EaseOutSinusoidal,
        EaseInOutSinusoidal,
        EaseInQuadratic,
        EaseOutQuadratic,
        EaseInOutQuadratic,
        EaseInCubic,
        EaseOutCubic,
        EaseInOutCubic,
        EaseInQuartic,
        EaseOutQuartic,
        EaseInOutQuartic,
        EaseInQuintic,
        EaseOutQuintic,
        EaseInOutQuintic,
        EaseInExponential,
        EaseOutExponential,
        EaseInOutExponential,
        EaseInCircular,
        EaseOutCircular,
        EaseInOutCircular,
        EaseInBack,
        EaseOutBack,
        EaseInOutBack,
        EaseInElastic,
        EaseOutElastic,
        EaseInOutElastic,
        EaseInBounce,
        EaseOutBounce,
        EaseInOutBounce,
    };
    Q_ENUM(InterpolationType)

    /// Two level model: parameters and keyframes on parameters
    enum Roles {
        NameRole = Qt::UserRole + 1, /// parameter or keyframe
        PropertyNameRole,            /// parameter only
        IsCurveRole,                 /// parameter only
        MinimumValueRole,            /// parameter only
        MaximumValueRole,            /// parameter only
        LowestValueRole,             /// parameter only
        HighestValueRole,            /// parameter only
        FrameNumberRole,             /// keyframe only
        KeyframeTypeRole,            /// keyframe only
        PrevKeyframeTypeRole,        /// keyframe only
        NumericValueRole,            /// keyframe only
        MinimumFrameRole,            /// keyframe only
        MaximumFrameRole             /// keyframe only
    };

    explicit KeyframesModel(QObject *parent = 0);
    virtual ~KeyframesModel();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    QHash<int, QByteArray> roleNames() const;
    void load(QmlFilter *, QmlMetadata *);
    Q_INVOKABLE bool remove(int parameterIndex, int keyframeIndex);
    int previousKeyframePosition(int parameterIndex, int currentPosition);
    int nextKeyframePosition(int parameterIndex, int currentPosition);
    Q_INVOKABLE int keyframeIndex(int parameterIndex, int currentPosition);
    Q_INVOKABLE int parameterIndex(const QString &propertyName) const;
    Q_INVOKABLE bool setInterpolation(int parameterIndex, int keyframeIndex, InterpolationType type);
    Q_INVOKABLE void setKeyframePosition(int parameterIndex, int keyframeIndex, int position);
    Q_INVOKABLE void addKeyframe(int parameterIndex,
                                 double value,
                                 int position,
                                 InterpolationType type);
    Q_INVOKABLE void addKeyframe(int parameterIndex, int position);
    Q_INVOKABLE void setKeyframeValue(int parameterIndex, int keyframeIndex, double value);
    Q_INVOKABLE void setKeyframeValuePosition(int parameterIndex,
                                              int keyframeIndex,
                                              double value,
                                              int position);
    Q_INVOKABLE bool isKeyframe(int parameterIndex, int position);
    Q_INVOKABLE bool advancedKeyframesInUse();
    Q_INVOKABLE void removeAdvancedKeyframes();
    Q_INVOKABLE bool simpleKeyframesInUse();
    Q_INVOKABLE void removeSimpleKeyframes();
    int keyframeCount(int index) const;

signals:
    void loaded();
    void keyframeAdded(QString parameter, int position);

public slots:
    void reload();
    void onFilterChanged(const QString &property);
    void onFilterInChanged(int delta);
    void trimFilterIn(int in);
    void trimFilterOut(int out);

private:
    QList<QString> m_propertyNames;
    QmlMetadata *m_metadata;
    QmlFilter *m_filter;
    QList<int> m_keyframeCounts;
    QList<int> m_metadataIndex;

    void updateNeighborsMinMax(int parameterIndex, int keyframeIndex);
    QStringList gangedProperties(int parameterIndex) const;
};

#endif // KEYFRAMESMODEL_H
