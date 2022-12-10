#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>

const std::string UPDATE_MIRROR = "mymirror.web.tld/version/new_version_folder/";
const std::string LOCAL_FILES_FOLDER = "./files/";

// function to check if a file exists
bool fileExists(const std::string& fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

// function to download a file from the update mirror
void downloadFile(const std::string& fileName)
{
    std::string filePath = UPDATE_MIRROR + fileName;
    std::string command = "curl -o " + LOCAL_FILES_FOLDER + fileName + " " + filePath;
    std::system(command.c_str());
}

// function to check if a file has been updated on the update mirror
bool isUpdated(const std::string& fileName)
{
    // get the local version of the file
    std::string localFilePath = LOCAL_FILES_FOLDER + fileName;
    std::ifstream localFile(localFilePath);
    std::string localFileContents((std::istreambuf_iterator<char>(localFile)),
                                  std::istreambuf_iterator<char>());

    // get the version of the file on the update mirror
    std::string mirrorFilePath = UPDATE_MIRROR + fileName;
    std::stringstream mirrorFileContents;
    std::string line;
    std::system(("curl " + mirrorFilePath + " >> mirrorFile.txt").c_str());
    std::ifstream mirrorFile("mirrorFile.txt");
    while (std::getline(mirrorFile, line))
    {
        mirrorFileContents << line << std::endl;
    }

    // compare the two files and return the result
    return localFileContents != mirrorFileContents.str();
}

int main()
{
    // list of files that the application uses
    std::vector<std::string> files = {"main.cpp", "functions.h", "functions.cpp"};

    // check each file and download it if it has been updated
    for (const auto& file : files)
    {
        if (fileExists(LOCAL_FILES_FOLDER + file))
        {
            if (isUpdated(file))
            {
                downloadFile(file);
            }
        }
        else
        {
            downloadFile(file);
        }
    }

    // continue with the rest of the application
    std::cout << "Application is running with the latest updates!" << std::endl;

    return 0;
}
