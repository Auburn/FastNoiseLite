#include <algorithm>
#include <cstdint>

#include <imgui_app/ImGui_Application.hpp>
#include <imgui_internal.h>

#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include "FastNoiseLite.h"
#include "../Cpp/FastNoiseLite.h"

class FastNoiseLitePreviewApp : public ImGui_Application
{
public:
    bool init() override
    {
        bool success = ImGui_Application::init();
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        UpdateTexture();   

        return success;
    }

    uint32_t noiseTex = 0;
    int noiseTexSize = 1024;

    FastNoiseLite fnl;
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
    
    int fnlCellularType = 0;
    int fnlCellularReturnType = 0;
    float fnlCellularJitter = 1.0f;

    int fnlDomainWarpType = 0;
    int fnlDomainWarpRotationType = 0;
    float fnlDomainWarpAmplitude = 30.0f;
    float fnlDomainWarpFrequency = 0.005f;

    int fnlDomainWarpFractalType = 0;
    int fnlDomainWarpFractalOctaves = 5;
    float fnlDomainWarpFractalLacunarity = 2.0f;
    float fnlDomainWarpFractalGain = 0.5f;

    void draw_ui() override 
    { 
        SetupDocking();     

        //ImGui::ShowDemoWindow();

        ImGui::Begin("FastNoise Lite");

        bool texUpdate = false;

        /// General
        ImGui::TextUnformatted("General");

        if( ImGui::Combo("Noise Type", &fnlNoiseType,"OpenSimplex2\0OpenSimplex2S\0Cellular\0Perlin\0ValueCubic\0Value\0") )
        {
            fnl.SetNoiseType( (FastNoiseLite::NoiseType)fnlNoiseType );
            texUpdate = true;
        }
        if( ImGui::Combo("Rotation Type 3D", &fnlRotationType,"None\0ImproveXYPlanes\0ImproveXZPlanes\0") )
        {
            fnl.SetRotationType3D( (FastNoiseLite::RotationType3D)fnlRotationType );
            texUpdate = true;
        }
        if( ImGui::DragInt("Seed", &fnlSeed) )
        {
            fnl.SetSeed( fnlSeed );
            texUpdate = true;
        }
        if( ImGui::DragFloat("Frequency", &fnlFrequency, 0.001f) )
        {
            fnl.SetFrequency( fnlFrequency );
            texUpdate = true;
        }

        /// Fractal
        ImGui::TextUnformatted("Fractal");

        if( ImGui::Combo("Type", &fnlFractalType,"None\0FBm\0Ridged\0PingPong\0") )
        {
            fnl.SetFractalType( (FastNoiseLite::FractalType)fnlFractalType );
            texUpdate = true;
        }
        if( ImGui::DragInt("Octaves", &fnlFractalOctaves, 0.1f) )
        {
            fnl.SetFractalOctaves( fnlFractalOctaves );
            texUpdate = true;
        }
        if( ImGui::DragFloat("Lacunarity", &fnlFractalLacunarity, 0.01f) )
        {
            fnl.SetFractalLacunarity( fnlFractalLacunarity );
            texUpdate = true;
        }
        if( ImGui::DragFloat("Gain", &fnlFractalGain, 0.01f) )
        {
            fnl.SetFractalGain( fnlFractalGain );
            texUpdate = true;
        }
        if( ImGui::DragFloat("Weighted Strength", &fnlFractalWeightedStrength, 0.01f) )
        {
            fnl.SetFractalWeightedStrength( fnlFractalWeightedStrength );
            texUpdate = true;
        }        
        ImGui::BeginDisabled( fnlFractalType != (int)FastNoiseLite::FractalType_PingPong );
        if( ImGui::DragFloat("Ping Pong Strength", &fnlFractalPingPongStrength, 0.01f) )
        {
            fnl.SetFractalPingPongStrength( fnlFractalPingPongStrength );
            texUpdate = true;
        }
        ImGui::EndDisabled();



        ImGui::End();
        
        if(texUpdate)
        {         
            UpdateTexture();  
        }

        ImGui::Begin("Noise Texture");        
        ImGui::Image((void*)(intptr_t)noiseTex, ImVec2(noiseTexSize, noiseTexSize));
        ImGui::End();
    }

    void SetupDocking()
    {
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

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

                // split the dockspace into 2 nodes -- DockBuilderSplitNode takes in the following args in the following order
                //   window ID to split, direction, fraction (between 0 and 1), the final two setting let's us choose which id we want (which ever one we DON'T set as NULL, will be returned by the function)
                //                                                              out_id_at_dir is the id of the node in the direction we specified earlier, out_id_at_opposite_dir is in the opposite direction
                auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.3f, nullptr, &dockspace_id);

                // we now dock our windows into the docking node we made above
                ImGui::DockBuilderDockWindow("Noise Texture", dockspace_id);
                ImGui::DockBuilderDockWindow("FastNoise Lite", dock_id_left);
                ImGui::DockBuilderFinish(dockspace_id);
            }
        }

        ImGui::End();
    }

    void UpdateTexture()
    {
        if(noiseTex != 0)
        {
            glDeleteTextures(1, &noiseTex);
        }

        unsigned char *pixelArray = new unsigned char[noiseTexSize * noiseTexSize * 4];
        int index = 0;

        for(int y = 0; y < noiseTexSize; y++)
        {            
            for(int x = 0; x < noiseTexSize; x++)
            {
                float noise = fnl.GetNoise((double)x, (double)y);
                unsigned char cNoise = (unsigned char)std::max(0.0f, std::min(255.0f, noise * 127.5f + 127.5f));
                pixelArray[index++] = cNoise;
                pixelArray[index++] = cNoise;
                pixelArray[index++] = cNoise;
                pixelArray[index++] = 255;
            }
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, noiseTexSize, noiseTexSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelArray);
        delete[] pixelArray;
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