Qt3D Mesh Problem
--
Trying to solve a Qt3D mesh issue...

![Cylinder with wireframe 1](images/cylinder1.png?raw=true "Cylinder 1")
![Cylinder with wireframe 2](images/cylinder2.png?raw=true "Cylinder 2")

Where do the extra vert at (0.0, 0.0, 0.0) and faces using it come from?

**Update [25 Jan 2016]**

Found it.  This is a bug in [QCylinderGeometry](https://doc-snapshots.qt.io/qt5-5.6/qt3drender-qcylindergeometry.html).  It is calculating the number of indices incorrectly in *CylinderIndexDataFunctor::operator ()* and in *createSidesIndices()*.

You can also see the problem if you move the camera "inside" a QCylinderMesh:

![Cylinder with wireframe 3](images/cylinder3.png?raw=true "Cylinder 3")
