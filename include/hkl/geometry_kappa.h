#ifndef _GEOMETRY_KAPPA_H_
#define _GEOMETRY_KAPPA_H_

#include "geometry.h"

namespace hkl {
    namespace geometry {

        /**
         * \brief An Geometry for a the kappa 6 circle soleil generic diffractometer.
         */
        class Kappa : public Geometry
        {
        public:

          /**
           * \brief The destructor
           */
          virtual ~Kappa(void);

          /** 
           * @brief Return the m_alpha parameter of the Kappa geometry.
           * 
           * @return The m_alpha parameter of the Kappa geometry.
           */
          double const & get_alpha(void) const {return m_alpha;}

          /*!
           * \brief put the angleConfiguration into a stream
           * \param flux
           */
          ostream & printToStream(ostream & flux) const;

          /*!
           * \brief Save the Geometry into a stream.
           * \param flux the stream to save the Geometry into.
           * \return The stream with the Geometry.
           */
          ostream & toStream(ostream & flux) const;

          /*!
           * \brief Restore an Geometry from a stream.
           * \param flux The stream containing the Geometry.
           */
          istream & fromStream(istream & flux);

        protected:
          double m_alpha; //!< The alpha angle of the Kappa geometry.

          /**
           * \brief The default constructor -- Private to be an abstract class.
           * @param alpha The alpha parameter of the Kappa geometry
           */
          Kappa(double alpha);
        };

    } // namespace geometry
} // namespace hkl

#endif // _GEOMETRY_KAPPA_H_