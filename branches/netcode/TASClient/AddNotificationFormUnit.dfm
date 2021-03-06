object AddNotificationForm: TAddNotificationForm
  Left = 734
  Top = 638
  BorderStyle = bsDialog
  Caption = 'Add notification'
  ClientHeight = 236
  ClientWidth = 305
  Color = clBtnFace
  Constraints.MaxHeight = 921
  Constraints.MaxWidth = 1280
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  Scaled = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object SpTBXTitleBar1: TSpTBXTitleBar
    Left = 0
    Top = 0
    Width = 305
    Height = 236
    Caption = 'Add notification'
    FixedSize = True
    Options.Minimize = False
    Options.Maximize = False
    TBXStyleBackground = True
    object NotifsComboBox: TSpTBXComboBox
      Left = 24
      Top = 64
      Width = 257
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 1
      OnChange = NotifsComboBoxChange
      Items.Strings = (
        '<player> joins <channel>'
        'Player joins battle user is participating in'
        '<player> changes his status to in-battle'
        'Battle opens using specified mod')
    end
    object Edit1: TSpTBXEdit
      Left = 88
      Top = 112
      Width = 121
      Height = 21
      TabOrder = 2
      Text = 'Player'
    end
    object Edit2: TSpTBXEdit
      Left = 88
      Top = 136
      Width = 121
      Height = 21
      TabOrder = 3
      Text = '#main'
    end
    object AddButton: TSpTBXButton
      Left = 64
      Top = 192
      Width = 75
      Height = 25
      Caption = 'Add'
      TabOrder = 4
      OnClick = AddButtonClick
      LinkFont.Charset = DEFAULT_CHARSET
      LinkFont.Color = clBlue
      LinkFont.Height = -11
      LinkFont.Name = 'MS Sans Serif'
      LinkFont.Style = [fsUnderline]
    end
    object CancelButton: TSpTBXButton
      Left = 168
      Top = 192
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 5
      OnClick = CancelButtonClick
      Cancel = True
      LinkFont.Charset = DEFAULT_CHARSET
      LinkFont.Color = clBlue
      LinkFont.Height = -11
      LinkFont.Name = 'MS Sans Serif'
      LinkFont.Style = [fsUnderline]
    end
    object SpTBXLabel1: TSpTBXLabel
      Left = 16
      Top = 40
      Width = 79
      Height = 13
      Caption = 'Notification type:'
      LinkFont.Charset = DEFAULT_CHARSET
      LinkFont.Color = clBlue
      LinkFont.Height = -11
      LinkFont.Name = 'MS Sans Serif'
      LinkFont.Style = [fsUnderline]
    end
    object FieldLabel1: TSpTBXLabel
      Left = 24
      Top = 112
      Width = 32
      Height = 13
      Caption = 'Player:'
      LinkFont.Charset = DEFAULT_CHARSET
      LinkFont.Color = clBlue
      LinkFont.Height = -11
      LinkFont.Name = 'MS Sans Serif'
      LinkFont.Style = [fsUnderline]
    end
    object FieldLabel2: TSpTBXLabel
      Left = 24
      Top = 136
      Width = 42
      Height = 13
      Caption = 'Channel:'
      LinkFont.Charset = DEFAULT_CHARSET
      LinkFont.Color = clBlue
      LinkFont.Height = -11
      LinkFont.Name = 'MS Sans Serif'
      LinkFont.Style = [fsUnderline]
    end
  end
end
