using Sharpmake;
using System;
using System.Collections.Generic;
using System.IO;

[Generate]
public class MainProject : Project
{
    private string m_Root = @"[project.SharpmakeCsPath]\..\..\..";
    private const string mc_ProjectName = "dagger";

    public MainProject()
    {
        Name = mc_ProjectName;
        SourceRootPath = Path.Combine(m_Root, @"source", mc_ProjectName);
        
        AddTargets(new Target(
            Platform.win64,
            DevEnv.vs2019,
            Optimization.Debug | Optimization.Release
        ));
    }

    [Configure]
    public void ConfigureAll(Project.Configuration config, Target target)
    {
        config.ProjectPath = Path.Combine(m_Root, @"projects", mc_ProjectName);

        config.VcxprojUserFile = new Configuration.VcxprojUserFileSettings();
        config.VcxprojUserFile.LocalDebuggerWorkingDirectory = Path.Combine(m_Root, @"data");

        // Additional includes
        config.IncludePaths.Add(Path.Combine(m_Root, @"source", mc_ProjectName));
        config.IncludePaths.Add(Path.Combine(m_Root, @"libs", @"spdlog", @"include"));
        config.IncludePaths.Add(Path.Combine(m_Root, @"libs", @"stb", @"include"));
        config.IncludePaths.Add(Path.Combine(m_Root, @"libs", @"entt", @"single_include"));
        config.IncludePaths.Add(Path.Combine(m_Root, @"libs", @"fmt", @"include"));
        config.IncludePaths.Add(Path.Combine(m_Root, @"libs", @"glfw3", @"include"));
        config.IncludePaths.Add(Path.Combine(m_Root, @"libs", @"json", @"include"));
        config.IncludePaths.Add(Path.Combine(m_Root, @"libs", @"sparse-map", @"include"));
        config.IncludePaths.Add(Path.Combine(m_Root, @"libs", @"glad", @"include"));
        config.IncludePaths.Add(Path.Combine(m_Root, @"libs", @"imgui", @"include"));
        config.IncludePaths.Add(Path.Combine(m_Root, @"libs", @"imgui", @"include", @"imgui"));
        config.IncludePaths.Add(Path.Combine(m_Root, @"libs", @"imgui", @"include", @"imgui", @"backends"));
        config.IncludePaths.Add(Path.Combine(m_Root, @"libs", @"glm", @"include"));

        // Add libs
        config.LibraryPaths.Add(Path.Combine(m_Root, @"libs", "glfw3-lib"));
        config.LibraryFiles.Add("glfw3.lib");
        config.LibraryFiles.Add("glu32.lib");
        config.LibraryFiles.Add("opengl32.lib");

        // For external CPP files
        string external = Path.Combine(m_Root, @"..\source", mc_ProjectName, @"external");
        if (!Directory.Exists(external))
        {
            Directory.CreateDirectory(external);
        }

        // IMGUI
        string imguiRoot = Path.Combine(m_Root, @"..\libs", @"imgui", @"include", @"imgui");
        string imguiDest = Path.Combine(external, @"imgui");

        if(!Directory.Exists(imguiDest))
        {
            Directory.CreateDirectory(imguiDest);
        }

        File.Copy(Path.Combine(imguiRoot, @"imgui.cpp"), Path.Combine(imguiDest, @"imgui.cpp"), true);
        File.Copy(Path.Combine(imguiRoot, @"imgui_widgets.cpp"), Path.Combine(imguiDest, @"imgui_widgets.cpp"), true);
        File.Copy(Path.Combine(imguiRoot, @"imgui_draw.cpp"), Path.Combine(imguiDest, @"imgui_draw.cpp"), true);
        File.Copy(Path.Combine(imguiRoot, @"backends", @"imgui_impl_glfw.cpp"), Path.Combine(imguiDest, @"imgui_impl_glfw.cpp"), true);
        File.Copy(Path.Combine(imguiRoot, @"backends", @"imgui_impl_opengl3.cpp"), Path.Combine(imguiDest, @"imgui_impl_opengl3.cpp"), true);

        // GLAD
        string gladRoot = Path.Combine(m_Root, @"..\libs", @"glad", @"include", @"glad");
        string gladDest = Path.Combine(external, @"glad");

        if (!Directory.Exists(gladDest))
        {
            Directory.CreateDirectory(gladDest);
        }

        File.Copy(Path.Combine(gladRoot, @"glad.c"), Path.Combine(gladDest, @"glad.c"), true);

        // Setup additional compiler options
        config.TargetPath = Path.Combine(m_Root, @"bin", config.Platform.ToString());
        
        config.Options.Add(Options.Vc.Compiler.CppLanguageStandard.CPP17);
        config.Options.Add(Options.Vc.Compiler.RuntimeLibrary.MultiThreadedDebugDLL);
    }

    public override void PostResolve()
    {
        base.PostResolve();
    }
}

[Generate]
public class MainSolution : Solution
{
    private const string mc_SolutionName = "Dagger";
    private string m_Root = Path.Combine(@"[project.SharpmakeCsPath]", @"\..\..\..");

    public MainSolution()
    {
        Name = mc_SolutionName;
        
        AddTargets(new Target(
            Platform.win64,
            DevEnv.vs2019,
            Optimization.Debug | Optimization.Release
        ));
    }

    [Configure]
    public void ConfigureAll(Solution.Configuration config, Target target)
    {
        config.SolutionPath = Path.Combine(m_Root, "projects");
        config.AddProject<MainProject>(target);
    }
}

public static class Main
{
    [Sharpmake.Main]
    public static void SharpmakeMain(Arguments sharpmakeArgs)
    {
        KitsRootPaths.SetUseKitsRootForDevEnv(DevEnv.vs2019, KitsRootEnum.KitsRoot10, Options.Vc.General.WindowsTargetPlatformVersion.Latest);
        sharpmakeArgs.Generate<MainSolution>();
    }
}
