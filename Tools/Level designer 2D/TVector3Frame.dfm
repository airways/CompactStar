object Vector3Frame: TVector3Frame
  Left = 0
  Top = 0
  Width = 312
  Height = 44
  DoubleBuffered = True
  Padding.Left = 3
  Padding.Top = 3
  Padding.Right = 3
  Padding.Bottom = 3
  ParentDoubleBuffered = False
  TabOrder = 0
  OnResize = FrameResize
  object paLabels: TPanel
    Left = 3
    Top = 3
    Width = 306
    Height = 17
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    object laX: TLabel
      AlignWithMargins = True
      Left = 0
      Top = 0
      Width = 100
      Height = 17
      Margins.Left = 0
      Margins.Top = 0
      Margins.Bottom = 0
      Align = alLeft
      AutoSize = False
      Caption = 'x'
      Layout = tlCenter
      ExplicitHeight = 13
    end
    object laY: TLabel
      AlignWithMargins = True
      Left = 103
      Top = 0
      Width = 100
      Height = 17
      Margins.Left = 0
      Margins.Top = 0
      Margins.Bottom = 0
      Align = alLeft
      AutoSize = False
      Caption = 'y'
      Layout = tlCenter
      ExplicitLeft = 168
    end
    object laZ: TLabel
      AlignWithMargins = True
      Left = 206
      Top = 0
      Width = 100
      Height = 17
      Margins.Left = 0
      Margins.Top = 0
      Margins.Bottom = 0
      Align = alLeft
      AutoSize = False
      Caption = 'z'
      Layout = tlCenter
      ExplicitLeft = 240
    end
  end
  object paValues: TPanel
    Left = 3
    Top = 20
    Width = 306
    Height = 21
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 1
    object edX: TEdit
      AlignWithMargins = True
      Left = 0
      Top = 0
      Width = 100
      Height = 21
      Margins.Left = 0
      Margins.Top = 0
      Margins.Bottom = 0
      Align = alLeft
      TabOrder = 0
      Text = '0'
      OnChange = OnEditValueChange
      OnEnter = OnEditEnter
      OnExit = OnEditExit
      OnKeyUp = OnEditKeyUp
    end
    object edY: TEdit
      AlignWithMargins = True
      Left = 103
      Top = 0
      Width = 100
      Height = 21
      Margins.Left = 0
      Margins.Top = 0
      Margins.Bottom = 0
      Align = alLeft
      TabOrder = 1
      Text = '0'
      OnChange = OnEditValueChange
      OnEnter = OnEditEnter
      OnExit = OnEditExit
      OnKeyUp = OnEditKeyUp
    end
    object edZ: TEdit
      AlignWithMargins = True
      Left = 206
      Top = 0
      Width = 100
      Height = 21
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      Align = alLeft
      TabOrder = 2
      Text = '0'
      OnChange = OnEditValueChange
      OnEnter = OnEditEnter
      OnExit = OnEditExit
      OnKeyUp = OnEditKeyUp
    end
  end
end
