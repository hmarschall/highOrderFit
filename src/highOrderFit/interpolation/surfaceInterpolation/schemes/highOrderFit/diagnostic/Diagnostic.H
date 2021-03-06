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
    Foam::highOrderFit::Diagnostic

Description
    A diagnostic object for a particular face and face flux which can be
    inserted into an Ostream.

SourceFiles
    Diagnostic.C

\*---------------------------------------------------------------------------*/

#ifndef Diagnostic_H
#define Diagnostic_H

#include "label.H"
#include "extendedUpwindCellToFaceStencil.H"
#include "IOstreams.H"
#include "surfaceFields.H"
#include "weightsDiagnostic.H"
#include "weightsFieldPair.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Forward declaration of classes                                               
class Ostream;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace highOrderFit
{
 
// Forward declaration of friend functions and operators
template<class Type> class Diagnostic;
template<class Type> Ostream& operator<<
(
    Ostream&,
    const Diagnostic<Type>&
);


/*---------------------------------------------------------------------------*\
                         Class Diagnostics Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class Diagnostic
{
    // Private data

        const label facei_;
        const bool owner_;
        List<List<Type>> values_;
        List<List<point>> cellCentres_;
        scalarList weights_;
        autoPtr<weightsDiagnostic> weightsDiagnostic_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        Diagnostic(const Diagnostic&);

        //- Disallow default bitwise assignment
        void operator=(const Diagnostic&);


public:

    // Constructors

        Diagnostic
        (
            const label facei,
            const GeometricField<Type, fvPatchField, volMesh>& field,
            const surfaceScalarField& faceFlux,
            const fvMesh& mesh,
            const extendedUpwindCellToFaceStencil& stencils,
            const weightsFieldPair& weights
        );


    //- Destructor
    ~Diagnostic();


    // Member Functions

        //- The number of cells in the stencil
        label size() const
        {
            return weights_.size();
        }


    // IOstream Operators

        //- Output the diagnostics in human-readable form
        friend Ostream& operator<< <Type>
        (
            Ostream& os,
            const Diagnostic<Type>& diagnostic
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace highOrderFit

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "Diagnostic.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
