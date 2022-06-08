#include <stdio.h>
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/tools/gen_op_registration.h"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include <iostream>

#define TFLITE_MINIMAL_CHECK(x) \
  if(!(x)) {                                                    \
    fprintf(stderr, "Error at %s:%d\n",  __FILE__, __LINE__); \
    exit(1); \
  }

void ProcessInputWithFloatModel(uint8_t* input, float* buffer);
cv::Mat ProcessOutputWithFloatModel(float* input);
const int wanted_input_width = 256;
const int wanted_input_height = 256;
const int wanted_input_channels = 3;

int main(){
    std::unique_ptr<tflite::FlatBufferModel> encoder_model = tflite::FlatBufferModel::BuildFromFile("reveal_model.tflite");
    if(!encoder_model){
        printf("Failed to mmap encoder_model\n");
        exit(0);
    }
    tflite::ops::builtin::BuiltinOpResolver resolver;
    std::unique_ptr<tflite::Interpreter> interpreter;
    tflite::InterpreterBuilder(*encoder_model.get(), resolver)(&interpreter);

    // Resize input tensors, if desired.
    //interpreter->AllocateTensors();
    TFLITE_MINIMAL_CHECK(interpreter->AllocateTensors() == kTfLiteOk);
    const char* imagefile = "output_img.jpg"; //"1.jpg";
    cv::Mat image = cv::imread(imagefile);
    //cv::imshow("Input",imagefile);
    cv::waitKey(1);

    // write to input
    int input = interpreter->inputs()[0];
    float* out = interpreter->typed_tensor<float>(0); //input
    uint8_t* in = image.ptr<uint8_t>(0);
    ProcessInputWithFloatModel(in, out);

    // run interpreter
    if (interpreter->Invoke() != kTfLiteOk) {
        std::cout << "error!!!" << std::endl;
    }
    // get output      
    int output_idx = interpreter->outputs()[0];
    float* output = interpreter->typed_output_tensor<float>(0); //output_idx
    cv::Mat outputMat = ProcessOutputWithFloatModel(output);
    cv::imwrite("Extract_QR.png", outputMat);
        return 0;
}

/// Preprocess the input image and feed the TFLite interpreter buffer for a float model.
void ProcessInputWithFloatModel(uint8_t* input, float* buffer) {
  for (int y = 0; y < wanted_input_height; ++y) {
    float* out_row = buffer + (y * wanted_input_width * wanted_input_channels);
    for (int x = 0; x < wanted_input_width; ++x) {
      uint8_t* input_pixel = input + (y * wanted_input_width * wanted_input_channels) + (x * wanted_input_channels);
      float* out_pixel = out_row + (x * wanted_input_channels);
      for (int c = 0; c < wanted_input_channels; ++c) {
        out_pixel[c] = input_pixel[c] / 255.0f;
      }
    }
  }
}

cv::Mat ProcessOutputWithFloatModel(float* input) {
  cv::Mat image = cv::Mat::zeros(wanted_input_height, wanted_input_width, CV_8UC3);
  for (int y = 0; y < wanted_input_height; ++y) {
    for (int x = 0; x < wanted_input_width; ++x) {
      float* input_pixel = input + (y * wanted_input_width * wanted_input_channels) + (x * wanted_input_channels);
      cv::Vec3b & color = image.at<cv::Vec3b>(cv::Point(x, y));
      float digit;

      digit = floor(input_pixel[0] * 255.0f);
      if (digit < 0.0) digit = 1.0;
      if (digit > 254.0) digit = 254.0;
      color[0] = (uchar) digit;

      digit = floor(input_pixel[1] * 255.0f);
      if (digit < 0.0) digit = 1.0;
      if (digit > 254.0) digit = 254.0;
      color[1] = (uchar) digit;

      digit = floor(input_pixel[2] * 255.0f);
      if (digit < 0.0) digit = 1.0;
      if (digit > 254.0) digit = 254.0;
      color[2] = (uchar) digit;
    }
  }
  return image;
}