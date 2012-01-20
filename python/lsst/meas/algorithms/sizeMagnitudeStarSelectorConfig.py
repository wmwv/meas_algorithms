# 
# LSST Data Management System
# Copyright 2008, 2009, 2010 LSST Corporation.
# 
# This product includes software developed by the
# LSST Project (http://www.lsst.org/).
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the LSST License Statement and 
# the GNU General Public License along with this program.  If not, 
# see <http://www.lsstcorp.org/LegalNotices/>.
#
import lsst.pex.config as pexConfig
from .algorithmsLib import SizeMagnitudeStarSelector

class SizeMagnitudeStarSelectorConfig(pexConfig.Config):
    minSize = pexConfig.Field(
        doc = "Minimum size to use",
        dtype = float,
        default = 0.0,
    )
    maxSize = pexConfig.Field(
        doc = "Maximum size to use",
        dtype = float,
        default = 1.0e100,
    )
    isSizeLog = pexConfig.Field(
        doc = "Are sizes already log(size)?",
        dtype = bool,
        default = False,
    )
    minMag = pexConfig.Field(
        doc = "Minimum magnitude to use",
        dtype = float,
        default = 0.0,
    )
    maxMag = pexConfig.Field(
        doc = "Maximum magnitude to use",
        dtype = float,
        default = 1.0e100,
    )
    starFrac = pexConfig.Field(
        doc = "What fraction of objects are likely stars?",
        dtype = float,
        default = 0.5,
    )
    startN = pexConfig.Field(
        doc = "Fraction of objects to use in first pass",
        dtype = float,
        default = 0.1,
    )
    fitOrder = pexConfig.Field(
        doc = "Order of polynomial of fit of size(x,y)",
        dtype = int,
        default = 1,
    )
    fitSigClip = pexConfig.Field(
        doc = "nSigma to reject a star as an outlier",
        dtype = float,
        default = 4.0,
    )
    fitStars = pexConfig.Field(
        doc = "Perform size(x,y) fit with fitStars brightest stars",
        dtype = int,
        default = 30,
    )
    purity = pexConfig.Field(
        doc = "Smaller = purer smaple of stars, larger = more stars",
        dtype = float,
        default = 0.05,
    )
    aperture = pexConfig.Field(
        doc = "nSigma to reject a star as an outlier",
        dtype = float,
        default = 5.0,
    )
    
    def makeAlgorithm(self):
        """Make a SizeMagnitudeStarSelector using the current config
        """
        policy = pexConfig.makePolicy(self)
        return SizeMagnitudeStarSelector(policy)
