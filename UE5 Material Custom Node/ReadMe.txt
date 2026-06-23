1. In a material, create a Custom node
2. Paste code in from "CustomNodeCode.hlsl"
  a. fnlGetNoise3D is the activated function by default. If you wish to change this, go to the bottom of the code and comment/uncomment the last 4 lines appropriately
3. You MUST add the following inputs in the custom node (and all MUST be connected to a variable, otherwise it will error):

WorldPosition
Seed_In
Frequency_In
Noise_Type_In
Rotation_Type_In
Fractal_Type_In
Octaves_In
Lacunarity_In
Gain_In
Weighted_Strength_In
Ping_Pong_Strength_In
Cellular_Distance_Func_In
Cellular_Return_Type_In
Cellular_Jitter_Mod_In
Domain_Warp_Amp_In
Domain_Warp_Type_In

4. VOILA