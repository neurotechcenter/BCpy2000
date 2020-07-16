#include "EnobioStatusConsumer.h"
#include "BCIStream.h"

using namespace std;

EnobioStatusConsumer::EnobioStatusConsumer(){
	clearBuffer();
}
EnobioStatusConsumer::~EnobioStatusConsumer(){
}
void EnobioStatusConsumer::receiveData(const PData &data) {
	int* newdata=(int*)data.getData();
	int sd=newdata[0];
	string stateType=StatusData::getTypeFromCode((StatusData::statusCode)sd);
	if(stateType.compare("ERROR")==0){
		bcierr<<StatusData::getStringFromCode((StatusData::statusCode)sd)<<endl;
		setErrorMessage(StatusData::getStringFromCode((StatusData::statusCode)sd));
	}else{
		bciout<<StatusData::getStringFromCode((StatusData::statusCode)sd)<<endl;
	}
}
string EnobioStatusConsumer::getLastError(){	
	return error_message;
}
void EnobioStatusConsumer::setErrorMessage(string s){
	error_message=s;
}

void EnobioStatusConsumer::clearBuffer(){	
	error_message="";
}