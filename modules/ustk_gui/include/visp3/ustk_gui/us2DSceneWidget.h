/****************************************************************************
 *
 * This file is part of the ustk software.
 * Copyright (C) 2016 - 2017 by Inria. All rights reserved.
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ustk with software that can not be combined with the GNU
 * GPL, please contact Inria about acquiring a ViSP Professional
 * Edition License.
 *
 * This software was developed at:
 * Inria Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 *
 * If you have questions regarding the use of this file, please contact
 * Inria at ustk@inria.fr
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Authors:
 * Marc Pouliquen
 *
 *****************************************************************************/

/**
 * @file us2DSceneWidget.h
 * @brief Class used to render a 2D slice of a vtkImageData in a vtk scene in a QWidget (based on QVTKWidget)
 */


#ifndef __us2DSceneWidget_h_
#define __us2DSceneWidget_h_

// VISP includes
#include <visp3/ustk_core/usConfig.h>

#ifdef USTK_HAVE_VTK_QT

#include <visp3/core/vpException.h>
#include <visp3/core/vpHomogeneousMatrix.h>

#include <visp3/ustk_gui/usVTKConverter.h>
#include <visp3/ustk_gui/usViewerWidget.h>
#include <visp3/ustk_core/us.h>

//VTK includes
#include <vtkPlane.h>
#include <vtkImageSlice.h>
#include <vtkImageResliceMapper.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkImageReslice.h>
#include <vtkImageMapToColors.h>
#include <vtkImageActor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkLookupTable.h>

#include <vtkSmartPointer.h>
#include <vtkImageReader2.h>
#include <vtkMatrix4x4.h>
#include <vtkImageReslice.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkImageActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkCommand.h>
#include <vtkImageData.h>
#include <vtkImageMapper3D.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkInformation.h>
#include <vtkMetaImageReader.h>
#include <vtkResliceCursor.h>
#include <vtkResliceCursorWidget.h>
#include <vtkAnnotatedCubeActor.h>
#include <vtkPNGWriter.h>
#include <vtkCoordinate.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkPointPicker.h>
#include <vtkPoints.h>
#include <vtkPropPicker.h>
#include <vtkPolygon.h>
#include <vtkCellArray.h>
#include <vtkLine.h>
#include <vtkSTLReader.h>

// Qt includes
#if defined(USTK_HAVE_VTK_QT4)
#  include <QApplication>
#  include <QtGui/QMainWindow>
#  include <QtGui/QGridLayout>
#  include <QtGui/QPushButton>
#  include <QtGui/QWheelEvent>
# include <QtCore/QPoint>
# include <QtGui/QPainter>
#elif defined USTK_HAVE_VTK_QT5
#  include <QApplication>
#  include <QtWidgets/QMainWindow>
#  include <QtWidgets/QGridLayout>
#  include <QtWidgets/QPushButton>
#  include <QWheelEvent>
#endif

#include <QVTKWidget.h>

/**
 * @class us2DSceneWidget
 * @brief Class used to render a 2D slice of a vtkImageData in a vtk scene in a QWidget (based on QVTKWidget)
 * @ingroup module_ustk_gui
 */

class VISP_EXPORT us2DSceneWidget : public usViewerWidget
{
  Q_OBJECT
public:

  // Constructor/Destructor
  us2DSceneWidget(QWidget* parent = NULL, Qt::WindowFlags f = 0);
  ~us2DSceneWidget() {}

  void drawLine(double u1,double v1,double w1,double u2,double v2,double w2);

  void getClick(vpColVector & vec);

  void getCurrentSlice(usImagePostScan2D<unsigned char> &image2D);

  vtkImageData * getImageData();

  vtkMatrix4x4 * getResliceMatrix();

  vtkPlane* getReslicePlane();

  void init();

  //get key pressed/released (to make rotations of the plane)
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);

  void 	mouseMoveEvent(QMouseEvent * event);
  void 	mousePressEvent(QMouseEvent * event);

  //Catch paint events, in case we want to display some informations (writing in this widget) over the vtk scene
  void paintEvent( QPaintEvent* event );

  //Set view color
  void setColor(double r,double g,double b);

  //Set image to display
  void setImageData(vtkImageData* imageData);

  //Set planes
  void setResliceMatrix(vtkMatrix4x4* matrix);

  void setPolyDataPlaneContour(vtkPolyData *polyData);
  void setPolyDataMeshContour(vtkPolyData *polyData);

  //catch scroll events to slice in image
  void wheelEvent(QWheelEvent *event);

public slots:
  void updateImageData(vtkImageData* imageData);

  void changeMatrix(vpHomogeneousMatrix matrix);

  void saveViewSlot();
  void updateView();

signals:
  void matrixChanged(vtkMatrix4x4* matrix);
  void voxelPicked(vpColVector vector);

private:
  //image
  vtkImageData * m_imageData;

  // R T matrix
  vtkMatrix4x4 * m_resliceMatrix;

  //image reslice
  vtkImageReslice * m_reslice;

  //LUT
  vtkLookupTable* m_table;

  //color mapper
  vtkImageMapToColors* m_color;

  //actor
  vtkImageActor* m_actor;

  //vtk renderer
  vtkRenderer* m_renderer;
  vtkRenderer* m_polyDataRenderer;

  //polydata of plane contour
  vtkPolyData* m_polydataPlaneContour;
  vtkPolyDataMapper * m_polyDataPlaneContourMapper;
  vtkActor * m_polydataPlaneContourActor;

  //polydata of mesh contour in the plane
  vtkPolyData* m_polydataMeshContour;
  vtkPolyDataMapper * m_polyDataMeshContourMapper;
  vtkActor * m_polydataMeshContourActor;

  //vtk picker
  vtkPropPicker* m_propPicker;

  //to know if r key is currently pressed
  bool m_rPressed;
  bool m_pPressed;

  //mouse button pressed
  bool m_mousePressed;

  //for rotations
  int m_lastmouserPosX;
  int m_lastmouserPosY;

  //for getClick blocking
  bool m_pickingState;
  vpColVector m_pickedVoxel;
};
#endif
#endif // __us2DSceneWidget_h_
