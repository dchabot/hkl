/* This file is part of the hkl library.
 *
 * The hkl library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The hkl library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the hkl library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2003-2013 Synchrotron SOLEIL
 *                         L'Orme des Merisiers Saint-Aubin
 *                         BP 48 91192 GIF-sur-YVETTE CEDEX
 *
 * Authors: Picca Frédéric-Emmanuel <picca@synchrotron-soleil.fr>
 */
#include "hkl.h"
#include <tap/basic.h>
#include <tap/hkl-tap.h>

static void solution(void)
{
	int res = HKL_TRUE;
	HklEngineList *engines;
	HklEngine *engine;
	const HklFactory *factory;
	HklGeometry *geometry;
	const HklGeometryList *geometries;
	HklDetector *detector;
	HklSample *sample;
	HklLattice *lattice;
	static double hkl[] = {1, 1, 0};
	HklParameterList *pseudo_axes;
	HklMatrix *U;

	/* get the geometry and set the source */
	factory = hkl_factory_get_by_name("ZAXIS");
	geometry = hkl_factory_create_new_geometry(factory);
	hkl_geometry_wavelength_set(geometry, 0.842);

	/* set up the sample */
	sample = hkl_sample_new("test");
	lattice = hkl_lattice_new(5.432, 5.432, 5.432,
				  90 * HKL_DEGTORAD,
				  90 * HKL_DEGTORAD,
				  90 * HKL_DEGTORAD);
	hkl_sample_lattice_set(sample, lattice);
	U = hkl_matrix_new_euler(-90*HKL_DEGTORAD, 0, 0);
	hkl_sample_U_set(sample, U);
	hkl_lattice_free(lattice);
	hkl_matrix_free(U);

	/* use a 0D detector */
	detector = hkl_detector_factory_new(HKL_DETECTOR_TYPE_0D);
	hkl_detector_idx_set(detector, 1);

	/* select the hkl pseudo axis */
	engines = hkl_factory_create_new_engine_list(factory);
	hkl_engine_list_init(engines, geometry, detector, sample);
	geometries = hkl_engine_list_geometries(engines);
	engine = hkl_engine_list_get_by_name(engines, "hkl");
	pseudo_axes = hkl_engine_pseudo_axes(engine);

	/* the init part must succed */
	hkl_geometry_set_values_unit_v(geometry, 1., 0., 0., 0.);

	/* compute the 1 1 0 */
	hkl_parameter_list_values_set(pseudo_axes, hkl, ARRAY_SIZE(hkl), NULL);
	if (hkl_engine_set(engine, NULL)){
		const darray_item *items = hkl_geometry_list_items_get(geometries);
		HklGeometryListItem **item;

		darray_foreach(item, *items){
			static double null[] = {0, 0, 0};

			hkl_parameter_list_values_set(pseudo_axes, null, ARRAY_SIZE(null), NULL);
			hkl_geometry_set(geometry,
					 hkl_geometry_list_item_geometry_get(*item));
			hkl_engine_get(engine, NULL);
			res &= check_pseudoaxes(engine, hkl, 3);
		}
	}else
		res = HKL_FALSE;
	if(!res)
		hkl_engine_fprintf(stdout, engine);

	hkl_engine_list_free(engines);
	hkl_detector_free(detector);
	hkl_sample_free(sample);
	hkl_geometry_free(geometry);

	ok(res == HKL_TRUE, "solution");
}

int main(int argc, char** argv)
{
	plan(1);

	solution();

	return 0;
}