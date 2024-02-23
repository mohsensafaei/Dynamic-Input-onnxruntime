#include<onnxruntime_cxx_api.h>
#include <chrono>
#include <cmath>
#include <exception>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

std::vector<std::vector<int64_t>> m_inputShapes;  // a vector for save shape of inputs
std::vector<std::vector<int64_t>> m_outputShapes; // a vector for save shape of outputs

std::vector<int64_t> m_inputTensorSizes;
std::vector<int64_t> m_outputTensorSizes;

uint8_t m_numInputs; // number of inputs
uint8_t m_numOutputs;// number of outputs

std::vector<char*> m_inputNodeNames; // a vector for save name of inputs
std::vector<char*> m_outputNodeNames;// a vector for save name of outputs

std::vector<float*> outputData;
std::vector<Ort::Value> outputTensorss;


void run_onnx_model(std::string modelFilepath){

    // initialize onnxruntime environment
    std::string instanceName{"image-mmdeploy-inference"};
    Ort::Env m_env(OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING,
                   instanceName.c_str());
    Ort::SessionOptions sessionOptions;
    sessionOptions.SetIntraOpNumThreads(1);

    sessionOptions.SetGraphOptimizationLevel(
                GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

    //create session
    Ort::Session session(m_env, modelFilepath.c_str(), sessionOptions);

    //create allocator
    Ort::AllocatorWithDefaultOptions allocator;

    // get number of output and inputs
    int m_numInputs = session.GetInputCount();
    int m_numOutputs = session.GetOutputCount();

    // resize vectors based on number of input and outputs
    m_inputNodeNames.reserve(m_numInputs);
    m_inputTensorSizes.reserve(m_numInputs);
    m_outputNodeNames.reserve(m_numOutputs);
    m_outputTensorSizes.reserve(m_numOutputs);
     // Example input shapes to be added to m_inputShapes
    std::vector<int64_t> inputshape_test_1{input_shape[0]};
    std::vector<int64_t> inputshape_test_2{input_shape[0],64,4};
    m_inputShapes.emplace_back(inputshape_test_1);
    m_inputShapes.emplace_back(inputshape_test_2);
    // get the shapes and tensor size of inputs
    for (int i = 0; i < m_numInputs; i++) {

        Ort::TypeInfo typeInfo = session.GetInputTypeInfo(i);
        auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
        // m_inputShapes.emplace_back(tensorInfo.GetShape());

        // std::cout << tensorInfo.GetShape() << std::endl;

        const auto& curInputShape = m_inputShapes[i];
        std::cout << "Dimension of input" << m_inputShapes[i] <<std::endl;
        m_inputTensorSizes.emplace_back(
                    std::accumulate(std::begin(curInputShape), std::end(curInputShape), 1, std::multiplies<int64_t>()));

        char* inputName = session.GetInputName(i, allocator);
        m_inputNodeNames.emplace_back(strdup(inputName));
        allocator.Free(inputName);
    }

    // get the shapes and tensor size of inputs and outputs
    for (int i = 0; i < m_numOutputs; ++i) {
        Ort::TypeInfo typeInfo = session.GetOutputTypeInfo(i);
        auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();

        m_outputShapes.emplace_back(tensorInfo.GetShape());

        char* outputName = session.GetOutputName(i, allocator);
        m_outputNodeNames.emplace_back(strdup(outputName));
        allocator.Free(outputName);
    }


    Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

    // create a vector of type ort::value for onnx_runtime
    std::vector<Ort::Value> inputTensors;
    inputTensors.reserve(m_numInputs);    // resize vector based on number of inputs
    // push back our input vectors to inputTensors vector
    inputTensors.emplace_back(std::move(
                                  Ort::Value::CreateTensor<int32_t>(memoryInfo, const_cast<int32_t*>(num_points_data32.data()), m_inputTensorSizes[0],
                                  m_inputShapes[0].data(), m_inputShapes[0].size())));
    inputTensors.emplace_back(std::move(
                                  Ort::Value::CreateTensor<float>(memoryInfo, const_cast<float*>(voxels_data.data()), m_inputTensorSizes[1],
                                  m_inputShapes[1].data(), m_inputShapes[1].size())));


    // create a vector of type ort::value for onnx_runtime output
    std::vector<Ort::Value> outputTensors;
    outputTensors.reserve(m_numOutputs);
  
    outputTensorss = session.Run(Ort::RunOptions{nullptr}, m_inputNodeNames.data(), inputTensors.data(),
                                      m_numInputs, m_outputNodeNames.data(), m_numOutputs);

    // get the pointer vector to outputs
    outputData.reserve(m_numOutputs);
    for (int i=0; i<m_numOutputs; i++){
        outputData.push_back(outputTensorss[i].GetTensorMutableData<float>());
        // print information of output matrix
        auto outputInfo = outputTensorss[i].GetTensorTypeAndShapeInfo();
        std::cout << "onnx model ouputs specifications: " << "\n";
        std::cout << "GetElementType: " << outputInfo.GetElementType() << "-- number of elements: " << outputInfo.GetElementCount() << "\n";
        std::cout << "Dimensions of the output: " << outputInfo.GetShape().size() << "\n";
        std::cout << "Shape of the output: ";
        for (unsigned int shapeI = 0; shapeI < outputInfo.GetShape().size(); shapeI++)
            std::cout << outputInfo.GetShape()[shapeI] << ", ";
        std::cout << "\n";

        // std::vector<float> temp = getVector(outputData.at(i), outputInfo.GetElementCount());
        // writeVector(temp, i);

    }




}
