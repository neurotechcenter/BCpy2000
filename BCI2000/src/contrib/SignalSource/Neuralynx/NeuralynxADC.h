////////////////////////////////////////////////////////////////////////////////
// $Id: NeuralynxADC.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: g.dimitriadis@donders.ru.nl
// Description: A source class that interfaces to the Neuralynx DigitalLynx
//              amplifier through its reversed engineered Matlab drivers
//
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
// 
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
// 
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#ifndef NEURALYNX_ADC_H
#define NEURALYNX_ADC_H


#include "GenericADC.h"
#include "NeuralynxThread.h"

class NeuralynxADC : public GenericADC
{
 public:
                    NeuralynxADC();
    virtual         ~NeuralynxADC();
    virtual void    Preflight( const SignalProperties&, SignalProperties& ) const;
    virtual void    Initialize( const SignalProperties&, const SignalProperties& );
    virtual void    Process( const GenericSignal&, GenericSignal& );
    virtual void    Halt();


 private:
    short samplingRate;
    void InitializeNeuralynxTread();//Start the Neuralynx data aquisition thread

protected:

    bool threadInitialized;
    bool keepNeuralynxThreadRunning;

    NeuralynxThread *dataThread;

};



#endif // NEURALYNX_ADC_H
