#ifndef CYLINDERGEOMETRY_H
#define CYLINDERGEOMETRY_H

#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QGeometry>


class cylinderGeometry : public Qt3DRender::QGeometry
{
      Q_OBJECT

   public:
      explicit cylinderGeometry( Qt3DCore::QNode *inParent = nullptr );

      void  setRings( int inRings );
      void  setSlices( int inSlices );

      void  setRadius( float inRadius );
      void  setLength( float inLength );

   private:
      void _updateVertices();
      void _updateIndices();

      int mRings;
      int mSlices;

      float mRadius;
      float mLength;

      Qt3DRender::QAttribute  *mPositionAttribute;
      Qt3DRender::QAttribute  *mNormalAttribute;
      Qt3DRender::QAttribute  *mTexCoordAttribute;
      Qt3DRender::QAttribute  *mIndexAttribute;
      Qt3DRender::QBuffer     *mVertexBuffer;
      Qt3DRender::QBuffer     *mIndexBuffer;
};

#endif // CYLINDERGEOMETRY_H
