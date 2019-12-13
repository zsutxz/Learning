using NUnit.Framework;
using System.Reflection;
using System;
using System.IO;
using System.Linq;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using System.Diagnostics;

namespace cs_cntk_core
{
    public class CntkReferencePathInfo
    {
        public static bool IsWindows =
            System.Runtime.InteropServices.RuntimeInformation.OSDescription.Contains("Windows");

        public static string CntkCoreManagedPath =
            Path.GetDirectoryName(
                Assembly
                    .GetAssembly(typeof(CNTK.CNTKLib))
                    .CodeBase
                    .Replace(IsWindows ? "file:///" : "file://", ""));

        public static string CntkVersion =
            CntkCoreManagedPath
                .Split(new[] { '/', '\\' })
                .Reverse()
                .SkipWhile(dir => !Regex.IsMatch(dir, @"^\d\.\d(\.\d)?$"))
                .First()
                .Replace("\\", "")
                .Replace("/", "");

        public static string PackagesPath =
            CntkCoreManagedPath
                .Split(new[] { '/', '\\' })
                .Reverse()
                .SkipWhile(dir => !dir.ToLower().Contains("cntk."))
                .Skip(1)
                .Reverse()
                .Aggregate((state, current) => $"{state}{Path.DirectorySeparatorChar}{current}");

        public static string ManagedSupportPath(BinaryKind kind) =>
            Path.GetFullPath(
                Path.Combine(
                    CntkCoreManagedPath,
                    $"../../support/x64/{(kind == BinaryKind.Debug ? "Debug" : "Release")}"));


        public static IEnumerable<string> CudaPaths =
            Directory
                .GetDirectories(PackagesPath, "cntk.deps.cu*")
                .SelectMany(dir => Directory.GetDirectories(Path.Combine(dir, $"{CntkVersion}/support/x64"))
                .Select(Path.GetFullPath));

    }
}