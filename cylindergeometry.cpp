#include <Qt3DRender/QBufferFunctor>

#include <QtMath>
#include <QVector3D>

#include "cylinderGeometry.h"


void createSidesVertices(float *&verticesPtr,
                         int rings,
                         int slices,
                         double radius,
                         double length)
{
   const float dY = length / static_cast<float>(rings - 1);
   const float dTheta = (M_PI * 2) / static_cast<float>(slices);

   for (int ring = 0; ring < rings; ++ring) {
      const float y = -length / 2.0f + static_cast<float>(ring) * dY;

      for (int slice = 0; slice <= slices; ++slice) {
         const float theta = static_cast<float>(slice) * dTheta;
         const float ct = qCos(theta);
         const float st = qSin(theta);

         *verticesPtr++ = radius * ct;
         *verticesPtr++ = y;
         *verticesPtr++ = radius * st;

         *verticesPtr++ = (y + length / 2.0) / length;
         *verticesPtr++ = theta / (M_PI * 2);

         QVector3D n(ct, 0.0f, st);
         n.normalize();
         *verticesPtr++ = n.x();
         *verticesPtr++ = n.y();
         *verticesPtr++ = n.z();
      }
   }
}

void createSidesIndices(quint16 *&indicesPtr, int rings, int slices)
{
   for (int ring = 0; ring < rings - 1; ++ring) {
      const int ringIndexStart = ring * (slices + 1);
      const int nextRingIndexStart = (ring + 1) * (slices + 1);

      for (int slice = 0; slice < slices; ++slice) {
         const int nextSlice = slice + 1;

         *indicesPtr++ = (ringIndexStart + slice);
         *indicesPtr++ = (nextRingIndexStart + slice);
         *indicesPtr++ = (ringIndexStart + nextSlice);
         *indicesPtr++ = (ringIndexStart + nextSlice);
         *indicesPtr++ = (nextRingIndexStart + slice);
         *indicesPtr++ = (nextRingIndexStart + nextSlice);
      }
   }
}

class CylinderVertexDataFunctor : public Qt3DRender::QBufferFunctor
{
   public:
      CylinderVertexDataFunctor(int rings, int slices, float radius, float length)
         : m_rings(rings)
         , m_slices(slices)
         , m_radius(radius)
         , m_length(length)
      {}

      QByteArray operator ()() Q_DECL_OVERRIDE
      {
//         const int verticesCount  = (m_slices + 1) * m_rings + 2 * (m_slices + 1) + 2;
         const int verticesCount  = (m_slices + 1) * m_rings;
         // vec3 pos, vec2 texCoord, vec3 normal
         const quint32 vertexSize = (3 + 2 + 3) * sizeof(float);

         QByteArray verticesData;
         verticesData.resize(vertexSize * verticesCount);
         float *verticesPtr = reinterpret_cast<float*>(verticesData.data());

         createSidesVertices(verticesPtr, m_rings, m_slices, m_radius, m_length);
//         createDiscVertices(verticesPtr, m_slices, m_radius, -m_length * 0.5f);
//         createDiscVertices(verticesPtr, m_slices, m_radius, m_length * 0.5f);

         return verticesData;
      }

      bool operator ==(const QBufferFunctor &other) const Q_DECL_OVERRIDE
      {
         const CylinderVertexDataFunctor *otherFunctor = functor_cast<CylinderVertexDataFunctor>(&other);
         if (otherFunctor != Q_NULLPTR)
            return (otherFunctor->m_rings == m_rings &&
                    otherFunctor->m_slices == m_slices &&
                    otherFunctor->m_radius == m_radius &&
                    otherFunctor->m_length == m_length);
         return false;
      }

      QT3D_FUNCTOR(CylinderVertexDataFunctor)

   private:
      int m_rings;
      int m_slices;
      float m_radius;
      float m_length;
};

class CylinderIndexDataFunctor : public Qt3DRender::QBufferFunctor
{
   public:
      CylinderIndexDataFunctor(int rings, int slices, float length)
         : m_rings(rings)
         , m_slices(slices)
         , m_length(length)
      {
      }

      QByteArray operator ()() Q_DECL_OVERRIDE
      {
//         const int facesCount = (m_slices * 2) * m_rings // two tris per side, for all rings
//                 + m_slices * 2; // two caps
         const int facesCount = (m_slices * 2) * (m_rings - 1); // two tris per side, for each pair of adjacent rings
         const int indicesCount = facesCount * 3;
         const int indexSize = sizeof(quint16);
         Q_ASSERT(indicesCount < 65536);

         QByteArray indicesBytes;
         indicesBytes.resize(indicesCount * indexSize);
         quint16 *indicesPtr = reinterpret_cast<quint16*>(indicesBytes.data());

         createSidesIndices(indicesPtr, m_rings, m_slices);
//         createDiscIndices(indicesPtr, m_rings * (m_slices + 1), m_slices, -m_length * 0.5);
//         createDiscIndices(indicesPtr, m_rings * (m_slices + 1) + m_slices + 2, m_slices, m_length * 0.5);

         return indicesBytes;
      }

      bool operator ==(const QBufferFunctor &other) const Q_DECL_OVERRIDE
      {
         const CylinderIndexDataFunctor *otherFunctor = functor_cast<CylinderIndexDataFunctor>(&other);
         if (otherFunctor != Q_NULLPTR)
            return (otherFunctor->m_rings == m_rings &&
                    otherFunctor->m_slices == m_slices &&
                    otherFunctor->m_length == m_length);
         return false;
      }

      QT3D_FUNCTOR(CylinderIndexDataFunctor)

   private:
      int m_rings;
      int m_slices;
      float m_length;
};

cylinderGeometry::cylinderGeometry( Qt3DCore::QNode *inParent ) :
   Qt3DRender::QGeometry( inParent ),
   mRings( 16 ),
   mSlices( 16 ),
   mRadius( 0.0 ),
   mLength( 1.0f ),
   mPositionAttribute( nullptr ),
   mNormalAttribute( nullptr ),
   mTexCoordAttribute( nullptr ),
   mIndexAttribute( nullptr ),
   mVertexBuffer( nullptr ),
   mIndexBuffer( nullptr )
{
   mPositionAttribute = new Qt3DRender::QAttribute( this );
   mNormalAttribute = new Qt3DRender::QAttribute( this );
   mTexCoordAttribute = new Qt3DRender::QAttribute( this );
   mIndexAttribute = new Qt3DRender::QAttribute( this );
   mVertexBuffer = new Qt3DRender::QBuffer( Qt3DRender::QBuffer::VertexBuffer, this );
   mIndexBuffer = new Qt3DRender::QBuffer( Qt3DRender::QBuffer::IndexBuffer, this );

   // vec3 pos, vec2 tex, vec3 normal
   const quint32 cElementSize = 3 + 2 + 3;
   const quint32 cStride = cElementSize * sizeof( float );

//   const int nVerts = (m_slices + 1) * (m_rings + 1);
   const int cNumVerts = (mSlices + 1) * mRings;
   //   const int faces = (m_slices * 2) * m_rings + (2 * m_slices);
   const int cNumFaces = (mSlices * 2) * mRings;

   mPositionAttribute->setName( Qt3DRender::QAttribute::defaultPositionAttributeName() );
   mPositionAttribute->setDataType( Qt3DRender::QAttribute::Float );
   mPositionAttribute->setDataSize( 3 );
   mPositionAttribute->setAttributeType( Qt3DRender::QAttribute::VertexAttribute );
   mPositionAttribute->setBuffer( mVertexBuffer );
   mPositionAttribute->setByteStride( cStride );
   mPositionAttribute->setCount( cNumVerts );

   mTexCoordAttribute->setName( Qt3DRender::QAttribute::defaultTextureCoordinateAttributeName() );
   mTexCoordAttribute->setDataType( Qt3DRender::QAttribute::Float );
   mTexCoordAttribute->setDataSize( 2 );
   mTexCoordAttribute->setAttributeType( Qt3DRender::QAttribute::VertexAttribute );
   mTexCoordAttribute->setBuffer( mVertexBuffer );
   mTexCoordAttribute->setByteStride( cStride );
   mTexCoordAttribute->setByteOffset( 3 * sizeof( float ) );
   mTexCoordAttribute->setCount( cNumVerts );

   mNormalAttribute->setName( Qt3DRender::QAttribute::defaultNormalAttributeName() );
   mNormalAttribute->setDataType( Qt3DRender::QAttribute::Float );
   mNormalAttribute->setDataSize( 3 );
   mNormalAttribute->setAttributeType( Qt3DRender::QAttribute::VertexAttribute );
   mNormalAttribute->setBuffer( mVertexBuffer );
   mNormalAttribute->setByteStride( cStride );
   mNormalAttribute->setByteOffset( 5 * sizeof( float ) );
   mNormalAttribute->setCount( cNumVerts );

   mIndexAttribute->setAttributeType( Qt3DRender::QAttribute::IndexAttribute );
   mIndexAttribute->setDataType( Qt3DRender::QAttribute::UnsignedShort );
   mIndexAttribute->setBuffer( mIndexBuffer );

   mIndexAttribute->setCount( cNumFaces * 3 );

   mVertexBuffer->setBufferFunctor( Qt3DRender::QBufferFunctorPtr( new CylinderVertexDataFunctor( mRings, mSlices, mRadius, mLength ) ) );
   mIndexBuffer->setBufferFunctor( Qt3DRender::QBufferFunctorPtr( new CylinderIndexDataFunctor( mRings, mSlices, mLength ) ) );

   addAttribute( mPositionAttribute );
   addAttribute( mTexCoordAttribute );
   addAttribute( mNormalAttribute );
   addAttribute( mIndexAttribute );
}

void  cylinderGeometry::setRings( int inRings )
{
   if ( inRings != mRings )
   {
      mRings = inRings;
      _updateVertices();
      _updateIndices();
   }
}

void  cylinderGeometry::setSlices( int inSlices )
{
   if ( inSlices != mSlices )
   {
      mSlices = inSlices;
      _updateVertices();
      _updateIndices();
   }
}

void  cylinderGeometry::setRadius( float inRadius )
{
   if ( inRadius != mRadius )
   {
      mRadius = inRadius;
      _updateVertices();
      _updateIndices();
   }
}

void  cylinderGeometry::setLength( float inLength )
{
   if ( inLength != mLength )
   {
      mLength = inLength;
      _updateVertices();
      _updateIndices();
   }
}

void  cylinderGeometry::_updateVertices()
{
//   const int nVerts = (d->m_slices + 1) * (d->m_rings + 1);
   const int nVerts = (mSlices + 1) * mRings;
   mPositionAttribute->setCount(nVerts);
   mTexCoordAttribute->setCount(nVerts);
   mNormalAttribute->setCount(nVerts);

   mVertexBuffer->setBufferFunctor( Qt3DRender::QBufferFunctorPtr( new CylinderVertexDataFunctor( mRings, mSlices, mRadius, mLength ) ) );
}

void  cylinderGeometry::_updateIndices()
{
//   const int faces = (d->m_slices * 2) * d->m_rings + (2 * d->m_slices);
   const int faces = (mSlices * 2) * mRings;
   mIndexAttribute->setCount(faces * 3);

   mIndexBuffer->setBufferFunctor( Qt3DRender::QBufferFunctorPtr( new CylinderIndexDataFunctor( mRings, mSlices, mLength ) ) );
}
