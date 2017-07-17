#include <Core/Core.hpp>
#include <Core/File/FileManager.hpp>
#include <Core/Process/ArgumentParser.hpp>
#include <Core/Stream/FileWriter.hpp>
#include <iostream>

#define VERSION				"1.0.1"
#define ARGUMENT_INPUT		"--input"
#define ARGUMENT_OUTPUT		"--output"
#define UNITY_MAKE_MAX_FILE	100

using namespace Gorilla;

//!	@brief		Entry
//!	@date		2017-07-16
int main(int argc, const char** argv) 
{
	ArgumentParser kParser("Unity Make", "Unity Make generate all files needed for an Unity Build", VERSION);
	kParser.Add(ARGUMENT_INPUT, "Define the path the the C++ source files", true);
	kParser.Add(ARGUMENT_OUTPUT, "Define the path where the ", true);

	String sUsage;
	if(!kParser.Parse(argc, argv, &sUsage))
	{
		printf(sUsage.GetBuffer());
		return -1;
	}

	// Retrieve argument
	String sInput = kParser.Get<String>(ARGUMENT_INPUT);
	String sOutput = kParser.Get<String>(ARGUMENT_OUTPUT);
	Path::Format(sInput);
	Path::Format(sOutput);

	// Check input directory
	if(!FileManager::IsDirectoryExist(sInput.GetBuffer()))
	{
		printf("Input directory %s doesn't exist\n", sInput.GetBuffer());
		return -1;
	}

	// Create output path
	String sUnityPath;
	sUnityPath.Set(sOutput).Append("_unity_files_").Append("\\");
	FileManager::CreateADirectory(sUnityPath.GetBuffer());

	// Get the number of unity file already created
	Vector<String> vFile;
	FileManager::GetAllFiles(sUnityPath.GetBuffer(), vFile, false);
	uint32 uiUnityIndex = vFile.GetSize();

	// Retieve all c/c++ file
	vFile.Clear();
	FileManager::GetAllFiles(sInput.GetBuffer(), vFile, true, "c");
	const uint32 uiFileCount = vFile.GetSize();
	if(uiFileCount)
	{
		FileWriter kFileWriter;
		for(uint32 uiFile = 0; uiFile < uiFileCount; ++uiFile)
		{
			// Create new unity file
			if(uiFile >= UNITY_MAKE_MAX_FILE) kFileWriter.Close();
			if(!kFileWriter.CanWrite())
			{
				String sUnityFile;
				sUnityFile.Set(sUnityPath).Append("unity_").Append(++uiUnityIndex).Append(".cpp");
				kFileWriter.Open(sUnityFile.GetBuffer());
			}

			// Write the file in the unity file
			static String sInclude("#include \"");
			String& sFile = vFile[uiFile];
			kFileWriter.Write(sInclude.GetBuffer(), sInclude.GetLength());
			kFileWriter.Write(sFile.GetBuffer(), sFile.GetLength());
			kFileWriter.Write("\"\n", 2);
		}
	}

	return 0;
}
