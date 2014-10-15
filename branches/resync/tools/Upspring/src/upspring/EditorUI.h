// generated by Fast Light User Interface Designer (fluid) version 2.1000

#ifndef EditorUI_h
#define EditorUI_h
#include "EditorIncl.h"
#include "EditorDef.h"
#include "Tools.h"
#include "Model.h"
#include "AnimationUI.h"
#include <fltk/Window.h>
#include "View.h"
#include <fltk/MenuBar.h>
#include <fltk/PopupMenu.h>
#include <fltk/Item.h>

class MappingUI  {
public:
  fltk::Window* CreateUI();
  fltk::Window *window;
    UVViewWindow *view;
private:
        inline void cb_Flip_i(fltk::Item*, void*);
        static void cb_Flip(fltk::Item*, void*);
        inline void cb_Mirror_i(fltk::Item*, void*);
        static void cb_Mirror(fltk::Item*, void*);
#include "MappingCB.h"
};
#include <fltk/ReturnButton.h>
#include <fltk/Button.h>
#include <fltk/Input.h>
#include <fltk/file_chooser.h>
#include <fltk/Browser.h>

class ArchiveListUI  {
public:
  ArchiveListUI(const ArchiveList& s);
  fltk::Window *window;
private:
    inline void cb_OK_i(fltk::ReturnButton*, void*);
    static void cb_OK(fltk::ReturnButton*, void*);
    inline void cb_Cancel_i(fltk::Button*, void*);
    static void cb_Cancel(fltk::Button*, void*);
    fltk::Input *edit;
    inline void cb_Browse_i(fltk::Button*, void*);
    static void cb_Browse(fltk::Button*, void*);
public:
    fltk::Browser *archlist;
private:
    inline void cb_archlist_i(fltk::Browser*, void*);
    static void cb_archlist(fltk::Browser*, void*);
    inline void cb_Remove_i(fltk::Button*, void*);
    static void cb_Remove(fltk::Button*, void*);
    inline void cb_Add_i(fltk::Button*, void*);
    static void cb_Add(fltk::Button*, void*);
public:
  ArchiveList settings;
  ~ArchiveListUI();
private:
  fltk::Widget* selected();
};
#include <fltk/TiledGroup.h>
#include "TextureBrowser.h"
#include <fltk/Choice.h>

class TexGroupUI  {
public:
  fltk::Window* CreateUI();
  fltk::Window *window;
private:
    inline void cb_Add1_i(fltk::Button*, void*);
    static void cb_Add1(fltk::Button*, void*);
public:
      TextureBrowser *groupTexBrowser;
      TextureBrowser *texBrowser;
    fltk::Choice *groups;
private:
    inline void cb_groups_i(fltk::Choice*, void*);
    static void cb_groups(fltk::Choice*, void*);
    inline void cb_Set_i(fltk::Button*, void*);
    static void cb_Set(fltk::Button*, void*);
    inline void cb_Remove1_i(fltk::Button*, void*);
    static void cb_Remove1(fltk::Button*, void*);
    inline void cb_Add2_i(fltk::Button*, void*);
    static void cb_Add2(fltk::Button*, void*);
    inline void cb_Close_i(fltk::Button*, void*);
    static void cb_Close(fltk::Button*, void*);
    inline void cb_Remove2_i(fltk::Button*, void*);
    static void cb_Remove2(fltk::Button*, void*);
    inline void cb_Select_i(fltk::Button*, void*);
    static void cb_Select(fltk::Button*, void*);
    inline void cb_Load_i(fltk::Button*, void*);
    static void cb_Load(fltk::Button*, void*);
    inline void cb_Save_i(fltk::Button*, void*);
    static void cb_Save(fltk::Button*, void*);
#include "TexGroupCB.h"
};
#include <fltk/TabGroup.h>
#include <fltk/Group.h>
#include <fltk/CheckButton.h>

class TexBuilderUI  {
public:
  fltk::Window* CreateUI();
  fltk::Window *window;
          fltk::Input *selfIllumTex;
private:
          inline void cb_Browse1_i(fltk::Button*, void*);
          static void cb_Browse1(fltk::Button*, void*);
          inline void cb_Browse2_i(fltk::Button*, void*);
          static void cb_Browse2(fltk::Button*, void*);
public:
          fltk::Input *reflectTex;
          fltk::Input *output2;
private:
          inline void cb_Browse3_i(fltk::Button*, void*);
          static void cb_Browse3(fltk::Button*, void*);
        inline void cb_Build_i(fltk::Button*, void*);
        static void cb_Build(fltk::Button*, void*);
public:
          fltk::Input *colorTex;
          fltk::Input *teamColorTex;
private:
          inline void cb_Browse4_i(fltk::Button*, void*);
          static void cb_Browse4(fltk::Button*, void*);
          inline void cb_Browse5_i(fltk::Button*, void*);
          static void cb_Browse5(fltk::Button*, void*);
public:
          fltk::CheckButton *invertTeamCol;
          fltk::Input *output1;
private:
          inline void cb_Browse6_i(fltk::Button*, void*);
          static void cb_Browse6(fltk::Button*, void*);
        inline void cb_Build1_i(fltk::Button*, void*);
        static void cb_Build1(fltk::Button*, void*);
#include "TexBuilderCB.h"
};
#include <fltk/NumericInput.h>

class RotatorUI  {
public:
  void ApplyRotation(bool absolute, int axis, fltk::Input* o);
  fltk::Window* CreateUI(IEditor *editor);
  fltk::Window *window;
      fltk::NumericInput *inputAbsX;
private:
      inline void cb_inputAbsX_i(fltk::NumericInput*, void*);
      static void cb_inputAbsX(fltk::NumericInput*, void*);
public:
      fltk::NumericInput *inputAbsY;
private:
      inline void cb_inputAbsY_i(fltk::NumericInput*, void*);
      static void cb_inputAbsY(fltk::NumericInput*, void*);
public:
      fltk::NumericInput *inputAbsZ;
private:
      inline void cb_inputAbsZ_i(fltk::NumericInput*, void*);
      static void cb_inputAbsZ(fltk::NumericInput*, void*);
public:
      fltk::NumericInput *inputRelX;
private:
      inline void cb_inputRelX_i(fltk::NumericInput*, void*);
      static void cb_inputRelX(fltk::NumericInput*, void*);
public:
      fltk::NumericInput *inputRelY;
private:
      inline void cb_inputRelY_i(fltk::NumericInput*, void*);
      static void cb_inputRelY(fltk::NumericInput*, void*);
public:
      fltk::NumericInput *inputRelZ;
private:
      inline void cb_inputRelZ_i(fltk::NumericInput*, void*);
      static void cb_inputRelZ(fltk::NumericInput*, void*);
    inline void cb_Reset_i(fltk::Button*, void*);
    static void cb_Reset(fltk::Button*, void*);
    inline void cb_Apply_i(fltk::Button*, void*);
    static void cb_Apply(fltk::Button*, void*);
  IEditor *editorCallback;
public:
  void Update();
  void Show();
  ~RotatorUI();
  void Hide();
  void ResetRotation();
  void ApplyRotationToGeom();
};
#include <fltk/InvisibleBox.h>
#include <fltk/Output.h>
#include "ViewsGroup.h"
#include <fltk/MultiBrowser.h>
#include <fltk/FileInput.h>
#include <fltk/ProgressBar.h>
#include <fltk/Divider.h>
#include <fltk/ItemGroup.h>
#include "ModelDrawer.h"

class EditorUI  {
public:
  EditorUI();
  fltk::Window *window;
private:
  inline void cb_window_i(fltk::Window*, void*);
  static void cb_window(fltk::Window*, void*);
public:
    fltk::Group *toolbox;
      fltk::Button *selectCameraTool;
private:
      inline void cb_selectCameraTool_i(fltk::Button*, void*);
      static void cb_selectCameraTool(fltk::Button*, void*);
public:
      fltk::Button *selectMoveTool;
private:
      inline void cb_selectMoveTool_i(fltk::Button*, void*);
      static void cb_selectMoveTool(fltk::Button*, void*);
public:
      fltk::Button *selectRotateTool;
private:
      inline void cb_selectRotateTool_i(fltk::Button*, void*);
      static void cb_selectRotateTool(fltk::Button*, void*);
public:
      fltk::Button *selectScaleTool;
private:
      inline void cb_selectScaleTool_i(fltk::Button*, void*);
      static void cb_selectScaleTool(fltk::Button*, void*);
public:
      fltk::Button *selectTextureTool;
private:
      inline void cb_selectTextureTool_i(fltk::Button*, void*);
      static void cb_selectTextureTool(fltk::Button*, void*);
public:
      fltk::Button *selectColorTool;
private:
      inline void cb_selectColorTool_i(fltk::Button*, void*);
      static void cb_selectColorTool(fltk::Button*, void*);
public:
      fltk::Button *selectFlipTool;
private:
      inline void cb_selectFlipTool_i(fltk::Button*, void*);
      static void cb_selectFlipTool(fltk::Button*, void*);
public:
      fltk::Button *selectRotateTexTool;
private:
      inline void cb_selectRotateTexTool_i(fltk::Button*, void*);
      static void cb_selectRotateTexTool(fltk::Button*, void*);
public:
    fltk::Output *status;
    ViewsGroup *viewsGroup;
        fltk::NumericInput *inputCenterX;
private:
        inline void cb_inputCenterX_i(fltk::NumericInput*, void*);
        static void cb_inputCenterX(fltk::NumericInput*, void*);
public:
        fltk::NumericInput *inputCenterY;
private:
        inline void cb_inputCenterY_i(fltk::NumericInput*, void*);
        static void cb_inputCenterY(fltk::NumericInput*, void*);
public:
        fltk::NumericInput *inputCenterZ;
private:
        inline void cb_inputCenterZ_i(fltk::NumericInput*, void*);
        static void cb_inputCenterZ(fltk::NumericInput*, void*);
        inline void cb_Estimate_i(fltk::Button*, void*);
        static void cb_Estimate(fltk::Button*, void*);
public:
        fltk::NumericInput *inputRadius;
private:
        inline void cb_inputRadius_i(fltk::NumericInput*, void*);
        static void cb_inputRadius(fltk::NumericInput*, void*);
        inline void cb_Estimate1_i(fltk::Button*, void*);
        static void cb_Estimate1(fltk::Button*, void*);
public:
        fltk::NumericInput *inputHeight;
private:
        inline void cb_inputHeight_i(fltk::NumericInput*, void*);
        static void cb_inputHeight(fltk::NumericInput*, void*);
public:
      fltk::Group *objectView;
        fltk::MultiBrowser *objectTree;
private:
        inline void cb_Delete_i(fltk::Button*, void*);
        static void cb_Delete(fltk::Button*, void*);
        inline void cb_Add3_i(fltk::Button*, void*);
        static void cb_Add3(fltk::Button*, void*);
        inline void cb_Copy_i(fltk::Button*, void*);
        static void cb_Copy(fltk::Button*, void*);
        inline void cb_Cut_i(fltk::Button*, void*);
        static void cb_Cut(fltk::Button*, void*);
        inline void cb_Paste_i(fltk::Button*, void*);
        static void cb_Paste(fltk::Button*, void*);
        inline void cb_Apply1_i(fltk::Button*, void*);
        static void cb_Apply1(fltk::Button*, void*);
        inline void cb_Uniform_i(fltk::Button*, void*);
        static void cb_Uniform(fltk::Button*, void*);
        inline void cb_Set1_i(fltk::Button*, void*);
        static void cb_Set1(fltk::Button*, void*);
public:
        fltk::NumericInput *inputPosX;
private:
        inline void cb_inputPosX_i(fltk::NumericInput*, void*);
        static void cb_inputPosX(fltk::NumericInput*, void*);
public:
        fltk::NumericInput *inputPosY;
private:
        inline void cb_inputPosY_i(fltk::NumericInput*, void*);
        static void cb_inputPosY(fltk::NumericInput*, void*);
public:
        fltk::NumericInput *inputPosZ;
private:
        inline void cb_inputPosZ_i(fltk::NumericInput*, void*);
        static void cb_inputPosZ(fltk::NumericInput*, void*);
public:
        fltk::NumericInput *inputScaleX;
private:
        inline void cb_inputScaleX_i(fltk::NumericInput*, void*);
        static void cb_inputScaleX(fltk::NumericInput*, void*);
public:
        fltk::NumericInput *inputScaleY;
private:
        inline void cb_inputScaleY_i(fltk::NumericInput*, void*);
        static void cb_inputScaleY(fltk::NumericInput*, void*);
public:
        fltk::NumericInput *inputScaleZ;
private:
        inline void cb_inputScaleZ_i(fltk::NumericInput*, void*);
        static void cb_inputScaleZ(fltk::NumericInput*, void*);
public:
        fltk::NumericInput *inputRotX;
private:
        inline void cb_inputRotX_i(fltk::NumericInput*, void*);
        static void cb_inputRotX(fltk::NumericInput*, void*);
public:
        fltk::NumericInput *inputRotZ;
private:
        inline void cb_inputRotZ_i(fltk::NumericInput*, void*);
        static void cb_inputRotZ(fltk::NumericInput*, void*);
public:
        fltk::NumericInput *inputRotY;
private:
        inline void cb_inputRotY_i(fltk::NumericInput*, void*);
        static void cb_inputRotY(fltk::NumericInput*, void*);
        inline void cb_Swap_i(fltk::Button*, void*);
        static void cb_Swap(fltk::Button*, void*);
        inline void cb_Rotator_i(fltk::Button*, void*);
        static void cb_Rotator(fltk::Button*, void*);
public:
        fltk::Choice *mappingChooser;
private:
          inline void cb_S3O_i(fltk::Item*, void*);
          static void cb_S3O(fltk::Item*, void*);
          inline void cb_3DO_i(fltk::Item*, void*);
          static void cb_3DO(fltk::Item*, void*);
public:
        fltk::Group *texGroupS3O;
          fltk::FileInput *inputTexture1;
private:
          inline void cb_inputTexture1_i(fltk::FileInput*, void*);
          static void cb_inputTexture1(fltk::FileInput*, void*);
public:
          fltk::FileInput *inputTexture2;
private:
          inline void cb_inputTexture2_i(fltk::FileInput*, void*);
          static void cb_inputTexture2(fltk::FileInput*, void*);
          inline void cb_Browse7_i(fltk::Button*, void*);
          static void cb_Browse7(fltk::Button*, void*);
          inline void cb_Browse8_i(fltk::Button*, void*);
          static void cb_Browse8(fltk::Button*, void*);
          inline void cb_Reload_i(fltk::Button*, void*);
          static void cb_Reload(fltk::Button*, void*);
          inline void cb_Reload1_i(fltk::Button*, void*);
          static void cb_Reload1(fltk::Button*, void*);
          inline void cb_Example_i(fltk::Button*, void*);
          static void cb_Example(fltk::Button*, void*);
            inline void cb_Full_i(fltk::Item*, void*);
            static void cb_Full(fltk::Item*, void*);
            inline void cb_Basic_i(fltk::Item*, void*);
            static void cb_Basic(fltk::Item*, void*);
            inline void cb_Texture_i(fltk::Item*, void*);
            static void cb_Texture(fltk::Item*, void*);
            inline void cb_Texture1_i(fltk::Item*, void*);
            static void cb_Texture1(fltk::Item*, void*);
public:
        fltk::Group *texGroup3DO;
        TextureBrowser *texBrowser;
        fltk::Choice *textureGroupMenu;
private:
        inline void cb_textureGroupMenu_i(fltk::Choice*, void*);
        static void cb_textureGroupMenu(fltk::Choice*, void*);
        inline void cb_Add4_i(fltk::Button*, void*);
        static void cb_Add4(fltk::Button*, void*);
public:
    fltk::ProgressBar *progress;
    fltk::MenuBar *menu;
      fltk::PopupMenu *menuFile;
private:
        inline void cb_New_i(fltk::Item*, void*);
        static void cb_New(fltk::Item*, void*);
        inline void cb_Load1_i(fltk::Item*, void*);
        static void cb_Load1(fltk::Item*, void*);
        inline void cb_Save1_i(fltk::Item*, void*);
        static void cb_Save1(fltk::Item*, void*);
        inline void cb_Save2_i(fltk::Item*, void*);
        static void cb_Save2(fltk::Item*, void*);
        inline void cb_Exit_i(fltk::Item*, void*);
        static void cb_Exit(fltk::Item*, void*);
public:
      fltk::PopupMenu *menuObject;
private:
        inline void cb_Insert_i(fltk::Item*, void*);
        static void cb_Insert(fltk::Item*, void*);
        inline void cb_Save3_i(fltk::Item*, void*);
        static void cb_Save3(fltk::Item*, void*);
        inline void cb_Replace_i(fltk::Item*, void*);
        static void cb_Replace(fltk::Item*, void*);
        inline void cb_Merge_i(fltk::Item*, void*);
        static void cb_Merge(fltk::Item*, void*);
        inline void cb_Approximate_i(fltk::Item*, void*);
        static void cb_Approximate(fltk::Item*, void*);
          inline void cb_object_i(fltk::Item*, void*);
          static void cb_object(fltk::Item*, void*);
          inline void cb_object1_i(fltk::Item*, void*);
          static void cb_object1(fltk::Item*, void*);
          inline void cb_object2_i(fltk::Item*, void*);
          static void cb_object2(fltk::Item*, void*);
        inline void cb_Recalculate_i(fltk::Item*, void*);
        static void cb_Recalculate(fltk::Item*, void*);
        inline void cb_Flip1_i(fltk::Item*, void*);
        static void cb_Flip1(fltk::Item*, void*);
        inline void cb_Export_i(fltk::Item*, void*);
        static void cb_Export(fltk::Item*, void*);
        inline void cb_Import_i(fltk::Item*, void*);
        static void cb_Import(fltk::Item*, void*);
        inline void cb_Show_i(fltk::Item*, void*);
        static void cb_Show(fltk::Item*, void*);
        inline void cb_Show1_i(fltk::Item*, void*);
        static void cb_Show1(fltk::Item*, void*);
        inline void cb_Texture2_i(fltk::Item*, void*);
        static void cb_Texture2(fltk::Item*, void*);
        inline void cb_Texture3_i(fltk::Item*, void*);
        static void cb_Texture3(fltk::Item*, void*);
        inline void cb_Save4_i(fltk::Item*, void*);
        static void cb_Save4(fltk::Item*, void*);
        inline void cb_Revert_i(fltk::Item*, void*);
        static void cb_Revert(fltk::Item*, void*);
        inline void cb_Set2_i(fltk::Item*, void*);
        static void cb_Set2(fltk::Item*, void*);
        inline void cb_Set3_i(fltk::Item*, void*);
        static void cb_Set3(fltk::Item*, void*);
public:
      fltk::PopupMenu *menuAnimation;
private:
        inline void cb_Show2_i(fltk::Item*, void*);
        static void cb_Show2(fltk::Item*, void*);
        inline void cb_Show3_i(fltk::Item*, void*);
        static void cb_Show3(fltk::Item*, void*);
        inline void cb_Show4_i(fltk::Item*, void*);
        static void cb_Show4(fltk::Item*, void*);
public:
      fltk::PopupMenu *menuScript;
private:
        inline void cb_Load2_i(fltk::Item*, void*);
        static void cb_Load2(fltk::Item*, void*);
public:
        fltk::ItemGroup *menuScriptList;
      fltk::PopupMenu *menuHelp;
private:
        inline void cb_About_i(fltk::Item*, void*);
        static void cb_About(fltk::Item*, void*);
#include "EditorCB.h"
};
#endif