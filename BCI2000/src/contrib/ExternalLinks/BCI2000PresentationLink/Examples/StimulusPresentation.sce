# This is an example scenario to demonstrate how BCI2000 may be controlled 
# from Presentation via PCL.
# In order to record Presentation events into BCI2000, a BCI2000 data port 
# must have been created via Settings->Ports->Data Ports->Add.
scenario = "BCI2000 Stimulus Presentation";

default_picture_duration = 750;
default_trial_duration = 1250;
default_font_size = 48;

begin;

picture {} default;

trial {
  picture {
    text { caption = "1"; };
    x = 0; y = 0;
  };
  code = 1;
} trial1;

trial {
  picture {
    text { caption = "2"; };
    x = 0; y = 0;
  };
  code = 2;
} trial2;

begin_pcl;

array<trial> sequence[0];
loop int i = 1 until i > 5
begin
  sequence.add( trial1 );
  sequence.add( trial2 );
  i = i + 1;
end;
sequence.shuffle();

# Create a BCI2000 controller object.
bci2000::controller bci = new bci2000::controller();
bci.hide_window();

# For a stimulus presentation scenario, we don't need any signal processing,
# so we use the DummySignalProcessing module.
# Also, Presentation plays the role of the Application module, so we start
# the DummyApplication module.
array<string> modules[3] =
{ "SignalGenerator", "DummySignalProcessing", "DummyApplication" };
bci.startup_modules( modules );

# Synchronize BCI2000 subject id with Presentation's.
bci.set_subject( logfile.subject() );
bci.set_session( "001" );

bci.load_parameters_remote( "../parms/examples/CursorTask_SignalGenerator.prm" );

# Suppress display of default visualization windows.
bci.set_parameter( "VisualizeTiming", "0" );
bci.set_parameter( "VisualizeSource", "0" );

bci.set_config();
bci.start();

loop int i = 1 until i > sequence.count()
begin
  sequence[i].present();
  i = i + 1;
end;

bci.stop();
