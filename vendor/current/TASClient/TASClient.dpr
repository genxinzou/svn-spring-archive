program TASClient;

uses
  Forms,
  Unit1 in 'Unit1.pas' {MainForm},
  PreferencesFormUnit in 'PreferencesFormUnit.pas' {PreferencesForm},
  StringParser in 'StringParser.pas',
  BattleFormUnit in 'BattleFormUnit.pas' {BattleForm},
  Utility in 'Utility.pas',
  Misc in 'Misc.pas',
  HostBattleFormUnit in 'HostBattleFormUnit.pas' {HostBattleForm},
  MinimapZoomedFormUnit in 'MinimapZoomedFormUnit.pas' {MinimapZoomedForm},
  WaitForAckUnit in 'WaitForAckUnit.pas' {WaitForAckForm},
  ChooseColorFormUnit in 'ChooseColorFormUnit.pas' {ChooseColorForm},
  DisableUnitsFormUnit in 'DisableUnitsFormUnit.pas' {DisableUnitsForm},
  InitWaitFormUnit in 'InitWaitFormUnit.pas' {InitWaitForm},
  HostInfoUnit in 'HostInfoUnit.pas' {HostInfoForm},
  HelpUnit in 'HelpUnit.pas' {HelpForm},
  GetFileUnit in 'GetFileUnit.pas' {GetFileForm},
  DebugUnit in 'DebugUnit.pas' {DebugForm},
  AddBotUnit in 'AddBotUnit.pas' {AddBotForm},
  UpdateBotUnit in 'UpdateBotUnit.pas' {UpdateBotForm},
  ReplaysUnit in 'ReplaysUnit.pas' {ReplaysForm},
  PasswordDialogUnit in 'PasswordDialogUnit.pas' {PasswordDlg},
  HttpGetUnit in 'HttpGetUnit.pas' {HttpGetForm},
  OnlineMapsUnit in 'OnlineMapsUnit.pas' {OnlineMapsForm},
  NotificationsUnit in 'NotificationsUnit.pas' {NotificationsForm},
  AddNotificationFormUnit in 'AddNotificationFormUnit.pas' {AddNotificationForm},
  ExceptionUnit in 'ExceptionUnit.pas' {ExceptionDialog},
  CrashDetailsUnit in 'CrashDetailsUnit.pas' {CrashDetailsDialog};

{$R *.res}
{$R 'sounds.RES'} // various sounds
{$R 'flags.RES'} // country flags

begin
  Application.Initialize;

  // preload preferences, since we need to know about "TaskbarButtons" field before we create other forms:
  Preferences := DEFAULT_PREFERENCES;
  PreferencesForm.ReadPreferencesRecordFromRegistry(Preferences);

  Application.CreateForm(TMainForm, MainForm);
  Application.CreateForm(TPreferencesForm, PreferencesForm);
  Application.CreateForm(TBattleForm, BattleForm);
  Application.CreateForm(THostBattleForm, HostBattleForm);
  Application.CreateForm(TMinimapZoomedForm, MinimapZoomedForm);
  Application.CreateForm(TWaitForAckForm, WaitForAckForm);
  Application.CreateForm(TChooseColorForm, ChooseColorForm);
  Application.CreateForm(TDisableUnitsForm, DisableUnitsForm);
  Application.CreateForm(TInitWaitForm, InitWaitForm);
  Application.CreateForm(THostInfoForm, HostInfoForm);
  Application.CreateForm(THelpForm, HelpForm);
  Application.CreateForm(TGetFileForm, GetFileForm);
  Application.CreateForm(TDebugForm, DebugForm);
  Application.CreateForm(TAddBotForm, AddBotForm);
  Application.CreateForm(TUpdateBotForm, UpdateBotForm);
  Application.CreateForm(TReplaysForm, ReplaysForm);
  Application.CreateForm(TPasswordDlg, PasswordDlg);
  Application.CreateForm(THttpGetForm, HttpGetForm);
  Application.CreateForm(TOnlineMapsForm, OnlineMapsForm);
  Application.CreateForm(TNotificationsForm, NotificationsForm);
  Application.CreateForm(TAddNotificationForm, AddNotificationForm);
  Application.CreateForm(TCrashDetailsDialog, CrashDetailsDialog);
  Application.Run;
end.
