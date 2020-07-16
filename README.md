# BCpy2000

BCPy2000 is a system for building experimental brain-computer interface systems, based on the BCI2000 project but using Python for rapid development. 

## Requirements

- [Python 3.6.8](https://www.python.org/downloads/release/python-368/)
- [BCI2000](www.bci2000.org)
- [Pipenv](https://docs.pipenv.org)

### Installing

```
git clone git@github.com:neurotechcenter/BCpy2000.git
cd BCpy2000
pipenv install
```

### Running 

```
pipenv shell

cd BCpy2000

cd batch

PythonDemo1_Triangle_p3.bat
```

## Structure 

BCPy2000 stands on the shoulders of giants. It consists of:

 - A number of dependencies (the Python programming language itself, and a number of third-party python packages) which you must first install on your computer.

 - The foundation consisting of a collection of official BCI2000 binaries as well as the python-specific binaries contributed by us. By convention these are kept in a directory called prog.

 - Standing on top of the foundation is the framework written by us. This consists of python files which provide the glue between the BCI2000 binaries and your developer files, and which also implement API methods for you to call in your python code. By default, these files are expected to be installed as a python package.
 - Supported by the framework are the developer files written by you, implementing the specific behaviour of the modules. By default, they go in the ..\python directory relative to prog.

## Status

As of our current release, you have to build your BCI2000 binaries by creating a build solution in Microsoft Visual Studio. Our main focus at the moment is to implement a fully portable version of BCpy2000 by compiling executables for the BCI2000 binaries rather than relying on the developer to build their own binaries. Many of the following features are partially complete, but this isn't indicated. Look at the Issues page if you need to know what is being worked on.

- [ ] Full Monty
  - [ ] 64x exectuables
  - [x] Pipenv installation
    - [ ] Dependencies
    - [x] BCI framework
  - [ ] Higher level tool support
- [ ] Documentation
  - [ ] API
    - [ ] BCICore
    - [ ] BCIGenericSource
    - [ ] BCIGenericSignalProcessing
    - [ ] BCIGeneric Application
  - [ ] Foundation(Binary exectuables)
    - [ ] Operator
    - [ ] Source
    - [ ] Signal Processing
    - [ ] application
- [ ] Demonstration


