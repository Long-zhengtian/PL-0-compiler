#! /bin/bash
g++ Lexical.cpp GAandSA.cpp TCG.cpp TCGmain.cpp -std=c++11 -o Compiler

g++ PL-0-interpreter.cpp -std=c++11 -o Interpreter