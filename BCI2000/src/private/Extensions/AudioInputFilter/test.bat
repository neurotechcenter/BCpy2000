@cd ..\..\..\..\prog


@set OnConnect=-
@set OnConnect=%OnConnect% ; SET PARAMETER MicName Para
@set OnConnect=%OnConnect% ; SET PARAMETER MicRecordingFreq 3
@set OnConnect=%OnConnect% ; SET PARAMETER MicBitDepth 2
@set OnConnect=%OnConnect% ; SET PARAMETER MicNumChannels 2
@set OnConnect=%OnConnect% ; SET PARAMETER VisualizeAudioInputFilter 1

@set OnSetConfig=-
@set OnSetConfig=%OnSetConfig% ; SET STATE Running 1

cd ..\prog

start Operator --OnConnect "%OnConnect%" --OnSetConfig "%OnSetConfig%"
start SignalGenerator
start DummySignalProcessing
start DummyApplication