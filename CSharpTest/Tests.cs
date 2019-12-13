using NUnit.Framework;
using System.Reflection;
using System;
using System.IO;
using System.Linq;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using System.Diagnostics;
using cs_cntk_core;

namespace Tests
{
    public class cs_cntk_core
    {

        // static void ResolveDepPaths()
        // {
        //     Console.WriteLine(System.Runtime.InteropServices.RuntimeInformation.OSDescription);

        //     var cntkPath = Assembly.GetAssembly(typeof(CNTK.CNTKLib)).CodeBase.Replace("file:///", "");
        //     var supportPath = Path.GetFullPath(Path.Combine(cntkPath, "../../../Support/x64/Release"));

        //     Console.WriteLine(supportPath, cntkPath);
        //     var baseIndex = cntkPath.Split('/').ToList().FindIndex(dir => dir.ToLower().Contains("cntk."));

        //     Console.WriteLine(baseIndex.ToString());

        //     var cudaPaths = cntkPath
        //         .Split("/")
        //         .Take(baseIndex)
        //         .Aggregate((state, current) => $"{state}/{current}")
        //         .Pipe(path => Directory
        //                 .GetDirectories(path, "cntk.deps.cu*")
        //                 .SelectMany(dir => Directory.GetDirectories(Path.Combine(dir, "2.6.0/support/x64"))
        //                 .Select(Path.GetFullPath)));

        //     Console.WriteLine(supportPath);

        //     var paths = Environment.GetEnvironmentVariable("PATH").Split(';').ToList();
        //     paths.Add(Path.GetDirectoryName(cntkPath));
        //     paths.Add(supportPath);
        //     paths.AddRange(cudaPaths);

        //     Environment.SetEnvironmentVariable("PATH", string.Join(';', paths));

        //     Console.WriteLine("Paths");
        //     Environment.GetEnvironmentVariable("PATH").Split(";").ToList().ForEach(Console.WriteLine);
        // }

        [SetUp]
        public void Setup()
        {
          
        }

        public PathEnvHelper helper;

        public void CntkTest(PathEnvHelper envHelper)
        {
            Console.WriteLine(Environment.GetEnvironmentVariable("PATH", EnvironmentVariableTarget.Process));

            try
            {
                var cpu = CNTK.DeviceDescriptor.UseDefaultDevice();
                Debug.WriteLine($"You are using CNTK for: {cpu.Type}");
            }
            catch (Exception ex)
            {
                var exx = new System.Exception(
                    ex.Message + Environment.NewLine +
                    ex.InnerException?.Message + Environment.NewLine +
                    string.Join(Environment.NewLine, envHelper.GetCurrentPaths().TakeLast(5)));

                throw exx;
            }
        }

        [Test]
        public void No_meddling_with_the_path_env_var()
        {
            var cpu = CNTK.DeviceDescriptor.UseDefaultDevice();
            Debug.WriteLine($"You are using CNTK for: {cpu.Type}");
            Assert.Pass();
        }

        [Test]
        public void Only_managed_path()
        {
            using (var envHelper = new PathEnvHelper())
            {
                envHelper.AddPaths(CntkReferencePathInfo.CntkCoreManagedPath);
                CntkTest(envHelper);
            }
        }

        [TestCase(BinaryKind.Debug)]
        [TestCase(BinaryKind.Release)]
        public void Μanaged_path_and_support(BinaryKind kind)
        {
            using (var envHelper = new PathEnvHelper())
            {
                envHelper.AddPaths(CntkReferencePathInfo.CntkCoreManagedPath);
                envHelper.AddPaths(CntkReferencePathInfo.ManagedSupportPath(kind));

                CntkTest(envHelper);
            }
        }

        [TestCase(BinaryKind.Debug)]
        [TestCase(BinaryKind.Release)]
        public void Managed_and_support_and_cuda(BinaryKind kind)
        {
            using (var envHelper = new PathEnvHelper())
            {
                envHelper.AddPaths(CntkReferencePathInfo.CntkCoreManagedPath);
                envHelper.AddPaths(CntkReferencePathInfo.ManagedSupportPath(kind));
                envHelper.AddPaths(CntkReferencePathInfo.CudaPaths.ToArray());

                CntkTest(envHelper);
            }
        }

        [TestCase(BinaryKind.Debug)]
        [TestCase(BinaryKind.Release)]
        public void Managed_and_support_and_cuda_globalHelper(BinaryKind kind)
        {
            CntkTest(helper);
        }
    }
}
