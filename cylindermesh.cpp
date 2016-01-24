#include "cylinderGeometry.h"
#include "cylinderMesh.h"


cylinderMesh::cylinderMesh( Qt3DCore::QNode *inParent ) :
   Qt3DRender::QGeometryRenderer( inParent )
{
   cylinderGeometry *geometry = new cylinderGeometry( this );

   QGeometryRenderer::setGeometry( geometry );
}

cylinderMesh::~cylinderMesh()
{
   QNode::cleanup();
}

void  cylinderMesh::setRings( int inRings )
{
   static_cast<cylinderGeometry *>(geometry())->setRings( inRings );
}

void  cylinderMesh::setSlices( int inSlices )
{
   static_cast<cylinderGeometry *>(geometry())->setSlices( inSlices );
}

void  cylinderMesh::setRadius( float inRadius )
{
   static_cast<cylinderGeometry *>(geometry())->setRadius( inRadius );
}

void  cylinderMesh::setLength( float inLength )
{
   static_cast<cylinderGeometry *>(geometry())->setLength( inLength );
}
