using CoffeeCup.Core;

namespace CoffeeCup.Scripts
{
    class GenerateProjectsScript : Script
    {
        public override void Run()
        {
            ScriptUtil.Run(@"tools/SharpMake/bin/Sharpmake.Application.exe /sources(@""./tools/BuildScripts/main.sharpmake.cs"")");
        }
    }
}
