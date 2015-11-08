//
// ImGuiRenderer.cpp
//

#include "ImGuiRenderer.h"
#include "imgui_impl_dx9.h"
#include "MQ2Navigation.h"
#include "MQ2Nav_Hooks.h"

#include <imgui.h>

//----------------------------------------------------------------------------

// These are defined in imgui
//unsigned int GetDroidSansCompressedSize();
//const unsigned int* GetDroidSansCompressedData();

std::shared_ptr<ImGuiRenderer> g_imguiRenderer;

ImGuiRenderer::ImGuiRenderer(HWND eqhwnd, IDirect3DDevice9* device)
	: m_pDevice(device)
{
	// Iniialize the ImGui overlay
	ImGui_ImplDX9_Init(eqhwnd, device);

	ImGuiStyle& mStyle = ImGui::GetStyle();
	mStyle.WindowMinSize = ImVec2(160, 20);
	mStyle.FramePadding = ImVec2(4, 2);
	mStyle.ItemSpacing = ImVec2(6, 2);
	mStyle.ItemInnerSpacing = ImVec2(6, 4);
	mStyle.Alpha = 0.95f;
	mStyle.WindowFillAlphaDefault = 1.0f;
	mStyle.WindowRounding = 4.0f;
	mStyle.FrameRounding = 2.0f;
	mStyle.IndentSpacing = 6.0f;
	mStyle.ItemInnerSpacing = ImVec2(2, 4);
	mStyle.ColumnsMinSpacing = 50.0f;
	mStyle.GrabMinSize = 14.0f;
	mStyle.GrabRounding = 16.0f;
	mStyle.ScrollbarSize = 12.0f;
	mStyle.ScrollbarRounding = 16.0f;

	ImGuiStyle& style = mStyle;
	style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.61f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.58f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.21f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.92f, 0.18f, 0.29f, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.47f, 0.77f, 0.83f, 0.32f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.86f, 0.93f, 0.89f, 0.16f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.86f, 0.93f, 0.89f, 0.39f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.92f, 0.18f, 0.29f, 0.43f);
	style.Colors[ImGuiCol_TooltipBg] = ImVec4(0.47f, 0.77f, 0.83f, 0.72f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.22f, 0.27f, 0.73f);


	m_pDevice->AddRef();

	m_prevHistoryPoint = std::chrono::system_clock::now();
	m_renderFrameRateHistory.clear();

	// Initialize font
	//ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->AddFontFromMemoryCompressedTTF(GetDroidSansCompressedData(),
	//	GetDroidSansCompressedSize(), 16);
}

ImGuiRenderer::~ImGuiRenderer()
{
	InvalidateDeviceObjects();

	// Cleanup the ImGui overlay
	ImGui_ImplDX9_Shutdown();

	m_pDevice->Release();
	m_pDevice = nullptr;
}

void ImGuiRenderer::InvalidateDeviceObjects()
{
	m_imguiReady = false;

	ImGui_ImplDX9_InvalidateDeviceObjects();
}

bool ImGuiRenderer::CreateDeviceObjects()
{
	return ImGui_ImplDX9_CreateDeviceObjects();
}

void ImGuiRenderer::Render(RenderPhase phase)
{
	if (phase != Render_UI)
		return;

	if (m_imguiReady)
	{
		if (m_imguiRender)
		{
			DrawUI();

			m_imguiRender = false;
		}

		ImGui::Render();
	}
}

void ImGuiRenderer::BeginNewFrame()
{
	m_imguiReady = ImGui_ImplDX9_NewFrame();
	m_imguiRender = true;
}

//----------------------------------------------------------------------------

void RenderInputUI();

static void DrawMatrix(D3DXMATRIX& matrix, const char* name)
{
	if (ImGui::CollapsingHeader(name, 0, true, true))
	{
		ImGui::Columns(4, name);
		ImGui::Separator();
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				FLOAT f = matrix(row, col);

				char label[32];
				sprintf(label, "%.2f", f);
				ImGui::Text(label); ImGui::NextColumn();
			}
		}
		ImGui::Columns(1);
	}
}

void ImGuiRenderer::DrawUI()
{
#if 0

	auto now = std::chrono::system_clock::now();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(now - m_prevHistoryPoint).count() >= 100)
	{
		if (m_renderFrameRateHistory.size() > 200) {
			m_renderFrameRateHistory.erase(m_renderFrameRateHistory.begin(), m_renderFrameRateHistory.begin() + 20);
		}
		m_renderFrameRateHistory.push_back(ImGui::GetIO().Framerate);
		m_prevHistoryPoint = now;
	}

	// 1. Show a simple window
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(325, 700), ImGuiSetCond_Once);
	ImGui::Begin("Debug");
	{
		if (m_renderFrameRateHistory.size() > 0)
		{
			ImGui::PlotLines("", &m_renderFrameRateHistory[0], (int)m_renderFrameRateHistory.size(), 0, nullptr, 0.0);
		}

		ImGui::LabelText("FPS", "%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		PSPAWNINFO me = GetCharInfo()->pSpawn;
		if (me)
		{
			float rotation = (float)me->Heading / 256.0 * PI;
			ImGui::LabelText("Position", "%.2f %.2f %.2f", me->X, me->Y, me->Z);
			ImGui::LabelText("Heading", "%.2f", rotation);
		}

		RenderInputUI();

		//DrawMatrix(m_worldMatrix, "World Matrix");
		//DrawMatrix(m_viewMatrix, "View Matrix");
		//DrawMatrix(m_projMatrix, "Projection Matrix");
	}
	ImGui::End();
#endif
	OnUpdateUI();
}


void RenderInputUI()
{
	return;

	if (ImGui::CollapsingHeader("Keyboard", "##keyboard", true, true))
	{
		ImGui::Text("Keyboard");
		ImGui::SameLine();

		if (ImGui::GetIO().WantCaptureKeyboard)
			ImGui::TextColored(ImColor(0, 255, 0), "Captured");
		else
			ImGui::TextColored(ImColor(255, 0, 0), "Not Captured");

		ImGui::LabelText("InputCharacters", "%S", ImGui::GetIO().InputCharacters);

		static char testEdit[256];
		ImGui::InputText("Test Edit", testEdit, 256);
	}

	if (ImGui::CollapsingHeader("Mouse", "##mouse", true, true))
	{
		ImGui::Text("Position: (%d, %d)", MouseLocation, MouseLocation.y);

		ImGui::Text("Mouse");
		ImGui::SameLine();

		if (ImGui::GetIO().WantCaptureMouse)
			ImGui::TextColored(ImColor(0, 255, 0), "Captured");
		else
			ImGui::TextColored(ImColor(255, 0, 0), "Not Captured");

		ImGui::Text("Mouse");
		ImGui::SameLine();

		if (!MouseBlocked)
			ImGui::TextColored(ImColor(0, 255, 0), "Not Blocked");
		else
			ImGui::TextColored(ImColor(255, 0, 0), "Blocked");

		// Clicks? Clicks!
		PMOUSECLICK clicks = EQADDR_MOUSECLICK;

		ImGui::SetNextTreeNodeOpened(true, ImGuiSetCond_Once);
		if (ImGui::TreeNode("##clicks", "EQ Mouse Clicks"))
		{
			ImGui::Columns(3);

			ImGui::Text("Button"); ImGui::NextColumn();
			ImGui::Text("Click"); ImGui::NextColumn();
			ImGui::Text("Confirm"); ImGui::NextColumn();

			for (int i = 0; i < 5; i++)
			{
				ImGui::Text("%d", i); ImGui::NextColumn();
				ImGui::Text("%d", clicks->Click[i]); ImGui::NextColumn();
				ImGui::Text("%d", clicks->Confirm[i]); ImGui::NextColumn();
			}

			ImGui::Columns(1);
			ImGui::TreePop();
		}

		ImGui::SetNextTreeNodeOpened(true, ImGuiSetCond_Once);
		if (ImGui::TreeNode("##position", "EQ Mouse State"))
		{
			ImGui::LabelText("Position", "(%d, %d)", MouseState->x, MouseState->y);
			ImGui::LabelText("Scroll", "%d", MouseState->Scroll);
			ImGui::LabelText("Relative", "%d, %d", MouseState->relX, MouseState->relY);
			ImGui::LabelText("Extra", "%d", MouseState->InWindow);

			ImGui::TreePop();
		}

		ImGui::SetNextTreeNodeOpened(true, ImGuiSetCond_Once);
		if (ImGui::TreeNode("##mouseinfo", "EQ Mouse Info"))
		{
			int pos[2] = { EQADDR_MOUSE->X, EQADDR_MOUSE->Y };
			ImGui::InputInt2("Pos", pos);

			int speed[2] = { EQADDR_MOUSE->SpeedX, EQADDR_MOUSE->SpeedY };
			ImGui::InputInt2("Speed", speed);

			int scroll = MouseInfo->Scroll;
			ImGui::InputInt("Scroll", &scroll);

			ImGui::TreePop();
		}
	}
}