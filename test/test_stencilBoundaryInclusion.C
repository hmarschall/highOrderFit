/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2017 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "catch.hpp"
#include "checks.H"
#include "stencilBoundaryInclusion.H"
#include "testCase.H"

using namespace Foam;

namespace Test
{

TEST_CASE("stencilBoundaryInclusion_excludes_empty_boundary_faces")
{
    const Test::testCase c("cartesian4x3Mesh");
    const stencilBoundaryInclusion policy;

    boolList includedBoundaryFaces;
    policy.applyTo(c.mesh(), includedBoundaryFaces);

    CHECK( includedBoundaryFaces.size() == 38 );
    CHECK( Test::countMatches(includedBoundaryFaces, true) == 14 );
}

}

// ************************************************************************* //
