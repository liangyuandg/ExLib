/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkMacro.h"

#include "otbScalarImageToTexturesFilter.h"
#include "otbImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "otbStandardFilterWatcher.h"

int main(int argc, char * argv[])
{
  if (argc < 5)
    {
    std::cerr << "Usage: " << argv[0] << " infname outprefix nbBins radius [offsetx offsety offsetz minIntensity maxIntensity]" << std::endl;
    return EXIT_FAILURE;
    }
  const char *       infname      = argv[1];
  const char *       outprefix    = argv[2];
  const unsigned int nbBins       = atoi(argv[3]);
  const unsigned int radius       = atoi(argv[4]);
  int          offsetx      = 0;
  int          offsety      = 0;
  int          offsetz      = 0;

  if( argc > 5 )
    {
    offsetx      = atoi(argv[5]);
    }
  if( argc > 6 )
    {
    offsety      = atoi(argv[6]);
    }
  if( argc > 7 )
    {
    offsetz      = atoi(argv[7]);
    }

  const unsigned int Dimension = 3;
  typedef float                            PixelType;
  // assume images are scaled between [0,1]
  PixelType    minIntensity = 0.;
  PixelType    maxIntensity = 1.;
  if( argc > 8 )
    {
    minIntensity = atof(argv[8]);
    }
  if( argc > 9 )
    {
    maxIntensity = atof(argv[9]);
    }

  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::ScalarImageToTexturesFilter
  <ImageType, ImageType>                        TexturesFilterType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
  typedef itk::ImageFileWriter<ImageType> WriterType;

  ReaderType::Pointer         reader = ReaderType::New();
  TexturesFilterType::Pointer filter = TexturesFilterType::New();
  WriterType::Pointer         writer = WriterType::New();

  // Read image
  reader->SetFileName(infname);

  // Build radius
  TexturesFilterType::SizeType sradius;
  sradius.Fill(radius);

  // Build offset
  TexturesFilterType::OffsetType offset;
  offset[0] = offsetx;
  offset[1] = offsety;
  offset[2] = offsetz;

  filter->SetInput(reader->GetOutput());
  filter->SetRadius(sradius);
  filter->SetOffset(offset);

  otb::StandardFilterWatcher watcher(filter, "Textures filter");

  filter->SetNumberOfBinsPerAxis(nbBins);
  filter->SetInputImageMinimum( minIntensity);
  filter->SetInputImageMaximum( maxIntensity);
  filter->Update();
  //filter->Print(std::cout);

  // Write outputs
  std::ostringstream oss;

  //writer->SetNumberOfDivisionsStrippedStreaming(2);

  oss.str("");
  oss << outprefix << "Energy_" << radius << ".nii.gz";
  writer->SetInput(filter->GetEnergyOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "Entropy_" << radius << ".nii.gz";
  writer->SetInput(filter->GetEntropyOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "Correlation_" << radius << ".nii.gz";
  writer->SetInput(filter->GetCorrelationOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "InverseDifferenceMoment_" << radius << ".nii.gz";
  writer->SetInput(filter->GetInverseDifferenceMomentOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "Inertia_" << radius << ".nii.gz";
  writer->SetInput(filter->GetInertiaOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "ClusterShade_" << radius << ".nii.gz";
  writer->SetInput(filter->GetClusterShadeOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "ClusterProminence_" << radius << ".nii.gz";
  writer->SetInput(filter->GetClusterProminenceOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "HaralickCorrelation_" << radius << ".nii.gz";
  writer->SetInput(filter->GetHaralickCorrelationOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  return EXIT_SUCCESS;
}
