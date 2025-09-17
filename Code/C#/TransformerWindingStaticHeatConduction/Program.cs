
using Core.FileSystem;
using FiniteElementAnalysis;
using FiniteElementAnalysis.Boundaries;
using FiniteElementAnalysis.Boundaries.Thermal;
using FiniteElementAnalysis.Plotting;
using FiniteElementAnalysis.Polyhedrals;
using FiniteElementAnalysis.Solvers;
using System.Reflection;
using Core.Enums;
using Core.Timing;
using Core.Maths;
using GMRES;
using System;
using FiniteElementAnalysis.Fields;
using InfernoDispatcher;
using Core.MemoryManagement;
using Shutdown;
using Logging;
using FiniteElementAnalysis.Ply;
using FiniteElementAnalysis.CloudCompare;
using FiniteElementAnalysis.Results;
using FiniteElementAnalysis.Mesh.Tetrahedral;
using FiniteElementAnalysis.Mesh.Generation;
using TransformerWindingStaticHeatConduction;
namespace StaticHeatConductionExperimentation
{
    // Example usage:
    public class Program
    {
        /*
         * https://wias-berlin.de/software/tetgen/fformats.html
         * */
        public static void Main(string[] args)
        {
            ShutdownManager.Initialize(Environment.Exit, () => Logs.Default);
            GpuMemoryInfoNVML.Initialize();
            Dispatcher.InitializeWithNative(Console.WriteLine);
            BeamWithHeatSource();
        }
        private static void BeamWithHeatSource()
        {
            BoundariesCollection boundaries = new BoundariesCollection(
                new FixedHeatFluxNeumannBoundary("WindingHeatBoundary", 183),
                new FixedTemperatureDirichletBoundary("FixedTemperatureBoundary", temperatureK: 298.15),
                new AdiabaticBoundaryInsulated("AdiabaticBoundary"),
                new MaterialBoundary("MaterialBoundary")
            );
            VolumesCollection volumes = new VolumesCollection(
               new StaticHeatVolume("WindingVolume", thermalConductivity: 401),
               new StaticHeatVolume("MineralOilVolume", thermalConductivity: 0.12),
               new StaticHeatVolume("CoreVolume", thermalConductivity: 5),
               new StaticHeatVolume("BobinVolume", thermalConductivity: 0.21)//
            );
            PolyhedralDomain domain = ObjFileToPoly.Read(
                MeshesResource.Primary, volumes, boundaries,
                out Dictionary<int, Boundary> mapMarkerToBoundary, Units.Meters, maxDistanceNodeMergeMeters:0.000001);
            using (TemporaryDirectory temporaryDirectory = new TemporaryDirectory())
            {
                using (TemporaryWorkingDirectoryManager workingDirectoryManager = new TemporaryWorkingDirectoryManager())
                {
                    string projectDirectory = DirectoryHelper.GetProjectDirectory();
                    string outputDirectory = Path.Combine(projectDirectory, "output");
                    Console.WriteLine($"Output to: \"{outputDirectory}\"");
                    DirectoryHelper.DeleteRecursively(outputDirectory, throwOnError: false);
                    string polyFilePath = Path.Combine(temporaryDirectory.AbsolutePath, "mesh.poly");
                    PolyFileGenerator.Generate(polyFilePath, domain);
                    using (Tetgen tetgen = new Tetgen())
                    {
                        TetgenGenerateMeshResult generateMeshResult = tetgen.GenerateTetrahedralMesh(polyFilePath,
                            new TetgenParameters
                            {
                                RefineMesh = true,
                                MaximumTetrahedralVolumeConstraint = 0.000001,
                                CheckConsistencyOfFinalMesh = true
                            });
                        Console.WriteLine(generateMeshResult.Output);
                        if (generateMeshResult.ExitCode != 0)
                        {
                            string moreExceptionInfo = generateMeshResult.GetMoreExceptionInfo();
                            Console.WriteLine(moreExceptionInfo);
                            throw new Exception(generateMeshResult.Output);
                        }
                        TetrahedralMesh mesh = generateMeshResult.ToMesh(boundaries, volumes, mapMarkerToBoundary);
                        long startTimeSolve = TimeHelper.MillisecondsNow;
                        HeatConductionResult solverResult = new HeatConductionSolver().Solve(mesh, workingDirectoryManager,
                            solverMethod: SolverMethod.BlockMatrixInversionGpuOnly);
                        long timeTaken = TimeHelper.MillisecondsNow - startTimeSolve;

                        solverResult.Print();
                        ContourPlotHelper.Plot(mesh, 100, outputDirectory, "plot", PlotPlaneType.Z);
                        double valueTFrom = mesh.ElementsBVHTree.QueryBVH(new Core.Maths.Tensors.Vector3D(0.01, 0, 0)).First().NodeA.ScalarValue;
                        double valueMiddle = mesh.ElementsBVHTree.QueryBVH(new Core.Maths.Tensors.Vector3D(0.205, 0, 0)).First().NodeA.ScalarValue;
                        double valueTo = mesh.ElementsBVHTree.QueryBVH(new Core.Maths.Tensors.Vector3D(0.4, 0, 0)).First().NodeA.ScalarValue;
                        string plyFilePath =
                            Path.Combine(outputDirectory, "temperatures.ply");
                        PlyWriter.WritePlyFile(
                            plyFilePath,
                            mesh.Nodes,
                            mesh.BoundaryFaces,
                            new ScalarFieldResult(
                                "temperature",
                                solverResult.NodalTemperatures
                            )
                        );
                        CloudCompareHelper.Open(plyFilePath);
                    }
                    Tetgen.CopyTetViewToDirectory(temporaryDirectory.AbsolutePath);
                    DirectoryHelper.CopyRecurively(temporaryDirectory.AbsolutePath, outputDirectory);
                }
            }
        }
    }
}