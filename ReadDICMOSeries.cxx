#include "itkImage.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageToVTKImageFilter.h"

#include <string>
#include <vector>

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " DicomDirectory" << std::endl;
		return EXIT_FAILURE;
	}

	using PixelType = signed short;
	constexpr unsigned int Dimension = 3; // The dimension is 3, not 2

	using ImageType = itk::Image<PixelType, Dimension>;

	using ImageIOType = itk::GDCMImageIO;
	ImageIOType::Pointer dicomIO = ImageIOType::New();

	using NamesGeneratorType = itk::GDCMSeriesFileNames;
	NamesGeneratorType::Pointer nameGenarator = NamesGeneratorType::New();
	nameGenarator->SetDirectory(argv[1]);

	using FilenamesContainer = std::vector<std::string>;
	FilenamesContainer filenames = nameGenarator->GetInputFileNames();

	using ReaderType = itk::ImageSeriesReader<ImageType>;
	ReaderType::Pointer reader = ReaderType::New();
	reader->SetImageIO(dicomIO);
	reader->SetFileNames(filenames);
	try
	{
		reader->Update();
	}
	catch (itk::ExceptionObject& ex)
	{
		std::cout << ex << std::endl;
		return EXIT_FAILURE;
	}

	char name[512];
	dicomIO->GetPatientName(name);
	std::cout << "patient: " << name << std::endl;

	using Dictionary = itk::MetaDataDictionary;
	using MetaDataStringType = itk::MetaDataObject<std::string>;
	const Dictionary& dic = dicomIO->GetMetaDataDictionary();
	auto itr = dic.Begin();
	auto end = dic.End();
	std::string entryId = "0010|0010";
	auto tagItr = dic.Find(entryId);
	if (tagItr != end)
	{
		MetaDataStringType::ConstPointer entryvalue =
			dynamic_cast<const MetaDataStringType*> (tagItr->second.GetPointer());
		if (entryvalue)
		{
			std::string tagvalue = entryvalue->GetMetaDataObjectValue();
			std::cout << "patient: " << tagvalue << std::endl;
		}
	}

	using ImageConnector = itk::ImageToVTKImageFilter<ImageType>;
	ImageConnector::Pointer imageConnector = ImageConnector::New();
	imageConnector->SetInput(reader->GetOutput());
	imageConnector->Update();

	return EXIT_SUCCESS;
}