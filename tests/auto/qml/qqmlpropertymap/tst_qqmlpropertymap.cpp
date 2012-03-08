/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <qtest.h>
#include <QtQml/qqmlengine.h>
#include <QtQml/qqmlcontext.h>
#include <QtQml/qqmlpropertymap.h>
#include <QtQml/qqmlcomponent.h>
#include <QtQuick/private/qquicktext_p.h>
#include <QSignalSpy>

class tst_QQmlPropertyMap : public QObject
{
    Q_OBJECT
public:
    tst_QQmlPropertyMap() {}

private slots:
    void insert();
    void operatorInsert();
    void operatorValue();
    void clear();
    void changed();
    void count();

    void crashBug();
    void QTBUG_17868();
};

void tst_QQmlPropertyMap::insert()
{
    QQmlPropertyMap map;
    map.insert(QLatin1String("key1"),100);
    map.insert(QLatin1String("key2"),200);
    QVERIFY(map.keys().count() == 2);
    QVERIFY(map.contains(QLatin1String("key1")));

    QCOMPARE(map.value(QLatin1String("key1")), QVariant(100));
    QCOMPARE(map.value(QLatin1String("key2")), QVariant(200));

    map.insert(QLatin1String("key1"),"Hello World");
    QCOMPARE(map.value(QLatin1String("key1")), QVariant("Hello World"));

    //inserting property names same with existing method(signal, slot, method) names is not allowed
    //QQmlPropertyMap has an invokable keys() method
    QTest::ignoreMessage(QtWarningMsg, "Creating property with name \"keys\" is not permitted, conflicts with internal symbols. ");
    map.insert(QLatin1String("keys"), 1);
    QVERIFY(map.keys().count() == 2);
    QVERIFY(!map.contains(QLatin1String("keys")));
    QVERIFY(map.value(QLatin1String("keys")).isNull());

    //QQmlPropertyMap has a deleteLater() slot
    QTest::ignoreMessage(QtWarningMsg, "Creating property with name \"deleteLater\" is not permitted, conflicts with internal symbols. ");
    map.insert(QLatin1String("deleteLater"), 1);
    QVERIFY(map.keys().count() == 2);
    QVERIFY(!map.contains(QLatin1String("deleteLater")));
    QVERIFY(map.value(QLatin1String("deleteLater")).isNull());

    //QQmlPropertyMap has an valueChanged() signal
    QTest::ignoreMessage(QtWarningMsg, "Creating property with name \"valueChanged\" is not permitted, conflicts with internal symbols. ");
    map.insert(QLatin1String("valueChanged"), 1);
    QVERIFY(map.keys().count() == 2);
    QVERIFY(!map.contains(QLatin1String("valueChanged")));
    QVERIFY(map.value(QLatin1String("valueChanged")).isNull());

    //but 'valueChange' should be ok
    map.insert(QLatin1String("valueChange"), 1);
    QVERIFY(map.keys().count() == 3);
    QVERIFY(map.contains(QLatin1String("valueChange")));
    QCOMPARE(map.value(QLatin1String("valueChange")), QVariant(1));

    //'valueCHANGED' should be ok, too
    map.insert(QLatin1String("valueCHANGED"), 1);
    QVERIFY(map.keys().count() == 4);
    QVERIFY(map.contains(QLatin1String("valueCHANGED")));
    QCOMPARE(map.value(QLatin1String("valueCHANGED")), QVariant(1));
}

void tst_QQmlPropertyMap::operatorInsert()
{
    QQmlPropertyMap map;
    map[QLatin1String("key1")] = 100;
    map[QLatin1String("key2")] = 200;
    QVERIFY(map.keys().count() == 2);

    QCOMPARE(map.value(QLatin1String("key1")), QVariant(100));
    QCOMPARE(map.value(QLatin1String("key2")), QVariant(200));

    map[QLatin1String("key1")] = "Hello World";
    QCOMPARE(map.value(QLatin1String("key1")), QVariant("Hello World"));
}

void tst_QQmlPropertyMap::operatorValue()
{
    QQmlPropertyMap map;
    map.insert(QLatin1String("key1"),100);
    map.insert(QLatin1String("key2"),200);
    QVERIFY(map.count() == 2);
    QVERIFY(map.contains(QLatin1String("key1")));

    const QQmlPropertyMap &constMap = map;

    QCOMPARE(constMap.value(QLatin1String("key1")), QVariant(100));
    QCOMPARE(constMap.value(QLatin1String("key2")), QVariant(200));
    QCOMPARE(constMap[QLatin1String("key1")], constMap.value(QLatin1String("key1")));
    QCOMPARE(constMap[QLatin1String("key2")], constMap.value(QLatin1String("key2")));
}

void tst_QQmlPropertyMap::clear()
{
    QQmlPropertyMap map;
    map.insert(QLatin1String("key1"),100);
    QVERIFY(map.keys().count() == 1);

    QCOMPARE(map.value(QLatin1String("key1")), QVariant(100));

    map.clear(QLatin1String("key1"));
    QVERIFY(map.keys().count() == 1);
    QVERIFY(map.contains(QLatin1String("key1")));
    QCOMPARE(map.value(QLatin1String("key1")), QVariant());
}

void tst_QQmlPropertyMap::changed()
{
    QQmlPropertyMap map;
    QSignalSpy spy(&map, SIGNAL(valueChanged(const QString&, const QVariant&)));
    map.insert(QLatin1String("key1"),100);
    map.insert(QLatin1String("key2"),200);
    QCOMPARE(spy.count(), 0);

    map.clear(QLatin1String("key1"));
    QCOMPARE(spy.count(), 0);

    //make changes in QML
    QQmlEngine engine;
    QQmlContext *ctxt = engine.rootContext();
    ctxt->setContextProperty(QLatin1String("testdata"), &map);
    QQmlComponent component(&engine);
    component.setData("import QtQuick 2.0\nText { text: { testdata.key1 = 'Hello World'; 'X' } }",
            QUrl::fromLocalFile(""));
    QVERIFY(component.isReady());
    QQuickText *txt = qobject_cast<QQuickText*>(component.create());
    QVERIFY(txt);
    QCOMPARE(txt->text(), QString('X'));
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.count(), 2);
    QCOMPARE(arguments.at(0).toString(),QLatin1String("key1"));
    QCOMPARE(arguments.at(1).value<QVariant>(),QVariant("Hello World"));
    QCOMPARE(map.value(QLatin1String("key1")), QVariant("Hello World"));
}

void tst_QQmlPropertyMap::count()
{
    QQmlPropertyMap map;
    QCOMPARE(map.isEmpty(), true);
    map.insert(QLatin1String("key1"),100);
    map.insert(QLatin1String("key2"),200);
    QCOMPARE(map.count(), 2);
    QCOMPARE(map.isEmpty(), false);

    map.insert(QLatin1String("key3"),"Hello World");
    QCOMPARE(map.count(), 3);

    //clearing doesn't remove the key
    map.clear(QLatin1String("key3"));
    QCOMPARE(map.count(), 3);
    QCOMPARE(map.size(), map.count());
}

void tst_QQmlPropertyMap::crashBug()
{
    QQmlPropertyMap map;

    QQmlEngine engine;
    QQmlContext context(&engine);
    context.setContextProperty("map", &map);

    QQmlComponent c(&engine);
    c.setData("import QtQuick 2.0\nBinding { target: map; property: \"myProp\"; value: 10 + 23 }",QUrl());
    QObject *obj = c.create(&context);
    delete obj;
}

void tst_QQmlPropertyMap::QTBUG_17868()
{
    QQmlPropertyMap map;

    QQmlEngine engine;
    QQmlContext context(&engine);
    context.setContextProperty("map", &map);
    map.insert("key", 1);
    QQmlComponent c(&engine);
    c.setData("import QtQuick 2.0\nItem {property bool error:false; Component.onCompleted: {try{ map.keys(); }catch(e) {error=true;}}}",QUrl());
    QObject *obj = c.create(&context);
    QVERIFY(obj);
    QVERIFY(!obj->property("error").toBool());
    delete obj;

}

QTEST_MAIN(tst_QQmlPropertyMap)

#include "tst_qqmlpropertymap.moc"