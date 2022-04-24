/**
 * @author Seongchang Park (scsc1125@gmail.com)
 * @date 2022-04-24 10:45
 */
#include <iostream>

#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>

#include <PlyFile.h>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    cout << "[ERROR] Specify the input ply folder" << endl;
    return 0;
  }
  string folder_name = argv[1];
  cout << "input folder_name is [" <<  folder_name << "]" << endl;
  std::string dir_name = "../ct_icp/";
  std::string dir_name_output = "../ct_icp_test/";

  dir_name += folder_name + "/";
  dir_name_output += folder_name + "/";

  string temp_path = "";
  float timestamp = 0.0;
  for (const fs::directory_entry& entry : fs::directory_iterator(dir_name))
  {
    string ply_name = "";
    temp_path = entry.path();
    cout << temp_path << endl;

    int idx = temp_path.find(".ply");
    for (int j(10); j > -4; j--) ply_name += temp_path[idx - j];
    cout << "ply_name: " << ply_name << endl;
  
    string path_input = dir_name + ply_name;
    string path_output = dir_name_output + ply_name;

    cout << "   [READ]" << endl;
    // Read from ply frame file
    /**
     * @param path_input *.ply file path 
     * @param fileOpenMode_IN ios::in | ios::binary
     */
    PlyFile plyFileIn(path_input, fileOpenMode_IN);
    char *dataIn = nullptr;
    int sizeOfPointsIn = 0;
    int numPointsIn = 0;

    plyFileIn.readFile(dataIn, sizeOfPointsIn, numPointsIn);
    cout << "input_header: " << plyFileIn._header << endl;
    cout << "sizeOfPointsIn: " << sizeOfPointsIn << endl;
    cout << "numPointsIn: " << numPointsIn << endl;
    plyFileIn.displayInfos();

    cout << "\n===========          =========\n" << endl;

    // For write custom ply file
    PlyFile plyFileOut(path_output, fileOpenMode_OUT);
    plyFileOut.timestamp_ = timestamp;
    std::string headerOut = plyFileIn._header;
    // plyFileOut._header = headerOut;
    
    int sizeOfPointsOut = 16; 
    int numPointsOut = plyFileIn._numPoints;
    char *dataOut = new char[sizeOfPointsOut * numPointsOut]; // buffer = sizeOfPointsOut * numPointsOut
    memset(dataOut, 0, sizeOfPointsOut * numPointsOut);

    list<string> properties;
    list<plyTypes> types; // 0 for float32

    properties.push_back("x");
    properties.push_back("y");
    properties.push_back("z");
    properties.push_back("timestamp");
    const int numProperties = properties.size();

    types.push_back(float32);
    types.push_back(float32);
    types.push_back(float32);
    types.push_back(float32);

    cout << "   [Write]" << endl;

    // Generate Points
    plyFileOut.generatePoints(dataOut, sizeOfPointsOut, numPointsOut, numProperties, dataIn);

    // Write
    plyFileOut.writeFile(dataOut, sizeOfPointsOut, numPointsOut, properties, types);
    plyFileOut.displayInfos();
    
    delete[] dataOut;
  }
  return 0;
}