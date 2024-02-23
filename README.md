# Dynamic Input Deep Learning Model

## Introduction

Welcome to dynamic input onnxruntime, a project aimed at providing a comprehensive guide and implementation for running a model with dynamic input via onnxruntime.

## Overview

The goal of this project is to fill the gap in available guides and resources by providing a clear and concise example of running a model with dynamic input using onnxruntime. This project aims to simplify the process and provide a helpful resource for developers and researchers who are working with dynamic inputs in their models.

## Setup

To get started with this project, follow these steps:

1. **Clone the Repository**: Clone this repository to your local machine using `https://github.com/mohsensafaei/Dynamic-Input-onnxruntime.git`.

2. **Install Dependencies**: Make sure you have all the necessary dependencies installed.

    1. [Download onnxruntime gpu-1.12.0](https://github.com/microsoft/onnxruntime/releases?page=2).
    2. Unpack the downloaded `onnxruntime-linux-x64-gpu-1.12.0.tgz` file.

3. **Download the Model**: Obtain the ONNX model you want to use with dynamic input support and place it in the appropriate directory within the project structure.

4. **Run the Code**: Execute the provided example script to see how to run a model with dynamic input:

    ```bash
    mkdir build && cd build
    cmake ..
    make
    ```
