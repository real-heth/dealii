// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2022 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------

// Convert a deal.II cell to a cgal Surface_mesh.

#include <deal.II/base/config.h>

#include <deal.II/base/point.h>

#include <deal.II/cgal/surface_mesh.h>

#include <deal.II/fe/mapping_q.h>

#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/tria.h>

#include <CGAL/IO/io.h>

#include "../tests.h"


using namespace CGALWrappers;
using CGALPoint = CGAL::Point_3<CGAL::Simple_cartesian<double>>;

template <int dim, int spacedim>
void
test()
{
  deallog << "dim= " << dim << ",\t spacedim= " << spacedim << std::endl;
  using namespace ReferenceCells;
  std::vector<std::vector<ReferenceCell>> ref_cells = {
    {},
    {Line},
    {Triangle, Quadrilateral},
    {Tetrahedron, Pyramid, Wedge, Hexahedron}};
  for (const auto &r_cell : ref_cells[dim])
    {
      Triangulation<dim, spacedim>  tria;
      CGAL::Surface_mesh<CGALPoint> mesh;

      const auto mapping =
        r_cell.template get_default_mapping<dim, spacedim>(1);
      GridGenerator::reference_cell(tria, r_cell);

      const auto cell = tria.begin_active();
      dealii_cell_to_cgal_surface_mesh(cell, *mapping, mesh);

      Assert(mesh.is_valid(), dealii::ExcMessage("The CGAL mesh is not valid"));
      deallog << "deal vertices: " << cell->n_vertices() << ", cgal vertices "
              << mesh.num_vertices() << std::endl;
      deallog << "deal faces: " << cell->n_faces() << ", cgal faces "
              << mesh.num_faces() << std::endl;
      deallog << "Valid mesh: " << std::boolalpha << mesh.is_valid()
              << std::endl;
      deallog << mesh << std::endl;
    }
}

int
main()
{
  initlog();
  test<2, 2>();
  test<2, 3>();
  test<3, 3>();
}
