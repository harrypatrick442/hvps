using CircuitAnalysis;
using Core.FileSystem;
using FiniteElementAnalysis.Boundaries;
using FiniteElementAnalysis.MeshGeneration;
using FiniteElementAnalysis.Polyhedrals;
using FiniteElementAnalysis;
using FiniteElementAnalysis.Solvers;
using CircuitAnalysis.ComponentSpecs;
using Core.Enums;
using FiniteElementAnalysis.SourceRegions;
using FiniteElementAnalysis.Fields;
using Core.Pool;
using FiniteElementAnalysis.Ply;
using FiniteElementAnalysis.CloudCompare;
using Core.Maths.Tensors;
using FiniteElementAnalysis.Results;
using FiniteElementAnalysis.Results.Composites;
using FiniteElementAnalysis.Mesh.Tetrahedral;
using FiniteElementAnalysis.Mesh.Generation;
using FiniteElementAnalysis.Mesh;

namespace VoltageMultiplier.FlybackTransformerModelling
{
    // Example usage:
    public class Permeabilities
    {
        public const double FREE_SPACE = 4 * Math.PI * 1e-7;
    }
}