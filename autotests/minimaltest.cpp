/*
 * Copyright 2014 Alex Merry <alex.merry@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <QMetaObject>
#include <QPluginLoader>
#include <QTest>

#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
#include <QtUiPlugin/QDesignerCustomWidgetInterface>
#else
#include <QDesignerCustomWidgetInterface>
#endif

static bool widgetLessThan(QDesignerCustomWidgetInterface* w1, QDesignerCustomWidgetInterface* w2)
{
    return w1->name() < w2->name();
}

class MinimalTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase() {
        QPluginLoader loader(QStringLiteral(MINIMAL_PLUGIN));
        m_pluginInstance = loader.instance();
        QVERIFY2(m_pluginInstance, qPrintable(loader.errorString()));
        m_collection = qobject_cast<QDesignerCustomWidgetCollectionInterface*>(m_pluginInstance);
        QVERIFY(m_collection);
        m_widgets = m_collection->customWidgets();
        std::sort(m_widgets.begin(), m_widgets.end(), widgetLessThan);
    }

    void testPluginName() {
        QCOMPARE(m_pluginInstance->metaObject()->className(),
                 "WidgetsPlugin");
    }

    void testWidgetList() {
        QCOMPARE(m_widgets.count(), 2);
    }

    void testWidget_data() {
        QTest::addColumn<int>("index");
        QTest::addColumn<QString>("name");
        QTest::addColumn<QString>("includefile");
        QTest::addColumn<QString>("domxml");

        QTest::newRow("QCheckBox")
            << 0
            << "QCheckBox"
            << "qcheckbox.h"
            << "<widget class=\"QCheckBox\" name=\"qcheckbox\"/>";

        QTest::newRow("QPushButton")
            << 1
            << "QPushButton"
            << "qpushbutton.h"
            << "<widget class=\"QPushButton\" name=\"qpushbutton\"/>";
    }

    void testWidget() {
        QFETCH(int, index);
        QFETCH(QString, name);
        QFETCH(QString, includefile);
        QFETCH(QString, domxml);

        QVERIFY(m_widgets.count() > index);
        QDesignerCustomWidgetInterface *wiface = m_widgets.at(index);
        QVERIFY(wiface);
        QCOMPARE(wiface->name(), name);
        QCOMPARE(wiface->group(), QString("Custom"));
        QCOMPARE(wiface->toolTip(), QString(name + " Widget"));
        QCOMPARE(wiface->whatsThis(), QString(name + " Widget"));
        QCOMPARE(wiface->includeFile(), includefile);
        QVERIFY(!wiface->icon().isNull());
        QVERIFY(!wiface->isContainer());
        QCOMPARE(wiface->codeTemplate(), QString());
        QCOMPARE(wiface->domXml(), domxml);

        QVERIFY(!wiface->isInitialized());
        wiface->initialize(0);

        QWidget *widget = wiface->createWidget(0);
        QVERIFY(widget);
        QCOMPARE(widget->metaObject()->className(),
                 name.toLatin1().constData());
    }

private:
    QDesignerCustomWidgetCollectionInterface *m_collection;
    QObject *m_pluginInstance;
    QList<QDesignerCustomWidgetInterface*> m_widgets;
};

QTEST_MAIN(MinimalTest)

#include <minimaltest.moc>
