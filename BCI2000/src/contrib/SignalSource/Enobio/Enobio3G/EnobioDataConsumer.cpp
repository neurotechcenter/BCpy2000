#include "EnobioDataConsumer.h"
#include "BCIStream.h"

const int buffer_length=1000;

EnobioDataConsumer::EnobioDataConsumer(){
	clearBuffer();
  num_channels = 8;
  mutex = CreateMutex( NULL, FALSE, NULL );   /* Cleared */
}
EnobioDataConsumer::~EnobioDataConsumer(){
  CloseHandle( mutex );
}
void EnobioDataConsumer::receiveData( const PData &data) {
	int *newdata;
	ChannelData* d;
	if(current_length>buffer_length){
		//todo error handle
		bciwarn<< ".";	
	}else{
		d= (ChannelData*) data.getData();
		newdata = (int*) malloc(sizeof(int)*(num_channels + 1));
    newdata[0] = num_channels;
    for(int i = 0; i < num_channels; i++)
    {
      newdata[i + 1]=d->data()[i];
    }
    WaitForSingleObject( mutex, INFINITE );
		data_queue.push(newdata);
		current_length++;
    ReleaseMutex( mutex );
	}
}
int EnobioDataConsumer::consumeData(int *buffer,int size,int count){
  count = 0;
  bool isEmpty;
	while(count < size){
    WaitForSingleObject( mutex, INFINITE );
    isEmpty = data_queue.empty();
    ReleaseMutex( mutex );
    if (isEmpty)
    {
      Sleep(0);
    }
    else
    {
      WaitForSingleObject( mutex, INFINITE );
      int * e = data_queue.front();
      ReleaseMutex( mutex );
      for (int i = 0; i < e[0]; i++)
      {
        buffer[count*e[0] + i]= e[i + 1];
      }
		  count++;
      WaitForSingleObject( mutex, INFINITE );
      data_queue.pop();
      current_length--;
      ReleaseMutex( mutex );
		  free(e);
    }
	}
	return count;

}
void EnobioDataConsumer::clearBuffer(){
  WaitForSingleObject( mutex, INFINITE );
	while(!data_queue.empty()){
		int* e=data_queue.front();
		data_queue.pop();
		free(e);
	}
  ReleaseMutex( mutex );
	current_length=0;
}

void EnobioDataConsumer::setNumberOfChannels(int channels)
{
  if (channels <= 32)
  {
    num_channels = channels;
  }
}