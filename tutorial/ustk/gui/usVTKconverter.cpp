#include <visp3/ustk_gui/usGuiConfig.h>

#ifdef USTK_HAVE_VTK_QT

#include <vtkMetaImageWriter.h>
#include <vtkImageImport.h>
#include <visp3/ustk_io/usImageIo.h>
#include <visp3/ustk_gui/usVTKConverter.h>

int main(int argc, char** argv)
{
  std::string mhd_filename;

  for (int i=0; i<argc; i++) {
    if (std::string(argv[i]) == "--input")
      mhd_filename = std::string(argv[i+1]);
    else if (std::string(argv[i]) == "--help") {
      std::cout << "\nUsage: " << argv[0] << " [--input <3D volume.mhd>] [--help]\n" << std::endl;
      return 0;
    }
  }

  // Get the ustk-dataset package path or USTK_DATASET_PATH environment variable value
  if (mhd_filename.empty()) {
    std::string env_ipath = us::getDataSetPath();
    if (! env_ipath.empty())
      mhd_filename = env_ipath + "/pre-scan/3D_mhd/volume31.mhd";
    else {
      std::cout << "You should set USTK_DATASET_PATH environment var to access to ustk dataset" << std::endl;
      return 0;
    }
  }

  //read 2 us images to simulate double buffer system for real grabbing

  // 1st us image
  usImagePostScan3D<unsigned char> postScanImage;
  usImageIo::read(postScanImage, mhd_filename);
  // 2nd us image
  usImagePostScan3D<unsigned char> postScanImage2;
  usImageIo::read(postScanImage2, mhd_filename);

  //check addresses of each image
  std::cout << "image 1 @ : " << (void *)postScanImage.getConstData() << std::endl;
  std::cout << "image 2 @ : " << (void *)postScanImage2.getConstData() << std::endl;

  //pre-compute the importer to save time during the data import in vtk
  //this importer will switch between the 2 previsous usImages at each iteration
  vtkSmartPointer<vtkImageData> vtkImage = vtkSmartPointer<vtkImageData>::New();
  vtkImageImport *importer = vtkImageImport::New();
  importer->SetDataScalarTypeToUnsignedChar();
  importer->SetImportVoidPointer((void *)postScanImage.getConstData());
  importer->SetWholeExtent(0,postScanImage.getDimX()-1,0, postScanImage.getDimY()-1, 0, postScanImage.getDimZ()-1);
  importer->SetDataExtentToWholeExtent();
  importer->SetNumberOfScalarComponents(1);

  //trying successive conversions to simulate the double buffer system that should be used to grab and display sucessive volumes
  for(int i=0;i<20;i++) {
    double t1 = vpTime::measureTimeMs();

    if(i%2==0)
      usVTKConverter::convert(postScanImage,vtkImage,importer);
    else
      usVTKConverter::convert(postScanImage2,vtkImage,importer);

    double t2 = vpTime::measureTimeMs();

    std::cout << "vtk convert time (" << i << ") = " << t2-t1 << std::endl;
    std::cout << "vtk image @ : " << vtkImage->GetScalarPointer() << std::endl;
  }
  //picking random voxels to compare pointers
  /*std::cout << "us ptr = " << (void*)postScanImage.getConstData() << std::endl;
  std::cout << "vtk ptr = " << vtkImage->GetScalarPointer(0,0,0) << std::endl;

  unsigned char *ptr = postScanImage.getConstData();
  ptr += (postScanImage.getDimX() * postScanImage.getDimY()) * 10 + postScanImage.getDimX()*10 + 10;
  std::cout << "us ptr (10,10,10) = " << (void*)ptr << std::endl;
  std::cout << "vtk ptr (10,10,10) = " << vtkImage->GetScalarPointer(10,10,10) << std::endl;*/

  // Set the default output path
  std::string username;
#if defined(_WIN32)
  std::string opath = "C:/temp";
#else
  std::string opath = "/tmp";
#endif
/*
  // Get the user login name
  vpIoTools::getUserName(username);

  // Append to the output path string, the login name of the user
  std::string dirname = vpIoTools::createFilePath(opath, username);

  // Test if the output path exist. If no try to create it
  if (vpIoTools::checkDirectory(dirname) == false) {
    vpIoTools::makeDirectory(dirname);
  }
  std::string ofilename = dirname + "/volumeTestWritingVTK.mhd";
  //write mhd using VTK
  vtkSmartPointer<vtkMetaImageWriter> writer = vtkSmartPointer<vtkMetaImageWriter>::New();
  writer->SetFileName(ofilename.c_str());
  writer->SetInputData(vtkImage);
  writer->Write();
*/
  return 0;
}
#else
#include <iostream>

int main()
{
  std::cout << "Install vtk with qt4 or qt5 support to run this tutorial." << std::endl;
}

#endif
