#include <sstream>
#include "constants.h"
#include "geometry_kappa6C_test.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GeometryKappa6CTest );

void
GeometryKappa6CTest::setUp(void)
{
    m_alpha = 50 * constant::math::degToRad;
    m_geometry = new geometry::Kappa6C(m_alpha);
}

void 
GeometryKappa6CTest::tearDown(void)
{
    delete m_geometry;
}

void
GeometryKappa6CTest::equal(void)
{
    geometry::Kappa6C geometry(m_alpha);
    CPPUNIT_ASSERT_EQUAL(*m_geometry, geometry);
}

void 
GeometryKappa6CTest::copyConstructor(void)
{
    geometry::Kappa6C geometry(*m_geometry);

    CPPUNIT_ASSERT_EQUAL(*m_geometry, geometry);
}

void 
GeometryKappa6CTest::otherConstructors(void)
{
    double mu = 9 * constant::math::degToRad;
    double komega = 10 * constant::math::degToRad;
    double kappa = 11 * constant::math::degToRad;
    double kphi = 12 * constant::math::degToRad;
    
    double gamma =13 * constant::math::degToRad;
    double delta =14 * constant::math::degToRad;

    geometry::Kappa6C geometry_ref(m_alpha);
    geometry::Kappa6C geometry(m_alpha, mu, komega, kappa, kphi, gamma, delta);

    geometry_ref.get_axe("mu").set_value(mu);
    geometry_ref.get_axe("komega").set_value(komega);
    geometry_ref.get_axe("kappa").set_value(kappa);
    geometry_ref.get_axe("kphi").set_value(kphi);
    geometry_ref.get_axe("gamma").set_value(gamma);
    geometry_ref.get_axe("delta").set_value(delta);

    CPPUNIT_ASSERT_EQUAL(geometry_ref, geometry);
}

void
GeometryKappa6CTest::getAxesNames(void)
{
    vector<MyString> v = m_geometry->getAxesNames();
    CPPUNIT_ASSERT_EQUAL(MyString("mu"), v[0]);
    CPPUNIT_ASSERT_EQUAL(MyString("komega"), v[1]);
    CPPUNIT_ASSERT_EQUAL(MyString("kappa"), v[2]);
    CPPUNIT_ASSERT_EQUAL(MyString("kphi"), v[3]);
    CPPUNIT_ASSERT_EQUAL(MyString("gamma"), v[4]);
    CPPUNIT_ASSERT_EQUAL(MyString("delta"), v[5]);
}

void
GeometryKappa6CTest::getSampleQuaternion(void)
{
    m_geometry->get_axe("mu").set_value(90 * constant::math::degToRad);

    CPPUNIT_ASSERT_EQUAL(Quaternion(1./sqrt(2), 0, 0, 1./sqrt(2)), m_geometry->getSampleQuaternion());
}

void
GeometryKappa6CTest::getSampleRotationMatrix(void)
{
    m_geometry->get_axe("mu").set_value(90. * constant::math::degToRad);

    smatrix M( 0.,-1., 0.,
               1., 0., 0.,
               0., 0., 1.);

    CPPUNIT_ASSERT_EQUAL(M, m_geometry->getSampleRotationMatrix());
}

void
GeometryKappa6CTest::getQ(void)
{
    m_geometry->get_axe("gamma").set_value(0. * constant::math::degToRad);
    CPPUNIT_ASSERT_EQUAL(svector(0., 0., 0.), m_geometry->getQ());

    m_geometry->get_axe("gamma").set_value(45. * constant::math::degToRad);
    m_geometry->get_axe("delta").set_value(45. * constant::math::degToRad);
    CPPUNIT_ASSERT_EQUAL(svector(-.5, .5, sqrt(2.)/2.), m_geometry->getQ());
}

void
GeometryKappa6CTest::getDistance(void)
{
    geometry::Kappa6C g1(m_alpha,
                         10 * constant::math::degToRad,
                         20 * constant::math::degToRad,
                         30 * constant::math::degToRad,
                         40 * constant::math::degToRad,
                         50 * constant::math::degToRad,
                         60 * constant::math::degToRad);

    geometry::Kappa6C g2(m_alpha,
                         11 * constant::math::degToRad,
                         21 * constant::math::degToRad,
                         31 * constant::math::degToRad,
                         41 * constant::math::degToRad,
                         51 * constant::math::degToRad,
                         61 * constant::math::degToRad);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(6. * constant::math::degToRad, g1.getDistance(g2), constant::math::epsilon_0);

    g2.get_axe("mu").set_value(10 * constant::math::degToRad);
    g2.get_axe("komega").set_value(20 * constant::math::degToRad);
    g2.get_axe("kappa").set_value(30 * constant::math::degToRad);
    g2.get_axe("kphi").set_value(40 * constant::math::degToRad);
    g2.get_axe("gamma").set_value(50 * constant::math::degToRad);
    g2.get_axe("delta").set_value(60 * constant::math::degToRad);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0. * constant::math::degToRad, g1.getDistance(g2), constant::math::epsilon_0);
}

void
GeometryKappa6CTest::persistanceIO(void)
{
    geometry::Kappa6C geometry1(m_alpha);
    geometry::Kappa6C geometry2(m_alpha);
    stringstream flux;

    m_geometry->get_axe("komega").set_value(2.);
    m_geometry->toStream(flux);
    m_geometry->get_axe("komega").set_value(3.);
    m_geometry->toStream(flux);  
    geometry1.fromStream(flux);
    geometry2.fromStream(flux);

    m_geometry->get_axe("komega").set_value(2.);
    CPPUNIT_ASSERT_EQUAL(*m_geometry, geometry1);
    m_geometry->get_axe("komega").set_value(3.);
    CPPUNIT_ASSERT_EQUAL(*m_geometry, geometry2);
}