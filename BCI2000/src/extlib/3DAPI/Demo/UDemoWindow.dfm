object DemoWindow: TDemoWindow
  Left = 375
  Top = 290
  Caption = '3D Environment Demo'
  ClientHeight = 473
  ClientWidth = 640
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnKeyDown = FormKeyDown
  OnPaint = FormPaint
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object FrameUpdate: TTimer
    Enabled = False
    Interval = 50
    OnTimer = FrameUpdateTimer
    Left = 24
    Top = 24
  end
end
