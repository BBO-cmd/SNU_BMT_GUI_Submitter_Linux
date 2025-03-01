#ifndef SNU_BMT_INTERFACE_H
#define SNU_BMT_INTERFACE_H

#ifdef _WIN32 //(.dll)
#define EXPORT_SYMBOL __declspec(dllexport)
#else //Linux(.so) and other operating systems
#define EXPORT_SYMBOL
#endif
#include <vector>
#include <iostream>
#include <variant>
#include <cstdint>//To ensure the Submitter side recognizes the uint8_t type in VariantType, this header must be included.

using namespace std;

// Represents the result of the inference process for a single batch.
// Fields:
// - Classification_ImageNet2012_PredictedIndex_0_to_999: An integer representing the predicted class index (0-999) for the ImageNet dataset.
struct EXPORT_SYMBOL BMTReult
{
    int Classification_ImageNet2012_PredictedIndex_0_to_999;
};

// Stores optional system configuration data provided by the Submitter.
// These details will be uploaded to the database along with the performance data.
struct EXPORT_SYMBOL Optional_Data
{
    string CPU_Type;
    string Accelerator_Type;
};


// A variant can store and manage values only from a fixed set of types determined at compile time.
// Since variant manages types statically, it can be used with minimal runtime type-checking overhead.
// std::get<DataType>(variant) checks if the requested type matches the stored type and returns the value if they match.
using VariantType = variant<uint8_t*, int*, float*, // Define variant pointer types
                            vector<uint8_t>, vector<int>, vector<float>>; // Define variant vector types

class EXPORT_SYMBOL SNU_BMT_Interface
{
public:
   virtual ~SNU_BMT_Interface(){}

   // This is not mandatory but can be implemented if needed.
   // The virtual function getOptionalData() returns an Optional_Data object,
   // which includes fields like CPU_Type and Accelerator_Type.
   // By default, these fields are initialized as empty strings.
   virtual Optional_Data getOptionalData()
   {
       Optional_Data data;
       data.CPU_Type = "";
       data.Accelerator_Type = "";
       return data;
   }

   // It is recommended to use this instead of a constructor,
   // as it allows handling additional errors that cannot be managed within the constructor.
   // The Initialize function is guaranteed to be called before convertToData and runInference are executed.
   virtual void Initialize() = 0;

   // Perform preprocessing and convert to the desired data type required by the AI Processing Unit.
   virtual VariantType convertToData(const string& imagePath) = 0;

   // Returns the final BMTResult value of the batch required for performance evaluation in the App.
   virtual vector<BMTReult> runInference(const vector<VariantType>& data) = 0;
};

#endif // SNU_BMT_INTERFACE_H


