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

#include "weightsField.H"
#include "uniformMultipliers.H"
#include "weights.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::highOrderFit::weightsField::weightsField
(
    const Foam::highOrderFit::stencilField& stencils
)
:
    scalarListList(stencils.size())
{
    for (label facei = 0; facei < stencils.mesh().nInternalFaces(); facei++)
    {
        scalarList& w = (*this)[facei];
        const label size = stencils[facei].size();

		if (size < 2)
		{
			FatalErrorInFunction
	            << "stencil for facei " << facei << " has fewer than two cells"
                << abort(FatalError);
		}

        w.setSize(size);

        const uniformMultipliers multipliers(size);

        const weights weights;
        weights.calculate(w, multipliers);

        w[0] -= 1;
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::highOrderFit::weightsField::~weightsField()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //


// ************************************************************************* //