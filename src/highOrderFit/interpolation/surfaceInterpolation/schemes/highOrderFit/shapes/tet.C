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

#include "tet.H"
#include "tensor.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::highOrderFit::tet::tet()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::highOrderFit::tet::~tet()
{}


Foam::highOrderFit::tet::tet
(
    const point a,
    const point b,
    const point c
)
:
    Vector<point>(a, b, c)
{}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

Foam::scalar Foam::highOrderFit::tet::volumeMoment
(
    const Foam::highOrderFit::order& order
) const
{
    tensor A(*this);
    A = A.T();

    List<exponentTensor> exponentTensors;
    order.calculateExponentTensors(exponentTensors);

    scalar sumOfExponentTensorTerms = 0.0;

    forAll(exponentTensors, i)
    {
        const exponentTensor& exponentTensor = exponentTensors[i];

        sumOfExponentTensorTerms +=
            exponentTensor.factorialRatio() * 
            exponentTensor.productOfExponentials(A);
    }

    return det(A) * order.factorialRatio(3) * sumOfExponentTensorTerms;
}

// ************************************************************************* //