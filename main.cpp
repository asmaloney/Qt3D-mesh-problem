#include <QGuiApplication>

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QCamera>
#include <Qt3DCore/QCameraLens>
#include <Qt3DCore/QTransform>
#include <Qt3DCore/QAspectEngine>

#include <Qt3DInput/QInputAspect>

#include <Qt3DRender/QRenderAspect>
#include <Qt3DRender/QFrameGraph>
#include <Qt3DRender/QForwardRenderer>
#include <Qt3DRender/QPhongMaterial>

#include "cylinderMesh.h"
#include "window.h"


int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    Window view;

    Qt3DCore::QAspectEngine engine;
    engine.registerAspect(new Qt3DRender::QRenderAspect());
    Qt3DInput::QInputAspect *input = new Qt3DInput::QInputAspect;
    engine.registerAspect(input);
    QVariantMap data;
    data.insert(QStringLiteral("surface"), QVariant::fromValue(static_cast<QSurface *>(&view)));
    data.insert(QStringLiteral("eventSource"), QVariant::fromValue(&view));
    engine.setData(data);

    // Root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

    // Camera
    Qt3DCore::QCamera *cameraEntity = new Qt3DCore::QCamera(rootEntity);
    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D( 20.0f, 21.0f, 20.0f ));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));
    input->setCamera(cameraEntity);

    // FrameGraph
    Qt3DRender::QFrameGraph *frameGraph = new Qt3DRender::QFrameGraph();
    Qt3DRender::QForwardRenderer *forwardRenderer = new Qt3DRender::QForwardRenderer();
    forwardRenderer->setCamera(cameraEntity);
    frameGraph->setActiveFrameGraph(forwardRenderer);

    // Material
    Qt3DRender::QPhongMaterial *material = new Qt3DRender::QPhongMaterial;
    material->setDiffuse( Qt::red );

    // Cylinder
    Qt3DCore::QEntity *cylinderEntity = new Qt3DCore::QEntity;

    cylinderMesh  *cylinder = new cylinderMesh;
    cylinder->setRadius( 5.0 );
    cylinder->setLength( 16.0 );
    cylinder->setRings( 3 );

    cylinderEntity->addComponent( cylinder );
    cylinderEntity->addComponent( material );
    cylinderEntity->setParent( rootEntity );

    rootEntity->addComponent(frameGraph);

    engine.setRootEntity(rootEntity);
    view.show();

    return app.exec();
}
