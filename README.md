# BCpy2000

BCPy2000 is a system for building experimental brain-computer interface systems, based on the BCI2000 project but using Python for rapid development. 

## Requirements

- [Python 3.6.8](https://www.python.org/downloads/release/python-368/)
- [BCI2000](https://www.bci2000.org/)

### Installing

```
git clone git@github.com:neurotechcenter/BCpy2000.git
Copy this folder to your PythonHome/Lib/site-packages
Open you system variable settings menu and add a new path called PYTHONHOME_BCI2000
Set the new PYTHONHOME_BCI2000 path to your PythonHome
Follow instructions to download and install BCI2000 [BCI2000](https://www.bci2000.org/mediawiki/index.php/Programming_Howto:Building_and_Customizing_BCI2000)
For more details on how to install BCPy2000, please refer to our [wiki page](https://www.bci2000.org/mediawiki/index.php/BCPy2000) on the topic
```

## Structure 

BCPy2000 stands on the shoulders of giants. It consists of:

 - A number of dependencies (the Python programming language itself, and a number of third-party python packages) which you must first install on your computer.

 - The foundation consisting of a collection of official BCI2000 binaries as well as the python-specific binaries contributed by us. By convention these are kept in a directory called prog.

 - Standing on top of the foundation is the framework written by us. This consists of python files which provide the glue between the BCI2000 binaries and your developer files, and which also implement API methods for you to call in your python code. By default, these files are expected to be installed as a python package.
 - Supported by the framework are the developer files written by you, implementing the specific behaviour of the modules. By default, they go in the ..\python directory relative to prog.

## Status

As of our current release, you have to build your BCI2000 binaries by creating a build solution in Microsoft Visual Studio. Our main focus at the moment is to provide the Python site packages which is necessary to build the Python environment upon which BCPy2000 is reliant.
