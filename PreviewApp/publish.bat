mkdir publish

dotnet publish -c Release -r win-x64 --no-self-contained /p:PublishSingleFile=true

move /y .\bin\Release\netcoreapp3.1\win-x64\publish\FastNoiseLiteGUI.exe .\publish\FastNoiseLiteGUI-Win64.exe

dotnet publish -c Release -r linux-x64 --no-self-contained /p:PublishSingleFile=true

move /y .\bin\Release\netcoreapp3.1\linux-x64\publish\FastNoiseLiteGUI .\publish\FastNoiseLiteGUI-Linux64

dotnet publish -c Release -r osx-x64 --no-self-contained 

tar.exe -a -c -f .\publish\FastNoiseLiteGUI-OSX64.zip -C .\bin\Release\netcoreapp3.1\osx-x64\publish .\FastNoiseLiteGUI.app

pause