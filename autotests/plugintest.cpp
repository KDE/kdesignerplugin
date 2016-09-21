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

#include <QGroupBox>
#include <QMetaObject>
#include <QPluginLoader>
#include <QTest>

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

static bool widgetLessThan(QDesignerCustomWidgetInterface* w1, QDesignerCustomWidgetInterface* w2)
{
    return w1->name() < w2->name();
}

class PluginTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase() {
        QPluginLoader loader(QStringLiteral(TEST_PLUGIN));
        m_pluginInstance = loader.instance();
        QVERIFY2(m_pluginInstance, qPrintable(loader.errorString()));
        m_collection = qobject_cast<QDesignerCustomWidgetCollectionInterface*>(m_pluginInstance);
        QVERIFY(m_collection);
        m_widgets = m_collection->customWidgets();
        std::sort(m_widgets.begin(), m_widgets.end(), widgetLessThan);
    }

    void testPluginName() {
        QCOMPARE(m_pluginInstance->metaObject()->className(),
                 "SomeClass");
    }

    void testWidgetList() {
        QCOMPARE(m_widgets.count(), 4);
    }

    void testWidget_data() {
        QTest::addColumn<int>("index");
        QTest::addColumn<QString>("name");
        QTest::addColumn<QString>("implclass");
        QTest::addColumn<QString>("group");
        QTest::addColumn<QString>("tooltip");
        QTest::addColumn<QString>("whatsthis");
        QTest::addColumn<QString>("includefile");
        QTest::addColumn<QString>("codetemplate");
        QTest::addColumn<QString>("domxml");
        QTest::addColumn<bool>("iscontainer");
        QTest::addColumn<bool>("inittoggles");

        QTest::newRow("QCheckBox")
            << 0
            << "QCheckBox"
            << "QCheckBox"
            << "The default group"
            << "The check box tooltip"
            << "The check box whats this text"
            << "qcheckbox.h"
            << ""
            << "<widget class=\"QCheckBox\" name=\"qcheckbox\"/>"
            << false
            << true;

        QTest::newRow("QGroupBox")
            << 1
            << "QGroupBox"
            << "QGroupBox"
            << "The default group"
            << "QGroupBox Widget"
            << "QGroupBox Widget"
            << "qgroupbox.h"
            << ""
            << "<widget class=\"QGroupBox\" name=\"qgroupbox\"/>"
            << true
            << false;

        // 2 is QLabel, but the widget creation will fail

        QTest::newRow("QPushButton")
            << 3
            << "QPushButton"
            << "QCommandLinkButton"
            << "A group for QPushButton"
            << "A tooltip for QPushButton"
            << "A whatsthis for QPushButton"
            << "QCommandLinkButton"
            << ""
            << "<widget class=\"QPushButton\" name=\"qpushbutton\"/>"
            << false
            << false;
    }

    void testWidget() {
        QFETCH(int, index);
        QFETCH(QString, name);
        QFETCH(QString, implclass);
        QFETCH(QString, group);
        QFETCH(QString, tooltip);
        QFETCH(QString, whatsthis);
        QFETCH(QString, includefile);
        QFETCH(QString, codetemplate);
        QFETCH(QString, domxml);
        QFETCH(bool, iscontainer);
        QFETCH(bool, inittoggles);

        QVERIFY(m_widgets.count() > index);
        QDesignerCustomWidgetInterface *wiface = m_widgets.at(index);
        QVERIFY(wiface);
        QCOMPARE(wiface->name(), name);
        QCOMPARE(wiface->group(), group);
        QCOMPARE(wiface->toolTip(), tooltip);
        QCOMPARE(wiface->whatsThis(), whatsthis);
        QCOMPARE(wiface->includeFile(), includefile);
        QVERIFY(!wiface->icon().isNull());
        QCOMPARE(wiface->isContainer(), iscontainer);
        QCOMPARE(wiface->codeTemplate(), codetemplate);
        QCOMPARE(wiface->domXml(), domxml);

        QVERIFY(!wiface->isInitialized());
        wiface->initialize(0);
        QVERIFY(wiface->isInitialized());
        wiface->initialize(0);
        if (inittoggles) {
            QVERIFY(!wiface->isInitialized());
        } else {
            QVERIFY(wiface->isInitialized());
        }

        QWidget *widget = wiface->createWidget(0);
        QVERIFY(widget);
        QCOMPARE(widget->metaObject()->className(),
                 implclass.toLatin1().constData());
    }

    void testConstructorArgs() {
        // QGroupBox
        QVERIFY(m_widgets.count() > 1);
        QDesignerCustomWidgetInterface *wiface = m_widgets.at(1);
        QVERIFY(wiface);
        QGroupBox *box = qobject_cast<QGroupBox*>(wiface->createWidget(0));
        QVERIFY(box);
        QCOMPARE(box->title(), QString("the title"));
    }

    void testCreateWidget() {
        // QLabel
        QVERIFY(m_widgets.count() > 2);
        QDesignerCustomWidgetInterface *wiface = m_widgets.at(2);
        QVERIFY(wiface);
        QVERIFY(!wiface->createWidget(0));
        QWidget widget;
        QCOMPARE(wiface->createWidget(&widget), &widget);
    }

    void testIcon() {
        QVERIFY(m_widgets.count() > 3);

        QDesignerCustomWidgetInterface *checkboxiface = m_widgets.at(0);
        QVERIFY(checkboxiface);
        QIcon checkboxIcon(":/sth.png");
        QCOMPARE(checkboxiface->icon().pixmap(22,22),
                 checkboxIcon.pixmap(22,22));

        QDesignerCustomWidgetInterface *pushbuttoniface = m_widgets.at(3);
        QVERIFY(pushbuttoniface);
        QIcon pushbuttonIcon(":/pics/pushbuttonview.png");
        QCOMPARE(pushbuttoniface->icon().pixmap(22,22),
                 pushbuttonIcon.pixmap(22,22));
    }

private:
    QDesignerCustomWidgetCollectionInterface *m_collection;
    QObject *m_pluginInstance;
    QList<QDesignerCustomWidgetInterface*> m_widgets;
};

QTEST_MAIN(PluginTest)

#include <plugintest.moc>
