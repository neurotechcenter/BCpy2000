
#include "GenericSignal.h"
#include "CustomIIR.h"

double gSamplingRate = 0.0;
CustomIIR * GetFilter();
void CleanUp();

#include <cstring>
#include "mex.h"
void mexFunction( int nargout, mxArray *argout[], int nargin, const mxArray *argin[] )
{
	mexAtExit( CleanUp );
	if( nargout > 1 ) mexErrMsgTxt("too many output arguments");
	if( nargin != 2 ) mexErrMsgTxt("two input arguments are required");
	
	const mxArray* arg = argin[0];
	if( !mxIsChar( arg )  | mxIsEmpty( arg ) | mxGetM( arg ) != 1 )
		mexErrMsgTxt("first input argument should be a one-line string");
	char *cmd = mxArrayToString( arg );		

	arg = argin[1];
	if( !mxIsDouble(arg) || mxIsSparse(arg) || mxIsComplex(arg) || mxGetNumberOfDimensions(arg) != 2 )
		mexErrMsgTxt("second input argument should be a full, real, double 2-D matrix");
	unsigned int nRows = mxGetM( arg ),
	             nCols = mxGetN( arg ),
	             nVals = mxGetNumberOfElements( arg );
	double *vals = mxGetPr( arg );
		
	if( strcmp( cmd, "fs" ) == 0 )
	{
		if( nVals != 1 ) mexErrMsgTxt("second input should be scalar for this sub-function");
		gSamplingRate = *vals;
		GetFilter()->Reset();
	}	
	else if( strcmp( cmd, "hp" ) == 0 )
	{
		if( nVals != 1 ) mexErrMsgTxt("second input should be scalar for this sub-function");
		if( gSamplingRate == 0.0 ) mexErrMsgTxt("set the sampling rate first with subcommand 'fs'");
		GetFilter()->HPCorner( *vals / gSamplingRate );
		GetFilter()->Reset();
	}	
	else if( strcmp( cmd, "lp" ) == 0 )
	{
		if( nVals != 1 ) mexErrMsgTxt("second input should be scalar for this sub-function");
		if( gSamplingRate == 0.0 ) mexErrMsgTxt("set the sampling rate first with subcommand 'fs'");
		GetFilter()->LPCorner( *vals / gSamplingRate );
		GetFilter()->Reset();
	}	
	else if( strcmp( cmd, "notch" ) == 0 )
	{
		if( nVals != 1 ) mexErrMsgTxt("second input should be scalar for this sub-function");
		if( gSamplingRate == 0.0 ) mexErrMsgTxt("set the sampling rate first with subcommand 'fs'");
		GetFilter()->NotchCenter( *vals / gSamplingRate );
		GetFilter()->Reset();
	}	
	else if( strcmp( cmd, "process" ) == 0 )
	{
		GenericSignal input, output;
		SignalProperties props;
		props.SetChannels( nCols );
		props.SetElements( nRows );
		input.SetProperties( props );
		output.SetProperties( props );
		for( unsigned int ch = 0; ch < nCols; ch++ ) // that's right, channels are columns of the matlab input
			for( unsigned int el = 0; el < nRows; el++ )
				input( ch, el ) = *vals++;
		GetFilter()->Process( input, output );
		mxArray* mxOut = mxCreateDoubleMatrix( nRows, nCols, mxREAL );
		vals = mxGetPr( mxOut );
		for( unsigned int ch = 0; ch < nCols; ch++ )
			for( unsigned int el = 0; el < nRows; el++ )
				*vals++ = output( ch, el );
		argout[0] = mxOut;
	}
	else mexErrMsgTxt("unknown subcommand");	
	mxFree( cmd );
}
CustomIIR * gFilter = NULL;
CustomIIR * GetFilter()
{
	if( gFilter == NULL )
	{
		gFilter = new CustomIIR;
		//printf("mex filter created\n");
	}
	return gFilter;
}
void CleanUp()
{
	if( gFilter )
	{
		delete gFilter;
		//printf("mex filter destroyed\n");
	}
	gFilter = NULL;
}
