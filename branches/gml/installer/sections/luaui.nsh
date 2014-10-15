!ifdef INSTALL

  ; Purge old file from 0.75 install.
  Delete "$INSTDIR\LuaUI\unitdefs.lua"

  SetOutPath "$INSTDIR"
  File "..\game\gui.lua"
  File "..\game\usericons.tdf"

  SetOutPath "$INSTDIR\LuaUI\Images\"
  File "..\game\LuaUI\Images\players.png"
  File "..\game\LuaUI\Images\highlight_strip.png"
  SetOutPath "$INSTDIR\LuaUI\Icons\"
  File "..\game\LuaUI\Icons\e.png"
  File "..\game\LuaUI\Icons\m.png"
  File "..\game\LuaUI\Icons\x.png"
  File "..\game\LuaUI\Icons\square.png"
  File "..\game\LuaUI\Icons\tri-up.png"
  File "..\game\LuaUI\Icons\cross.png"
  File "..\game\LuaUI\Icons\hemi-down.png"
  File "..\game\LuaUI\Icons\hemi-up.png"
  File "..\game\LuaUI\Icons\sphere.png"
  File "..\game\LuaUI\Icons\m-down.png"
  File "..\game\LuaUI\Icons\diamond.png"
  File "..\game\LuaUI\Icons\triangle-up.png"
  File "..\game\LuaUI\Icons\star-dark.png"
  File "..\game\LuaUI\Icons\square_+.png"
  File "..\game\LuaUI\Icons\star.png"
  File "..\game\LuaUI\Icons\hourglass.png"
  File "..\game\LuaUI\Icons\square_x.png"
  File "..\game\LuaUI\Icons\triangle-down.png"
  File "..\game\LuaUI\Icons\hourglass-side.png"
  File "..\game\LuaUI\Icons\m-up.png"
  File "..\game\LuaUI\Icons\tri-down.png"
  File "..\game\LuaUI\Icons\hemi.png"
  SetOutPath "$INSTDIR\LuaUI\"
  File "..\game\LuaUI\loadmodel.lua"
  File "..\game\LuaUI\ctrlpanel.txt"
  File "..\game\LuaUI\fonts.lua"
  File "..\game\LuaUI\savetable.lua"
  File "..\game\LuaUI\debug.lua"
  File "..\game\LuaUI\widgets.lua"
  File "..\game\LuaUI\NEWS.txt"
  File "..\game\LuaUI\system.lua"
  File "..\game\LuaUI\main.lua"
  File "..\game\LuaUI\selector.lua"
  File "..\game\LuaUI\setupdefs.lua"
  File "..\game\LuaUI\actions.lua"
  File "..\game\LuaUI\modui_dialog.lua"
  File "..\game\LuaUI\README.txt"
  File "..\game\LuaUI\callins.lua"
  File "..\game\LuaUI\tweakmode.lua"
  File "..\game\LuaUI\layout.lua"
  File "..\game\LuaUI\CHANGELOG.txt"
  File "..\game\LuaUI\utils.lua"
  SetOutPath "$INSTDIR\LuaUI\Widgets\"
  File "..\game\LuaUI\Widgets\hook_copyqueue.lua"
  File "..\game\LuaUI\Widgets\cmd_factoryclear.lua"
  File "..\game\LuaUI\Widgets\cmd_doline.lua"
  File "..\game\LuaUI\Widgets\gui_selbuttons.lua"
  File "..\game\LuaUI\Widgets\gui_hilight_unit.lua"
  File "..\game\LuaUI\Widgets\camera_ctrl.lua"
  File "..\game\LuaUI\Widgets\camera_smooth_move.lua"
  File "..\game\LuaUI\Widgets\gui_bigcursor.lua"
  File "..\game\LuaUI\Widgets\cmd_circle.lua"
  File "..\game\LuaUI\Widgets\unit_metal_maker.lua"
  File "..\game\LuaUI\Widgets\camera_shake.lua"
  File "..\game\LuaUI\Widgets\unit_factory_guard.lua"
  File "..\game\LuaUI\Widgets\dbg_debug.lua"
  File "..\game\LuaUI\Widgets\cmd_givemobile.lua"
  File "..\game\LuaUI\Widgets\gui_clock.lua"
  File "..\game\LuaUI\Widgets\gui_comm_ends.lua"
  File "..\game\LuaUI\Widgets\unit_immobile_buider.lua"
  File "..\game\LuaUI\Widgets\snd_chatterbox.lua"
  File "..\game\LuaUI\Widgets\gui_minimap.lua"
  File "..\game\LuaUI\Widgets\gui_tooltip.lua"
  File "..\game\LuaUI\Widgets\gui_team_platter.lua"
  File "..\game\LuaUI\Widgets\gui_xray_shader.lua"
  File "..\game\LuaUI\Widgets\gui_fps.lua"
  File "..\game\LuaUI\Widgets\gui_eyes.lua"
  File "..\game\LuaUI\Widgets\gui_mousetrail.lua"
  File "..\game\LuaUI\Widgets\unit_stockpile.lua"
  File "..\game\LuaUI\Widgets\gui_modeltest.lua"
  File "..\game\LuaUI\Widgets\cmd_savequeue.lua"
  File "..\game\LuaUI\Widgets\minimap_relative.lua"
  File "..\game\LuaUI\Widgets\minimap_startbox.lua"
  File "..\game\LuaUI\Widgets\ico_customicons.lua"
  SetOutPath "$INSTDIR\LuaUI\Sounds\"
  File "..\game\LuaUI\Sounds\flag_grab.wav"
  File "..\game\LuaUI\Sounds\teamgrab.wav"
  File "..\game\LuaUI\Sounds\message_admin.wav"
  File "..\game\LuaUI\Sounds\bounce.wav"
  File "..\game\LuaUI\Sounds\README.txt"
  File "..\game\LuaUI\Sounds\land.wav"
  File "..\game\LuaUI\Sounds\message_private.wav"
  File "..\game\LuaUI\Sounds\message_team.wav"
  File "..\game\LuaUI\Sounds\pop.wav"
  SetOutPath "$INSTDIR\LuaUI\Headers\"
  File "..\game\LuaUI\Headers\colors.h.lua"
  File "..\game\LuaUI\Headers\keysym.h.lua"
  SetOutPath "$INSTDIR\LuaUI\Models\"
  File "..\game\LuaUI\Models\colors.lua"
  File "..\game\LuaUI\Models\colors.mtl"
  File "..\game\LuaUI\Models\colors.obj"
  SetOutPath "$INSTDIR\LuaUI\Fonts\"
  File "..\game\LuaUI\Fonts\README"
  File "..\game\LuaUI\Fonts\FreeMonoBold_12.lua"
  File "..\game\LuaUI\Fonts\FreeMonoBold_12.png"

!else

  Delete "$INSTDIR\gui.lua"
  Delete "$INSTDIR\usericons.tdf"

  Delete "$INSTDIR\LuaUI\Images\players.png"
  Delete "$INSTDIR\LuaUI\Images\highlight_strip.png"
  RmDir "$INSTDIR\LuaUI\Images\"
  Delete "$INSTDIR\LuaUI\Icons\e.png"
  Delete "$INSTDIR\LuaUI\Icons\m.png"
  Delete "$INSTDIR\LuaUI\Icons\x.png"
  Delete "$INSTDIR\LuaUI\Icons\square.png"
  Delete "$INSTDIR\LuaUI\Icons\tri-up.png"
  Delete "$INSTDIR\LuaUI\Icons\cross.png"
  Delete "$INSTDIR\LuaUI\Icons\hemi-down.png"
  Delete "$INSTDIR\LuaUI\Icons\hemi-up.png"
  Delete "$INSTDIR\LuaUI\Icons\sphere.png"
  Delete "$INSTDIR\LuaUI\Icons\m-down.png"
  Delete "$INSTDIR\LuaUI\Icons\diamond.png"
  Delete "$INSTDIR\LuaUI\Icons\triangle-up.png"
  Delete "$INSTDIR\LuaUI\Icons\star-dark.png"
  Delete "$INSTDIR\LuaUI\Icons\square_+.png"
  Delete "$INSTDIR\LuaUI\Icons\star.png"
  Delete "$INSTDIR\LuaUI\Icons\hourglass.png"
  Delete "$INSTDIR\LuaUI\Icons\square_x.png"
  Delete "$INSTDIR\LuaUI\Icons\triangle-down.png"
  Delete "$INSTDIR\LuaUI\Icons\hourglass-side.png"
  Delete "$INSTDIR\LuaUI\Icons\m-up.png"
  Delete "$INSTDIR\LuaUI\Icons\tri-down.png"
  Delete "$INSTDIR\LuaUI\Icons\hemi.png"
  RmDir "$INSTDIR\LuaUI\Icons\"
  Delete "$INSTDIR\LuaUI\loadmodel.lua"
  Delete "$INSTDIR\LuaUI\ctrlpanel.txt"
  Delete "$INSTDIR\LuaUI\fonts.lua"
  Delete "$INSTDIR\LuaUI\savetable.lua"
  Delete "$INSTDIR\LuaUI\debug.lua"
  Delete "$INSTDIR\LuaUI\widgets.lua"
  Delete "$INSTDIR\LuaUI\NEWS.txt"
  Delete "$INSTDIR\LuaUI\system.lua"
  Delete "$INSTDIR\LuaUI\main.lua"
  Delete "$INSTDIR\LuaUI\selector.lua"
  Delete "$INSTDIR\LuaUI\setupdefs.lua"
  Delete "$INSTDIR\LuaUI\actions.lua"
  Delete "$INSTDIR\LuaUI\modui_dialog.lua"
  Delete "$INSTDIR\LuaUI\README.txt"
  Delete "$INSTDIR\LuaUI\callins.lua"
  Delete "$INSTDIR\LuaUI\tweakmode.lua"
  Delete "$INSTDIR\LuaUI\layout.lua"
  Delete "$INSTDIR\LuaUI\CHANGELOG.txt"
  Delete "$INSTDIR\LuaUI\utils.lua"
  RmDir "$INSTDIR\LuaUI\"
  Delete "$INSTDIR\LuaUI\Widgets\hook_copyqueue.lua"
  Delete "$INSTDIR\LuaUI\Widgets\cmd_factoryclear.lua"
  Delete "$INSTDIR\LuaUI\Widgets\cmd_doline.lua"
  Delete "$INSTDIR\LuaUI\Widgets\gui_selbuttons.lua"
  Delete "$INSTDIR\LuaUI\Widgets\gui_hilight_unit.lua"
  Delete "$INSTDIR\LuaUI\Widgets\camera_ctrl.lua"
  Delete "$INSTDIR\LuaUI\Widgets\camera_smooth_move.lua"
  Delete "$INSTDIR\LuaUI\Widgets\gui_bigcursor.lua"
  Delete "$INSTDIR\LuaUI\Widgets\cmd_circle.lua"
  Delete "$INSTDIR\LuaUI\Widgets\unit_metal_maker.lua"
  Delete "$INSTDIR\LuaUI\Widgets\camera_shake.lua"
  Delete "$INSTDIR\LuaUI\Widgets\unit_factory_guard.lua"
  Delete "$INSTDIR\LuaUI\Widgets\dbg_debug.lua"
  Delete "$INSTDIR\LuaUI\Widgets\cmd_givemobile.lua"
  Delete "$INSTDIR\LuaUI\Widgets\gui_clock.lua"
  Delete "$INSTDIR\LuaUI\Widgets\gui_comm_ends.lua"
  Delete "$INSTDIR\LuaUI\Widgets\unit_immobile_buider.lua"
  Delete "$INSTDIR\LuaUI\Widgets\snd_chatterbox.lua"
  Delete "$INSTDIR\LuaUI\Widgets\gui_minimap.lua"
  Delete "$INSTDIR\LuaUI\Widgets\gui_tooltip.lua"
  Delete "$INSTDIR\LuaUI\Widgets\gui_team_platter.lua"
  Delete "$INSTDIR\LuaUI\Widgets\gui_xray_shader.lua"
  Delete "$INSTDIR\LuaUI\Widgets\gui_fps.lua"
  Delete "$INSTDIR\LuaUI\Widgets\gui_eyes.lua"
  Delete "$INSTDIR\LuaUI\Widgets\gui_mousetrail.lua"
  Delete "$INSTDIR\LuaUI\Widgets\unit_stockpile.lua"
  Delete "$INSTDIR\LuaUI\Widgets\gui_modeltest.lua"
  Delete "$INSTDIR\LuaUI\Widgets\cmd_savequeue.lua"
  Delete "$INSTDIR\LuaUI\Widgets\minimap_relative.lua"
  Delete "$INSTDIR\LuaUI\Widgets\minimap_startbox.lua"
  Delete "$INSTDIR\LuaUI\Widgets\ico_customicons.lua"
  RmDir "$INSTDIR\LuaUI\Widgets\"
  Delete "$INSTDIR\LuaUI\Sounds\flag_grab.wav"
  Delete "$INSTDIR\LuaUI\Sounds\teamgrab.wav"
  Delete "$INSTDIR\LuaUI\Sounds\message_admin.wav"
  Delete "$INSTDIR\LuaUI\Sounds\bounce.wav"
  Delete "$INSTDIR\LuaUI\Sounds\README.txt"
  Delete "$INSTDIR\LuaUI\Sounds\land.wav"
  Delete "$INSTDIR\LuaUI\Sounds\message_private.wav"
  Delete "$INSTDIR\LuaUI\Sounds\message_team.wav"
  Delete "$INSTDIR\LuaUI\Sounds\pop.wav"
  RmDir "$INSTDIR\LuaUI\Sounds\"
  Delete "$INSTDIR\LuaUI\Headers\colors.h.lua"
  Delete "$INSTDIR\LuaUI\Headers\keysym.h.lua"
  RmDir "$INSTDIR\LuaUI\Headers\"
  Delete "$INSTDIR\LuaUI\Models\colors.lua"
  Delete "$INSTDIR\LuaUI\Models\colors.mtl"
  Delete "$INSTDIR\LuaUI\Models\colors.obj"
  RmDir "$INSTDIR\LuaUI\Models\"
  Delete "$INSTDIR\LuaUI\Fonts\README"
  Delete "$INSTDIR\LuaUI\Fonts\FreeMonoBold_12.lua"
  Delete "$INSTDIR\LuaUI\Fonts\FreeMonoBold_12.png"
  RmDir "$INSTDIR\LuaUI\Fonts\"

  RmDir "$INSTDIR\LuaUI"

!endif