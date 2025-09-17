using CircuitAnalysis;
using Core.FileSystem;
using FiniteElementAnalysis.Boundaries;
using FiniteElementAnalysis.Polyhedrals;
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
    public class Modeler
    {
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
        public static BasicCalculationsFlybackSpecs Model(
            bool hasFluxMeasurementBoundary,
            CoreSpecs coreSpecs,
            BobinSpecs bobinSpecs,
            WireSpecs primaryWireSpecs,
            WireSpecs secondaryWireSpecs,
            double switchingFrequency,
            double maximumDutyCycle,
            double minimumInputVoltage,
            double maximumInputVoltage,
            double powerOutRequired,
            double desiredOutputVoltage,
            double efficiency,
            bool useCachedSolverResults,
            //int nBundlesSecondaryWinding,
            byte[] modelObjFileBytes,
            VolumeConstraints volumeConstraints,
            int nMeasurementBoundariesFluxLinkage,
            Conductivities conductivities,
                Paths paths,
            out FEMResultFlybackSpecs femResultFlybackSpecs
            )
        {
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
                secondaryWireSpecs);
            double peakSecondaryCurrent = basicCalculationsFlybackSpecs.I_s_peak;
            //double turnsPerBundle = basicCalculationsFlybackSpecs.N_s / nBundlesSecondaryWinding;
            double secondaryTotalCurrentForWindingsAsSingleVolume =
                    basicCalculationsFlybackSpecs.I_s_peak
                    * basicCalculationsFlybackSpecs.N_s;
            VolumesCollection volumes = CreateVolumes(
                volumeConstraints, conductivities, coreSpecs);
            BoundariesCollection boundaries = CreateBoundaries(
                hasFluxMeasurementBoundaryWinding: hasFluxMeasurementBoundary,
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
                    basicCalculationsFlybackSpecs.N_s,
                    nMeasurementBoundariesFluxLinkage,
                    out double fluxLinkageCoreArea,
                    fluxMeasurementBoundaryCore
                );
                double fluxLinkageWindingsArea = 0;
                double fluxLinkageWindings = hasFluxMeasurementBoundary? magneticFieldSolverResult.CalculateFluxLinkage(
                    basicCalculationsFlybackSpecs.N_s,
                    nMeasurementBoundariesFluxLinkage,
                    out fluxLinkageWindingsArea,

                    fluxMeasurementBoundaryWinding
                ):0;
                double fluxLinkageSurroundings = magneticFieldSolverResult.CalculateFluxLinkage(
                    basicCalculationsFlybackSpecs.N_s,
                    nMeasurementBoundariesFluxLinkage,
                    out double fluxLinkageSurroundingsArea,
                    fluxMeasurementBoundarySurroundings
                );
                double expectedAe = coreSpecs.A_e * nMeasurementBoundariesFluxLinkage;
                if ((Math.Abs(fluxLinkageCoreArea) - expectedAe) / coreSpecs.A_e > 0.05)
                {
                    Console.WriteLine($"WARNING, A_e the effectice cross sectional area of the core was provided as {coreSpecs.A_e} and a value of {expectedAe} was expected for {nMeasurementBoundariesFluxLinkage} measurement boundaries. But a value of {fluxLinkageCoreArea} was obtained from the measurement boundary");
                }
                var coreSpec = coreSpecs;
                double secondaryFluxLinkage = fluxLinkageCore;
                double averageSecondaryMutualFluxDensity = secondaryFluxLinkage
                    / (basicCalculationsFlybackSpecs.N_s * coreSpecs.A_e);
                double secondaryInductance = secondaryFluxLinkage / basicCalculationsFlybackSpecs.I_s_peak;
                double secondaryLeakageInductange = (fluxLinkageSurroundings + fluxLinkageWindings )
                    / basicCalculationsFlybackSpecs.I_s_peak;
                double totalFluxDensity =(fluxLinkageCore+fluxLinkageWindings+fluxLinkageSurroundings)
                    / (basicCalculationsFlybackSpecs.N_s * coreSpecs.A_e);
            string[] resultLines = new string[] {
                    $"Average secondary mutual flux density: {averageSecondaryMutualFluxDensity}. This is proportion {averageSecondaryMutualFluxDensity/basicCalculationsFlybackSpecs.B_max} the expected value of {basicCalculationsFlybackSpecs.B_max}.",
                    $"Flux linkage core: {fluxLinkageCore} for area: {fluxLinkageCoreArea}",
                    hasFluxMeasurementBoundary?$"Flux linkage windings: {fluxLinkageWindings} for area: {fluxLinkageWindingsArea}":"No windings flux measurement boundary was included",
                    $"Total flux linkage: {secondaryFluxLinkage}. This is proportion {fluxLinkageCore/basicCalculationsFlybackSpecs.ExpectedFluxLinkageSecondary} the expected value of: {basicCalculationsFlybackSpecs.ExpectedFluxLinkageSecondary}",
                    $"Secondary inductance: {secondaryInductance}. This is proportion {secondaryInductance / basicCalculationsFlybackSpecs.L_s} of the expected value of: {basicCalculationsFlybackSpecs.L_s}",
                    $"Flux linkage surroundings: {fluxLinkageSurroundings} for area: {fluxLinkageSurroundingsArea}",
                    $"Secondary leakage inductance: {secondaryLeakageInductange}",
                    $"Total flux density was : {totalFluxDensity} which is {totalFluxDensity/basicCalculationsFlybackSpecs.B_max} the expected flux density of {basicCalculationsFlybackSpecs.B_max}",
                    $"Proportion of total inductance leaked: {secondaryLeakageInductange/(secondaryLeakageInductange+secondaryInductance)}",
                    $"At I_peak the magnetic flux density is {totalFluxDensity/coreSpecs.B_sat} the saturation magnetic flux density"
                };
                foreach (string resultLine in resultLines)
                    Console.WriteLine(resultLine);
                File.WriteAllLines(paths.ResultPlyFilePath, resultLines);
                //CloudCompareHelper.Open(paths.MagneticFieldPlyFilePath);
                femResultFlybackSpecs = new FEMResultFlybackSpecs(
                    L_ms:secondaryInductance, 
                    L_ks:secondaryLeakageInductange);
                return basicCalculationsFlybackSpecs;
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
            fluxMeasurementBoundaryWinding = hasFluxMeasurementBoundaryWinding?new MeasurementBoundary(FLUX_MEASUREMENT_BOUNDARY_WINDING):null;
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
                            CORE_VOLUME_ONE, coreSpecs.RelativePermeability*Permeabilities.FREE_SPACE
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
                            AIR_GAP_VOLUME, coreSpecs.RelativePermeability*Permeabilities.FREE_SPACE
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
    }
}