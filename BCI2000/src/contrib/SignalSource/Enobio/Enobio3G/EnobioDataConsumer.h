
#include <queue>
#include <windows.h>
#include "lib/ChannelData.h"
#include "lib/IDataConsumer.h"

using namespace std;
/*!
 * \class DefaultDataProducer DefaultDataProducer.h
 *
 * \brief Default implementation of the IDataProducer interface.
 */
class EnobioDataConsumer:public IDataConsumer
{
public:
    /*!
     * Default constructor
     */
    EnobioDataConsumer ();

    ~EnobioDataConsumer ();
   void setNumberOfChannels(int channels);
	 void receiveData (const PData& data) ;
	 void clearBuffer ();
	 int consumeData(int *buffer,int size,int count);
	 
	
private:
	queue <int*> data_queue;
	int current_length;
  int num_channels;
  HANDLE mutex;
};
