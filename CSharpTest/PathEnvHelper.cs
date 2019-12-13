using System.Reflection;
using System;
using System.IO;
using System.Linq;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using System.Diagnostics;

namespace cs_cntk_core
{
    public enum BinaryKind { Debug, Release }

    public class PathEnvHelper : IDisposable
    {
        public const string PATH = "PATH";
        public readonly string[] originalPathEnvContent;

        public List<string> GetCurrentPaths() =>
            Environment
                .GetEnvironmentVariable(PATH, EnvironmentVariableTarget.Process)
                .Split(Path.PathSeparator, StringSplitOptions.RemoveEmptyEntries)
                .ToList();

        public PathEnvHelper()
        {
            originalPathEnvContent =
                Environment
                    .GetEnvironmentVariable(PATH, EnvironmentVariableTarget.Process)
                    .Split(Path.PathSeparator);
        }

        public void Dispose()
        {
            Environment.SetEnvironmentVariable(
                PATH,
                string.Join(Path.PathSeparator, originalPathEnvContent),
                EnvironmentVariableTarget.Process);
        }

        public void AddPaths(params string[] paths)
        {
            var missingPath = paths.FirstOrDefault(path => !Directory.Exists(path));

            if (missingPath != null)
                throw new Exception($"Folder '{missingPath}' not found, won't add to env:PATH");

            var current_paths = GetCurrentPaths();
            current_paths.AddRange(paths);

            Environment.SetEnvironmentVariable(
                PATH,
                string.Join(Path.PathSeparator, current_paths.Distinct()),
                EnvironmentVariableTarget.Process);
        }
    }
}