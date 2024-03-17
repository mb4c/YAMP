#include "Themes.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

using namespace nlohmann;

ThemeData Themes::LoadTheme(const std::string& file)
{
	std::ifstream i(file);
	json json;
	i >> json;

	ThemeData data;
	data.author = json["Author"];
	data.name = json["Name"];

	ImGuiStyle style;
	style.Alpha = json["Style"]["Alpha"];
	style.DisabledAlpha = json["Style"]["DisabledAlpha"];
	style.WindowPadding.x = json["Style"]["WindowPadding"]["x"];
	style.WindowPadding.y = json["Style"]["WindowPadding"]["y"];
	style.WindowRounding = json["Style"]["WindowRounding"];
	style.WindowBorderSize = json["Style"]["WindowBorderSize"];
	style.WindowMinSize.x = json["Style"]["WindowMinSize"]["x"];
	style.WindowMinSize.y = json["Style"]["WindowMinSize"]["y"];
	style.WindowTitleAlign.x = json["Style"]["WindowTitleAlign"]["x"];
	style.WindowTitleAlign.y = json["Style"]["WindowTitleAlign"]["y"];
	style.WindowMenuButtonPosition = json["Style"]["WindowMenuButtonPosition"];
	style.ChildRounding = json["Style"]["ChildRounding"];
	style.ChildBorderSize = json["Style"]["ChildBorderSize"];
	style.PopupRounding = json["Style"]["PopupRounding"];
	style.PopupBorderSize = json["Style"]["PopupBorderSize"];
	style.FramePadding.x = json["Style"]["FramePadding"]["x"];
	style.FramePadding.y = json["Style"]["FramePadding"]["y"];
	style.FrameRounding = json["Style"]["FrameRounding"];
	style.FrameBorderSize = json["Style"]["FrameBorderSize"];
	style.ItemSpacing.x = json["Style"]["ItemSpacing"]["x"];
	style.ItemSpacing.y = json["Style"]["ItemSpacing"]["y"];
	style.ItemInnerSpacing.x = json["Style"]["ItemInnerSpacing"]["x"];
	style.ItemInnerSpacing.y = json["Style"]["ItemInnerSpacing"]["y"];
	style.CellPadding.x = json["Style"]["CellPadding"]["x"];
	style.CellPadding.y = json["Style"]["CellPadding"]["y"];
	style.IndentSpacing = json["Style"]["IndentSpacing"];
	style.ColumnsMinSpacing = json["Style"]["ColumnsMinSpacing"];
	style.ScrollbarSize = json["Style"]["ScrollbarSize"];
	style.ScrollbarRounding = json["Style"]["ScrollbarRounding"];
	style.GrabMinSize = json["Style"]["GrabMinSize"];
	style.GrabRounding = json["Style"]["GrabRounding"];
	style.TabRounding = json["Style"]["TabRounding"];
	style.TabBorderSize = json["Style"]["TabBorderSize"];
	style.TabMinWidthForCloseButton = json["Style"]["TabMinWidthForCloseButton"];
	style.ColorButtonPosition = json["Style"]["ColorButtonPosition"];
	style.ButtonTextAlign.x = json["Style"]["ButtonTextAlign"]["x"];
	style.ButtonTextAlign.y = json["Style"]["ButtonTextAlign"]["y"];
	style.SelectableTextAlign.x = json["Style"]["SelectableTextAlign"]["x"];
	style.SelectableTextAlign.y = json["Style"]["SelectableTextAlign"]["y"];

	style.Colors[ImGuiCol_Text].x = json["Colors"]["Text"]["r"];
	style.Colors[ImGuiCol_Text].y = json["Colors"]["Text"]["g"];
	style.Colors[ImGuiCol_Text].z = json["Colors"]["Text"]["b"];
	style.Colors[ImGuiCol_Text].w = json["Colors"]["Text"]["a"];

	style.Colors[ImGuiCol_TextDisabled].x = json["Colors"]["TextDisabled"]["r"];
	style.Colors[ImGuiCol_TextDisabled].y = json["Colors"]["TextDisabled"]["g"];
	style.Colors[ImGuiCol_TextDisabled].z = json["Colors"]["TextDisabled"]["b"];
	style.Colors[ImGuiCol_TextDisabled].w = json["Colors"]["TextDisabled"]["a"];

	style.Colors[ImGuiCol_WindowBg].x = json["Colors"]["WindowBg"]["r"];
	style.Colors[ImGuiCol_WindowBg].y = json["Colors"]["WindowBg"]["g"];
	style.Colors[ImGuiCol_WindowBg].z = json["Colors"]["WindowBg"]["b"];
	style.Colors[ImGuiCol_WindowBg].w = json["Colors"]["WindowBg"]["a"];

	style.Colors[ImGuiCol_ChildBg].x = json["Colors"]["ChildBg"]["r"];
	style.Colors[ImGuiCol_ChildBg].y = json["Colors"]["ChildBg"]["g"];
	style.Colors[ImGuiCol_ChildBg].z = json["Colors"]["ChildBg"]["b"];
	style.Colors[ImGuiCol_ChildBg].w = json["Colors"]["ChildBg"]["a"];

	style.Colors[ImGuiCol_PopupBg].x = json["Colors"]["PopupBg"]["r"];
	style.Colors[ImGuiCol_PopupBg].y = json["Colors"]["PopupBg"]["g"];
	style.Colors[ImGuiCol_PopupBg].z = json["Colors"]["PopupBg"]["b"];
	style.Colors[ImGuiCol_PopupBg].w = json["Colors"]["PopupBg"]["a"];

	style.Colors[ImGuiCol_Border].x = json["Colors"]["Border"]["r"];
	style.Colors[ImGuiCol_Border].y = json["Colors"]["Border"]["g"];
	style.Colors[ImGuiCol_Border].z = json["Colors"]["Border"]["b"];
	style.Colors[ImGuiCol_Border].w = json["Colors"]["Border"]["a"];

	style.Colors[ImGuiCol_BorderShadow].x = json["Colors"]["BorderShadow"]["r"];
	style.Colors[ImGuiCol_BorderShadow].y = json["Colors"]["BorderShadow"]["g"];
	style.Colors[ImGuiCol_BorderShadow].z = json["Colors"]["BorderShadow"]["b"];
	style.Colors[ImGuiCol_BorderShadow].w = json["Colors"]["BorderShadow"]["a"];

	style.Colors[ImGuiCol_FrameBg].x = json["Colors"]["FrameBg"]["r"];
	style.Colors[ImGuiCol_FrameBg].y = json["Colors"]["FrameBg"]["g"];
	style.Colors[ImGuiCol_FrameBg].z = json["Colors"]["FrameBg"]["b"];
	style.Colors[ImGuiCol_FrameBg].w = json["Colors"]["FrameBg"]["a"];

	style.Colors[ImGuiCol_FrameBgHovered].x = json["Colors"]["FrameBgHovered"]["r"];
	style.Colors[ImGuiCol_FrameBgHovered].y = json["Colors"]["FrameBgHovered"]["g"];
	style.Colors[ImGuiCol_FrameBgHovered].z = json["Colors"]["FrameBgHovered"]["b"];
	style.Colors[ImGuiCol_FrameBgHovered].w = json["Colors"]["FrameBgHovered"]["a"];

	style.Colors[ImGuiCol_FrameBgActive].x = json["Colors"]["FrameBgActive"]["r"];
	style.Colors[ImGuiCol_FrameBgActive].y = json["Colors"]["FrameBgActive"]["g"];
	style.Colors[ImGuiCol_FrameBgActive].z = json["Colors"]["FrameBgActive"]["b"];
	style.Colors[ImGuiCol_FrameBgActive].w = json["Colors"]["FrameBgActive"]["a"];

	style.Colors[ImGuiCol_TitleBg].x = json["Colors"]["TitleBg"]["r"];
	style.Colors[ImGuiCol_TitleBg].y = json["Colors"]["TitleBg"]["g"];
	style.Colors[ImGuiCol_TitleBg].z = json["Colors"]["TitleBg"]["b"];
	style.Colors[ImGuiCol_TitleBg].w = json["Colors"]["TitleBg"]["a"];

	style.Colors[ImGuiCol_TitleBgActive].x = json["Colors"]["TitleBgActive"]["r"];
	style.Colors[ImGuiCol_TitleBgActive].y = json["Colors"]["TitleBgActive"]["g"];
	style.Colors[ImGuiCol_TitleBgActive].z = json["Colors"]["TitleBgActive"]["b"];
	style.Colors[ImGuiCol_TitleBgActive].w = json["Colors"]["TitleBgActive"]["a"];

	style.Colors[ImGuiCol_TitleBgCollapsed].x = json["Colors"]["TitleBgCollapsed"]["r"];
	style.Colors[ImGuiCol_TitleBgCollapsed].y = json["Colors"]["TitleBgCollapsed"]["g"];
	style.Colors[ImGuiCol_TitleBgCollapsed].z = json["Colors"]["TitleBgCollapsed"]["b"];
	style.Colors[ImGuiCol_TitleBgCollapsed].w = json["Colors"]["TitleBgCollapsed"]["a"];

	style.Colors[ImGuiCol_MenuBarBg].x = json["Colors"]["MenuBarBg"]["r"];
	style.Colors[ImGuiCol_MenuBarBg].y = json["Colors"]["MenuBarBg"]["g"];
	style.Colors[ImGuiCol_MenuBarBg].z = json["Colors"]["MenuBarBg"]["b"];
	style.Colors[ImGuiCol_MenuBarBg].w = json["Colors"]["MenuBarBg"]["a"];

	style.Colors[ImGuiCol_ScrollbarBg].x = json["Colors"]["ScrollbarBg"]["r"];
	style.Colors[ImGuiCol_ScrollbarBg].y = json["Colors"]["ScrollbarBg"]["g"];
	style.Colors[ImGuiCol_ScrollbarBg].z = json["Colors"]["ScrollbarBg"]["b"];
	style.Colors[ImGuiCol_ScrollbarBg].w = json["Colors"]["ScrollbarBg"]["a"];

	style.Colors[ImGuiCol_ScrollbarGrab].x = json["Colors"]["ScrollbarGrab"]["r"];
	style.Colors[ImGuiCol_ScrollbarGrab].y = json["Colors"]["ScrollbarGrab"]["g"];
	style.Colors[ImGuiCol_ScrollbarGrab].z = json["Colors"]["ScrollbarGrab"]["b"];
	style.Colors[ImGuiCol_ScrollbarGrab].w = json["Colors"]["ScrollbarGrab"]["a"];

	style.Colors[ImGuiCol_ScrollbarGrabHovered].x = json["Colors"]["ScrollbarGrabHovered"]["r"];
	style.Colors[ImGuiCol_ScrollbarGrabHovered].y = json["Colors"]["ScrollbarGrabHovered"]["g"];
	style.Colors[ImGuiCol_ScrollbarGrabHovered].z = json["Colors"]["ScrollbarGrabHovered"]["b"];
	style.Colors[ImGuiCol_ScrollbarGrabHovered].w = json["Colors"]["ScrollbarGrabHovered"]["a"];

	style.Colors[ImGuiCol_ScrollbarGrabActive].x = json["Colors"]["ScrollbarGrabActive"]["r"];
	style.Colors[ImGuiCol_ScrollbarGrabActive].y = json["Colors"]["ScrollbarGrabActive"]["g"];
	style.Colors[ImGuiCol_ScrollbarGrabActive].z = json["Colors"]["ScrollbarGrabActive"]["b"];
	style.Colors[ImGuiCol_ScrollbarGrabActive].w = json["Colors"]["ScrollbarGrabActive"]["a"];

	style.Colors[ImGuiCol_CheckMark].x = json["Colors"]["CheckMark"]["r"];
	style.Colors[ImGuiCol_CheckMark].y = json["Colors"]["CheckMark"]["g"];
	style.Colors[ImGuiCol_CheckMark].z = json["Colors"]["CheckMark"]["b"];
	style.Colors[ImGuiCol_CheckMark].w = json["Colors"]["CheckMark"]["a"];

	style.Colors[ImGuiCol_SliderGrab].x = json["Colors"]["SliderGrab"]["r"];
	style.Colors[ImGuiCol_SliderGrab].y = json["Colors"]["SliderGrab"]["g"];
	style.Colors[ImGuiCol_SliderGrab].z = json["Colors"]["SliderGrab"]["b"];
	style.Colors[ImGuiCol_SliderGrab].w = json["Colors"]["SliderGrab"]["a"];

	style.Colors[ImGuiCol_SliderGrabActive].x = json["Colors"]["SliderGrabActive"]["r"];
	style.Colors[ImGuiCol_SliderGrabActive].y = json["Colors"]["SliderGrabActive"]["g"];
	style.Colors[ImGuiCol_SliderGrabActive].z = json["Colors"]["SliderGrabActive"]["b"];
	style.Colors[ImGuiCol_SliderGrabActive].w = json["Colors"]["SliderGrabActive"]["a"];

	style.Colors[ImGuiCol_Button].x = json["Colors"]["Button"]["r"];
	style.Colors[ImGuiCol_Button].y = json["Colors"]["Button"]["g"];
	style.Colors[ImGuiCol_Button].z = json["Colors"]["Button"]["b"];
	style.Colors[ImGuiCol_Button].w = json["Colors"]["Button"]["a"];

	style.Colors[ImGuiCol_ButtonHovered].x = json["Colors"]["ButtonHovered"]["r"];
	style.Colors[ImGuiCol_ButtonHovered].y = json["Colors"]["ButtonHovered"]["g"];
	style.Colors[ImGuiCol_ButtonHovered].z = json["Colors"]["ButtonHovered"]["b"];
	style.Colors[ImGuiCol_ButtonHovered].w = json["Colors"]["ButtonHovered"]["a"];

	style.Colors[ImGuiCol_ButtonActive].x = json["Colors"]["ButtonActive"]["r"];
	style.Colors[ImGuiCol_ButtonActive].y = json["Colors"]["ButtonActive"]["g"];
	style.Colors[ImGuiCol_ButtonActive].z = json["Colors"]["ButtonActive"]["b"];
	style.Colors[ImGuiCol_ButtonActive].w = json["Colors"]["ButtonActive"]["a"];

	style.Colors[ImGuiCol_Header].x = json["Colors"]["Header"]["r"];
	style.Colors[ImGuiCol_Header].y = json["Colors"]["Header"]["g"];
	style.Colors[ImGuiCol_Header].z = json["Colors"]["Header"]["b"];
	style.Colors[ImGuiCol_Header].w = json["Colors"]["Header"]["a"];

	style.Colors[ImGuiCol_HeaderHovered].x = json["Colors"]["HeaderHovered"]["r"];
	style.Colors[ImGuiCol_HeaderHovered].y = json["Colors"]["HeaderHovered"]["g"];
	style.Colors[ImGuiCol_HeaderHovered].z = json["Colors"]["HeaderHovered"]["b"];
	style.Colors[ImGuiCol_HeaderHovered].w = json["Colors"]["HeaderHovered"]["a"];

	style.Colors[ImGuiCol_HeaderActive].x = json["Colors"]["HeaderActive"]["r"];
	style.Colors[ImGuiCol_HeaderActive].y = json["Colors"]["HeaderActive"]["g"];
	style.Colors[ImGuiCol_HeaderActive].z = json["Colors"]["HeaderActive"]["b"];
	style.Colors[ImGuiCol_HeaderActive].w = json["Colors"]["HeaderActive"]["a"];

	style.Colors[ImGuiCol_Separator].x = json["Colors"]["Separator"]["r"];
	style.Colors[ImGuiCol_Separator].y = json["Colors"]["Separator"]["g"];
	style.Colors[ImGuiCol_Separator].z = json["Colors"]["Separator"]["b"];
	style.Colors[ImGuiCol_Separator].w = json["Colors"]["Separator"]["a"];

	style.Colors[ImGuiCol_SeparatorHovered].x = json["Colors"]["SeparatorHovered"]["r"];
	style.Colors[ImGuiCol_SeparatorHovered].y = json["Colors"]["SeparatorHovered"]["g"];
	style.Colors[ImGuiCol_SeparatorHovered].z = json["Colors"]["SeparatorHovered"]["b"];
	style.Colors[ImGuiCol_SeparatorHovered].w = json["Colors"]["SeparatorHovered"]["a"];

	style.Colors[ImGuiCol_SeparatorActive].x = json["Colors"]["SeparatorActive"]["r"];
	style.Colors[ImGuiCol_SeparatorActive].y = json["Colors"]["SeparatorActive"]["g"];
	style.Colors[ImGuiCol_SeparatorActive].z = json["Colors"]["SeparatorActive"]["b"];
	style.Colors[ImGuiCol_SeparatorActive].w = json["Colors"]["SeparatorActive"]["a"];

	style.Colors[ImGuiCol_ResizeGrip].x = json["Colors"]["ResizeGrip"]["r"];
	style.Colors[ImGuiCol_ResizeGrip].y = json["Colors"]["ResizeGrip"]["g"];
	style.Colors[ImGuiCol_ResizeGrip].z = json["Colors"]["ResizeGrip"]["b"];
	style.Colors[ImGuiCol_ResizeGrip].w = json["Colors"]["ResizeGrip"]["a"];

	style.Colors[ImGuiCol_ResizeGripHovered].x = json["Colors"]["ResizeGripHovered"]["r"];
	style.Colors[ImGuiCol_ResizeGripHovered].y = json["Colors"]["ResizeGripHovered"]["g"];
	style.Colors[ImGuiCol_ResizeGripHovered].z = json["Colors"]["ResizeGripHovered"]["b"];
	style.Colors[ImGuiCol_ResizeGripHovered].w = json["Colors"]["ResizeGripHovered"]["a"];

	style.Colors[ImGuiCol_ResizeGripActive].x = json["Colors"]["ResizeGripActive"]["r"];
	style.Colors[ImGuiCol_ResizeGripActive].y = json["Colors"]["ResizeGripActive"]["g"];
	style.Colors[ImGuiCol_ResizeGripActive].z = json["Colors"]["ResizeGripActive"]["b"];
	style.Colors[ImGuiCol_ResizeGripActive].w = json["Colors"]["ResizeGripActive"]["a"];

	style.Colors[ImGuiCol_Tab].x = json["Colors"]["Tab"]["r"];
	style.Colors[ImGuiCol_Tab].y = json["Colors"]["Tab"]["g"];
	style.Colors[ImGuiCol_Tab].z = json["Colors"]["Tab"]["b"];
	style.Colors[ImGuiCol_Tab].w = json["Colors"]["Tab"]["a"];

	style.Colors[ImGuiCol_TabHovered].x = json["Colors"]["TabHovered"]["r"];
	style.Colors[ImGuiCol_TabHovered].y = json["Colors"]["TabHovered"]["g"];
	style.Colors[ImGuiCol_TabHovered].z = json["Colors"]["TabHovered"]["b"];
	style.Colors[ImGuiCol_TabHovered].w = json["Colors"]["TabHovered"]["a"];

	style.Colors[ImGuiCol_TabActive].x = json["Colors"]["TabActive"]["r"];
	style.Colors[ImGuiCol_TabActive].y = json["Colors"]["TabActive"]["g"];
	style.Colors[ImGuiCol_TabActive].z = json["Colors"]["TabActive"]["b"];
	style.Colors[ImGuiCol_TabActive].w = json["Colors"]["TabActive"]["a"];

	style.Colors[ImGuiCol_TabUnfocused].x = json["Colors"]["TabUnfocused"]["r"];
	style.Colors[ImGuiCol_TabUnfocused].y = json["Colors"]["TabUnfocused"]["g"];
	style.Colors[ImGuiCol_TabUnfocused].z = json["Colors"]["TabUnfocused"]["b"];
	style.Colors[ImGuiCol_TabUnfocused].w = json["Colors"]["TabUnfocused"]["a"];

	style.Colors[ImGuiCol_TabUnfocusedActive].x = json["Colors"]["TabUnfocusedActive"]["r"];
	style.Colors[ImGuiCol_TabUnfocusedActive].y = json["Colors"]["TabUnfocusedActive"]["g"];
	style.Colors[ImGuiCol_TabUnfocusedActive].z = json["Colors"]["TabUnfocusedActive"]["b"];
	style.Colors[ImGuiCol_TabUnfocusedActive].w = json["Colors"]["TabUnfocusedActive"]["a"];

	style.Colors[ImGuiCol_PlotLines].x = json["Colors"]["PlotLines"]["r"];
	style.Colors[ImGuiCol_PlotLines].y = json["Colors"]["PlotLines"]["g"];
	style.Colors[ImGuiCol_PlotLines].z = json["Colors"]["PlotLines"]["b"];
	style.Colors[ImGuiCol_PlotLines].w = json["Colors"]["PlotLines"]["a"];

	style.Colors[ImGuiCol_PlotLinesHovered].x = json["Colors"]["PlotLinesHovered"]["r"];
	style.Colors[ImGuiCol_PlotLinesHovered].y = json["Colors"]["PlotLinesHovered"]["g"];
	style.Colors[ImGuiCol_PlotLinesHovered].z = json["Colors"]["PlotLinesHovered"]["b"];
	style.Colors[ImGuiCol_PlotLinesHovered].w = json["Colors"]["PlotLinesHovered"]["a"];

	style.Colors[ImGuiCol_PlotHistogram].x = json["Colors"]["PlotHistogram"]["r"];
	style.Colors[ImGuiCol_PlotHistogram].y = json["Colors"]["PlotHistogram"]["g"];
	style.Colors[ImGuiCol_PlotHistogram].z = json["Colors"]["PlotHistogram"]["b"];
	style.Colors[ImGuiCol_PlotHistogram].w = json["Colors"]["PlotHistogram"]["a"];

	style.Colors[ImGuiCol_PlotHistogramHovered].x = json["Colors"]["PlotHistogramHovered"]["r"];
	style.Colors[ImGuiCol_PlotHistogramHovered].y = json["Colors"]["PlotHistogramHovered"]["g"];
	style.Colors[ImGuiCol_PlotHistogramHovered].z = json["Colors"]["PlotHistogramHovered"]["b"];
	style.Colors[ImGuiCol_PlotHistogramHovered].w = json["Colors"]["PlotHistogramHovered"]["a"];

	style.Colors[ImGuiCol_TableHeaderBg].x = json["Colors"]["TableHeaderBg"]["r"];
	style.Colors[ImGuiCol_TableHeaderBg].y = json["Colors"]["TableHeaderBg"]["g"];
	style.Colors[ImGuiCol_TableHeaderBg].z = json["Colors"]["TableHeaderBg"]["b"];
	style.Colors[ImGuiCol_TableHeaderBg].w = json["Colors"]["TableHeaderBg"]["a"];

	style.Colors[ImGuiCol_TableBorderStrong].x = json["Colors"]["TableBorderStrong"]["r"];
	style.Colors[ImGuiCol_TableBorderStrong].y = json["Colors"]["TableBorderStrong"]["g"];
	style.Colors[ImGuiCol_TableBorderStrong].z = json["Colors"]["TableBorderStrong"]["b"];
	style.Colors[ImGuiCol_TableBorderStrong].w = json["Colors"]["TableBorderStrong"]["a"];

	style.Colors[ImGuiCol_TableBorderLight].x = json["Colors"]["TableBorderLight"]["r"];
	style.Colors[ImGuiCol_TableBorderLight].y = json["Colors"]["TableBorderLight"]["g"];
	style.Colors[ImGuiCol_TableBorderLight].z = json["Colors"]["TableBorderLight"]["b"];
	style.Colors[ImGuiCol_TableBorderLight].w = json["Colors"]["TableBorderLight"]["a"];

	style.Colors[ImGuiCol_TableRowBg].x = json["Colors"]["TableRowBg"]["r"];
	style.Colors[ImGuiCol_TableRowBg].y = json["Colors"]["TableRowBg"]["g"];
	style.Colors[ImGuiCol_TableRowBg].z = json["Colors"]["TableRowBg"]["b"];
	style.Colors[ImGuiCol_TableRowBg].w = json["Colors"]["TableRowBg"]["a"];

	style.Colors[ImGuiCol_TableRowBgAlt].x = json["Colors"]["TableRowBgAlt"]["r"];
	style.Colors[ImGuiCol_TableRowBgAlt].y = json["Colors"]["TableRowBgAlt"]["g"];
	style.Colors[ImGuiCol_TableRowBgAlt].z = json["Colors"]["TableRowBgAlt"]["b"];
	style.Colors[ImGuiCol_TableRowBgAlt].w = json["Colors"]["TableRowBgAlt"]["a"];

	style.Colors[ImGuiCol_TextSelectedBg].x = json["Colors"]["TextSelectedBg"]["r"];
	style.Colors[ImGuiCol_TextSelectedBg].y = json["Colors"]["TextSelectedBg"]["g"];
	style.Colors[ImGuiCol_TextSelectedBg].z = json["Colors"]["TextSelectedBg"]["b"];
	style.Colors[ImGuiCol_TextSelectedBg].w = json["Colors"]["TextSelectedBg"]["a"];

	style.Colors[ImGuiCol_DragDropTarget].x = json["Colors"]["DragDropTarget"]["r"];
	style.Colors[ImGuiCol_DragDropTarget].y = json["Colors"]["DragDropTarget"]["g"];
	style.Colors[ImGuiCol_DragDropTarget].z = json["Colors"]["DragDropTarget"]["b"];
	style.Colors[ImGuiCol_DragDropTarget].w = json["Colors"]["DragDropTarget"]["a"];

	style.Colors[ImGuiCol_NavHighlight].x = json["Colors"]["NavHighlight"]["r"];
	style.Colors[ImGuiCol_NavHighlight].y = json["Colors"]["NavHighlight"]["g"];
	style.Colors[ImGuiCol_NavHighlight].z = json["Colors"]["NavHighlight"]["b"];
	style.Colors[ImGuiCol_NavHighlight].w = json["Colors"]["NavHighlight"]["a"];

	style.Colors[ImGuiCol_NavWindowingHighlight].x = json["Colors"]["NavWindowingHighlight"]["r"];
	style.Colors[ImGuiCol_NavWindowingHighlight].y = json["Colors"]["NavWindowingHighlight"]["g"];
	style.Colors[ImGuiCol_NavWindowingHighlight].z = json["Colors"]["NavWindowingHighlight"]["b"];
	style.Colors[ImGuiCol_NavWindowingHighlight].w = json["Colors"]["NavWindowingHighlight"]["a"];

	style.Colors[ImGuiCol_NavWindowingDimBg].x = json["Colors"]["NavWindowingDimBg"]["r"];
	style.Colors[ImGuiCol_NavWindowingDimBg].y = json["Colors"]["NavWindowingDimBg"]["g"];
	style.Colors[ImGuiCol_NavWindowingDimBg].z = json["Colors"]["NavWindowingDimBg"]["b"];
	style.Colors[ImGuiCol_NavWindowingDimBg].w = json["Colors"]["NavWindowingDimBg"]["a"];

	style.Colors[ImGuiCol_ModalWindowDimBg].x = json["Colors"]["ModalWindowDimBg"]["r"];
	style.Colors[ImGuiCol_ModalWindowDimBg].y = json["Colors"]["ModalWindowDimBg"]["g"];
	style.Colors[ImGuiCol_ModalWindowDimBg].z = json["Colors"]["ModalWindowDimBg"]["b"];
	style.Colors[ImGuiCol_ModalWindowDimBg].w = json["Colors"]["ModalWindowDimBg"]["a"];

	data.style = style;
	return data;
}

void Themes::DumpTheme(const std::string &file)
{
	ImGuiStyle& style = ImGui::GetStyle();
	json json;
	json["Author"] = "No author";
	json["Name"] = "No name";
	json["Style"]["Alpha"] = style.Alpha;
	json["Style"]["DisabledAlpha"] = style.DisabledAlpha;
	json["Style"]["WindowPadding"]["y"] = style.WindowPadding.y;
	json["Style"]["WindowPadding"]["x"] = style.WindowPadding.x;
	json["Style"]["WindowRounding"] = style.WindowRounding;
	json["Style"]["WindowBorderSize"] = style.WindowBorderSize;
	json["Style"]["WindowMinSize"]["x"] = style.WindowMinSize.x;
	json["Style"]["WindowMinSize"]["y"] = style.WindowMinSize.y;
	json["Style"]["WindowTitleAlign"]["x"] = style.WindowTitleAlign.x;
	json["Style"]["WindowTitleAlign"]["y"] = style.WindowTitleAlign.y;
	json["Style"]["WindowMenuButtonPosition"] = style.WindowMenuButtonPosition;

	json["Style"]["ChildRounding"] = style.ChildRounding;
	json["Style"]["ChildBorderSize"] = style.ChildBorderSize;
	json["Style"]["PopupRounding"] = style.PopupRounding;
	json["Style"]["PopupBorderSize"] = style.PopupBorderSize;
	json["Style"]["FramePadding"]["x"] = style.FramePadding.x;
	json["Style"]["FramePadding"]["y"] = style.FramePadding.y;
	json["Style"]["FrameRounding"] = style.FrameRounding;
	json["Style"]["FrameBorderSize"] = style.FrameBorderSize;
	json["Style"]["ItemSpacing"]["x"] = style.ItemSpacing.x;
	json["Style"]["ItemSpacing"]["y"] = style.ItemSpacing.y;
	json["Style"]["ItemInnerSpacing"]["x"] = style.ItemInnerSpacing.x;
	json["Style"]["ItemInnerSpacing"]["y"] = style.ItemInnerSpacing.y;
	json["Style"]["CellPadding"]["x"] = style.CellPadding.x;
	json["Style"]["CellPadding"]["y"] = style.CellPadding.y;
	json["Style"]["IndentSpacing"] = style.IndentSpacing;
	json["Style"]["ColumnsMinSpacing"] = style.ColumnsMinSpacing;
	json["Style"]["ScrollbarSize"] = style.ScrollbarSize;
	json["Style"]["ScrollbarRounding"] = style.ScrollbarRounding;

	json["Style"]["GrabMinSize"] = style.GrabMinSize;
	json["Style"]["GrabRounding"] = style.GrabRounding;
	json["Style"]["TabRounding"] = style.TabRounding;
	json["Style"]["TabBorderSize"] = style.TabBorderSize;
	json["Style"]["TabMinWidthForCloseButton"] = style.TabMinWidthForCloseButton;
	json["Style"]["ColorButtonPosition"] = style.ColorButtonPosition;
	json["Style"]["ButtonTextAlign"]["x"] = style.ButtonTextAlign.x;
	json["Style"]["ButtonTextAlign"]["y"] = style.ButtonTextAlign.y;
	json["Style"]["SelectableTextAlign"]["x"] = style.SelectableTextAlign.x;
	json["Style"]["SelectableTextAlign"]["y"] = style.SelectableTextAlign.y;

	json["Colors"]["Text"]["r"] = style.Colors[ImGuiCol_Text].x;
	json["Colors"]["Text"]["g"] = style.Colors[ImGuiCol_Text].y;
	json["Colors"]["Text"]["a"] = style.Colors[ImGuiCol_Text].w;
	json["Colors"]["Text"]["b"] = style.Colors[ImGuiCol_Text].z;

	json["Colors"]["TextDisabled"]["r"] = style.Colors[ImGuiCol_TextDisabled].x;
	json["Colors"]["TextDisabled"]["g"] = style.Colors[ImGuiCol_TextDisabled].y;
	json["Colors"]["TextDisabled"]["b"] = style.Colors[ImGuiCol_TextDisabled].z;
	json["Colors"]["TextDisabled"]["a"] = style.Colors[ImGuiCol_TextDisabled].w;

	json["Colors"]["WindowBg"]["r"] = style.Colors[ImGuiCol_WindowBg].x;
	json["Colors"]["WindowBg"]["g"] = style.Colors[ImGuiCol_WindowBg].y;
	json["Colors"]["WindowBg"]["b"] = style.Colors[ImGuiCol_WindowBg].z;
	json["Colors"]["WindowBg"]["a"] = style.Colors[ImGuiCol_WindowBg].w;

	json["Colors"]["ChildBg"]["r"] = style.Colors[ImGuiCol_ChildBg].x;
	json["Colors"]["ChildBg"]["g"] = style.Colors[ImGuiCol_ChildBg].y;
	json["Colors"]["ChildBg"]["b"] = style.Colors[ImGuiCol_ChildBg].z;
	json["Colors"]["ChildBg"]["a"] = style.Colors[ImGuiCol_ChildBg].w;

	json["Colors"]["PopupBg"]["r"] = style.Colors[ImGuiCol_PopupBg].x;
	json["Colors"]["PopupBg"]["g"] = style.Colors[ImGuiCol_PopupBg].y;
	json["Colors"]["PopupBg"]["b"] = style.Colors[ImGuiCol_PopupBg].z;
	json["Colors"]["PopupBg"]["a"] = style.Colors[ImGuiCol_PopupBg].w;

	json["Colors"]["Border"]["r"] = style.Colors[ImGuiCol_Border].x;
	json["Colors"]["Border"]["g"] = style.Colors[ImGuiCol_Border].y;
	json["Colors"]["Border"]["b"] = style.Colors[ImGuiCol_Border].z;
	json["Colors"]["Border"]["a"] = style.Colors[ImGuiCol_Border].w;

	json["Colors"]["BorderShadow"]["r"] = style.Colors[ImGuiCol_BorderShadow].x;
	json["Colors"]["BorderShadow"]["g"] = style.Colors[ImGuiCol_BorderShadow].y;
	json["Colors"]["BorderShadow"]["b"] = style.Colors[ImGuiCol_BorderShadow].z;
	json["Colors"]["BorderShadow"]["a"] = style.Colors[ImGuiCol_BorderShadow].w;

	json["Colors"]["FrameBg"]["r"] = style.Colors[ImGuiCol_FrameBg].x;
	json["Colors"]["FrameBg"]["g"] = style.Colors[ImGuiCol_FrameBg].y;
	json["Colors"]["FrameBg"]["b"] = style.Colors[ImGuiCol_FrameBg].z;
	json["Colors"]["FrameBg"]["a"] = style.Colors[ImGuiCol_FrameBg].w;

	json["Colors"]["FrameBgHovered"]["r"] = style.Colors[ImGuiCol_FrameBgHovered].x;
	json["Colors"]["FrameBgHovered"]["g"] = style.Colors[ImGuiCol_FrameBgHovered].y;
	json["Colors"]["FrameBgHovered"]["b"] = style.Colors[ImGuiCol_FrameBgHovered].z;
	json["Colors"]["FrameBgHovered"]["a"] = style.Colors[ImGuiCol_FrameBgHovered].w;

	json["Colors"]["FrameBgActive"]["r"] = style.Colors[ImGuiCol_FrameBgActive].x;
	json["Colors"]["FrameBgActive"]["g"] = style.Colors[ImGuiCol_FrameBgActive].y;
	json["Colors"]["FrameBgActive"]["b"] = style.Colors[ImGuiCol_FrameBgActive].z;
	json["Colors"]["FrameBgActive"]["a"] = style.Colors[ImGuiCol_FrameBgActive].w;

	json["Colors"]["TitleBg"]["r"] = style.Colors[ImGuiCol_TitleBg].x;
	json["Colors"]["TitleBg"]["g"] = style.Colors[ImGuiCol_TitleBg].y;
	json["Colors"]["TitleBg"]["b"] = style.Colors[ImGuiCol_TitleBg].z;
	json["Colors"]["TitleBg"]["a"] = style.Colors[ImGuiCol_TitleBg].w;

	json["Colors"]["TitleBgActive"]["r"] = style.Colors[ImGuiCol_TitleBgActive].x;
	json["Colors"]["TitleBgActive"]["g"] = style.Colors[ImGuiCol_TitleBgActive].y;
	json["Colors"]["TitleBgActive"]["b"] = style.Colors[ImGuiCol_TitleBgActive].z;
	json["Colors"]["TitleBgActive"]["a"] = style.Colors[ImGuiCol_TitleBgActive].w;

	json["Colors"]["TitleBgCollapsed"]["r"] = style.Colors[ImGuiCol_TitleBgCollapsed].x;
	json["Colors"]["TitleBgCollapsed"]["g"] = style.Colors[ImGuiCol_TitleBgCollapsed].y;
	json["Colors"]["TitleBgCollapsed"]["b"] = style.Colors[ImGuiCol_TitleBgCollapsed].z;
	json["Colors"]["TitleBgCollapsed"]["a"] = style.Colors[ImGuiCol_TitleBgCollapsed].w;

	json["Colors"]["MenuBarBg"]["r"] = style.Colors[ImGuiCol_MenuBarBg].x;
	json["Colors"]["MenuBarBg"]["g"] = style.Colors[ImGuiCol_MenuBarBg].y;
	json["Colors"]["MenuBarBg"]["b"] = style.Colors[ImGuiCol_MenuBarBg].z;
	json["Colors"]["MenuBarBg"]["a"] = style.Colors[ImGuiCol_MenuBarBg].w;

	json["Colors"]["ScrollbarBg"]["r"] = style.Colors[ImGuiCol_ScrollbarBg].x;
	json["Colors"]["ScrollbarBg"]["g"] = style.Colors[ImGuiCol_ScrollbarBg].y;
	json["Colors"]["ScrollbarBg"]["b"] = style.Colors[ImGuiCol_ScrollbarBg].z;
	json["Colors"]["ScrollbarBg"]["a"] = style.Colors[ImGuiCol_ScrollbarBg].w;

	json["Colors"]["ScrollbarGrab"]["r"] = style.Colors[ImGuiCol_ScrollbarGrab].x;
	json["Colors"]["ScrollbarGrab"]["g"] = style.Colors[ImGuiCol_ScrollbarGrab].y;
	json["Colors"]["ScrollbarGrab"]["b"] = style.Colors[ImGuiCol_ScrollbarGrab].z;
	json["Colors"]["ScrollbarGrab"]["a"] = style.Colors[ImGuiCol_ScrollbarGrab].w;

	json["Colors"]["ScrollbarGrabHovered"]["r"] = style.Colors[ImGuiCol_ScrollbarGrabHovered].x;
	json["Colors"]["ScrollbarGrabHovered"]["g"] = style.Colors[ImGuiCol_ScrollbarGrabHovered].y;
	json["Colors"]["ScrollbarGrabHovered"]["b"] = style.Colors[ImGuiCol_ScrollbarGrabHovered].z;
	json["Colors"]["ScrollbarGrabHovered"]["a"] = style.Colors[ImGuiCol_ScrollbarGrabHovered].w;

	json["Colors"]["ScrollbarGrabActive"]["r"] = style.Colors[ImGuiCol_ScrollbarGrabActive].x;
	json["Colors"]["ScrollbarGrabActive"]["g"] = style.Colors[ImGuiCol_ScrollbarGrabActive].y;
	json["Colors"]["ScrollbarGrabActive"]["b"] = style.Colors[ImGuiCol_ScrollbarGrabActive].z;
	json["Colors"]["ScrollbarGrabActive"]["a"] = style.Colors[ImGuiCol_ScrollbarGrabActive].w;

	json["Colors"]["CheckMark"]["r"] = style.Colors[ImGuiCol_CheckMark].x;
	json["Colors"]["CheckMark"]["g"] = style.Colors[ImGuiCol_CheckMark].y;
	json["Colors"]["CheckMark"]["b"] = style.Colors[ImGuiCol_CheckMark].z;
	json["Colors"]["CheckMark"]["a"] = style.Colors[ImGuiCol_CheckMark].w;

	json["Colors"]["SliderGrab"]["r"] = style.Colors[ImGuiCol_SliderGrab].x;
	json["Colors"]["SliderGrab"]["g"] = style.Colors[ImGuiCol_SliderGrab].y;
	json["Colors"]["SliderGrab"]["b"] = style.Colors[ImGuiCol_SliderGrab].z;
	json["Colors"]["SliderGrab"]["a"] = style.Colors[ImGuiCol_SliderGrab].w;

	json["Colors"]["SliderGrabActive"]["r"] = style.Colors[ImGuiCol_SliderGrabActive].x;
	json["Colors"]["SliderGrabActive"]["g"] = style.Colors[ImGuiCol_SliderGrabActive].y;
	json["Colors"]["SliderGrabActive"]["b"] = style.Colors[ImGuiCol_SliderGrabActive].z;
	json["Colors"]["SliderGrabActive"]["a"] = style.Colors[ImGuiCol_SliderGrabActive].w;

	json["Colors"]["Button"]["r"] = style.Colors[ImGuiCol_Button].x;
	json["Colors"]["Button"]["g"] = style.Colors[ImGuiCol_Button].y;
	json["Colors"]["Button"]["b"] = style.Colors[ImGuiCol_Button].z;
	json["Colors"]["Button"]["a"] = style.Colors[ImGuiCol_Button].w;

	json["Colors"]["ButtonHovered"]["r"] = style.Colors[ImGuiCol_ButtonHovered].x;
	json["Colors"]["ButtonHovered"]["g"] = style.Colors[ImGuiCol_ButtonHovered].y;
	json["Colors"]["ButtonHovered"]["b"] = style.Colors[ImGuiCol_ButtonHovered].z;
	json["Colors"]["ButtonHovered"]["a"] = style.Colors[ImGuiCol_ButtonHovered].w;

	json["Colors"]["ButtonActive"]["r"] = style.Colors[ImGuiCol_ButtonActive].x;
	json["Colors"]["ButtonActive"]["g"] = style.Colors[ImGuiCol_ButtonActive].y;
	json["Colors"]["ButtonActive"]["b"] = style.Colors[ImGuiCol_ButtonActive].z;
	json["Colors"]["ButtonActive"]["a"] = style.Colors[ImGuiCol_ButtonActive].w;

	json["Colors"]["Header"]["r"] = style.Colors[ImGuiCol_Header].x;
	json["Colors"]["Header"]["g"] = style.Colors[ImGuiCol_Header].y;
	json["Colors"]["Header"]["b"] = style.Colors[ImGuiCol_Header].z;
	json["Colors"]["Header"]["a"] = style.Colors[ImGuiCol_Header].w;

	json["Colors"]["HeaderHovered"]["r"] = style.Colors[ImGuiCol_HeaderHovered].x;
	json["Colors"]["HeaderHovered"]["g"] = style.Colors[ImGuiCol_HeaderHovered].y;
	json["Colors"]["HeaderHovered"]["b"] = style.Colors[ImGuiCol_HeaderHovered].z;
	json["Colors"]["HeaderHovered"]["a"] = style.Colors[ImGuiCol_HeaderHovered].w;

	json["Colors"]["HeaderActive"]["r"] = style.Colors[ImGuiCol_HeaderActive].x;
	json["Colors"]["HeaderActive"]["g"] = style.Colors[ImGuiCol_HeaderActive].y;
	json["Colors"]["HeaderActive"]["b"] = style.Colors[ImGuiCol_HeaderActive].z;
	json["Colors"]["HeaderActive"]["a"] = style.Colors[ImGuiCol_HeaderActive].w;

	json["Colors"]["Separator"]["r"] = style.Colors[ImGuiCol_Separator].x;
	json["Colors"]["Separator"]["g"] = style.Colors[ImGuiCol_Separator].y;
	json["Colors"]["Separator"]["b"] = style.Colors[ImGuiCol_Separator].z;
	json["Colors"]["Separator"]["a"] = style.Colors[ImGuiCol_Separator].w;

	json["Colors"]["SeparatorHovered"]["r"] = style.Colors[ImGuiCol_SeparatorHovered].x;
	json["Colors"]["SeparatorHovered"]["g"] = style.Colors[ImGuiCol_SeparatorHovered].y;
	json["Colors"]["SeparatorHovered"]["b"] = style.Colors[ImGuiCol_SeparatorHovered].z;
	json["Colors"]["SeparatorHovered"]["a"] = style.Colors[ImGuiCol_SeparatorHovered].w;

	json["Colors"]["SeparatorActive"]["r"] = style.Colors[ImGuiCol_SeparatorActive].x;
	json["Colors"]["SeparatorActive"]["g"] = style.Colors[ImGuiCol_SeparatorActive].y;
	json["Colors"]["SeparatorActive"]["b"] = style.Colors[ImGuiCol_SeparatorActive].z;
	json["Colors"]["SeparatorActive"]["a"] = style.Colors[ImGuiCol_SeparatorActive].w;

	json["Colors"]["ResizeGrip"]["r"] = style.Colors[ImGuiCol_ResizeGrip].x;
	json["Colors"]["ResizeGrip"]["g"] = style.Colors[ImGuiCol_ResizeGrip].y;
	json["Colors"]["ResizeGrip"]["b"] = style.Colors[ImGuiCol_ResizeGrip].z;
	json["Colors"]["ResizeGrip"]["a"] = style.Colors[ImGuiCol_ResizeGrip].w;

	json["Colors"]["ResizeGripHovered"]["r"] = style.Colors[ImGuiCol_ResizeGripHovered].x;
	json["Colors"]["ResizeGripHovered"]["g"] = style.Colors[ImGuiCol_ResizeGripHovered].y;
	json["Colors"]["ResizeGripHovered"]["b"] = style.Colors[ImGuiCol_ResizeGripHovered].z;
	json["Colors"]["ResizeGripHovered"]["a"] = style.Colors[ImGuiCol_ResizeGripHovered].w;

	json["Colors"]["ResizeGripActive"]["r"] = style.Colors[ImGuiCol_ResizeGripActive].x;
	json["Colors"]["ResizeGripActive"]["g"] = style.Colors[ImGuiCol_ResizeGripActive].y;
	json["Colors"]["ResizeGripActive"]["b"] = style.Colors[ImGuiCol_ResizeGripActive].z;
	json["Colors"]["ResizeGripActive"]["a"] = style.Colors[ImGuiCol_ResizeGripActive].w;

	json["Colors"]["Tab"]["r"] = style.Colors[ImGuiCol_Tab].x;
	json["Colors"]["Tab"]["g"] = style.Colors[ImGuiCol_Tab].y;
	json["Colors"]["Tab"]["b"] = style.Colors[ImGuiCol_Tab].z;
	json["Colors"]["Tab"]["a"] = style.Colors[ImGuiCol_Tab].w;

	json["Colors"]["TabHovered"]["r"] = style.Colors[ImGuiCol_TabHovered].x;
	json["Colors"]["TabHovered"]["g"] = style.Colors[ImGuiCol_TabHovered].y;
	json["Colors"]["TabHovered"]["b"] = style.Colors[ImGuiCol_TabHovered].z;
	json["Colors"]["TabHovered"]["a"] = style.Colors[ImGuiCol_TabHovered].w;

	json["Colors"]["TabActive"]["r"] = style.Colors[ImGuiCol_TabActive].x;
	json["Colors"]["TabActive"]["g"] = style.Colors[ImGuiCol_TabActive].y;
	json["Colors"]["TabActive"]["b"] = style.Colors[ImGuiCol_TabActive].z;
	json["Colors"]["TabActive"]["a"] = style.Colors[ImGuiCol_TabActive].w;

	json["Colors"]["TabUnfocused"]["r"] = style.Colors[ImGuiCol_TabUnfocused].x;
	json["Colors"]["TabUnfocused"]["g"] = style.Colors[ImGuiCol_TabUnfocused].y;
	json["Colors"]["TabUnfocused"]["b"] = style.Colors[ImGuiCol_TabUnfocused].z;
	json["Colors"]["TabUnfocused"]["a"] = style.Colors[ImGuiCol_TabUnfocused].w;

	json["Colors"]["TabUnfocusedActive"]["r"] = style.Colors[ImGuiCol_TabUnfocusedActive].x;
	json["Colors"]["TabUnfocusedActive"]["g"] = style.Colors[ImGuiCol_TabUnfocusedActive].y;
	json["Colors"]["TabUnfocusedActive"]["b"] = style.Colors[ImGuiCol_TabUnfocusedActive].z;
	json["Colors"]["TabUnfocusedActive"]["a"] = style.Colors[ImGuiCol_TabUnfocusedActive].w;

	json["Colors"]["PlotLines"]["r"] = style.Colors[ImGuiCol_PlotLines].x;
	json["Colors"]["PlotLines"]["g"] = style.Colors[ImGuiCol_PlotLines].y;
	json["Colors"]["PlotLines"]["b"] = style.Colors[ImGuiCol_PlotLines].z;
	json["Colors"]["PlotLines"]["a"] = style.Colors[ImGuiCol_PlotLines].w;

	json["Colors"]["PlotLinesHovered"]["r"] = style.Colors[ImGuiCol_PlotLinesHovered].x;
	json["Colors"]["PlotLinesHovered"]["g"] = style.Colors[ImGuiCol_PlotLinesHovered].y;
	json["Colors"]["PlotLinesHovered"]["b"] = style.Colors[ImGuiCol_PlotLinesHovered].z;
	json["Colors"]["PlotLinesHovered"]["a"] = style.Colors[ImGuiCol_PlotLinesHovered].w;

	json["Colors"]["PlotHistogram"]["r"] = style.Colors[ImGuiCol_PlotHistogram].x;
	json["Colors"]["PlotHistogram"]["g"] = style.Colors[ImGuiCol_PlotHistogram].y;
	json["Colors"]["PlotHistogram"]["b"] = style.Colors[ImGuiCol_PlotHistogram].z;
	json["Colors"]["PlotHistogram"]["a"] = style.Colors[ImGuiCol_PlotHistogram].w;

	json["Colors"]["PlotHistogramHovered"]["r"] = style.Colors[ImGuiCol_PlotHistogramHovered].x;
	json["Colors"]["PlotHistogramHovered"]["g"] = style.Colors[ImGuiCol_PlotHistogramHovered].y;
	json["Colors"]["PlotHistogramHovered"]["b"] = style.Colors[ImGuiCol_PlotHistogramHovered].z;
	json["Colors"]["PlotHistogramHovered"]["a"] = style.Colors[ImGuiCol_PlotHistogramHovered].w;

	json["Colors"]["TableHeaderBg"]["r"] = style.Colors[ImGuiCol_TableHeaderBg].x;
	json["Colors"]["TableHeaderBg"]["g"] = style.Colors[ImGuiCol_TableHeaderBg].y;
	json["Colors"]["TableHeaderBg"]["b"] = style.Colors[ImGuiCol_TableHeaderBg].z;
	json["Colors"]["TableHeaderBg"]["a"] = style.Colors[ImGuiCol_TableHeaderBg].w;

	json["Colors"]["TableBorderStrong"]["r"] = style.Colors[ImGuiCol_Border].x;
	json["Colors"]["TableBorderStrong"]["g"] = style.Colors[ImGuiCol_Border].y;
	json["Colors"]["TableBorderStrong"]["b"] = style.Colors[ImGuiCol_Border].z;
	json["Colors"]["TableBorderStrong"]["a"] = style.Colors[ImGuiCol_Border].w;

	json["Colors"]["TableBorderLight"]["r"] = style.Colors[ImGuiCol_Border].x;
	json["Colors"]["TableBorderLight"]["g"] = style.Colors[ImGuiCol_Border].y;
	json["Colors"]["TableBorderLight"]["b"] = style.Colors[ImGuiCol_Border].z;
	json["Colors"]["TableBorderLight"]["a"] = 1.0f;

	json["Colors"]["TableRowBg"]["r"] = style.Colors[ImGuiCol_TableRowBg].x;
	json["Colors"]["TableRowBg"]["g"] = style.Colors[ImGuiCol_TableRowBg].y;
	json["Colors"]["TableRowBg"]["b"] = style.Colors[ImGuiCol_TableRowBg].z;
	json["Colors"]["TableRowBg"]["a"] = style.Colors[ImGuiCol_TableRowBg].w;

	json["Colors"]["TableRowBgAlt"]["r"] = style.Colors[ImGuiCol_TableRowBgAlt].x;
	json["Colors"]["TableRowBgAlt"]["g"] = style.Colors[ImGuiCol_TableRowBgAlt].y;
	json["Colors"]["TableRowBgAlt"]["b"] = style.Colors[ImGuiCol_TableRowBgAlt].z;
	json["Colors"]["TableRowBgAlt"]["a"] = style.Colors[ImGuiCol_TableRowBgAlt].w;

	json["Colors"]["TextSelectedBg"]["r"] = style.Colors[ImGuiCol_TextSelectedBg].x;
	json["Colors"]["TextSelectedBg"]["g"] = style.Colors[ImGuiCol_TextSelectedBg].y;
	json["Colors"]["TextSelectedBg"]["b"] = style.Colors[ImGuiCol_TextSelectedBg].z;
	json["Colors"]["TextSelectedBg"]["a"] = style.Colors[ImGuiCol_TextSelectedBg].w;

	json["Colors"]["DragDropTarget"]["r"] = style.Colors[ImGuiCol_DragDropTarget].x;
	json["Colors"]["DragDropTarget"]["g"] = style.Colors[ImGuiCol_DragDropTarget].y;
	json["Colors"]["DragDropTarget"]["b"] = style.Colors[ImGuiCol_DragDropTarget].z;
	json["Colors"]["DragDropTarget"]["a"] = style.Colors[ImGuiCol_DragDropTarget].w;

	json["Colors"]["NavHighlight"]["r"] = style.Colors[ImGuiCol_NavHighlight].x;
	json["Colors"]["NavHighlight"]["g"] = style.Colors[ImGuiCol_NavHighlight].y;
	json["Colors"]["NavHighlight"]["b"] = style.Colors[ImGuiCol_NavHighlight].z;
	json["Colors"]["NavHighlight"]["a"] = style.Colors[ImGuiCol_NavHighlight].w;

	json["Colors"]["NavWindowingHighlight"]["r"] = style.Colors[ImGuiCol_NavWindowingHighlight].x;
	json["Colors"]["NavWindowingHighlight"]["g"] = style.Colors[ImGuiCol_NavWindowingHighlight].y;
	json["Colors"]["NavWindowingHighlight"]["b"] = style.Colors[ImGuiCol_NavWindowingHighlight].z;
	json["Colors"]["NavWindowingHighlight"]["a"] = style.Colors[ImGuiCol_NavWindowingHighlight].w;

	json["Colors"]["NavWindowingDimBg"]["r"] = style.Colors[ImGuiCol_NavWindowingDimBg].x;
	json["Colors"]["NavWindowingDimBg"]["g"] = style.Colors[ImGuiCol_NavWindowingDimBg].y;
	json["Colors"]["NavWindowingDimBg"]["b"] = style.Colors[ImGuiCol_NavWindowingDimBg].z;
	json["Colors"]["NavWindowingDimBg"]["a"] = style.Colors[ImGuiCol_NavWindowingDimBg].w;

	json["Colors"]["ModalWindowDimBg"]["r"] = style.Colors[ImGuiCol_ModalWindowDimBg].x;
	json["Colors"]["ModalWindowDimBg"]["g"] = style.Colors[ImGuiCol_ModalWindowDimBg].y;
	json["Colors"]["ModalWindowDimBg"]["b"] = style.Colors[ImGuiCol_ModalWindowDimBg].z;
	json["Colors"]["ModalWindowDimBg"]["a"] = style.Colors[ImGuiCol_ModalWindowDimBg].w;


	std::ofstream o(file);
	o << json;
	o.close();

}

void Themes::ScanThemes()
{
	ThemeData data;
	data.name = "Dark";
	m_Themes.push_back(data);
	data.name = "Light";
	m_Themes.push_back(data);


	std::filesystem::path dirPath("res/themes");

	for (const auto& entry : std::filesystem::directory_iterator(dirPath))
	{
		if (std::filesystem::is_regular_file(entry.path()))
		{
			m_Themes.push_back(LoadTheme(entry.path().string()));
		}
	}
}

void Themes::LoadThemeFromName(const std::string &name)
{

	if (name == "Dark")
	{
		ImGui::StyleColorsDark();
		return;
	}
	if (name == "Light")
	{
		ImGui::StyleColorsLight();
		return;
	}

	for (const auto& theme : m_Themes)
	{
		if (theme.name == name)
		{
			ImGuiStyle& style = ImGui::GetStyle();
			style = theme.style;
		}
	}
}
