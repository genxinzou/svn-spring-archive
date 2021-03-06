unit PreferencesFormUnit;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ComCtrls, Buttons, Menus, JvExControls, JvComponent,
  JvXPCore, JvXPButtons, JvExStdCtrls, JvEdit, JvValidateEdit, ExtCtrls,
  JvPageList, JvNavigationPane, JvComponentBase, JvTabBar, JvLookOut,
  JvSpeedbar, JvExExtCtrls, JvCaptionPanel, JvExComCtrls, JvComCtrls, JvButton,
  JvMovableBevel, TntStdCtrls, SpTBXEditors, TBXThemes, TBXDkPanels,
  SpTBXControls, SpTBXItem, TBX, SpTBXTabs, TB2Item, VirtualTrees;

type

  TPreferences = record
    Version: Single; // we also need to save program version, since users could copy the old config file over the new one

    ServerIP: string[255];
    ServerPort: string[255];
    TabStyle: Byte;
    TimeStamps: Boolean;

    Username: string[255];
    Password: string[255];

    ConnectOnStartup: Boolean;

    SortStyle: Byte; // 0 - No sorting, 1 - Sort by name, 2 - Sort by status, 3 - Sort by rank, 4 - Sort by country
    BattleSortStyle: Byte; // 0 - Don't sort, 1 - Sort by status (open, full, in-game), 2 - sort by mod, 3 - sort by players, 4 - sort by map, 5 - sort by host, 6 - sort by description
                           // (use BattleSortStyleToColumn/ColumnToBattleSortStyle to transform between battle treeview column index and battle sort style)
    BattleSortDirection: Byte; // 0 - ASCENDING, 1 - DESCDENDING
    FilterJoinLeftMessages: Boolean;
    ShowFlags: Boolean;
    MarkUnknownMaps: Boolean; // if true, all maps which you don't have will be marked in the battle list with red color
    TaskbarButtons: Boolean; // if true, each form has its own taskbar button
    JoinMainChannel: Boolean; // if true, client will join #main once connection is established
    ReconnectToBackup: Boolean; // if true, client will try to connect to next backup host in list if current one fails
    SaveLogs: Boolean; // if true, program will automatically save all channel, private chat and battle logs.
    UseSoundNotifications: Boolean;

    UseProxy: Boolean;
    ProxyAddress: string;
    ProxyPort: Integer;
    ProxyUsername: string;
    ProxyPassword: string;

    HighlightColor: string; // e.g.: 'Green'
    UseNotificationsForHighlights: Boolean; // use notifications when highlighting selected keywords?

    UseIgnoreList: Boolean; // if true, then each time we receive something from a user we check if he is in our ignore list and filter out his message accordingly
    WarnIfUsingNATTraversing: Boolean; // if true, then there will be a warning icon displayed next to each battle in the battle list that uses any form of NAT traversing
  end;

  TPreferencesForm = class(TForm)
    AddressPopupMenu: TSpTBXPopupMenu;
    SpTBXTitleBar1: TSpTBXTitleBar;
    SpTBXTabControl1: TSpTBXTabControl;
    SpTBXTabItem6: TSpTBXTabItem;
    SpTBXTabItem5: TSpTBXTabItem;
    SpTBXTabItem4: TSpTBXTabItem;
    SpTBXTabItem3: TSpTBXTabItem;
    SpTBXTabItem2: TSpTBXTabItem;
    SpTBXTabItem1: TSpTBXTabItem;
    SpTBXTabSheet6: TSpTBXTabSheet;
    GroupBox1: TSpTBXGroupBox;
    AddressButton: TSpTBXSpeedButton;
    Label1: TSpTBXLabel;
    Label2: TSpTBXLabel;
    ServerAddressEdit: TSpTBXEdit;
    JvXPButton1: TJvXPButton;
    ServerPortEdit: TSpTBXEdit;
    CheckBox10: TSpTBXCheckBox;
    CheckBox2: TSpTBXCheckBox;
    CheckBox7: TSpTBXCheckBox;
    SpTBXTabSheet5: TSpTBXTabSheet;
    GroupBox2: TSpTBXGroupBox;
    RegisterAccountButton: TSpTBXSpeedButton;
    Label4: TSpTBXLabel;
    Label5: TSpTBXLabel;
    UsernameEdit: TSpTBXEdit;
    PasswordEdit: TSpTBXEdit;
    SpTBXTabSheet4: TSpTBXTabSheet;
    GroupBox3: TSpTBXGroupBox;
    ResetRegistryButton: TSpTBXSpeedButton;
    GameSettingsButton: TSpTBXSpeedButton;
    NotificationsButton: TSpTBXSpeedButton;
    HighlightingButton: TSpTBXSpeedButton;
    IgnoreListButton: TSpTBXSpeedButton;
    Label6: TSpTBXLabel;
    RadioButton4: TSpTBXRadioButton;
    RadioButton5: TSpTBXRadioButton;
    RadioButton6: TSpTBXRadioButton;
    CheckBox8: TSpTBXCheckBox;
    CheckBox9: TSpTBXCheckBox;
    SpTBXTabSheet3: TSpTBXTabSheet;
    GroupBox4: TSpTBXGroupBox;
    CheckBox1: TSpTBXCheckBox;
    CheckBox3: TSpTBXCheckBox;
    CheckBox4: TSpTBXCheckBox;
    CheckBox5: TSpTBXCheckBox;
    CheckBox6: TSpTBXCheckBox;
    SpTBXTabSheet2: TSpTBXTabSheet;
    GroupBox6: TSpTBXGroupBox;
    UseProxyCheckBox: TSpTBXCheckBox;
    ProxyPanel: TSpTBXPanel;
    ProxyEdit: TSpTBXEdit;
    Label7: TSpTBXLabel;
    Label8: TSpTBXLabel;
    JvProxyPortEdit: TJvValidateEdit;
    ProxyUserEdit: TSpTBXEdit;
    Label9: TSpTBXLabel;
    Label10: TSpTBXLabel;
    ProxyPassEdit: TSpTBXEdit;
    SpTBXTabSheet1: TSpTBXTabSheet;
    GroupBox5: TSpTBXGroupBox;
    SpTBXThemesComboBox: TSpTBXComboBox;
    SpTBXRadioButton1: TSpTBXRadioButton;
    SpTBXRadioButton2: TSpTBXRadioButton;
    Label3: TSpTBXLabel;
    SpTBXRadioButton3: TSpTBXRadioButton;
    Label11: TSpTBXLabel;
    ApplyAndCloseButton: TSpTBXButton;
    CancelAndCloseButton: TSpTBXButton;
    WarnNATCheckBox: TSpTBXCheckBox;

    function BattleSortStyleToColumn(SortStyle: Integer): Integer;
    function ColumnToBattleSortStyle(Column: Integer): Integer;

    procedure UpdatePreferencesTo(var p: TPreferences);
    procedure UpdatePreferencesFrom(p: TPreferences);

    procedure ReadPreferencesRecordFromRegistry(var Preferences: TPreferences);
    procedure ReadPreferencesFromRegistry;
    procedure WritePreferencesToRegistry;

    procedure SaveDefaultBattlePreferencesToRegistry;
    procedure LoadDefaultBattlePreferencesFromRegistry;

    procedure ApplyAndCloseButtonClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure CancelAndCloseButtonClick(Sender: TObject);
    procedure RegisterAccountButtonClick(Sender: TObject);
    procedure AddressButtonClick(Sender: TObject);
    procedure AddressPopupMenuItemClick(Sender: TObject);
    procedure GameSettingsButtonClick(Sender: TObject);
    procedure ResetRegistryButtonClick(Sender: TObject);
    procedure NotificationsButtonClick(Sender: TObject);
    procedure JvXPButton1Click(Sender: TObject);
    procedure UseProxyCheckBoxClick(Sender: TObject);
    procedure HighlightingButtonClick(Sender: TObject);
    procedure SpTBXThemesComboBoxChange(Sender: TObject);
    procedure SpTBXRadioButton1Click(Sender: TObject);
    procedure SpTBXRadioButton2Click(Sender: TObject);
    procedure SpTBXRadioButton3Click(Sender: TObject);
    procedure IgnoreListButtonClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

  TServerListItem =
  record
    Name: string;
    Address: string;
  end;

const
  DEFAULT_PREFERENCES: TPreferences = (ServerIP:'taspringmaster.clan-sy.com'; ServerPort:'8200'; TabStyle:2; TimeStamps: True; Username:''; Password:''; ConnectOnStartup: False; SortStyle: 1; BattleSortStyle: 1; BattleSortDirection: 0; FilterJoinLeftMessages: False; ShowFlags: True; MarkUnknownMaps: True; TaskbarButtons: False; JoinMainChannel: True; ReconnectToBackup: True; SaveLogs: True; UseSoundNotifications: True; UseProxy: False; HighlightColor: 'Green'; UseNotificationsForHighlights: True; UseIgnoreList: False; WarnIfUsingNATTraversing: True);

var
  PreferencesForm: TPreferencesForm;
  Preferences: TPreferences;
  SaveOnClose: Boolean;
  ProgramInitialized: Boolean = False; // if True, then main initialization has been done already. We use it so we don't reinit anything.
  SaveToRegOnExit: Boolean = True; // should we save preferences to registry on exit?

  ServerList: array[0..3] of TServerListItem = (
    (Name: 'Official host (taspringmaster.clan-sy.com)'; Address: 'taspringmaster.clan-sy.com'),
    (Name: 'Backup host (fnord.clan-sy.com)'; Address: 'fnord.clan-sy.com'),
    (Name: 'Test Server (compstomp.de)'; Address: 'compstomp.de'),
    (Name: 'Local server (localhost)'; Address: 'localhost')
  );

implementation

uses Unit1, Misc, ShellAPI, Registry, BattleFormUnit, HostBattleFormUnit, Math,
  OnlineMapsUnit, NotificationsUnit, PerformFormUnit, HighlightingUnit,
  NewAccountUnit, IgnoreListUnit;

{$R *.dfm}

procedure TPreferencesForm.ReadPreferencesRecordFromRegistry(var Preferences: TPreferences);
begin
  try
    try Preferences.ServerIP := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'ServerIP'); except end;
    try Preferences.ServerPort := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'ServerPort'); except end;
    try Preferences.TabStyle := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'TabStyle'); except end;
    try Preferences.TimeStamps := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'TimeStamps'); except end;
    try Preferences.Username := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'Username'); except end;
    try Preferences.Password := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'Password'); except end;
    try Preferences.ConnectOnStartup := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'ConnectOnStartup'); except end;
    try Preferences.SortStyle := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'SortStyle'); except end;
    try Preferences.BattleSortStyle := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'BattleSortStyle'); except end;
    try Preferences.BattleSortDirection := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'BattleSortDirection'); except end;
    try Preferences.FilterJoinLeftMessages := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'FilterJoinLeftMessages'); except end;
    try Preferences.ShowFlags := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'ShowFlags'); except end;
    try Preferences.MarkUnknownMaps := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'MarkUnknownMaps'); except end;
    try Preferences.TaskbarButtons := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'TaskbarButtons'); except end;
    try Preferences.JoinMainChannel := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'JoinMainChannel'); except end;
    try Preferences.ReconnectToBackup := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'ReconnectToBackup'); except end;
    try Preferences.SaveLogs := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'SaveLogs'); except end;
    try Preferences.UseSoundNotifications := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'UseSoundNotifications'); except end;
    try Preferences.UseProxy := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'UseProxy'); except end;
    try Preferences.ProxyAddress := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'ProxyAddress'); except end;
    try Preferences.ProxyPort := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'ProxyPort'); except end;
    try Preferences.ProxyUsername := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'ProxyUsername'); except end;
    try Preferences.ProxyPassword:= Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'ProxyPassword'); except end;
    try Preferences.HighlightColor:= Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'HighlightColor'); except end;
    try Preferences.UseNotificationsForHighlights := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'UseNotificationsForHighlights'); except end;
    try Preferences.UseIgnoreList := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'UseIgnoreList'); except end;
    try Preferences.WarnIfUsingNATTraversing := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'WarnIfUsingNATTraversing'); except end;
  except
  end;
end;

procedure TPreferencesForm.ReadPreferencesFromRegistry;
var
  i: Integer;
begin
  try
    try MainForm.Left := GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\MainForm', 'Left'); except end;
    try MainForm.Top := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\MainForm', 'Top'); except end;
    try MainForm.Width := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\MainForm', 'Width'); except end;
    try MainForm.Height := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\MainForm', 'Height'); except end;
    try MainForm.WindowState := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\MainForm', 'WindowState'); except end;
    try MainForm.Panel2.Width := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\MainForm', 'Splitter1'); except end;
    try MainForm.Panel3.Height := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\MainForm', 'Splitter2'); except end;
    with MainForm.VDTBattles.Header.Columns do
      for i := 0 to Count-1 do
      try
        Items[i].Width := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\MainForm', 'Column' + IntToStr(i));
      except
      end;

    try BattleForm.Left := GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm', 'Left'); except end;
    try BattleForm.Top := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm', 'Top'); except end;
    try BattleForm.Width := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm', 'Width'); except end;
    try BattleForm.Height := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm', 'Height'); except end;
    try BattleForm.WindowState := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm', 'WindowState'); except end;
    try BattleForm.Panel2.Width := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm', 'Splitter1'); except end;
    {-->} AllowBattleDetailsUpdate := False;
    try BattleForm.StartPosRadioGroup.ItemIndex := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm', 'StartPos'); except end;
    try BattleForm.GameEndRadioGroup.ItemIndex := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm', 'GameEnd'); except end;
    try BattleForm.LimitDGunCheckBox.Checked := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm', 'LimitDGun'); except end;
    {-->} AllowBattleDetailsUpdate := True;

    try OnlineMapsForm.Left := GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\OnlineMapsForm', 'Left'); except end;
    try OnlineMapsForm.Top := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\OnlineMapsForm', 'Top'); except end;
    try OnlineMapsForm.Width := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\OnlineMapsForm', 'Width'); except end;
    try OnlineMapsForm.Height := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\OnlineMapsForm', 'Height'); except end;
    try OnlineMapsForm.WindowState := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\OnlineMapsForm', 'WindowState'); except end;

    try NotificationsForm.CheckBox1.Checked := GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\NotificationsForm', 'CheckBox1'); except end;

    try HostBattleForm.PortEdit.Text := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\HostBattleForm', 'Port'); except end;
    try HostBattleForm.TitleEdit.Text := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\HostBattleForm', 'Description'); except end;
    try HostBattleForm.PasswordEdit.Text := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\HostBattleForm', 'Password'); except end;
    try HostBattleForm.ModsComboBox.ItemIndex := Max(0, HostBattleForm.ModsComboBox.Items.IndexOf(Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\HostBattleForm', 'Mod'))); except end;
    try HostBattleForm.NATRadioGroup.ItemIndex := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\HostBattleForm', 'NATTechnique'); except end;

    ReadPreferencesRecordFromRegistry(Preferences);
  except
  end;
end;

procedure TPreferencesForm.WritePreferencesToRegistry;
var
  i: Integer;
  Pl : TWindowPlacement; // used for API call
  R: TRect; // used for wdw pos
begin
  try

    { Calculate window's normal size and position using
      Windows API call - the form's Width, Height, Top and
      Left properties will give maximized window size if
      form is maximized, which is not what we want here }
    Pl.Length := SizeOf(TWindowPlacement);
    GetWindowPlacement(MainForm.Handle, @Pl);
    R := Pl.rcNormalPosition;

    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\MainForm', 'Left', rdInteger, R.Left);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\MainForm', 'Top', rdInteger, R.Top);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\MainForm', 'Width', rdInteger, R.Right-R.Left);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\MainForm', 'Height', rdInteger, R.Bottom-R.Top);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\MainForm', 'WindowState', rdInteger, MainForm.WindowState);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\MainForm', 'Splitter1', rdInteger, MainForm.Panel2.Width);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\MainForm', 'Splitter2', rdInteger, MainForm.Panel3.Height);
    with MainForm.VDTBattles.Header.Columns do
      for i := 0 to Count-1 do Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\MainForm', 'Column' + IntToStr(i), rdInteger, Items[i].Width);

    Pl.Length := SizeOf(TWindowPlacement);
    GetWindowPlacement(BattleForm.Handle, @Pl);
    R := Pl.rcNormalPosition;

    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm', 'Left', rdInteger, R.Left);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm', 'Top', rdInteger, R.Top);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm', 'Width', rdInteger, R.Right-R.Left);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm', 'Height', rdInteger, R.Bottom-R.Top);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm', 'WindowState', rdInteger, BattleForm.WindowState);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm', 'Splitter1', rdInteger, BattleForm.Panel2.Width);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm', 'StartPos', rdInteger, BattleForm.StartPosRadioGroup.ItemIndex);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm', 'GameEnd', rdInteger, BattleForm.GameEndRadioGroup.ItemIndex);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm', 'LimitDGun', rdInteger, Misc.BoolToInt(BattleForm.LimitDGunCheckBox.Checked));

    Pl.Length := SizeOf(TWindowPlacement);
    GetWindowPlacement(OnlineMapsForm.Handle, @Pl);
    R := Pl.rcNormalPosition;

    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\OnlineMapsForm', 'Left', rdInteger, R.Left);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\OnlineMapsForm', 'Top', rdInteger, R.Top);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\OnlineMapsForm', 'Width', rdInteger, R.Right-R.Left);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\OnlineMapsForm', 'Height', rdInteger, R.Bottom-R.Top);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\OnlineMapsForm', 'WindowState', rdInteger, OnlineMapsForm.WindowState);

    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\NotificationsForm', 'CheckBox1', rdInteger, Misc.BoolToInt(NotificationsForm.CheckBox1.Checked));

    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\HostBattleForm', 'Port', rdString, HostBattleForm.PortEdit.Text);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\HostBattleForm', 'Description', rdString, HostBattleForm.TitleEdit.Text);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\HostBattleForm', 'Password', rdString, HostBattleForm.PasswordEdit.Text);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\HostBattleForm', 'Mod', rdString, HostBattleForm.ModsComboBox.Text);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\HostBattleForm', 'NATTechnique', rdInteger, HostBattleForm.NATRadioGroup.ItemIndex);

    // write preferences record to registry:
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'ServerIP', rdString, Preferences.ServerIP);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'ServerPort', rdString, Preferences.ServerPort);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'TabStyle', rdInteger, Preferences.TabStyle);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'TimeStamps', rdInteger, Misc.BoolToInt(Preferences.TimeStamps));
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'Username', rdString, Preferences.Username);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'Password', rdString, Preferences.Password);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'ConnectOnStartup', rdInteger, Misc.BoolToInt(Preferences.ConnectOnStartup));
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'SortStyle', rdInteger, Preferences.SortStyle);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'BattleSortStyle', rdInteger, Preferences.BattleSortStyle);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'BattleSortDirection', rdInteger, Preferences.BattleSortDirection);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'FilterJoinLeftMessages', rdInteger, Misc.BoolToInt(Preferences.FilterJoinLeftMessages));
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'ShowFlags', rdInteger, Misc.BoolToInt(Preferences.ShowFlags));
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'MarkUnknownMaps', rdInteger, Misc.BoolToInt(Preferences.MarkUnknownMaps));
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'TaskbarButtons', rdInteger, Misc.BoolToInt(Preferences.TaskbarButtons));
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'JoinMainChannel', rdInteger, Misc.BoolToInt(Preferences.JoinMainChannel));
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'ReconnectToBackup', rdInteger, Misc.BoolToInt(Preferences.ReconnectToBackup));
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'SaveLogs', rdInteger, Misc.BoolToInt(Preferences.SaveLogs));
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'UseSoundNotifications', rdInteger, Misc.BoolToInt(Preferences.UseSoundNotifications));
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'UseProxy', rdInteger, Misc.BoolToInt(Preferences.UseProxy));
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'ProxyAddress', rdString, Preferences.ProxyAddress);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'ProxyPort', rdinteger, Preferences.ProxyPort);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'ProxyUsername', rdString, Preferences.ProxyUsername);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'ProxyPassword', rdString, Preferences.ProxyPassword);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'HighlightColor', rdString, Preferences.HighlightColor);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'UseNotificationsForHighlights', rdInteger, Preferences.UseNotificationsForHighlights);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'UseIgnoreList', rdInteger, Preferences.UseIgnoreList);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Preferences', 'WarnIfUsingNATTraversing', rdInteger, Preferences.WarnIfUsingNATTraversing);
  except
  end;
end;

procedure TPreferencesForm.SaveDefaultBattlePreferencesToRegistry;
begin
  try
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm\Default', 'StartPos', rdInteger, BattleForm.StartPosRadioGroup.ItemIndex);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm\Default', 'GameEnd', rdInteger, BattleForm.GameEndRadioGroup.ItemIndex);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm\Default', 'LimitDGun', rdInteger, Misc.BoolToInt(BattleForm.LimitDGunCheckBox.Checked));
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm\Default', 'StartMetal', rdInteger, BattleForm.MetalTracker.Value);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm\Default', 'StartEnergy', rdInteger, BattleForm.EnergyTracker.Value);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm\Default', 'MaxUnits', rdInteger, BattleForm.UnitsTracker.Value);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm\Default', 'GhostedBuildings', rdInteger, BattleForm.GhostedBuildingsCheckBox.Checked);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm\Default', 'DiminishingMMs', rdInteger, BattleForm.DiminishingMMsCheckBox.Checked);
    Misc.SetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm\Default', 'LockGameSpeed', rdInteger, BattleForm.LockGameSpeedCheckBox.Checked);
  except
    MessageDlg('Unable to write preferences to windows registry!', mtError, [mbOK], 0);
  end;
end;

procedure TPreferencesForm.LoadDefaultBattlePreferencesFromRegistry;
begin
  try BattleForm.StartPosRadioGroup.ItemIndex := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm\Default', 'StartPos'); except end;
  try BattleForm.GameEndRadioGroup.ItemIndex := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm\Default', 'GameEnd'); except end;
  try BattleForm.LimitDGunCheckBox.Checked := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm\Default', 'LimitDGun'); except end;
  try BattleForm.MetalTracker.Value := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm\Default', 'StartMetal'); except end;
  try BattleForm.EnergyTracker.Value := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm\Default', 'StartEnergy'); except end;
  try BattleForm.UnitsTracker.Value := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm\Default', 'MaxUnits'); except end;
  try BattleForm.GhostedBuildingsCheckBox.Checked := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm\Default', 'GhostedBuildings'); except end;
  try BattleForm.DiminishingMMsCheckBox.Checked := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm\Default', 'DiminishingMMs'); except end;
  try BattleForm.LockGameSpeedCheckBox.Checked := Misc.GetRegistryData(HKEY_CURRENT_USER, '\Software\TASClient\Forms\BattleForm\Default', 'LockGameSpeed'); except end;
end;

procedure TPreferencesForm.ApplyAndCloseButtonClick(Sender: TObject);
begin
  if UsernameEdit.Text = '' then
  begin
    MessageDlg('"Username" field should not be left empty!', mtWarning, [mbOK], 0);
    Exit;
  end;

  if (not VerifyName(UsernameEdit.Text)) or (not VerifyName(PasswordEdit.Text)) then
  begin
    MessageDlg('Your username and/or password consists of illegal characters!', mtWarning, [mbOK], 0);
    Exit;
  end;

  SaveOnClose := True;
  Close;
end;

{ Saves radio buttons/check boxes/etc. states into variable p }
procedure TPreferencesForm.UpdatePreferencesTo(var p: TPreferences);
var
  i: Integer;
  tmp: Boolean;
begin
  p.ServerIP := ServerAddressEdit.Text;
  p.ServerPort := ServerPortEdit.Text;

  if RadioButton4.Checked then i := 0
  else if RadioButton5.Checked then i := 1
  else if RadioButton6.Checked then i := 2
  else i := 0; // this should not happen!
  p.TabStyle := i;

  p.TimeStamps := CheckBox1.Checked;

  p.Username := UsernameEdit.Text;
  p.Password := PasswordEdit.Text;

  p.ConnectOnStartup := CheckBox2.Checked;

  // SortSytle and BattleSortSytles get updated automatically (in Preferences) when we select certain sorting sytle

  p.FilterJoinLeftMessages := CheckBox3.Checked;
  p.ShowFlags := CheckBox4.Checked;
  p.MarkUnknownMaps := CheckBox5.Checked;
  p.TaskbarButtons := CheckBox6.Checked;
  p.JoinMainChannel := CheckBox7.Checked;
  p.ReconnectToBackup := CheckBox10.Checked;
  tmp := p.SaveLogs;
  p.SaveLogs := CheckBox8.Checked;
  p.UseSoundNotifications := CheckBox9.Checked;

  p.UseProxy := UseProxyCheckBox.Checked;
  p.ProxyAddress := ProxyEdit.Text;
  p.ProxyPort := JvProxyPortEdit.Value;
  p.ProxyUsername := ProxyUserEdit.Text;
  p.ProxyPassword := ProxyPassEdit.Text;

  p.HighlightColor := HighlightingForm.JvColorComboBox1.Text;
  p.UseNotificationsForHighlights := HighlightingForm.UseNotificationsCheckBox.Checked;

  p.UseIgnoreList := IgnoreListForm.EnableIgnoresCheckBox.Checked;
  p.WarnIfUsingNATTraversing := WarnNATCheckBox.Checked;

  // update changes:
  MainForm.VDTBattles.Invalidate;
  MainForm.UpdateClientsListBox;
  if tmp <> p.SaveLogs then if p.SaveLogs then MainForm.OpenAllLogs else MainForm.CloseAllLogs;
end;

{ applies preferences from variable p to VCL controls (radio buttons, check boxes, etc.) }
procedure TPreferencesForm.UpdatePreferencesFrom(p: TPreferences);
begin
  ServerAddressEdit.Text := p.ServerIP;
  ServerPortEdit.Text := p.ServerPort;

  if p.TabStyle = 0 then RadioButton4.Checked := True
  else if p.TabStyle = 1 then RadioButton5.Checked := True
  else if p.TabStyle = 2 then RadioButton6.Checked := True
  else RadioButton4.Checked := True; // this should not happen!

  CheckBox1.Checked := p.TimeStamps;

  MainForm.PageControl1.Style := TTabStyle(p.TabStyle);

  UsernameEdit.Text := p.Username;
  PasswordEdit.Text := p.Password;

  CheckBox2.Checked := p.ConnectOnStartup;

  MainForm.SortPopupMenu.Items[p.SortStyle].Checked := True;
  MainForm.VDTBattles.Header.SortColumn := BattleSortStyleToColumn(p.BattleSortStyle);
  if p.BattleSortDirection = 0 then MainForm.VDTBattles.Header.SortDirection := sdAscending else MainForm.VDTBattles.Header.SortDirection := sdDescending;

  CheckBox3.Checked := p.FilterJoinLeftMessages;
  CheckBox4.Checked := p.ShowFlags;
  CheckBox5.Checked := p.MarkUnknownMaps;
  CheckBox6.Checked := p.TaskbarButtons;
  CheckBox7.Checked := p.JoinMainChannel;
  CheckBox10.Checked := p.ReconnectToBackup;
  CheckBox8.Checked := p.SaveLogs;
  CheckBox9.Checked := p.UseSoundNotifications;

  UseProxyCheckBox.Checked := p.UseProxy;
  ProxyEdit.Text := p.ProxyAddress;
  JvProxyPortEdit.Value := p.ProxyPort;
  ProxyUserEdit.Text := p.ProxyUsername;
  ProxyPassEdit.Text := p.ProxyPassword;

  HighlightingForm.JvColorComboBox1.ItemIndex := Max(0, HighlightingForm.JvColorComboBox1.Items.IndexOf(p.HighlightColor));
  HighlightingForm.UseNotificationsCheckBox.Checked := p.UseNotificationsForHighlights;

  IgnoreListForm.EnableIgnoresCheckBox.Checked := p.UseIgnoreList;
  WarnNATCheckBox.Checked := p.WarnIfUsingNATTraversing;
end;

procedure TPreferencesForm.FormCreate(Sender: TObject);
var
  i: Integer;
begin
  SpTBXTabControl1.ActiveTabIndex := 0;

  // let's populate popup menu with server descriptions:
  for i := 0 to High(ServerList) do
  begin
    AddressPopupMenu.Items.Add(TSpTBXItem.Create(AddressPopupMenu.Items.Owner));
    with AddressPopupMenu.Items[AddressPopupMenu.Items.Count-1] as TSpTBXItem do
    begin
      Caption := ServerList[i].Name;
      Tag := i;
      OnClick := AddressPopupMenuItemClick;
    end;
  end;

  // let's populate TBX themes:
  GetAvailableTBXThemes(SpTBXThemesComboBox.Items.AnsiStrings);
  // we will change theme and set ThemeType-s after initilization
  // since at this point not all forms have been created yet
  // so the changes would not be applied to all forms!
  
end;

procedure TPreferencesForm.FormShow(Sender: TObject);
begin
  SaveOnClose := False;
end;

procedure TPreferencesForm.FormClose(Sender: TObject;
  var Action: TCloseAction);
begin
  ActiveControl := nil;
  { Wonder why we need ActiveControl := nil ? We need it in this case: if focus was on the radio button
    when we closed the form (by clicking the 'X' so that focus remained on radio button), then that same
    radio button would get automatically selected when form is shown and so ignoring any changes we made
    in OnShow event to Checked properties of any of the radio buttons. If you call TRadioButton.SetFocus,
    it will automatically set it's Checked property to true. }

  if SaveOnClose then
  begin
    UpdatePreferencesTo(Preferences); // save changes to p
    UpdatePreferencesFrom(Preferences); // update changes from p
  end
  else
  begin
    UpdatePreferencesFrom(Preferences);
  end;
end;

procedure TPreferencesForm.CancelAndCloseButtonClick(Sender: TObject);
begin
  SaveOnClose := False;
  Close;
end;

procedure TPreferencesForm.RegisterAccountButtonClick(Sender: TObject);
begin
  if Status.ConnectionState = Connected then
  begin
    MessageDlg('You are not allowed to register new account while being logged-in!', mtError, [mbOK], 0);
    Exit;
  end;

  NewAccountForm.UsernameEdit.Text := UsernameEdit.Text;
  NewAccountForm.PasswordEdit.Text := '';
  NewAccountForm.ConfirmPasswordEdit.Text := '';

  if NewAccountForm.ShowModal = mrCancel then Exit;

  // now let's start account registration process:

  UsernameEdit.Text := NewAccountForm.UsernameEdit.Text;
  PasswordEdit.Text :=  NewAccountForm.PasswordEdit.Text;

  PostMessage(MainForm.Handle, WM_DOREGISTER, 0, 0);
  ApplyAndCloseButton.OnClick(nil);
end;

procedure TPreferencesForm.AddressButtonClick(Sender: TObject);
var
  p: TPoint;
begin
  with AddressButton do
    p := ClientToScreen(Point(0, 0));

  AddressPopupMenu.Popup(p.x, p.y);
end;

procedure TPreferencesForm.AddressPopupMenuItemClick(Sender: TObject);
begin
  ServerAddressEdit.Text := ServerList[(Sender as TSpTBXItem).Tag].Address;
end;

procedure TPreferencesForm.GameSettingsButtonClick(Sender: TObject);
var
  tmp: Integer;
begin
  tmp := ShellExecute(MainForm.Handle, 'open', PChar(ExtractFilePath(Application.ExeName) + 'settings.exe'), '', PChar(ExtractFilePath(Application.ExeName)), SW_SHOWNORMAL);
  if (tmp > 0) and (tmp < 32) then MessageDlg('Couldn''t find settings.exe!', mtWarning, [mbOK], 0);
end;

procedure TPreferencesForm.ResetRegistryButtonClick(Sender: TObject);
begin
  if MessageDlg('This will remove all TASClient data from registry. Do you wish to continue?', mtConfirmation, [mbYes, mbNo], 0) = mrNo then Exit;
  RemoveRegistryKey(HKEY_CURRENT_USER, '\Software\TASClient');
  SaveToRegOnExit := False;
end;

procedure TPreferencesForm.NotificationsButtonClick(Sender: TObject);
begin
  NotificationsForm.ShowModal;
end;

procedure TPreferencesForm.JvXPButton1Click(Sender: TObject);
begin
  PerformForm.ShowModal;
end;

procedure TPreferencesForm.UseProxyCheckBoxClick(Sender: TObject);
begin
  If UseProxyCheckBox.Checked then EnableControlAndChildren(ProxyPanel)
  else DisableControlAndChildren(ProxyPanel);
end;

procedure TPreferencesForm.HighlightingButtonClick(Sender: TObject);
begin
  HighlightingForm.ShowModal;
end;

procedure TPreferencesForm.SpTBXThemesComboBoxChange(Sender: TObject);
begin
  TBXSetTheme(SpTBXThemesComboBox.Items[SpTBXThemesComboBox.ItemIndex]);
end;

procedure TPreferencesForm.SpTBXRadioButton1Click(Sender: TObject);
var
  i: Integer;
begin
  for i := 0 to Screen.FormCount-1 do
    SpChangeThemeType(Screen.Forms[i], thtNone);
end;

procedure TPreferencesForm.SpTBXRadioButton2Click(Sender: TObject);
var
  i: Integer;
begin
  for i := 0 to Screen.FormCount-1 do
    SpChangeThemeType(Screen.Forms[i], thtWindows);
end;

procedure TPreferencesForm.SpTBXRadioButton3Click(Sender: TObject);
var
  i: Integer;
begin
  for i := 0 to Screen.FormCount-1 do
    SpChangeThemeType(Screen.Forms[i], thtTBX);
end;

procedure TPreferencesForm.IgnoreListButtonClick(Sender: TObject);
begin
  IgnoreListForm.ShowModal;
end;

// "Column" is battle treeview's column index, result is corresponding BattleSortStyle value
function TPreferencesForm.ColumnToBattleSortStyle(Column: Integer): Integer;
begin
  case Column of
    -1: Result := 0; // no sorting
    0: Result := 0; // no sorting
    1: Result := 6; // sort by description
    2: Result := 5; // sort by host
    3: Result := 4; // sort by map
    4: Result := 1; // sort by state
    5: Result := 2; // sort by mod
    6: Result := 3; // sort by players
  end; // case
end;

// "SortStyle" is battle sort style value, result is corresponding battle treeview's column index
function TPreferencesForm.BattleSortStyleToColumn(SortStyle: Integer): Integer;
begin
  case SortStyle of
    0: Result := -1; // no sorting
    1: Result := 4; // sort by state
    2: Result := 5; // sort by mod
    3: Result := 6; // sort by players
    4: Result := 3; // sort by map
    5: Result := 2; // sort by host
    6: Result := 1; // sort by description
  end; // case
end;

end.
