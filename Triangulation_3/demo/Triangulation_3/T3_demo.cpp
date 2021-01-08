// Copyright (c) 2010  INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL$
// $Id$
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
// Author(s)     : Sophie Fei Che <fei@cis.udel.edu>
//
// File Description : Demo of CGAL 3D Triangulation package

#include "MainWindow.h"
#include <QApplication>

int main(int argc, char** argv)
{
  QSurfaceFormat fmt;
  fmt.setVersion(2, 1);
  fmt.setRenderableType(QSurfaceFormat::OpenGL);
  fmt.setProfile(QSurfaceFormat::CoreProfile);
  fmt.setOption(QSurfaceFormat::DebugContext);
  QSurfaceFormat::setDefaultFormat(fmt);
  //for windows
  #if (QT_VERSION >= QT_VERSION_CHECK(5, 3, 0))
  QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
  #endif
  QApplication app(argc, argv);

  app.setOrganizationDomain("inria.fr");
  app.setOrganizationName("INRIA");
  app.setApplicationName("3D Triangulation Demo");

  MainWindow mw;
  mw.show();

  return app.exec();
}
