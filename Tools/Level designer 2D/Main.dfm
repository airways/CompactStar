object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'Level designer'
  ClientHeight = 603
  ClientWidth = 800
  Color = clBtnFace
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = mmMainMenu
  OldCreateOrder = False
  Position = poScreenCenter
  PrintScale = poNone
  Scaled = False
  OnResize = FormResize
  OnShortCut = FormShortCut
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object spMainView: TSplitter
    Left = 612
    Top = 0
    Height = 603
    Align = alRight
    ResizeStyle = rsUpdate
    OnMoved = spMainViewMoved
    ExplicitLeft = 797
    ExplicitTop = -8
    ExplicitHeight = 600
  end
  object paProperties: TPanel
    Left = 615
    Top = 0
    Width = 185
    Height = 603
    Align = alRight
    BevelOuter = bvNone
    Constraints.MinWidth = 185
    ParentBackground = False
    TabOrder = 0
    object cgProperties: TCategoryPanelGroup
      Left = 0
      Top = 0
      Width = 185
      Height = 603
      VertScrollBar.Tracking = True
      Align = alClient
      BevelInner = bvNone
      BevelOuter = bvNone
      HeaderFont.Charset = DEFAULT_CHARSET
      HeaderFont.Color = clWindowText
      HeaderFont.Height = -11
      HeaderFont.Name = 'Tahoma'
      HeaderFont.Style = []
      TabOrder = 0
    end
  end
  object paViews: TPanel
    Left = 0
    Top = 0
    Width = 612
    Height = 603
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
    object spViews: TSplitter
      Left = 0
      Top = 200
      Width = 612
      Height = 3
      Cursor = crVSplit
      Align = alTop
      ResizeStyle = rsUpdate
      OnMoved = spViewsMoved
      ExplicitTop = 41
      ExplicitWidth = 559
    end
    object paEngineView: TPanel
      Left = 0
      Top = 203
      Width = 612
      Height = 400
      Cursor = crArrow
      Align = alClient
      BevelOuter = bvNone
      DoubleBuffered = False
      ParentBackground = False
      ParentDoubleBuffered = False
      TabOrder = 0
      TabStop = True
      OnClick = OnViewClick
    end
    object paDesignerView: TPanel
      Left = 0
      Top = 0
      Width = 612
      Height = 200
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 1
      TabStop = True
      OnClick = OnViewClick
    end
  end
  object aeEvents: TApplicationEvents
    OnMessage = aeEventsMessage
    Left = 16
    Top = 8
  end
  object mmMainMenu: TMainMenu
    Left = 48
    Top = 8
    object miFile: TMenuItem
      Caption = 'File'
      object miFileNew: TMenuItem
        Caption = 'New...'
        OnClick = miFileNewClick
      end
      object miFileLoad: TMenuItem
        Caption = 'Load...'
        OnClick = miFileLoadClick
      end
      object miFileSave: TMenuItem
        Caption = 'Save...'
        OnClick = miFileSaveClick
      end
      object miFileSeparator: TMenuItem
        Caption = '-'
      end
      object miFileExit: TMenuItem
        Caption = 'Exit'
        OnClick = miFileExitClick
      end
    end
    object miDesigner: TMenuItem
      Caption = 'Designer'
      object miDesignerPlayMode: TMenuItem
        AutoCheck = True
        Caption = 'Play mode'
        OnClick = miDesignerPlayModeClick
      end
      object miDesignerRefresh: TMenuItem
        Caption = 'Refresh'
        object miDesignerRefreshImmediately: TMenuItem
          AutoCheck = True
          Caption = 'Immediately'
          Checked = True
          RadioItem = True
        end
        object miDesignerRefreshEvery500ms: TMenuItem
          AutoCheck = True
          Caption = 'Every 500ms'
          RadioItem = True
        end
        object miDesignerRefreshEvery1s: TMenuItem
          AutoCheck = True
          Caption = 'Every 1s'
          RadioItem = True
        end
        object miDesignerRefreshEvery2s: TMenuItem
          AutoCheck = True
          Caption = 'Every 2s'
          RadioItem = True
        end
        object miDesignerRefreshEvery5s: TMenuItem
          AutoCheck = True
          Caption = 'Every 5s'
          RadioItem = True
        end
        object miDesignerRefreshEvery10s: TMenuItem
          AutoCheck = True
          Caption = 'Every 10s'
          RadioItem = True
        end
      end
    end
    object miLandscape: TMenuItem
      Caption = 'Landscape'
      object miAdd: TMenuItem
        Caption = 'Add'
        object miAddBox: TMenuItem
          Caption = 'Box...'
          OnClick = miAddBoxClick
        end
        object miAddCylinder: TMenuItem
          Caption = 'Cylinder...'
          OnClick = miAddCylinderClick
        end
        object miAddDisk: TMenuItem
          Caption = 'Disk...'
          OnClick = miAddDiskClick
        end
        object miAddRing: TMenuItem
          Caption = 'Ring...'
          OnClick = miAddRingClick
        end
        object miAddSphere: TMenuItem
          Caption = 'Sphere...'
          OnClick = miAddSphereClick
        end
        object miAddSpiral: TMenuItem
          Caption = 'Spiral...'
          OnClick = miAddSpiralClick
        end
        object miAddSurface: TMenuItem
          Caption = 'Surface...'
          OnClick = miAddSurfaceClick
        end
        object miLandscapeAddSeparator: TMenuItem
          Caption = '-'
        end
        object miAddWaveFront: TMenuItem
          Caption = 'WaveFront model...'
          OnClick = miAddWaveFrontClick
        end
        object miAddMDLModel: TMenuItem
          Caption = 'MDL (Quake I) model...'
          OnClick = miAddMDLModelClick
        end
      end
      object miLandscapeSeparator: TMenuItem
        Caption = '-'
      end
      object miLandscapeResetViewport: TMenuItem
        Caption = 'Reset the viewport'
        OnClick = miLandscapeResetViewportClick
      end
    end
    object miSkyBox: TMenuItem
      Caption = 'Skybox'
      object miSkyboxAdd: TMenuItem
        Caption = 'Add...'
        OnClick = miSkyboxAddClick
      end
    end
    object miPostProcessing: TMenuItem
      Caption = 'Post-Processing'
      object miPostProcessingNone: TMenuItem
        AutoCheck = True
        Caption = 'None'
        RadioItem = True
        OnClick = miPostProcessingNoneClick
      end
      object miPostProcessingAntialiasing: TMenuItem
        AutoCheck = True
        Caption = 'Antialiasing'
        RadioItem = True
        OnClick = miPostProcessingAntialiasingClick
      end
      object miPostProcessingOilPainting: TMenuItem
        AutoCheck = True
        Caption = 'Oil painting'
        Checked = True
        RadioItem = True
        OnClick = miPostProcessingOilPaintingClick
      end
    end
    object miSound: TMenuItem
      Caption = 'Sound'
      object miSoundOpen: TMenuItem
        Caption = 'Open...'
        OnClick = miSoundOpenClick
      end
      object miSoundPause: TMenuItem
        AutoCheck = True
        Caption = 'Pause'
        OnClick = miSoundPauseClick
      end
    end
  end
  object odLevelOpen: TOpenDialog
    Filter = 'All level files|*.xml|XML file|*.xml'
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Left = 80
    Top = 8
  end
end
