#include "processing/imageprocessor.h"

#include <QFile>
#include <QImage>
#include <QTemporaryDir>
#include <QTest>

using papercutter::ImageJob;
using papercutter::ImageProcessor;
using papercutter::ProcessingRequest;

class ImageProcessorTest final : public QObject {
    Q_OBJECT

private slots:
    void packagedBackendOverridesHostDiscovery();
    void fillsTargetWithoutGaps();
    void savesACopyWithoutChangingTheOriginal();
    void refusesToReplaceTheOriginal();
};

void ImageProcessorTest::packagedBackendOverridesHostDiscovery()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString executablePath = directory.filePath(QStringLiteral("convert"));
    QFile executable(executablePath);
    QVERIFY(executable.open(QIODevice::WriteOnly));
    executable.write("#!/bin/sh\nexit 0\n");
    executable.close();
    QVERIFY(QFile::setPermissions(executablePath,
                                  QFileDevice::ReadOwner | QFileDevice::WriteOwner
                                      | QFileDevice::ExeOwner));

    const bool wasSet = qEnvironmentVariableIsSet("PAPERCUTTER_IMAGEMAGICK");
    const QByteArray previousValue = qgetenv("PAPERCUTTER_IMAGEMAGICK");
    qputenv("PAPERCUTTER_IMAGEMAGICK", executablePath.toUtf8());
    const QString selectedExecutable = ImageProcessor::backendExecutable();
    if (wasSet)
        qputenv("PAPERCUTTER_IMAGEMAGICK", previousValue);
    else
        qunsetenv("PAPERCUTTER_IMAGEMAGICK");

    QCOMPARE(selectedExecutable, executablePath);
}

void ImageProcessorTest::fillsTargetWithoutGaps()
{
    if (!ImageProcessor::isBackendAvailable())
        QSKIP("ImageMagick is not available");
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString sourcePath = directory.filePath(QStringLiteral("portrait.png"));
    const QString destinationPath = directory.filePath(QStringLiteral("output.png"));
    QImage source(60, 120, QImage::Format_ARGB32);
    source.fill(Qt::green);
    QVERIFY(source.save(sourcePath));

    ImageJob job;
    job.sourcePath = sourcePath;
    job.sourceSize = source.size();
    job.composition.targetSize = {160, 90};
    job.composition.normalizedOffset = {0.0, -1.0};
    const ProcessingRequest request{job, destinationPath, false};
    const auto result = ImageProcessor{}.process(request);
    QVERIFY2(result.succeeded, qPrintable(result.errorMessage));
    QCOMPARE(QImage(destinationPath).size(), QSize(160, 90));
    QCOMPARE(QImage(sourcePath).size(), QSize(60, 120));
}

void ImageProcessorTest::savesACopyWithoutChangingTheOriginal()
{
    if (!ImageProcessor::isBackendAvailable())
        QSKIP("ImageMagick is not available");
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString sourcePath = directory.filePath(QStringLiteral("wallpaper.png"));
    const QString destinationPath = directory.filePath(QStringLiteral("exports/wallpaper.png"));
    QImage source(120, 80, QImage::Format_ARGB32);
    source.fill(Qt::red);
    QVERIFY(source.save(sourcePath));

    ImageJob job;
    job.sourcePath = sourcePath;
    job.sourceSize = source.size();
    job.composition.targetSize = {60, 100};
    const ProcessingRequest request{job, destinationPath, false};
    const auto result = ImageProcessor{}.process(request);
    QVERIFY2(result.succeeded, qPrintable(result.errorMessage));
    QCOMPARE(QImage(sourcePath).size(), QSize(120, 80));
    QCOMPARE(QImage(destinationPath).size(), QSize(60, 100));
}

void ImageProcessorTest::refusesToReplaceTheOriginal()
{
    if (!ImageProcessor::isBackendAvailable())
        QSKIP("ImageMagick is not available");
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString sourcePath = directory.filePath(QStringLiteral("original.png"));
    QImage source(120, 80, QImage::Format_ARGB32);
    source.fill(Qt::blue);
    QVERIFY(source.save(sourcePath));

    ImageJob job;
    job.sourcePath = sourcePath;
    job.sourceSize = source.size();
    job.composition.targetSize = {60, 40};
    const auto result = ImageProcessor{}.process({job, sourcePath, true});
    QVERIFY(!result.succeeded);
    QCOMPARE(result.failureStage, QStringLiteral("validation"));
    QCOMPARE(QImage(sourcePath).size(), QSize(120, 80));
}

QTEST_MAIN(ImageProcessorTest)
#include "imageprocessor_test.moc"
