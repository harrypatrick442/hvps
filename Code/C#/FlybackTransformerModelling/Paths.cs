using Core.FileSystem;

namespace VoltageMultiplier.FlybackTransformerModelling
{
    public class Paths
    {
        public string TempDirectory { get; }
        public string OutputDirectory { get; }
        public string CacheFilePath { get; }
        public string TetgenOutputDirectoryPath { get; }
        public string MeshPlyFilePath { get; }
        public string VoltagesFirstHalfPlyFilePath { get; }
        public string CurrentDensitiesFirstHalfPlyFilePath { get; }
        public string VoltagesSecondHalfPlyFilePath { get; }
        public string CurrentDensitiesSecondHalfPlyFilePath { get; }
        public string MagneticFieldPlyFilePath { get; }
        public string PermeabilitiesPlyFilePath { get; }
        public string ResultPlyFilePath { get; }
        public string CircuitSimulationResultsDirectory { get; }
        public string FlybackTransformerModellingResultsDirectory { get; }
        public Paths(string tempDirectoryPath)
        {
            TempDirectory = tempDirectoryPath;
            OutputDirectory = Path.Combine(DirectoryHelper.GetProjectDirectory(), "output");
            CacheFilePath = Path.Combine(tempDirectoryPath, "cached_flyback_transformer");
            TetgenOutputDirectoryPath = Path.Combine(CacheFilePath, "tetgen_output");
            CircuitSimulationResultsDirectory = Path.Combine(OutputDirectory, "circuit_simulation");
            FlybackTransformerModellingResultsDirectory = Path.Combine(OutputDirectory, "flyback_modelling");
            MeshPlyFilePath = Path.Combine(FlybackTransformerModellingResultsDirectory, "mesh.ply");

            VoltagesFirstHalfPlyFilePath = Path.Combine(FlybackTransformerModellingResultsDirectory, "coilVoltagsFirstHalf.ply");
            CurrentDensitiesFirstHalfPlyFilePath = Path.Combine(FlybackTransformerModellingResultsDirectory, "coilCurrentDensitiesFirstHalf.ply");
            VoltagesSecondHalfPlyFilePath = Path.Combine(FlybackTransformerModellingResultsDirectory, "coilVoltagsSecondHalf.ply");
            CurrentDensitiesSecondHalfPlyFilePath = Path.Combine(FlybackTransformerModellingResultsDirectory, "coilCurrentDensitiesSecondHalf.ply");
            MagneticFieldPlyFilePath = Path.Combine(FlybackTransformerModellingResultsDirectory, "magneticField.ply");
            PermeabilitiesPlyFilePath = Path.Combine(FlybackTransformerModellingResultsDirectory, "permeabilities.ply");
            ResultPlyFilePath = Path.Combine(FlybackTransformerModellingResultsDirectory, "result.txt");
        }
    }
}