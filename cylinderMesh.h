#ifndef CYLINDERMESH_H
#define CYLINDERMESH_H

#include <Qt3DRender/QGeometryRenderer>


class cylinderMesh : public Qt3DRender::QGeometryRenderer
{
      Q_OBJECT

   public:
      explicit cylinderMesh( Qt3DCore::QNode *inParent = nullptr );
      ~cylinderMesh();

      void  setRings( int inRings );
      void  setSlices( int inSlices );

      void  setRadius( float inRadius );
      void  setLength( float inLength );

   private:
       // Exclude access to QGeometryRenderer's properties
       void setInstanceCount(int instanceCount);
       void setPrimitiveCount(int primitiveCount);
       void setBaseVertex(int baseVertex);
       void setBaseInstance(int baseInstance);
       void setRestartIndex(int index);
       void setPrimitiveRestart(bool enabled);
       void setGeometry(Qt3DRender::QGeometry *geometry);
       void setPrimitiveType(PrimitiveType primitiveType);
};

#endif // CYLINDERMESH_H
