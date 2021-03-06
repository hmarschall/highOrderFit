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
#include "Diagnostic.H"
#include "highOrderFitScheme.H"
#include "interpolation.H"
#include "IOobject.H"
#include "mesh.H"
#include "tmp.H"

using namespace Foam;

namespace Test
{

TEST_CASE("highOrderFit_interpolates_constant_scalar_field")
{
    Test::interpolation highOrderFit("cartesian4x3Mesh");
    highOrderFit.T() = dimensionedScalar("T", dimless, scalar(1));
    const surfaceScalarField expectedTf
    (
        IOobject
        (
            "expectedTf",
            highOrderFit.runTime().timeName(),
            highOrderFit.mesh()
        ),
        highOrderFit.mesh(),
        dimensionedScalar("expectedTf", dimless, scalar(1))
    );

    const tmp<surfaceScalarField> Tf = highOrderFit.interpolateT();

    Test::checkEqual(Tf, expectedTf);
}

TEST_CASE("highOrderFit_averages_values_in_stencil_for_vertical_face")
{
    Test::interpolation highOrderFit("cartesian4x3Mesh");
    const Test::mesh testMesh(highOrderFit.mesh());
    highOrderFit.setTlinearInX();

    const tmp<surfaceScalarField> Tf = highOrderFit.interpolateT();

    const label facei = testMesh.indexOfFaceWithCentreAt(point(3, 1.5, 0));
    CHECK( Tf()[facei] == Test::approx(10.0) );
}

TEST_CASE("highOrderFit_averages_values_in_stencil_for_reversed_wind")
{
    Test::interpolation highOrderFit("cartesian4x3Mesh");
    highOrderFit.negateFaceFlux();
    highOrderFit.setTlinearInX();
    const Test::mesh testMesh(highOrderFit.mesh());

    const tmp<surfaceScalarField> Tf = highOrderFit.interpolateT();

    const label facei = testMesh.indexOfFaceWithCentreAt(point(1, 1.5, 0));
    CHECK( Tf()[facei] == Test::approx(10.0) );
}

TEST_CASE("highOrderFit_ignores_boundary_values")
{
    Test::interpolation highOrderFit("cartesian4x3Mesh");
    const Test::mesh testMesh(highOrderFit.mesh());
    const label facei = testMesh.indexOfFaceWithCentreAt(point(1, 1.5, 0));

    const highOrderFitScheme<scalar>& scheme =
        dynamic_cast<const highOrderFitScheme<scalar>&>
        (
            highOrderFit.scheme()
        );

    const autoPtr<highOrderFit::Diagnostic<scalar>> diagnostic =
        scheme.diagnose(facei, highOrderFit.T());
    CHECK( diagnostic->size() == 6 );
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Test

// ************************************************************************* //
