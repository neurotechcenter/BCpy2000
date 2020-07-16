from setuptools import setup

setup(
		name = 'BCPy2000',
		version = '0.10',
		packages = [
			'BCPy2000',
			'BCPy2000.AppTools',
			'BCPy2000.LangTools',
			'BCPy2000.SigTools',
			'BCPy2000.WavTools',
			'BCPy2000.BCI2000Tools',
			'BCPy2000.Documentation',
		],
		package_data = {
			'BCPy2000.AppTools': ['*.dll'],
			'BCPy2000.Documentation': ['*.*', 'styles/gears/*.*'],
		},
		scripts = [
			#'BCPy2000_postinstall.py',
		],
		install_requires = ['numpy', 'IPython']
	)