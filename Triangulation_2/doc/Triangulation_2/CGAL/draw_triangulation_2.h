namespace CGAL {

/*!
\ingroup PkgDrawTriangulation2

opens a new window and draws a triangulation.
A call to this function is blocking, that is the program continues as soon as the user closes the window. This function requires `CGAL_Qt5`, and is only available if the macro `CGAL_USE_BASIC_VIEWER` is defined.
Linking with the cmake target `CGAL::CGAL_Basic_viewer` will link with
`CGAL_Qt5` and add the definition `CGAL_USE_BASIC_VIEWER`.

\tparam T2 a triangulation class derived from `Triangulation_2`  or `Constrained_triangulation_2`
\param at2 the triangulation to draw.

*/
template<class T2>
void draw(const T2& at2);

  /*!
\ingroup PkgDrawTriangulation2

opens a new window and draws a triangulation.
A call to this function is blocking, that is the program continues as soon as the user closes the window. This function requires `CGAL_Qt5`, and is only available if the macro `CGAL_USE_BASIC_VIEWER` is defined.
Linking with the cmake target `CGAL::CGAL_Basic_viewer` will link with `CGAL_Qt5` and add the definition `CGAL_USE_BASIC_VIEWER`.

\tparam CT a triangulation class derived from `Triangulation_2`  or `Constrained_triangulation_2`
\tparam InDomainPMap a class model of `ReadWritePropertyMap`  with
`CT::Face_handle`  as key type and  `bool`  as value type.

\param act the constrained triangulation to draw.
\param ipm the property map defining the faces which are in the domain.

*/
  template<class CT, class InDomainPMap>
  void draw(const CT2& act, InDomainPMap ipm);

} /* namespace CGAL */
