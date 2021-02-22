dotnet publish -o .\publish -c Release -r win-x64 --no-self-contained /p:PublishSingleFile=true /p:AssemblyName=FastNoiseLiteGUI-Win64

dotnet publish -o .\publish -c Release -r linux-x64 --no-self-contained /p:PublishSingleFile=true /p:AssemblyName=FastNoiseLiteGUI-Linux64

dotnet publish -o .\publish -c Release -r osx-x64 --no-self-contained /p:MacBundleDotNet=False /p:PublishSingleFile=true /p:AssemblyName=FastNoiseLiteGUI-OSX64

pause