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

Class
    Foam::highOrderFit::stencilField

Description
    Stencils of cells for cellToFace surface interpolation indexed by face.

SourceFiles
    stencilField.C

\*---------------------------------------------------------------------------*/

#ifndef stencilField_H
#define stencilField_H

#include "extendedCellToFaceStencil.H"
#include "fvMesh.H"
#include "List.H"
#include "stencil.H"
#include "transformer.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace highOrderFit
{

/*---------------------------------------------------------------------------*\
                         Class stencilField Declaration
\*---------------------------------------------------------------------------*/

class stencilField
:
    public List<stencil>
{
    // Private data

        const fvMesh& mesh_;

    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=(const stencilField&);


public:

    // Constructors

        //- Construct from a specified list of stencils of cell labels.
        stencilField
        (
            const labelListList& stencilCellsList,
            const mapDistribute& map,
            const fvMesh& mesh,
            const transformer& transformer
        );


    //- Destructor
    ~stencilField();


    // Member Functions


};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace highOrderFit

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
