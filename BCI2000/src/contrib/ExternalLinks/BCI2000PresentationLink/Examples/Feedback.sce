# This is an example scenario to demonstrate how Presentation may be used to
# display a feedback signal which is computed by BCI2000.

# In order to record Presentation events into BCI2000, a BCI2000 data port
# must have been created via Settings->Ports->Data Ports->Add.
scenario = "BCI2000 Feedback";

begin;

picture {
  text { caption = "waiting ..."; font_size = 48; } text1;
  x = 0; y = 0;
} picture1;

begin_pcl;

int trial_duration = 3000; # ms
int update_interval = 50; # ms

sub
  show_text( string message )
begin
  text1.set_caption( message );
  text1.redraw();
  picture1.present();
end;

sub
  move_to( double x, double y )
begin
  picture1.set_part_x( 1, x );
  picture1.set_part_y( 1, y );
  picture1.present();
end;

picture1.present();

array<int> sequence[0];
loop int i = 1 until i > 5
begin
  sequence.add( 1 );
  sequence.add( 2 );
  i = i + 1;
end;
sequence.shuffle();

# Create a BCI2000 controller object.
bci2000::controller bci = new bci2000::controller();
bci.hide_window();

# Presentation plays the role of a BCI2000 application module.
# This implies that it must create and set state variables that
# reflect the current task, and the current feedback state.

# States must be added before core modules are connected.
# Add an 8 bit wide TargetCode state with a default value of 0:
bci.add_state_variable( "TargetCode", 8, 0 );
# Add a 1 bit wide Feedback state with a default value of 0:
bci.add_state_variable( "Feedback", 1, 0 );

# We use the ARSignalProcessing module in order to compute the feedback
# signal, and the DummyApplication module because Presentation plays the
# role of the Application module.
array<string> modules[3] =
{ "SignalGenerator", "ARSignalProcessing", "DummyApplication" };
bci.startup_modules( modules );

# Synchronize BCI2000 subject id with Presentation's.
bci.set_subject( logfile.subject() );
bci.set_session( "002" );

bci.load_parameters_remote( "../parms/examples/CursorTask_SignalGenerator.prm" );
bci.load_parameters_remote( "../parms/fragments/feedback/CursorTask1D.prm" );

# Suppress display of default visualization windows.
bci.set_parameter( "VisualizeTiming", "0" );
bci.set_parameter( "VisualizeSource", "0" );

bci.set_config();
bci.start();

loop int i = 1 until i > sequence.count()
begin
  # Trial begin.
  bci.set_state_variable( "TargetCode", sequence[i] );
  if( sequence[i] == 1 ) then
    show_text( "move me up" );
  else
     show_text( "move me down" );
  end;
  i = i + 1;
  # Feedback begin.
  bci.set_state_variable( "Feedback", 1 );
  double x = 0.0, y = 0.0;
  loop int t = 0 until t > trial_duration
  begin
    x = x + 10.0 * bci.get_control_signal( 1, 1 );
    y = y + 10.0 * bci.get_control_signal( 2, 1 );
    move_to( x, y );
    wait_interval( update_interval );
    t = t + update_interval;
   end;
  # Feedback end.
  bci.set_state_variable( "Feedback", 0 );
  # Trial end.
  bci.set_state_variable( "TargetCode", 0 );
end;

# End of Run.
show_text( "finished" );
bci.stop();
wait_interval( trial_duration );
