#ifndef QMLDATA_H
#define QMLDATA_H

#include <QObject>
#include <QDebug>

class QmlData : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QString message READ message NOTIFY messageChanged)
public:
    explicit QmlData(QObject *parent = nullptr);
public slots:
    void updateMessage(const QString &newMessage) {
        qDebug() << "from qml" << newMessage;
    }
signals:
};

#endif // QMLDATA_H
