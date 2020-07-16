#include <string>
#include "lib/StatusData.h"
#include "lib/IDataConsumer.h"
using namespace std;

/*!
 * \class StatusConsumer StatusConsumer.h
 *
 * \brief Default implementation of the IDataConsumer interface.
 */
class EnobioStatusConsumer: public IDataConsumer
{
public:
    /*!
     * Default constructor
     */
	
     EnobioStatusConsumer ();
    ~EnobioStatusConsumer ();
	void receiveData (const PData& data) ;
	void clearBuffer();
	
	void setErrorMessage(string s);
	string getLastError();
	string error_message;
private:
	
	
};

