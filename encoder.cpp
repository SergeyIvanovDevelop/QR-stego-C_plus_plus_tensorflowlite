#include <stdio.h>
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/tools/gen_op_registration.h"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include <iostream>

void check_function(cv::Mat& image)
{
    // We iterate over all pixels of the image
    for(int r = 0; r < image.rows; r++) {
        // We obtain a pointer to the beginning of row r
        cv::Vec3b* ptr = image.ptr<cv::Vec3b>(r);

        for(int c = 0; c < image.cols; c++) {
            // We invert the blue and red values of the pixel

        	if (ptr[c][0] > 250) ptr[c][0] = 250;
        	if (ptr[c][1] > 250) ptr[c][1] = 250;
        	if (ptr[c][2] > 250) ptr[c][2] = 250;


        	if (ptr[c][0] < 0) ptr[c][0] = 0;
        	if (ptr[c][1] < 0) ptr[c][1] = 0;
        	if (ptr[c][2] < 0) ptr[c][2] = 0;

            ptr[c] = cv::Vec3b(ptr[c][0], ptr[c][1], ptr[c][2]);
        }
    }
}

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
    std::unique_ptr<tflite::FlatBufferModel> encoder_model = tflite::FlatBufferModel::BuildFromFile("encoder_model.tflite");
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
    const char* imagefile =  "input_img.png"; //"value_1.jpg";
    const char* imagefile_s = "My_QR.png";
    cv::Mat image = cv::imread(imagefile);
    cv::Mat image_s = cv::imread(imagefile_s);

    //cv::imshow("Input",imagefile);
    cv::waitKey(10);

    // write to input
    int input = interpreter->inputs()[0];

    uint8_t* in_1 = image.ptr<uint8_t>(0);
    uint8_t* in_2 = image_s.ptr<uint8_t>(0);

    ProcessInputWithFloatModel(in_1, &interpreter->typed_input_tensor<float>(0)[wanted_input_width * wanted_input_height * wanted_input_channels]);
    ProcessInputWithFloatModel(in_2, &interpreter->typed_input_tensor<float>(0)[0]);
    // input
    //ProcessInputWithFloatModel(in_2, out_2);

    // run interpreter
    if (interpreter->Invoke() != kTfLiteOk) {
        std::cout << "error!!!" << std::endl;
    }
    // get output      
    int output_idx = interpreter->outputs()[0];
    float* output = interpreter->typed_output_tensor<float>(0); // output_idx
    cv::Mat outputMat = ProcessOutputWithFloatModel(output);

    //check_function(outputMat);

    cv::imwrite("output_img.jpg", outputMat);
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