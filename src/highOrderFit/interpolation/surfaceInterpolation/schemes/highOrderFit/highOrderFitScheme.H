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
    Foam::highOrderFitScheme

Description
    A high-order approximation for interpolating surface fluxes from an
    upwind-biased stencil of cell averages.

SourceFiles
    highOrderFitScheme.C

\*---------------------------------------------------------------------------*/

#ifndef highOrderFitScheme_H
#define highOrderFitScheme_H

#include "autoPtr.H"
#include "Diagnostic.H"
#include "extendedUpwindCellToFaceStencil.H"
#include "multipliers.H"
#include "order.H"
#include "stencilBoundaryPolicy.H"
#include "stencilBoundaryExclusion.H"
#include "stencilField.H"
#include "transformer.H"
#include "upwind.H"
#include "upwindCPCCellToFaceStencilObject.H"
#include "weightsField.H"
#include "weightsFieldPair.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class highOrderFitScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class highOrderFitScheme
:
    public upwind<Type>
{
    // Private data

        //- Controls the minimum angle and relative lengths of opposing faces
        //- for constructing the upwind-biased stencil
        const scalar minOpposedness_ = 0.5;

        const fvMesh& mesh_;
        const extendedUpwindCellToFaceStencil& stencils_;
        const autoPtr<highOrderFit::transformer> transformer_;
        const List<highOrderFit::order> moments_;
        const autoPtr<highOrderFit::multipliers> multipliers_;
        const highOrderFit::weightsFieldPair& weights_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        highOrderFitScheme(const highOrderFitScheme&);

        //- Disallow default bitwise assignment
        void operator=(const highOrderFitScheme&);

        const extendedUpwindCellToFaceStencil& initialiseStencils()
        {
            return upwindCPCCellToFaceStencilObject::New
            (
                mesh_,
                false, // not pure upwind
                minOpposedness_,
                false, // not a correction on linear
                autoPtr<stencilBoundaryPolicy>
                (
                    new stencilBoundaryExclusion()
                )
            );
        }

        const highOrderFit::weightsFieldPair& initialiseWeights()
        {
            return highOrderFit::weightsFieldPair::New
            (
                mesh_,
                stencils_,
                transformer_,
                moments_,
                multipliers_
            );
        }


public:

    //- Runtime type information
    TypeName("highOrderFit");

    // Constructors

        //- Construct using the configuration provided by the \c Istream \c is.
        // @param[in] is an fvSchemes configuration string.
        //               The first word identifies the \c faceFlux
        //               \c surfaceScalarField.
        highOrderFitScheme(const fvMesh& mesh, Istream& is)
        :
            upwind<Type>
            (
                mesh,
                mesh.lookupObject<surfaceScalarField>(word(is))
            ),
            mesh_(mesh),
            stencils_(initialiseStencils()),
            transformer_(highOrderFit::transformer::New(word(is))),
            moments_(List<highOrderFit::order>(is)),
            multipliers_(highOrderFit::multipliers::New(word(is))),
            weights_(initialiseWeights())
        {}

        //- Construct a highOrderFitScheme using the specified \c faceFlux
        // @param[in] faceFlux determines the upwind direction for each face
        highOrderFitScheme
        (
            const fvMesh& mesh,
            const surfaceScalarField& faceFlux,
            Istream& is
        )
        :
            upwind<Type>(mesh, faceFlux),
            mesh_(mesh),
            stencils_(initialiseStencils()),
            transformer_(highOrderFit::transformer::New(word(is))),
            moments_(List<highOrderFit::order>(is)),
            multipliers_(highOrderFit::multipliers::New(word(is))),
            weights_(initialiseWeights())
        {}


    // Member Functions

        //- Returns \c true since a high-order correction on upwind is applied.
        virtual bool corrected() const
        {
            return true;
        }

        //- Explicit correction to the surface flux interpolate
        virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        correction
        (
            const GeometricField<Type, fvPatchField, volMesh>& vf
        ) const
        {
            return stencils_.weightedSum
            (
                upwind<Type>::faceFlux_,
                vf,
                weights_.owner(),
                weights_.neighbour()
            );
        }

        //- Obtain interpolation diagnostics for a face of the specified field.
        autoPtr<highOrderFit::Diagnostic<Type>> diagnose
        (
            const label facei,
            const GeometricField<Type, fvPatchField, volMesh>& field
        ) const
        {
            return autoPtr<highOrderFit::Diagnostic<Type>>
            (
                new highOrderFit::Diagnostic<Type>
                (
                    facei,
                    field,
                    upwind<Type>::faceFlux_,
                    mesh_,
                    stencils_,
                    weights_
                )
            );
        }

        //- Access stencil weights for diagnostic purposes.
        const highOrderFit::weightsFieldPair& weights() const
        {
            return weights_;
        }

};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
