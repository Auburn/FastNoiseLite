#include <algorithm>
#include <chrono>
#include <cstdint>
#include <sstream>

#include <imgui_app/ImGui_Application.hpp>
#include <imgui_internal.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <emscripten.h>
#include <emscripten_browser_file.h>

#include "../Cpp/FastNoiseLite.h"
#include "FastNoiseLite.h"

class FastNoiseLitePreviewApp : public ImGui_Application
{
public:
    bool init() override
    {
        bool success = ImGui_Application::init();
        set_window_title("FastNoise Lite GUI");

        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        return success;
    }

    uint32_t noiseTex = 0;
    ImVec2 noiseTexSize;
    int noiseTexSizeGenX;
    int noiseTexSizeGenY;
    float noiseTexMin = -1;
    float noiseTexMax = 1;
    int previewSize[2] = { 768, 768 };
    bool preview3d = false;
    float previewScroll = 0;
    double previewPosZ = 0;
    bool previewDomainWarp = false;
    bool previewAutoSize = false;

    float previewGenTime = 0;
    float previewGenTimeFinal = 0;
    float previewMin = 0;
    float previewMinFinal = 0;
    float previewMax = 0;
    float previewMaxFinal = 0;
    float previewMean = 0;
    float previewMeanFinal = 0;
    bool previewTriggerSave = false;
    int previewPixelY = 0;
    unsigned char* previewPixelArray = nullptr;

    FastNoiseLite fnl;
    FastNoiseLite fnlWarp;

    int fnlNoiseType = 0;
    int fnlRotationType = 0;
    int fnlSeed = 1337;
    float fnlFrequency = 0.01f;

    int fnlFractalType = 0;
    int fnlFractalOctaves = 3;
    float fnlFractalLacunarity = 2.0f;
    float fnlFractalGain = 0.5f;
    float fnlFractalWeightedStrength = 0.0f;
    float fnlFractalPingPongStrength = 2.0f;

    int fnlCellularType = 1;
    int fnlCellularReturnType = 1;
    float fnlCellularJitter = 1.0f;

    int fnlDomainWarpSeed = 1337;
    float fnlDomainWarpFrequency = 0.01f;
    int fnlDomainWarpType = 0;
    int fnlDomainWarpRotationType = 0;
    float fnlDomainWarpAmplitude = 1.0f;

    int fnlDomainWarpFractalType = 0;
    int fnlDomainWarpFractalOctaves = 3;
    float fnlDomainWarpFractalLacunarity = 2.0f;
    float fnlDomainWarpFractalGain = 0.5f;

    void draw_ui() override
    {
        static const char* enumNoiseType[] = { "OpenSimplex2", "OpenSimplex2S", "Cellular", "Perlin", "Value Cubic", "Value" };
        static const char* enumRotationType[] = { "None", "Improve XY Planes", "Improve XZ Planes" };
        static const char* enumFractalType[] = { "None", "FBm", "Ridged", "Ping Pong" };
        static const char* enumCellularType[] = { "Euclidean", "Euclidean Sq", "Manhattan", "Hybrid" };
        static const char* enumCellularReturnType[] = { "Cell Value", "Distance", "Distance 2", "Distance 2 Add", "Distance 2 Sub", "Distance 2 Mul", "Distance 2 Div" };
        static const char* enumDomainWarpType[] = { "None", "OpenSimplex2", "OpenSimplex2 Reduced", "Basic Grid" };
        static const char* enumDomainWarpFractalType[] = { "None", "Progressive", "Independent" };

        SetupDocking();

        // ImGui::ShowDemoWindow();

        bool texUpdate = false;

        ImGui::Begin("Settings");
        ImGui::PushItemWidth(120);

        ImGui::BeginTabBar("Tabs");
        if (ImGui::BeginTabItem("FastNoise Lite"))
        {
            /// General
            ImGui::TextUnformatted("General");

            if (ImGui::Combo("Noise Type", &fnlNoiseType, enumNoiseType, IM_ARRAYSIZE(enumNoiseType)))
            {
                fnl.SetNoiseType((FastNoiseLite::NoiseType)fnlNoiseType);
                texUpdate = true;
            }
            ImGui::BeginDisabled(!preview3d);
            if (ImGui::Combo("Rotation Type 3D", &fnlRotationType, enumRotationType, IM_ARRAYSIZE(enumRotationType)))
            {
                fnl.SetRotationType3D((FastNoiseLite::RotationType3D)fnlRotationType);
                texUpdate = true;
            }
            ImGui::EndDisabled();
            if (ImGui::DragInt("Seed", &fnlSeed))
            {
                fnl.SetSeed(fnlSeed);
                texUpdate = true;
            }
            if (ImGui::DragFloat("Frequency", &fnlFrequency, 0.0002f))
            {
                fnl.SetFrequency(fnlFrequency);
                texUpdate = true;
            }

            /// Fractal
            ImGui::TextUnformatted("Fractal");

            if (ImGui::Combo("Type", &fnlFractalType, enumFractalType, IM_ARRAYSIZE(enumFractalType)))
            {
                fnl.SetFractalType((FastNoiseLite::FractalType)fnlFractalType);
                texUpdate = true;
            }
            ImGui::BeginDisabled(fnlFractalType == 0);
            if (ImGui::DragInt("Octaves", &fnlFractalOctaves, 0.1f, 1, 20))
            {
                fnl.SetFractalOctaves(fnlFractalOctaves);
                texUpdate = true;
            }
            if (ImGui::DragFloat("Lacunarity", &fnlFractalLacunarity, 0.01f))
            {
                fnl.SetFractalLacunarity(fnlFractalLacunarity);
                texUpdate = true;
            }
            if (ImGui::DragFloat("Gain", &fnlFractalGain, 0.01f))
            {
                fnl.SetFractalGain(fnlFractalGain);
                texUpdate = true;
            }
            if (ImGui::DragFloat("Weighted Strength", &fnlFractalWeightedStrength, 0.01f))
            {
                fnl.SetFractalWeightedStrength(fnlFractalWeightedStrength);
                texUpdate = true;
            }
            ImGui::BeginDisabled(fnlFractalType != (int)FastNoiseLite::FractalType_PingPong);
            if (ImGui::DragFloat("Ping Pong Strength", &fnlFractalPingPongStrength, 0.01f))
            {
                fnl.SetFractalPingPongStrength(fnlFractalPingPongStrength);
                texUpdate = true;
            }
            ImGui::EndDisabled();
            ImGui::EndDisabled();

            /// Cellular
            ImGui::TextUnformatted("Cellular");

            ImGui::BeginDisabled(fnlNoiseType != (int)FastNoiseLite::NoiseType_Cellular);
            if (ImGui::Combo("Distance Function", &fnlCellularType, enumCellularType, IM_ARRAYSIZE(enumCellularType)))
            {
                fnl.SetCellularDistanceFunction((FastNoiseLite::CellularDistanceFunction)fnlCellularType);
                texUpdate = true;
            }
            if (ImGui::Combo("Return Type", &fnlCellularReturnType, enumCellularReturnType, IM_ARRAYSIZE(enumCellularReturnType)))
            {
                fnl.SetCellularReturnType((FastNoiseLite::CellularReturnType)fnlCellularReturnType);
                texUpdate = true;
            }
            if (ImGui::DragFloat("Jitter", &fnlCellularJitter, 0.01f))
            {
                fnl.SetCellularJitter(fnlCellularJitter);
                texUpdate = true;
            }
            ImGui::EndDisabled();

            /// Domain Warp
            ImGui::PushID("Domain Warp");
            ImGui::TextUnformatted("Domain Warp");

            if (ImGui::Combo("Type", &fnlDomainWarpType, enumDomainWarpType, IM_ARRAYSIZE(enumDomainWarpType)))
            {
                fnlWarp.SetDomainWarpType((FastNoiseLite::DomainWarpType)(fnlDomainWarpType - 1));
                texUpdate = true;
            }
            ImGui::BeginDisabled(fnlDomainWarpType == 0);
            ImGui::BeginDisabled(!preview3d);
            if (ImGui::Combo("Rotation Type 3D", &fnlDomainWarpRotationType, enumRotationType, IM_ARRAYSIZE(enumRotationType)))
            {
                fnlWarp.SetRotationType3D((FastNoiseLite::RotationType3D)fnlDomainWarpRotationType);
                texUpdate = true;
            }
            ImGui::EndDisabled();
            if (ImGui::DragFloat("Amplitude", &fnlDomainWarpAmplitude, 0.5f))
            {
                fnlWarp.SetDomainWarpAmp(fnlDomainWarpAmplitude);
                texUpdate = true;
            }
            if (ImGui::DragInt("Seed", &fnlDomainWarpSeed))
            {
                fnlWarp.SetSeed(fnlDomainWarpSeed);
                texUpdate = true;
            }
            if (ImGui::DragFloat("Frequency", &fnlDomainWarpFrequency, 0.001f))
            {
                fnlWarp.SetFrequency(fnlDomainWarpFrequency);
                texUpdate = true;
            }

            /// Domain Warp Fractal
            ImGui::PushID("Domain Warp Fractal");
            ImGui::TextUnformatted("Domain Warp Fractal");

            if (ImGui::Combo("Type", &fnlDomainWarpFractalType, enumDomainWarpFractalType, IM_ARRAYSIZE(enumDomainWarpFractalType)))
            {
                fnlWarp.SetFractalType((FastNoiseLite::FractalType)(fnlDomainWarpFractalType ? fnlDomainWarpFractalType + 3 : 0));
                texUpdate = true;
            }
            ImGui::BeginDisabled(fnlDomainWarpFractalType == 0);
            if (ImGui::DragInt("Octaves", &fnlDomainWarpFractalOctaves, 0.1f, 1, 20))
            {
                fnlWarp.SetFractalOctaves(fnlDomainWarpFractalOctaves);
                texUpdate = true;
            }
            if (ImGui::DragFloat("Lacunarity", &fnlDomainWarpFractalLacunarity, 0.01f))
            {
                fnlWarp.SetFractalLacunarity(fnlDomainWarpFractalLacunarity);
                texUpdate = true;
            }
            if (ImGui::DragFloat("Gain", &fnlFractalGain, 0.01f))
            {
                fnlWarp.SetFractalGain(fnlDomainWarpFractalGain);
                texUpdate = true;
            }
            ImGui::EndDisabled();
            ImGui::EndDisabled();
            ImGui::PopID();
            ImGui::PopID();

            ImGui::NewLine();
            ImGui::TextUnformatted(FNL_VERSION);

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Preview Settings"))
        {
            int sizeXY[] = { (int)noiseTexSize.x, (int)noiseTexSize.y };

            ImGui::Checkbox("Auto Size", &previewAutoSize);
            ImGui::BeginDisabled(previewAutoSize);
            ImGui::DragInt2("Size", previewSize, 1, 32, 4096);
            ImGui::EndDisabled();
            if (ImGui::DragFloat("Black Point", &noiseTexMin, 0.01f))
            {
                texUpdate = true;
            }
            if (ImGui::DragFloat("White Point", &noiseTexMax, 0.01f))
            {
                texUpdate = true;
            }
            if (ImGui::Checkbox("3D", &preview3d))
            {
                texUpdate = true;
            }
            if (preview3d)
            {
                ImGui::Indent();
                ImGui::DragFloat("Scroll Speed", &previewScroll, 0.02f);

                ImGui::BeginDisabled(previewScroll != 0);
                float floatPosZ = (float)previewPosZ;
                if (ImGui::DragFloat("Z Position", &floatPosZ))
                {
                    previewPosZ = floatPosZ;
                    texUpdate = true;
                }
                ImGui::EndDisabled();

                ImGui::Unindent();
            }

            ImGui::NewLine();
            if (ImGui::Button("Save Preview"))
            {
                previewTriggerSave = true;
            }

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
        ImGui::PopItemWidth();
        ImGui::End();

        ImGui::Begin("Noise Texture");

        if (previewAutoSize)
        {
            ImVec2 autoSize = ImGui::GetContentRegionAvail();
            previewSize[0] = autoSize.x;
            previewSize[1] = autoSize.y;
        }
        if (previewPixelY == 0)
        {
            if (noiseTexSizeGenX != previewSize[0] || noiseTexSizeGenY != previewSize[1])
            {
                texUpdate = true;
            }
            if (preview3d && previewScroll != 0)
            {
                previewPosZ += previewScroll;
                texUpdate = true;
            }
        }

        UpdateTexture(texUpdate);

        if (previewTriggerSave && previewPixelArray && previewPixelY == 0)
        {
            previewTriggerSave = false;
            std::string bmpFile = EncodeBMP((int)noiseTexSize.x, (int)noiseTexSize.y, previewPixelArray).str();
            emscripten_browser_file::download("FastNoiseLite.bmp", "image/bmp", bmpFile);
        }

        ImGui::Image((void*)(intptr_t)noiseTex, noiseTexSize);
        ImGui::End();

        ImGui::BeginViewportSideBar("status", ImGui::GetMainViewport(), ImGuiDir_Down, 32, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        float textOffset = 0;
        ImGui::Text("Preview Stats: %0.02fms", previewGenTimeFinal);
        ImGui::SameLine(textOffset += 200);
        ImGui::Text("Min: %0.04f", previewMinFinal);
        ImGui::SameLine(textOffset += 100);
        ImGui::Text("Max: %0.04f", previewMaxFinal);
        ImGui::SameLine(textOffset += 100);
        ImGui::Text("Mean: %0.04f", previewMeanFinal);

        ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::CalcTextSize("GitHub").x - 15);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2);
        if (ImGui::Button("GitHub"))
        {
            emscripten_run_script("window.open('https://github.com/Auburn/FastNoiseLite', '_blank').focus();");
        }
        ImGui::PopStyleVar();
        ImGui::End();
    }

    void SetupDocking()
    {
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", nullptr, window_flags);
        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

            static auto first_time = true;
            if (first_time)
            {
                first_time = false;

                ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
                ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

                float split = 280 / viewport->Size.x;

                // split the dockspace into 2 nodes -- DockBuilderSplitNode takes in the following args in the following order
                //   window ID to split, direction, fraction (between 0 and 1), the final two setting let's us choose which id we want (which ever one we DON'T set as NULL, will be returned by the function)
                //                                                              out_id_at_dir is the id of the node in the direction we specified earlier, out_id_at_opposite_dir is in the opposite direction
                auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, split, nullptr, &dockspace_id);

                // we now dock our windows into the docking node we made above
                ImGui::DockBuilderDockWindow("Noise Texture", dockspace_id);
                ImGui::DockBuilderDockWindow("Settings", dock_id_left);
                ImGui::DockBuilderGetNode(dockspace_id)->SetLocalFlags(ImGuiDockNodeFlags_NoTabBar);
                ImGui::DockBuilderGetNode(dock_id_left)->SetLocalFlags(ImGuiDockNodeFlags_NoTabBar);
                ImGui::DockBuilderFinish(dockspace_id);
            }
        }

        ImGui::End();
    }

    std::stringstream EncodeBMP(size_t width, size_t height, unsigned char* data)
    {
        std::stringstream file;

        struct BmpHeader
        {
            // File header (14)
            // char b = 'B';
            // char m = 'M';
            uint32_t fileSize;
            uint32_t reserved = 0;
            uint32_t dataOffset = 14u + 12u + (256u * 3u);
            // Bmp Info Header (12)
            uint32_t headerSize = 12u;
            uint16_t sizeX;
            uint16_t sizeY;
            uint16_t colorPlanes = 1u;
            uint16_t bitDepth = 8u;
        };

        int paddedSizeX = (int)width;
        int padding = paddedSizeX % 4;
        if (padding)
        {
            padding = 4 - padding;
            paddedSizeX += padding;
        }

        BmpHeader header;
        header.fileSize = header.dataOffset + (uint32_t)(paddedSizeX * height);
        header.sizeX = (uint16_t)width;
        header.sizeY = (uint16_t)height;

        file << 'B' << 'M';
        file.write(reinterpret_cast<char*>(&header), sizeof(BmpHeader));

        // Colour map
        for (int i = 0; i < 256; i++)
        {
            char b3[] = { (char)i, (char)i, (char)i };
            file.write(b3, 3);
        }

        int idx = 0;

        for (size_t y = 0; y < height; y++)
        {
            for (size_t x = 0; x < height; x++)
            {
                file.write(reinterpret_cast<char*>(data + idx), 1);
                idx += 4;
            }

            if (padding)
            {
                int zero = 0;
                file.write(reinterpret_cast<char*>(&zero), padding);
            }
        }

        return file;
    }

    void UpdateTexture(bool newPreview)
    {
        if (previewPixelY == 0 && !newPreview)
        {
            return;
        }

        if (newPreview)
        {
            if (previewPixelArray)
            {
                delete[] previewPixelArray;
            }
            previewPixelY = 0;
            previewGenTime = 0;

            noiseTexSizeGenX = previewSize[0];
            noiseTexSizeGenY = previewSize[1];

            previewMin = INFINITY;
            previewMax = -INFINITY;
            previewMean = 0;
            previewPixelArray = new unsigned char[noiseTexSizeGenX * noiseTexSizeGenY * 4];
        }

        int index = noiseTexSizeGenX * previewPixelY * 4;
        float scale = 255 / (noiseTexMax - noiseTexMin);

        auto timer = std::chrono::high_resolution_clock::now();

        for (int y = previewPixelY; y < noiseTexSizeGenY; y++)
        {
            previewPixelY = y + 1;
            for (int x = 0; x < noiseTexSizeGenX; x++)
            {
                float noise;
                double posX = (double)(x - noiseTexSizeGenX / 2);
                double posY = (double)(y - noiseTexSizeGenY / 2);

                if (preview3d)
                {
                    double posZ = previewPosZ;
                    if (fnlDomainWarpType > 0)
                    {
                        fnlWarp.DomainWarp(posX, posY, posZ);
                    }
                    noise = fnl.GetNoise(posX, posY, posZ);
                }
                else
                {
                    if (fnlDomainWarpType > 0)
                    {
                        fnlWarp.DomainWarp(posX, posY);
                    }
                    noise = fnl.GetNoise(posX, posY);
                }

                unsigned char cNoise = (unsigned char)std::max(0.0f, std::min(255.0f, (noise - noiseTexMin) * scale));
                previewPixelArray[index++] = cNoise;
                previewPixelArray[index++] = cNoise;
                previewPixelArray[index++] = cNoise;
                previewPixelArray[index++] = 255;

                previewMin = std::min(previewMin, noise);
                previewMax = std::max(previewMax, noise);
                previewMean += noise;
            }

            if ((y % 8) == 0 && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timer).count() >= 80)
            {
                break;
            }
        }

        previewGenTime += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - timer).count() / 1000000.f;

        if (previewPixelY >= noiseTexSizeGenY)
        {
            noiseTexSize.x = noiseTexSizeGenX;
            noiseTexSize.y = noiseTexSizeGenY;
            previewPixelY = 0;
            previewMeanFinal = previewMean / (noiseTexSize.x * noiseTexSize.y);
            previewMinFinal = previewMin;
            previewMaxFinal = previewMax;
            previewGenTimeFinal = previewGenTime;

            if (noiseTex != 0)
            {
                glDeleteTextures(1, &noiseTex);
            }

            // Create a OpenGL texture identifier
            glGenTextures(1, &noiseTex);
            glBindTexture(GL_TEXTURE_2D, noiseTex);

            // Setup filtering parameters for display
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

            // Upload pixels into texture
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, noiseTexSizeGenX, noiseTexSizeGenY, 0, GL_RGBA, GL_UNSIGNED_BYTE, previewPixelArray);
        }
    }
};

int main()
{
    FastNoiseLitePreviewApp app;

    if (!app.init())
        return 1;

    app.run_main_loop();

    return 0;
}