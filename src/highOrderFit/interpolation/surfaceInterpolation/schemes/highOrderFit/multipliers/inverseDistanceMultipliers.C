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

#include "inverseDistanceMultipliers.H"
#include "addToRunTimeSelectionTable.H"

namespace Foam
{
namespace highOrderFit
{

defineTypeNameAndDebug(inverseDistanceMultipliers, 0);
addToRunTimeSelectionTable
(
    multipliers,
    inverseDistanceMultipliers,
    word
);

}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::highOrderFit::inverseDistanceMultipliers::inverseDistanceMultipliers()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::highOrderFit::inverseDistanceMultipliers::~inverseDistanceMultipliers()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void Foam::highOrderFit::inverseDistanceMultipliers::calculate
(
    const Foam::highOrderFit::stencil& stencil,
    Foam::scalarList& m
) const
{
    m.setSize(stencil.size());
    m = 1.0;
    m[0] = 1024.0;
    m[1] = 1024.0;
}


// ************************************************************************* //
