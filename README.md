<p align="center">
  <a href="https://github.com/SergeyIvanovDevelop/QR-stego-C_plus_plus_tensorflowlite">
    <img alt="QR-Stego-TFLite-C++" src="./resources/logo.png" />
  </a>
</p>
<h1 align="center">
  QR-Stego-TFLite-C++
</h1>

## QR-Stego-TFLite-C++ &middot; [![GitHub license](https://img.shields.io/badge/license-CC%20BY--NC--SA%203.0-blue)](./LICENSE) [![C++](https://img.shields.io/badge/language-C%2B%2B-yellowgreen)](https://isocpp.org/) [![TFLite](https://img.shields.io/badge/ML-TFLite-important)](https://www.tensorflow.org/lite/guide) [![LinkedIn](https://img.shields.io/badge/linkedin-Sergey%20Ivanov-blue)](https://www.linkedin.com/in/sergey-ivanov-33413823a/) [![Telegram](https://img.shields.io/badge/telegram-%40SergeyIvanov__dev-blueviolet)](https://t.me/SergeyIvanov_dev) ##

This repository contains code that runs pre-trained artificial neural network models in the `TensorFlow` framework in `Python` that have been converted to `TFLite` models using the `TFLite C++ API`.

## :computer: Getting Started  ##

**Building**

1. Install `TFLite` for `C++`.

2. Go to home directory and clone repository from github: `cd ~ && git clone https://SergeyIvanovDevelop@github.com/SergeyIvanovDevelop/QR-stego-C_plus_plus_tensorflowlite`

3. Change to the project directory: `cd QR-stego-C_plus_plus_tensorflowlite`

4. Run command to build the project: `make`

**Data embedding**<br>

5. Run command: `./encoder`<br>

When the file `./encoder` is launched, the image `My_QR.png` is embedded into the image `input_img.png` using neural steganography, which the artificial neural network `encoder_model.tflite` is trained to perform. The result of the program will be the image `output_img.jpg` containing the image `My_QR.png`.

**Data extraction**<br>

6. Run command: `./decoder`<br>

When the file `./decoder` is launched, the image `My_QR.png` is extracted from the image `output_img.jpg` into the image `Extract_QR.png` using neural steganography, which the artificial neural network `reveal_model.tflite` is trained to perform.

### :bookmark_tabs: Licence ###
QR-Stego-TFLite-C++ is [CC BY-NC-SA 3.0 licensed](./LICENSE).
