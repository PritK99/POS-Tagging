# POS-Tagging

## Table of Contents

- [Project](#POS-Tagging)
  - [Table of Contents](#table-of-contents)
  - [About The Project](#introduction)
  - [Demo](#demo)
  - [Documentation](#documentation)
  - [File Structure](#file-structure)
  - [Project Workflow](#project-workflow)
  - [Getting started](#Getting-Started)
  - [Future Work](#future-work)
  - [Contributors](#contributors)
  - [Acknowledgements and Resources](#acknowledgements-and-references)
  - [License](#license)

## Introduction

Part-Of-Speech (POS) tagging is the process of assigning a part-of-speech tag (Noun, Verb, Adjective, etc.) to each word in an input text. In other words, the main objective is to identify which grammatical category do each word in given test belong to.

<img src = "assets/POS-Tagging.jpg" alt="POS-Tagging">

POS Tagging is difficult because some words can represent more than one part of speech at different times, i.e. They are Ambiguous in nature. Consider the following example:

The whole team played <b>well</b>. ```adverb```

You are doing <b>well</b> for yourself. ```adjective```

<b>Well</b>, this is a lot of work. ```interjection```

The <b>well</b> is dry. ```noun```

Tears were beginning to <b>well</b> in her eyes. ```verb```

### Domains Explored

Machine Learning, Natural Language Processing, Dynamic Programming

## Demo

## Documentation

For Documentation, click <a href="/documentation/README.md">here</a> or refer ```/documentation/README.md```

## File Structure
```
👨‍💻POS-Tagging
 ┣ 📂assets                            // Contains all the reference gifs, images
 ┣ 📂components                        // Header Files
 ┃ ┣ 📄data.cpp
 ┃ ┣ 📄data.hpp
 ┃ ┣ 📄tokenize.cpp
 ┃ ┣ 📄tokenize.hpp
 ┃ ┣ 📄viterbi.cpp
 ┃ ┣ 📄viterbi.hpp
 ┣ 📂data                              // Dataset
 ┃ ┣ 📄dataset.pos
 ┃ ┣ 📄sample.pos
 ┃ ┣ 📄test.pos
 ┣ 📂documentation                     // Notes & Documentation for project
 ┃ ┣ 📄notes.pdf
 ┃ ┣ 📄README.md
 ┣ 📂Miscellaneous                     // .ipynb implementation
 ┃ ┣ 📄POS-Tagging-C2_W2_Assignment
 ┣ 📄main.cpp
 ┣ 📄README.md
``` 

## Project Workflow

## Getting Started

### Prerequisites
To download and use this code, the minimum requirements are:

* [ESP_IDF](https://github.com/espressif/esp-idf)
* Windows 7 or later (64-bit), Ubuntu 20.04 or later
* [Microsoft VS Code](https://code.visualstudio.com/download) or any other IDE 

### Installation

Clone the project by typing the following command in your Terminal/CommandPrompt

```
git clone https://github.com/PritK99/MazeBlaze-v2.1.git 
```
Navigate to the MazeBlaze-v2.1 folder

```
cd POS-Tagging
```

### Usage

Once the requirements are satisfied, you can easily download the project and use it on your machine.
After following the above steps , use the following command to run the code:

```
get_idf
```

To build the code 

```
idf.py build
```

To flash the code

```
idf.py -p (PORT) flash monitor
```

## Acknowledgements and References
 
## License
[MIT License](https://opensource.org/licenses/MIT)


