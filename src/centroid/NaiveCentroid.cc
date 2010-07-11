// -*- LSST-C++ -*-

/* 
 * LSST Data Management System
 * Copyright 2008, 2009, 2010 LSST Corporation.
 * 
 * This product includes software developed by the
 * LSST Project (http://www.lsst.org/).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the LSST License Statement and 
 * the GNU General Public License along with this program.  If not, 
 * see <http://www.lsstcorp.org/LegalNotices/>.
 */
 
#include "lsst/pex/exceptions.h"
#include "lsst/pex/logging/Trace.h"
#include "lsst/meas/algorithms/Centroid.h"

namespace pexExceptions = lsst::pex::exceptions;
namespace pexLogging = lsst::pex::logging;

namespace lsst {
namespace meas {
namespace algorithms {

namespace {
/**
 * @brief A class that knows how to calculate centroids as a simple unweighted first moment
 * of the 3x3 region around a pixel
 */
template<typename ImageT>
class NaiveMeasureCentroid : public MeasureCentroid<ImageT> {
public:
    typedef MeasureCentroid<ImageT> MeasurePropertyBase;

    NaiveMeasureCentroid(typename ImageT::ConstPtr image) : MeasureCentroid<ImageT>(image) {}
private:
    Centroid doApply(ImageT const& image, int x, int y, PSF const* psf, double background) const;
};

/**
 * @brief Given an image and a pixel position, return a Centroid using a naive 3x3 weighted moment
 */
template<typename ImageT>
Centroid NaiveMeasureCentroid<ImageT>::doApply(ImageT const& image, ///< The Image wherein dwells the object
                                          int x,               ///< object's column position
                                          int y,               ///< object's row position
                                          PSF const*,          ///< image's PSF
                                          double background    ///< image's background level
                                         ) const {
    x -= image.getX0();                 // work in image Pixel coordinates
    y -= image.getY0();

    typename ImageT::xy_locator im = image.xy_at(x, y);

    double const sum =
        (im(-1,  1) + im( 0,  1) + im( 1,  1) +
         im(-1,  0) + im( 0,  0) + im( 1,  0) +
         im(-1, -1) + im( 0, -1) + im( 1, -1)) - 9*background;

    if (sum == 0.0) {
        throw LSST_EXCEPT(pexExceptions::RuntimeErrorException,
                          (boost::format("Object at (%d, %d) has no counts") % x % y).str());
    }

    double const sum_x =
        -im(-1,  1) + im( 1,  1) +
        -im(-1,  0) + im( 1,  0) +
        -im(-1, -1) + im( 1, -1);
    double const sum_y =
        (im(-1,  1) + im( 0,  1) + im( 1,  1)) -
        (im(-1, -1) + im( 0, -1) + im( 1, -1));

    return Centroid(lsst::afw::image::indexToPosition(x + image.getX0()) + sum_x/sum,
                    lsst::afw::image::indexToPosition(y + image.getY0()) + sum_y/sum);
}

//
// Explicit instantiations
//
// We need to make an instance here so as to register it with MeasureCentroid
//
// \cond
#define MAKE_CENTROIDERS(IMAGE_T) \
    registerMe<NaiveMeasureCentroid, lsst::afw::image::Image<IMAGE_T> >("NAIVE")
                
volatile bool isInstance[] = {
    MAKE_CENTROIDERS(int),
    MAKE_CENTROIDERS(float)
};

// \endcond

}}}}
