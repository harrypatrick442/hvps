using CircuitAnalysis;
using CircuitAnalysis.ComponentSpecs;
using VoltageMultiplier.CircuitSimulation;
using ModifiedNodalAnalysis;
using DD1800 = CircuitAnalysis.DD1800;
using ModifiedNodalAnalysis.Solver;
using Shutdown;
using Logging;
using Core.FileSystem;
using Core.MemoryManagement;
using Core.Maths.CUBLAS;
using InfernoDispatcher;
using Core.Enums;
using Core.Maths.Tensors;
using Core.Pool;
using FiniteElementAnalysis.Boundaries;
using FiniteElementAnalysis.Fields;
using FiniteElementAnalysis.Mesh.Generation;
using FiniteElementAnalysis.Mesh.Tetrahedral;
using FiniteElementAnalysis.Mesh;
using FiniteElementAnalysis.Ply;
using FiniteElementAnalysis.Polyhedrals;
using FiniteElementAnalysis.Results.Composites;
using FiniteElementAnalysis.Results;
using FiniteElementAnalysis.Solvers;
using FiniteElementAnalysis.SourceRegions;
using System.Reflection;
namespace FlybackModelling
{
    public class Program
    {
        private const int N_CUDA_CONTEXT = 10;
        private const string TEMP_DIRECTORY_PATH = "D:\\temp\\";
        private const double I_S_PEAK = 0.0833;
        private const double N_S = 5477;
        private const bool HAS_FLUX_MEASUREMENT_BOUNDARY = true;
        private const int N_MEASUREMENT_BOUNDARIES_FLUX_LINKAGE = 2;
        private static readonly CoreSpecs CORE_SPECS = new PC40_UY30_MnZn_U();
        private static readonly VolumeConstraints VOLUME_CONSTRAINTS = new VolumeConstraints(
                        windingElement: 1e-8,//1e-9,
                        coreElement: 5e-8,//5e-8,//8,
                        freeSpaceElement: 5e-7,//5e-8,
                        airGapElement: 3e-10//3e-10
                    );
        private static readonly Conductivities CONDUCTIVITIES = new Conductivities();
        //Expects mesh file to be in Meshes folder.
        private const string MESH_FILE_NAME = "UsingUY30MnZnPC40130x95mm.obj";

        const string OPERATION_WINDING_CURRENT_FIRST_HALF = "windingCurrentFirstHalf",
            OPERATION_WINDING_CURRENT_SECOND_HALF = "windingCurrentSecondHalf",
            OPERATION_MAGNETIC_FIELD = "magneticField",
            MATERIAL_BOUNDARY = "MaterialBoundary",
            WINDING_CURRENT_FIRST_BOUNDARY = "WindingCurrentFirstBoundary",
            WINDING_CURRENT_SECOND_BOUNDARY = "WindingCurrentSecondBoundary",
            FAR_FIELD_MAGNETIC_BOUNDARY = "FarFieldMagneticBoundary",
            WINDING_VOLUME_FIRST_HALF = "WindingVolumeFirstHalf",
            WINDING_VOLUME_SECOND_HALF = "WindingVolumeSecondHalf",
            CORE_VOLUME_ONE = "CoreVolumeOne",
            FREE_SPACE_VOLUME = "FreeSpaceVolume",
            AIR_GAP_VOLUME = "AirGapVolume";
        private const string FLUX_MEASUREMENT_BOUNDARY_CORE = "FluxMeasurementBoundaryCore",
            FLUX_MEASUREMENT_BOUNDARY_WINDING = "FluxMeasurementBoundaryWinding",
            FLUX_MEASUREMENT_BOUNDARY_SURROUNDINGS = "FluxMeasurementBoundarySurroundings";
        public static void Main(string[] args)
        {
            string projectDirectory = Assembly.GetEntryAssembly()!.Location;
            while (Path.GetFileName(projectDirectory) != "FlybackModelling")
            {
                projectDirectory = Directory.GetParent(projectDirectory)!.FullName;
            }
            string meshPath = Path.Combine(projectDirectory, "Meshes", MESH_FILE_NAME);
            Console.WriteLine(projectDirectory);
            Logs.Initialize(LogFilePathDefault.Value);
            ShutdownManager.Initialize(Environment.Exit, () => Logs.Default);
            GpuMemoryInfoNVML.Initialize();
            CudaContextAssignedThreadPool? mnaCudaThreadPool = null;
            mnaCudaThreadPool = new CudaContextAssignedThreadPool(N_CUDA_CONTEXT);
            MNAInversionHandler.Initialize(mnaCudaThreadPool);
            Dispatcher.InitializeWithNative(Console.WriteLine);
            byte[] modelObjFileBytes = File.ReadAllBytes(meshPath);
            Console.WriteLine("Used cached flyback transformer FEA results? (y/n) [y]:");
            bool useCachedSolverResults = EnteredYes();
            Paths paths = new Paths(TEMP_DIRECTORY_PATH);            /*
            BasicCalculationsFlybackSpecs basicCalculationsFlybackSpecs = FlybackDesignEquations
                .Calculate(
                throwExceptions: true, 
                P_out:powerOutRequired, 
                V_in_min:minimumInputVoltage, 
                V_in_max:maximumInputVoltage,
                f_s:switchingFrequency, 
                D_max:maximumDutyCycle, 
                isCCMElseDCM: false,
                V_out:desiredOutputVoltage, 
                efficiency,
                coreSpecs,
                bobinSpecs,
                primaryWireSpecs,
                secondaryWireSpecs);*/
            //double turnsPerBundle = basicCalculationsFlybackSpecs.N_s / nBundlesSecondaryWinding;
            double secondaryTotalCurrentForWindingsAsSingleVolume =
                   I_S_PEAK
                    * N_S;
            VolumesCollection volumes = CreateVolumes(
                VOLUME_CONSTRAINTS, CONDUCTIVITIES, CORE_SPECS);
            BoundariesCollection boundaries = CreateBoundaries(
                hasFluxMeasurementBoundaryWinding: HAS_FLUX_MEASUREMENT_BOUNDARY,
                secondaryTotalCurrentForWindingsAsSingleVolume,
            out MeasurementBoundary fluxMeasurementBoundaryCore,
            out MeasurementBoundary? fluxMeasurementBoundaryWinding,
            out MeasurementBoundary fluxMeasurementBoundarySurroundings);

            TemporaryDirectory? temporaryDirectory = null;
            TemporaryWorkingDirectoryManager? workingDirectoryManager = null;
            try
            {
                temporaryDirectory = TemporaryDirectory.InCustomTempDirectory(paths.TempDirectory);
                FileCachedItems fileCachedItems = new FileCachedItems(paths.CacheFilePath);
                workingDirectoryManager = new TemporaryWorkingDirectoryManager(
                        Path.Combine(temporaryDirectory.AbsolutePath, "workings"));
                Console.WriteLine($"Output directory path: \"{paths.FlybackTransformerModellingResultsDirectory}\"");
                DirectoryHelper.DeleteRecursively(paths.FlybackTransformerModellingResultsDirectory, throwOnError: false);
                TetrahedralMesh mesh = LoadMesh(modelObjFileBytes, volumes, boundaries, paths.TetgenOutputDirectoryPath,
                    fileCachedItems, useCachedSolverResults);
                PlyWriter.Write(paths.MeshPlyFilePath, mesh);
                StaticCurrentConductionSolver staticCurrentSolver = new StaticCurrentConductionSolver();

                TetrahedralMesh firstHalfWindingMesh = mesh.ToOperationSpecificMesh(
                    OPERATION_WINDING_CURRENT_FIRST_HALF);
                StaticCurrentConductionResult firstHalfWindingResult
                 = staticCurrentSolver.Solve(
                    firstHalfWindingMesh,
                    workingDirectoryManager,
                    OPERATION_WINDING_CURRENT_FIRST_HALF,
                    cachedSolverResult: fileCachedItems.GetItem<CoreSolverResult>(
                        CachedItems.FirstHalfWindingStaticCurrentSolverResult),
                    useCachedSolverResults: useCachedSolverResults);
                firstHalfWindingResult.Print();

                TetrahedralMesh secondHalfWindingMesh = mesh.ToOperationSpecificMesh(
                    OPERATION_WINDING_CURRENT_SECOND_HALF);
                StaticCurrentConductionResult secondHalfWindingStaticCurrentSolverResult
                    = staticCurrentSolver.Solve(
                    secondHalfWindingMesh,
                    workingDirectoryManager,
                    OPERATION_WINDING_CURRENT_SECOND_HALF,
                    cachedSolverResult: fileCachedItems.GetItem<CoreSolverResult>(
                        CachedItems.SecondHalfWindingStaticCurrentSolverResult),
                    useCachedSolverResults: useCachedSolverResults);
                secondHalfWindingStaticCurrentSolverResult.Print();

                CompositeStaticCurrentResult fullWindingResult =
                    new CompositeStaticCurrentResult(secondHalfWindingStaticCurrentSolverResult,
                    firstHalfWindingResult);

                PlyWriter.Write(
                    paths.VoltagesFirstHalfPlyFilePath,
                    firstHalfWindingMesh,
                    new ScalarFieldResult(
                        "voltage", firstHalfWindingResult.NodalVoltages
                    )
                );
                //CloudCompareHelper.Open(VOLTAGES_FIRST_HALF_PLY_FILE_PATH);

                var firstHalfVolumetricCurrentDensities = firstHalfWindingResult
                    .GetNodalVolumeCurrentDensities("volume_current_density");
                PlyWriter.Write(
                    paths.CurrentDensitiesFirstHalfPlyFilePath,
                    firstHalfWindingMesh,
                    firstHalfVolumetricCurrentDensities,
                    ScalarFieldResult.MagnitudeFromVectorField(
                        "volume_current_density_magnitude", firstHalfVolumetricCurrentDensities.Values)
                );
                //CloudCompareHelper.Open(CURRENT_DENTISITIES_FIRST_HALF_PLY_FILE_PATH);


                PlyWriter.Write(
                    paths.VoltagesSecondHalfPlyFilePath,
                    secondHalfWindingMesh,
                    new ScalarFieldResult(
                        "voltage", secondHalfWindingStaticCurrentSolverResult.NodalVoltages
                    )
                );
                //CloudCompareHelper.Open(VOLTAGES_SECOND_HALF_PLY_FILE_PATH);

                var secondHalfVolumetricCurrentDensities = secondHalfWindingStaticCurrentSolverResult
                    .GetNodalVolumeCurrentDensities("volume_current_density");
                PlyWriter.Write(
                    paths.CurrentDensitiesSecondHalfPlyFilePath,
                    secondHalfWindingMesh,
                    secondHalfVolumetricCurrentDensities,
                    ScalarFieldResult.MagnitudeFromVectorField(
                        "volume_current_density_magnitude", secondHalfVolumetricCurrentDensities.Values)
                );
                //CloudCompareHelper.Open(CURRENT_DENTISITIES_SECOND_HALF_PLY_FILE_PATH);



                StaticMagneticConductionSolver magneticSolver = new StaticMagneticConductionSolver();

                TetrahedralMesh magneticFieldMesh = mesh.ToOperationSpecificMesh(OPERATION_MAGNETIC_FIELD);
                StaticMagneticConductionResult magneticFieldSolverResult = magneticSolver.Solve(magneticFieldMesh,
                    workingDirectoryManager, OPERATION_MAGNETIC_FIELD,
                    new DelegateApplySourceRegion[] {
                                fullWindingResult.ApplyVolumetricCurrentDensities
                    },
                    SolverMethod.BlockMatrixInversionGpuOnly,
                    cachedSolverResult: fileCachedItems
                        .GetItem<CoreSolverResult>(CachedItems.MagneticFieldSolverResult),
                    useCachedSolverResults: useCachedSolverResults
                );

                Dictionary<int, double> mapNodeToPermeability = new Dictionary<int, double>();
                foreach (TetrahedronElement element in magneticFieldMesh.Elements)
                {
                    double permeability = ((StaticMagneticConductionVolume)element.VolumeIsAPartOf).Permeability;
                    foreach (Node node in element.Nodes)
                    {
                        mapNodeToPermeability[node.Identifier] = permeability;
                    }
                }
                double[] nodePermeabilities = new double[mesh.Nodes.Length];
                foreach (var kvp in mapNodeToPermeability)
                {
                    int nodeIndex = magneticFieldMesh.MapNodeIdentifierToGlobalIndex[kvp.Key];
                    nodePermeabilities[nodeIndex] = kvp.Value;
                }

                PlyWriter.Write(
                    paths.PermeabilitiesPlyFilePath,
                    magneticFieldMesh,
                    new ScalarFieldResult(
                        "permeability",
                        nodePermeabilities
                    )
                );

                magneticFieldSolverResult.Print();
                PlyWriter.Write(
                    paths.MagneticFieldPlyFilePath,
                    magneticFieldMesh,
                    new VectorFieldResult(
                        "magnetic_vector_potential",
                        magneticFieldSolverResult.NodalMagneticVectorPotentials
                    ),
                    ScalarFieldResult.MagnitudeFromVectorField(
                        "magnetic_vector_potential_magnitude",
                        magneticFieldSolverResult.NodalMagneticVectorPotentials
                    ),
                    new VectorFieldResult(
                        "magnetic_flux_density",
                        magneticFieldSolverResult.GetNodalMagneticFluxDensityB(),
                        includeMagnitude: true
                    )
                );
                double fluxLinkageCore = magneticFieldSolverResult.CalculateFluxLinkage(
                    N_S,
                    N_MEASUREMENT_BOUNDARIES_FLUX_LINKAGE,
                    out double fluxLinkageCoreArea,
                    fluxMeasurementBoundaryCore
                );
                double fluxLinkageWindingsArea = 0;
                double fluxLinkageWindings = HAS_FLUX_MEASUREMENT_BOUNDARY ? magneticFieldSolverResult.CalculateFluxLinkage(
                    N_S,
                    N_MEASUREMENT_BOUNDARIES_FLUX_LINKAGE,
                    out fluxLinkageWindingsArea,

                    fluxMeasurementBoundaryWinding
                ) : 0;
                double fluxLinkageSurroundings = magneticFieldSolverResult.CalculateFluxLinkage(
                    N_S,
                    N_MEASUREMENT_BOUNDARIES_FLUX_LINKAGE,
                    out double fluxLinkageSurroundingsArea,
                    fluxMeasurementBoundarySurroundings
                );
                double expectedAe = CORE_SPECS.A_e * N_MEASUREMENT_BOUNDARIES_FLUX_LINKAGE;
                if ((Math.Abs(fluxLinkageCoreArea) - expectedAe) / CORE_SPECS.A_e > 0.05)
                {
                    Console.WriteLine($"WARNING, A_e the effectice cross sectional area of the core was provided as {CORE_SPECS.A_e} and a value of {expectedAe} was expected for {N_MEASUREMENT_BOUNDARIES_FLUX_LINKAGE} measurement boundaries. But a value of {fluxLinkageCoreArea} was obtained from the measurement boundary");
                }
                double secondaryFluxLinkage = fluxLinkageCore;
                double averageSecondaryMutualFluxDensity = secondaryFluxLinkage
                    / (N_S * CORE_SPECS.A_e);
                double secondaryInductance = secondaryFluxLinkage / I_S_PEAK;
                double secondaryLeakageInductange = (fluxLinkageSurroundings + fluxLinkageWindings)
                    / I_S_PEAK;
                double totalFluxDensity = (fluxLinkageCore + fluxLinkageWindings + fluxLinkageSurroundings)
                    / (N_S * CORE_SPECS.A_e);
                string[] resultLines = new string[] {
                    $"Average secondary mutual flux density: {averageSecondaryMutualFluxDensity}.",
                    $"Flux linkage core: {fluxLinkageCore} for area: {fluxLinkageCoreArea}",
                    HAS_FLUX_MEASUREMENT_BOUNDARY?$"Flux linkage windings: {fluxLinkageWindings} for area: {fluxLinkageWindingsArea}":"No windings flux measurement boundary was included",
                    $"Total flux linkage: {secondaryFluxLinkage}.",
                    $"Secondary inductance: {secondaryInductance}.",
                    $"Flux linkage surroundings: {fluxLinkageSurroundings} for area: {fluxLinkageSurroundingsArea}",
                    $"Secondary leakage inductance: {secondaryLeakageInductange}",
                    $"Total flux density was : {totalFluxDensity} ",
                    $"Proportion of total inductance leaked: {secondaryLeakageInductange/(secondaryLeakageInductange+secondaryInductance)}",
                    $"At I_peak the magnetic flux density is {totalFluxDensity/CORE_SPECS.B_sat} the saturation magnetic flux density"
                };
                foreach (string resultLine in resultLines)
                    Console.WriteLine(resultLine);
                File.WriteAllLines(paths.ResultPlyFilePath, resultLines);
                //CloudCompareHelper.Open(paths.MagneticFieldPlyFilePath);
                FEMResultFlybackSpecs femResultFlybackSpecs = new FEMResultFlybackSpecs(
                    L_ms: secondaryInductance,
                    L_ks: secondaryLeakageInductange);
                double flybackEnergySingleCycle = 0.5d * femResultFlybackSpecs.L_ms
                    * Math.Pow(I_S_PEAK, 2);
                Console.WriteLine($"Maximum energy into flyback for single cycle is {flybackEnergySingleCycle}");

            }
            finally
            {
                temporaryDirectory?.Dispose();
                workingDirectoryManager?.Dispose();
            }
        }
        private static BoundariesCollection CreateBoundaries(
            bool hasFluxMeasurementBoundaryWinding,
            double secondaryTotalCurrentForWindingsAsSingleVolume,
            out MeasurementBoundary fluxMeasurementBoundaryCore,
            out MeasurementBoundary? fluxMeasurementBoundaryWinding,
            out MeasurementBoundary fluxMeasurementBoundarySurroundings)
        {
            fluxMeasurementBoundaryCore = new MeasurementBoundary(FLUX_MEASUREMENT_BOUNDARY_CORE);
            fluxMeasurementBoundaryWinding = hasFluxMeasurementBoundaryWinding ? new MeasurementBoundary(FLUX_MEASUREMENT_BOUNDARY_WINDING) : null;
            fluxMeasurementBoundarySurroundings = new MeasurementBoundary(FLUX_MEASUREMENT_BOUNDARY_SURROUNDINGS);
            return new BoundariesCollection(
                new MultipleOperationBoundary(
                    WINDING_CURRENT_FIRST_BOUNDARY,
                    OPERATION_WINDING_CURRENT_FIRST_HALF,
                        new FixedVoltageDirichletBoundary(
                            WINDING_CURRENT_FIRST_BOUNDARY,
                            0
                        ),
                    OPERATION_WINDING_CURRENT_SECOND_HALF,
                        new FixedVoltageDirichletBoundary(
                            WINDING_CURRENT_FIRST_BOUNDARY,
                            0
                        ),
                    OPERATION_MAGNETIC_FIELD,
                        new MaterialBoundary(
                            WINDING_CURRENT_FIRST_BOUNDARY)
                        ),
                new MultipleOperationBoundary(
                    WINDING_CURRENT_SECOND_BOUNDARY,
                    OPERATION_WINDING_CURRENT_FIRST_HALF,
                        new FixedCurrentBoundary(
                            WINDING_CURRENT_SECOND_BOUNDARY,
                            secondaryTotalCurrentForWindingsAsSingleVolume
                        ),
                    OPERATION_WINDING_CURRENT_SECOND_HALF,
                        new FixedCurrentBoundary(
                            WINDING_CURRENT_SECOND_BOUNDARY,
                            -secondaryTotalCurrentForWindingsAsSingleVolume
                        ),
                    OPERATION_MAGNETIC_FIELD,
                        new MaterialBoundary(
                            WINDING_CURRENT_SECOND_BOUNDARY
                        )
                    ),
                new MultipleOperationBoundary(
                    FAR_FIELD_MAGNETIC_BOUNDARY,
                    OPERATION_WINDING_CURRENT_FIRST_HALF,
                    null,
                    OPERATION_WINDING_CURRENT_SECOND_HALF,
                    null,
                    OPERATION_MAGNETIC_FIELD,
                        new FixedMagneticVectorPotentialDirichletBoundary(
                            FAR_FIELD_MAGNETIC_BOUNDARY, new Vector3D(0, 0, 0)
                        )
                    ),
                //new AdiabaticBoundaryInsulated(ADIABATIC_BOUNDARY),
                new MaterialBoundary(MATERIAL_BOUNDARY),
                fluxMeasurementBoundaryCore,
                fluxMeasurementBoundaryWinding,//Causing the issue.
                fluxMeasurementBoundarySurroundings
            );
        }
        private static VolumesCollection CreateVolumes(
            VolumeConstraints volumeConstraints,
            Conductivities conductivities,
            CoreSpecs coreSpecs)
        {
            return new VolumesCollection(
                new MultipleOperationVolume(
                    WINDING_VOLUME_SECOND_HALF,
                    OPERATION_WINDING_CURRENT_FIRST_HALF,
                        null,
                    OPERATION_WINDING_CURRENT_SECOND_HALF,
                        new StaticCurrentVolume(
                            WINDING_VOLUME_SECOND_HALF, conductivities.Wire),
                    OPERATION_MAGNETIC_FIELD,
                        new StaticMagneticConductionVolume(
                            WINDING_VOLUME_SECOND_HALF, Permeabilities.FREE_SPACE),
                    maximumTetrahedralVolumeConstraint: volumeConstraints.WindingElement
                ),
                new MultipleOperationVolume(
                    WINDING_VOLUME_FIRST_HALF,
                    OPERATION_WINDING_CURRENT_FIRST_HALF,
                        new StaticCurrentVolume(WINDING_VOLUME_FIRST_HALF,
                            conductivities.Wire),
                    OPERATION_WINDING_CURRENT_SECOND_HALF,
                        null,
                    OPERATION_MAGNETIC_FIELD,
                        new StaticMagneticConductionVolume(
                            WINDING_VOLUME_FIRST_HALF, Permeabilities.FREE_SPACE),
                    maximumTetrahedralVolumeConstraint: volumeConstraints.WindingElement
                )/*,
                new MultipleOperationVolume(
                    BOBIN_VOLUME,
                    OPERATION_WINDING_CURRENT_FIRST_HALF,
                        null,
                    OPERATION_WINDING_CURRENT_SECOND_HALF,
                        null,
                    OPERATION_MAGNETIC_FIELD,
                        new StaticMagneticConductionVolume(
                            BOBIN_VOLUME, THREED_PRINTED_RESIN_PERMEABILITY)
                )*/,
                new MultipleOperationVolume(
                    CORE_VOLUME_ONE,
                    OPERATION_WINDING_CURRENT_FIRST_HALF,
                        null,
                    OPERATION_WINDING_CURRENT_SECOND_HALF,
                        null,
                    OPERATION_MAGNETIC_FIELD,
                        new StaticMagneticConductionVolume(
                            CORE_VOLUME_ONE, coreSpecs.RelativePermeability * Permeabilities.FREE_SPACE
                    ),
                    maximumTetrahedralVolumeConstraint: volumeConstraints.CoreElement/*2.5mm cubed*/
                ),
                new MultipleOperationVolume(
                    FREE_SPACE_VOLUME,
                    OPERATION_WINDING_CURRENT_FIRST_HALF,
                        null,
                    OPERATION_WINDING_CURRENT_SECOND_HALF,
                        null,
                    OPERATION_MAGNETIC_FIELD,
                        new StaticMagneticConductionVolume(
                            FREE_SPACE_VOLUME, Permeabilities.FREE_SPACE
                    ),
                    maximumTetrahedralVolumeConstraint: volumeConstraints.FreeSpaceElement
                ),
                new MultipleOperationVolume(
                    AIR_GAP_VOLUME,
                    OPERATION_WINDING_CURRENT_FIRST_HALF,
                        null,
                    OPERATION_WINDING_CURRENT_SECOND_HALF,
                        null,
                    OPERATION_MAGNETIC_FIELD,
                        new StaticMagneticConductionVolume(
                            AIR_GAP_VOLUME, coreSpecs.RelativePermeability * Permeabilities.FREE_SPACE
                    ),
                    maximumTetrahedralVolumeConstraint: volumeConstraints.AirGapElement
                )
            );
        }
        private static TetrahedralMesh LoadMesh(
            byte[] modelObjFileBytes,
            VolumesCollection volumes,
            BoundariesCollection boundaries,
            string directoryPath,
            FileCachedItems fileCachedItems,
            bool useCachedSolverResults)
        {
            PolyhedralDomain domain = ObjFileToPoly.Read(modelObjFileBytes, volumes, boundaries,
                out Dictionary<int, Boundary> mapMarkerToBoundary, Units.Millimeters,
                0.0001d);
            domain.CheckForNodesTooCloseTogether();
            FileCachedItem<TetgenGenerateMeshResult> cachedGenerateMeshResult =
                fileCachedItems.GetItem<TetgenGenerateMeshResult>(CachedItems.TetgenGenerateMeshResult);
            if (useCachedSolverResults)
            {

                if (cachedGenerateMeshResult.TryGet(out TetgenGenerateMeshResult? cachedTetgenGenerateMeshResult))
                {
                    return cachedTetgenGenerateMeshResult!.ToMesh(boundaries, volumes, mapMarkerToBoundary);
                }
            }
            DirectoryHelper.DeleteRecursively(directoryPath, false);
            Directory.CreateDirectory(directoryPath);
            string polyFilePath = Path.Combine(directoryPath, "mesh.poly");
            PolyFileGenerator.Generate(polyFilePath, domain);
            Tetgen.CopyTetViewToDirectory(directoryPath);
            using (Tetgen tetgen = new Tetgen())
            {
                TetgenGenerateMeshResult generateMeshResult = tetgen.GenerateTetrahedralMesh(
                    polyFilePath,
                    new TetgenParameters
                    {
                        RefineMesh = true,
                        MaximumTetrahedralVolumeConstraint = 0.0001,
                        CheckConsistencyOfFinalMesh = true
                    });
                Console.WriteLine(generateMeshResult.Output);
                if (generateMeshResult.ExitCode != 0)
                {
                    string moreExceptionInfo = generateMeshResult.GetMoreExceptionInfo();
                    Console.WriteLine(moreExceptionInfo);
                    throw new Exception(generateMeshResult.Output);
                }
                cachedGenerateMeshResult.Set(generateMeshResult);
                TetrahedralMesh mesh = generateMeshResult
                    .ToMesh(boundaries, volumes, mapMarkerToBoundary);
                return mesh;
            }
        }

        private static bool EnteredYes()
        {
            bool v = (Console.ReadKey().KeyChar + "").ToLower() != "n";
            Console.WriteLine();
            return v;
        }

    }
}
      
