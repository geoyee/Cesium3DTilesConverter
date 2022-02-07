#pragma once

#include <OSGBLevel.h>
#include <QObject>
#include <QRunnable>
#include <QString>
#include <QDebug>
#include <QThreadPool>
#include <QVector>
#include <QPair>
#include <QSharedPointer>
#include <QDomDocument>

namespace gzpi {
    class OSGBConvertTask;

    class OSGBConvertJob: public QObject{
        Q_OBJECT
    public:
        explicit OSGBConvertJob(const QString& input, const QString &output)
            :input(input), output(output), threadPool(new QThreadPool) {}

        void setMaxThread(int thread){
            maxThread = thread;
            threadPool->setMaxThreadCount(maxThread);
        }

        void run();

        virtual ~OSGBConvertJob() {
            if(threadPool != nullptr)
                delete threadPool;
        }

        void setHeight(double h) {height = h;}
        void setMaxLevel(int level){ maxLevel = level; }
    private:
        QString input;
        QString output;
        double height = 0;
        QThreadPool *threadPool;
        int maxThread = 4;
        int maxLevel = std::numeric_limits<int>::max();
        QVector<QSharedPointer<OSGBConvertTask>> tasks;
    };

    class OSGBConvertTask: public QObject, public QRunnable{
        Q_OBJECT
    public:
        OSGBConvertTask(const QString& input, const QString& output, int maxLevel = std::numeric_limits<int>::max())
            :osgbLevel(input), output(output), maxLevel(maxLevel){
            setAutoDelete(false);
        };

        virtual void run() override {
            qInfo() << "Strart process tile: " << osgbLevel.nodeName;
            isSucceed = osgbLevel.convertTiles(tile, output, maxLevel);
            if(isSucceed){
                qInfo() << "Finish process tile: " << osgbLevel.nodeName;
            }else{
                qInfo() << "Not finish process tile: " << osgbLevel.nodeName;
            }

        }

        OSGBLevel osgbLevel;
        bool isSucceed;
        BaseTile tile;
    private:
        QString output;
        int maxLevel;
    };
}