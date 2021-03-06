unit HelpUnit;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls, ComCtrls, JvExControls, JvComponent,
  JvXPCore, JvXPButtons, SpTBXTabs, TB2Item, TBX, SpTBXItem, TBXDkPanels,
  SpTBXControls;

type
  THelpForm = class(TForm)
    SpTBXTitleBar1: TSpTBXTitleBar;
    PageControl1: TSpTBXTabControl;
    SpTBXTabItem3: TSpTBXTabItem;
    SpTBXTabItem2: TSpTBXTabItem;
    SpTBXTabItem1: TSpTBXTabItem;
    SpTBXTabSheet3: TSpTBXTabSheet;
    Image1: TImage;
    Image4: TImage;
    Image8: TImage;
    Image2: TImage;
    Image3: TImage;
    Image6: TImage;
    Image7: TImage;
    Image9: TImage;
    Label4: TSpTBXLabel;
    Label66: TSpTBXLabel;
    Label29: TSpTBXLabel;
    Label3: TSpTBXLabel;
    Label2: TSpTBXLabel;
    Label1: TSpTBXLabel;
    Label8: TSpTBXLabel;
    Label5: TSpTBXLabel;
    Label6: TSpTBXLabel;
    Label7: TSpTBXLabel;
    Label9: TSpTBXLabel;
    Label10: TSpTBXLabel;
    Label11: TSpTBXLabel;
    Label12: TSpTBXLabel;
    Label13: TSpTBXLabel;
    Label14: TSpTBXLabel;
    Label59: TSpTBXLabel;
    Label60: TSpTBXLabel;
    Label61: TSpTBXLabel;
    Label62: TSpTBXLabel;
    Label63: TSpTBXLabel;
    Label64: TSpTBXLabel;
    Label65: TSpTBXLabel;
    Label71: TSpTBXLabel;
    Label15: TSpTBXLabel;
    Label16: TSpTBXLabel;
    Label56: TSpTBXLabel;
    Label57: TSpTBXLabel;
    Label17: TSpTBXLabel;
    SpTBXTabSheet1: TSpTBXTabSheet;
    Label18: TSpTBXLabel;
    Label19: TSpTBXLabel;
    Label20: TSpTBXLabel;
    Label21: TSpTBXLabel;
    Label34: TSpTBXLabel;
    Label22: TSpTBXLabel;
    Label23: TSpTBXLabel;
    Label24: TSpTBXLabel;
    Label25: TSpTBXLabel;
    Label26: TSpTBXLabel;
    Label27: TSpTBXLabel;
    Label28: TSpTBXLabel;
    Label67: TSpTBXLabel;
    Label68: TSpTBXLabel;
    Label44: TSpTBXLabel;
    Label69: TSpTBXLabel;
    Label70: TSpTBXLabel;
    SpTBXTabSheet2: TSpTBXTabSheet;
    Image5: TImage;
    Label30: TSpTBXLabel;
    Label31: TSpTBXLabel;
    Label32: TSpTBXLabel;
    Label33: TSpTBXLabel;
    Label40: TSpTBXLabel;
    Label35: TSpTBXLabel;
    Label36: TSpTBXLabel;
    Label37: TSpTBXLabel;
    Label38: TSpTBXLabel;
    Label39: TSpTBXLabel;
    Label58: TSpTBXLabel;
    Label50: TSpTBXLabel;
    Label45: TSpTBXLabel;
    Label46: TSpTBXLabel;
    Label47: TSpTBXLabel;
    Label48: TSpTBXLabel;
    Label49: TSpTBXLabel;
    Label42: TSpTBXLabel;
    Label41: TSpTBXLabel;
    Button3: TSpTBXButton;
    Button1: TSpTBXButton;
    Button2: TSpTBXButton;
    SpTBXLabel1: TSpTBXLabel;
    Label72: TSpTBXLabel;
    Image10: TImage;
    Label43: TSpTBXLabel;
    SpTBXLabel2: TSpTBXLabel;
    Image11: TImage;
    SpTBXLabel3: TSpTBXLabel;
    SpTBXLabel4: TSpTBXLabel;
    SpTBXLabel5: TSpTBXLabel;
    SpTBXLabel6: TSpTBXLabel;
    SpTBXLabel7: TSpTBXLabel;
    SpTBXLabel8: TSpTBXLabel;
    SpTBXLabel9: TSpTBXLabel;
    SpTBXTabItem4: TSpTBXTabItem;
    SpTBXTabSheet4: TSpTBXTabSheet;
    Image12: TImage;
    Image13: TImage;
    Image14: TImage;
    SpTBXLabel10: TSpTBXLabel;
    SpTBXLabel11: TSpTBXLabel;
    SpTBXLabel12: TSpTBXLabel;
    SpTBXLabel13: TSpTBXLabel;
    SpTBXLabel14: TSpTBXLabel;
    procedure Button1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure JvXPButton1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  HelpForm: THelpForm;

implementation

uses MainUnit, Misc;

{$R *.dfm}

procedure THelpForm.Button1Click(Sender: TObject);
begin
  Close;
end;

procedure THelpForm.FormCreate(Sender: TObject);
begin
  PageControl1.ActiveTabIndex := 0;
  if not SpTBXTitleBar1.Active then
    RemoveSpTBXTitleBarMarges(self);
end;

procedure THelpForm.Button3Click(Sender: TObject);
begin
  PageControl1.ActiveTabIndex := (PageControl1.ActiveTabIndex + 1) mod PageControl1.PagesCount;
end;

procedure THelpForm.Button2Click(Sender: TObject);
begin
  if PageControl1.ActiveTabIndex = 0 then PageControl1.ActiveTabIndex := PageControl1.PagesCount-1
  else PageControl1.ActiveTabIndex := PageControl1.ActiveTabIndex - 1;
end;

procedure THelpForm.JvXPButton1Click(Sender: TObject);
begin
  Misc.OpenURLInDefaultBrowser(WIKI_PAGE_LINK);
end;

end.
