unit HostInfoUnit;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, SpTBXControls, TBXDkPanels, SpTBXItem;

type
  THostInfoForm = class(TForm)
    SpTBXTitleBar1: TSpTBXTitleBar;
    Label1: TSpTBXLabel;
    Label2: TSpTBXLabel;
    Label3: TSpTBXLabel;
    Label4: TSpTBXLabel;
    Label5: TSpTBXLabel;
    Label6: TSpTBXLabel;
    Button1: TSpTBXButton;

    procedure CreateParams(var Params: TCreateParams); override;

    procedure Label3Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Label5Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  HostInfoForm: THostInfoForm;

implementation

uses
  ShellAPI, Unit1, PreferencesFormUnit, BattleFormUnit;

{$R *.dfm}

procedure THostInfoForm.CreateParams(var Params: TCreateParams);
begin
  inherited CreateParams(Params);

  if not Preferences.TaskbarButtons then Exit;

  with Params do begin
    ExStyle := ExStyle or WS_EX_APPWINDOW;
    WndParent := BattleForm.Handle;
  end;
end;

procedure THostInfoForm.Label3Click(Sender: TObject);
begin
  ShellExecute(MainForm.Handle, nil, 'http://www.portforward.com/routers.htm', '', '', SW_SHOW);
end;

procedure THostInfoForm.Button1Click(Sender: TObject);
begin
  Close;
end;

procedure THostInfoForm.Label5Click(Sender: TObject);
begin
  ShellExecute(MainForm.Handle, nil, 'http://taspring.clan-sy.com/wiki/FAQ', '', '', SW_SHOW);
end;

end.
